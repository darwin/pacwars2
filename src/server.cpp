/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   server side code
//## author:  Antonin Hildebrand
//##         
//## started: 1.4.2000
//## revised: 2.3.2001
//## 
//###########################################################################

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL.h"
#include "SDL_image.h"
#include "const.h"
#include "SDL_net.h"
#include "SDL_Console.h"
#include "net.h"
#include "server.h"
#include "client.h"
#include "scripting.h"
#include "stdconfig.h"


//###########################################################################
//## Globals
//###########################################################################

int net_server_status;

int server_sent;            
int server_received; 


// temp
Uint32    sltime = 0;
Uint32    ssum = 0;
Uint32    servertime = 0;
Uint32    snum = 0;

int SV_SetName(char* name);

void CHs_maxclients(cvar_t* var, int* changed)
{
  if (var->value>PWP_TOTALMAX_CLIENTS)
  {
    ConOutEx(SERVER_FONT, "Server: sv_maxclients(%d) is greater than PWP_TOTALMAX_CLIENTS(%d).", (int)var->value, PWP_TOTALMAX_CLIENTS);
    ConOutEx(SERVER_FONT, "You will need to recompile game with greater PWP_TOTALMAX_CLIENTS, but see SDLNET_MAX_UDPCHANNELS value in SDL_net.h for updating too.");
    (*changed) = false;
    return;
  }
  ConOutEx(SERVER_FONT, "This will take effect after starting new server.");
}

void CHs_name(cvar_t* var, int* changed)
{
  SV_SetName(var->string);
}

// cvars
cvar_t	s_maxclients = {"s_maxclients","8",true, true, CHs_maxclients};  // !!! keep SDLNET_MAX_UDPCHANNELS >= server_info.maxclients+1
cvar_t	s_name = {"s_name", STD_SERVER_NAME, true, true, CHs_name};
cvar_t	s_welcome_msg = {"s_welcome_msg", STD_WELCOME_MSG, true};

cvar_t	s_next_map = {"s_next_map", "store", true};
cvar_t	s_next_script = {"s_next_script", "store", true};
cvar_t	s_next_gt = {"s_next_gt", "0", true, true};
cvar_t	s_timelimit = {"s_timelimit", "500", true, true};
cvar_t	s_fraglimit = {"s_fraglimit", "20", true, true};

cvar_t	s_deletetmps = {"s_deltmps","1",true};
cvar_t	s_downloading = {"s_downloading", "0", true, true};
cvar_t	s_uploading = {"s_uploading", "0", true, true};
cvar_t	s_remote = {"s_remote","0",true, true};
cvar_t	s_acpass = {"s_acpass","pacrulez",true};

//###########################################################################
//## Server part
//###########################################################################

SDLNet_SocketSet ssocket_set = NULL;

UDPpacket **game_spackets = NULL;  // packets for receiving
UDPpacket *game_spacket = NULL;    // packet for sending
UDPpacket **msg_spackets = NULL;   // packets for receiving
UDPpacket *msg_spacket = NULL;     // packet for sending

UDPsocket game_ssock = NULL;  // comunicating on channels 0 - (MAXPLAYERS-1)
UDPsocket msg_ssock = NULL;   // comunicating on channels 0 - (MAXPLAYERS-1)

net_client client[PWP_TOTALMAX_CLIENTS];   // server clients
net_server_info server_info;          // local server information

int SV_ClearFileTransfer(int i, int how)
{
  char tmp[_MAX_PATH];
  
  if (client[i].file_sock)
  {
    SDLNet_TCP_DelSocket(ssocket_set, client[i].file_sock);
    SDLNet_TCP_Close(client[i].file_sock);
    client[i].file_sock = NULL;
  }
  
  switch (how) {
  case FT_FINISHED_RENAME:
    if (client[i].file)
    {
      fclose(client[i].file);
      client[i].file = NULL;
    }
    // rename tmp file
    TempName(client[i].local_fn, tmp);
    if (rename(tmp, client[i].local_fn))
    {
      ConOutEx(SERVER_FONT, "Server: error renaming received file !");
    }
    break;
  case FT_FINISHED:
    if (client[i].file)
    {
      fclose(client[i].file);
      client[i].file = NULL;
    }
    break;
  case FT_ABANDONED:
    if (client[i].file)
    {
      fclose(client[i].file);
      client[i].file = NULL;
      
      // delete broken tmp file ?
      if (s_deletetmps.value)
      {
        TempName(client[i].local_fn, tmp);
        if (remove(tmp)!=0)
        {
          ConOutEx(SERVER_FONT, "Server: error removing broken tmp file !");
        }
      }
    }
    break;
  }
  return 0;
}

int PurgePlayer(int cnum)
{
  if (client[cnum].status!=CS_UNUSED)
  {
    for (GAME_MAXOBJS_TYPE i=0; i<GAME_MAX_OBJS; i++) 
    {
      if ((server_info.game.objs[i]->state&OSTATE_ACTIVE) && (server_info.game.objs[i]->GetType()==ot_player))
      {
        GPlayer * p = (GPlayer*)server_info.game.objs[i];
        if (p->brain_owner==cnum)
        {
          SV_BroadcastPrintf("System: Player \"%s\" has left the game", p->player_name.GetValRef()->chars);
          p->state = 0;
        }
      }
    }
  
    return 0;
  }
  else
    return 1;
}

int ClearServerClient(int cnum)
{
  if (client[cnum].status!=CS_UNUSED)
  {
    SDLNet_UDP_Unbind(msg_ssock, cnum);
    SDLNet_UDP_Unbind(game_ssock, cnum);
    
    client[cnum].status = CS_UNUSED;
    client[cnum].con_attach = 0;
    client[cnum].stats.Clear();
    client[cnum].file_status = FS_READY;
    if (client[cnum].file) 
      fclose(client[cnum].file);
    client[cnum].file = NULL;
    client[cnum].ping = 0;
    client[cnum].pingreturned = true;
    client[cnum].pingsent = 0;
    client[cnum].replicator.Reset();
    ClearPool(&client[cnum].msg_pool);
    ClearPool(&client[cnum].game_pool);
    
    SV_ClearFileTransfer(cnum, FT_FINISHED);
    return 0;
  }
  else
    return 1;
}

int PrepareClientForNewGame(int cnum)
{
  if (client[cnum].status==CS_ACTIVE)
  {
      client[cnum].ping = 0;
      client[cnum].pingreturned = true;
      client[cnum].pingsent = 0;
      ClearPool(&client[cnum].game_pool);
      ScriptMan->RunScript(map_script_num, "csInit");
      return 0;
  }
  else 
    return 1;
}

int SV_Clear()
{
  if (net_server_status!=NS_UNINITED)
  {
    // clear clients' slots
    for (int i=0; i<server_info.maxclients; i++)
      ClearServerClient(i);
    // clear server info
    server_info.game.Clear();
    server_info.active = false;
    net_server_status=NS_INITED;
    server_received = 0;
    server_sent = 0;
    
  }  
  return 0;
}

int SV_Start()
{
  if (net_server_status != NS_UNINITED) 
  {  
    SDLNet_SetError("Server has already started");
    return 1;
  }
  server_info.maxclients = (int) s_maxclients.value;
  if (server_info.maxclients>=SDLNET_MAX_UDPCHANNELS)
  {
    ConOutEx(SERVER_FONT, "Server: sv_maxclients(%d) is equal or greater than SDLNET_MAX_UDPCHANNELS(%d).", server_info.maxclients, SDLNET_MAX_UDPCHANNELS);
    ConOutEx(SERVER_FONT, "You will need to recompile game with greater SDLNET_MAX_UDPCHANNELS value in SDL_net.h");
    SDLNet_SetError("");
    return 1;
  }
  strncpy(server_info.name, s_name.string, PWP_MAX_SERVER_NAME);
  
  int i;
  // prepare clients' slots
  for (i=0; i<server_info.maxclients; i++)
  {
    client[i].status = CS_UNUSED;
    client[i].file_sock = NULL;
    if (CreatePool(&client[i].msg_pool, MSG_SPOOL_SIZE, MSG_RPOOL_SIZE, PWP_MMP_SIZE))
    {
      SDLNet_SetError("Couldn't create msg pool (server): Out of memory\n");
      return 1;
    }
    if (CreatePool(&client[i].game_pool, GAME_SPOOL_SIZE, GAME_RPOOL_SIZE, PWP_MGP_SIZE))
    {
      SDLNet_SetError("Couldn't create game pool (server): Out of memory\n");
      return 1;
    }
    client[i].buffer = (Uint8*)malloc(PWP_FBUFFER_SIZE);
    if (!client[i].buffer)
    {
      SDLNet_SetError("Couldn't create file buffer (server): Out of memory\n");
      return 1;
    }
    client[i].bsize = PWP_FBUFFER_SIZE;
    client[i].quantum = (Uint8*)malloc(PWP_FQUANTUM_SIZE);
    if (!client[i].quantum)
    {
      SDLNet_SetError("Couldn't create quantum buffer (server): Out of memory\n");
      return 1;
    }
    client[i].qsize = PWP_FQUANTUM_SIZE;
  }
  SV_Clear();
  
  // allocate net structs
  game_spackets = SDLNet_AllocPacketV(PWP_MGP_SCOUNT, PWP_MGP_SIZE);
  if ( game_spackets == NULL ) {
    SDLNet_SetError("Couldn't allocate packets: Out of memory\n");
    return 1;
  }
  
  game_spacket = SDLNet_AllocPacket(PWP_MGP_SIZE);
  if ( game_spacket == NULL ) {
    SDLNet_SetError("Couldn't allocate packets: Out of memory\n");
    return 1;
  }
  
  msg_spackets = SDLNet_AllocPacketV(PWP_MMP_SCOUNT, PWP_MMP_SIZE);
  if ( msg_spackets == NULL ) {
    SDLNet_SetError("Couldn't allocate packets: Out of memory\n");
    return 1;
  }
  
  msg_spacket = SDLNet_AllocPacket(PWP_MMP_SIZE);
  if ( msg_spacket == NULL ) {
    SDLNet_SetError("Couldn't allocate packets: Out of memory\n");
    return 1;
  }
  
  game_ssock = SDLNet_UDP_Open(PWP_GAME_SPORT);
  if ( game_ssock == NULL ) {
    SDLNet_SetError("Couldn't create UDP endpoint (game)\n");
    return 1;
  }
  
  msg_ssock = SDLNet_UDP_Open(PWP_MSG_SPORT);
  if ( msg_ssock == NULL ) {
    SDLNet_SetError("Couldn't create UDP endpoint (msg)\n");
    return 1;
  }
  
  ssocket_set = SDLNet_AllocSocketSet(1+1+server_info.maxclients); // zbytek je pro download/upload
  if ( ssocket_set == NULL ) {
    SDLNet_SetError("Couldn't create socket set: %s\n",
						SDLNet_GetError());
    return 3;
  }
  
  SDLNet_UDP_AddSocket(ssocket_set,  msg_ssock);
  SDLNet_UDP_AddSocket(ssocket_set, game_ssock);
  
  net_server_status = NS_INITED;
  server_info.active = false;
  return 0;
}

int SV_Run()
{
  if (net_server_status==NS_INITED)
  {
    server_info.game.SetGameType((Uint8)s_next_gt.value);
    if (SV_SetMap(s_next_map.string, s_next_script.string)) return 1;
    
    net_server_status=NS_RUNNING;
  }
  return 0;
}

int SV_Shutdown(Uint8 type)
{
  // inform clients that server is about to die
  Uint8 buf[2];
  net_msg MSG(buf);
  if (net_server_status==NS_RUNNING)
  {
    MSG.pos = 0;
    MSG << MSGID_SERVERDOWN;
    MSG << type;
    SV_BroadcastMsg(MSG, msg_ssock, RESEND_SYSTEM);
  }
  return SV_Clear();
}

int SV_ChangeGame()
{
  SV_Shutdown(SD_CHANGING_GAME);

  // TODO: set s_next_map to next map in queue
  // TODO: set s_next_gt
  // ...

  return SV_Run();
}

int SV_End()
{
  if (net_server_status == NS_UNINITED) return 0;
  SV_Shutdown(SD_OFFLINE);  
  int i;
  for (i=0; i<server_info.maxclients; i++)
  {
    ClearServerClient(i);
    if (client[i].file_sock) {
      SDLNet_TCP_Close(client[i].file_sock);
      client[i].file_sock = NULL;
    }
    DestroyPool(&client[i].msg_pool);
    DestroyPool(&client[i].game_pool);
    free(client[i].buffer);
    client[i].buffer = NULL;
    free(client[i].quantum);
    client[i].quantum = NULL;
  }
  
  if ( game_ssock != NULL ) {
    SDLNet_UDP_Close(game_ssock);
    game_ssock = NULL;
  }
  if ( msg_ssock != NULL ) {
    SDLNet_UDP_Close(msg_ssock);
    msg_ssock = NULL;
  }
  if ( ssocket_set != NULL ) {
    SDLNet_FreeSocketSet(ssocket_set);
    ssocket_set = NULL;
  }
  if ( game_spackets != NULL ) {
    SDLNet_FreePacketV(game_spackets);
    game_spackets = NULL;
  }
  if ( game_spacket != NULL ) {
    SDLNet_FreePacket(game_spacket);
    game_spacket = NULL;
  }
  if ( msg_spackets != NULL ) {
    SDLNet_FreePacketV(msg_spackets);
    msg_spackets = NULL;
  }
  if ( msg_spacket != NULL ) {
    SDLNet_FreePacket(msg_spacket);
    msg_spacket = NULL;
  }
  
  net_server_status = NS_UNINITED;
  server_info.active = false;
  return 0;
}

int SV_BroadcastMsg(net_msg& MSG, UDPsocket sock, Uint32 resend)
{
  for (int i=0; i<server_info.maxclients; i++)
  {
    if (client[i].status!=CS_UNUSED) 
    {
      MSG.Send(&client[i].msg_pool, &client[i].stats, sock, i, true, resend);
    }
  }
  return 0;
}

// Sends text across to be displayed 
void SV_ClientPrintf(int c, char *fmt, ...)
{
  va_list		argptr;
  char		string[1024];
  
  va_start (argptr,fmt);
  vsprintf (string, fmt,argptr);
  va_end (argptr);
  
  Uint8 sendbuf[MSG_MAX_BODY_SIZE];
  net_msg MSG(sendbuf);
  MSG << MSGID_CHATWRITE;
  MSG.nprintf(MSG_MAX_BODY_SIZE - POS_MSG_TEXT - 1, "%s", string);
  MSG.Send(&client[c].msg_pool, &client[c].stats, msg_ssock, c, true, RESEND_CHAT);
}

// Sends text to all active clients
void SV_BroadcastPrintf(char *fmt, ...)
{
  va_list		argptr;
  char		string[1024];
  
  va_start (argptr,fmt);
  vsprintf (string, fmt,argptr);
  va_end (argptr);
  
  Uint8 sendbuf[MSG_MAX_BODY_SIZE];
  net_msg MSG(sendbuf);
  MSG << MSGID_CHATWRITE;
  MSG.nprintf(MSG_MAX_BODY_SIZE - POS_MSG_TEXT - 1, "%s", string);
  SV_BroadcastMsg(MSG, msg_ssock, RESEND_CHAT);

  if (!client_info.active)
    ConOutEx(SERVER_FONT, string);
}

// Sends sound to all active clients
void SV_BroadcastSound(Uint8 id, Uint8 volume)
{
  Uint8 sendbuf[MSG_MAX_BODY_SIZE];
  net_msg MSG(sendbuf);
  MSG << MSGID_SOUND << id << volume;
  SV_BroadcastMsg(MSG, msg_ssock, RESEND_CHAT);
}

void SV_ClientSound(int c,Uint8 id, Uint8 volume)
{
  Uint8 sendbuf[MSG_MAX_BODY_SIZE];
  net_msg MSG(sendbuf);
  MSG << MSGID_SOUND << id << volume;
  MSG.Send(&client[c].msg_pool, &client[c].stats, msg_ssock, c, true);
}

// Sends voice to all active clients
void SV_BroadcastVoice(Uint8 id, Uint8 volume, Uint8 priority)
{
  Uint8 sendbuf[MSG_MAX_BODY_SIZE];
  net_msg MSG(sendbuf);
  MSG << MSGID_VOICE << id << volume << priority;
  SV_BroadcastMsg(MSG, msg_ssock, RESEND_CHAT);
}

void SV_ClientVoice(int c, Uint8 id, Uint8 volume, Uint8 priority)
{
  Uint8 sendbuf[MSG_MAX_BODY_SIZE];
  net_msg MSG(sendbuf);
  MSG << MSGID_VOICE << id << volume << priority;
  MSG.Send(&client[c].msg_pool, &client[c].stats, msg_ssock, c, true);
}

void UploadRequest(int cnum, char* file, Uint16 port)
{
  IPaddress nIP;
  char tmp[_MAX_PATH];
  
  if (client[cnum].status!=CS_UNUSED)
  {
    strcpy(client[cnum].local_fn, file);
    
    nIP = client[cnum].msg_ip;
    nIP.port = 	SDL_SwapBE16(port);
    
    client[cnum].file_sock = SDLNet_TCP_Open(&nIP);
    if (client[cnum].file_sock==NULL) 
    {
      ConOutEx(SERVER_FONT, "Server: File transfer connection failed !");
      SV_ClearFileTransfer(cnum, FT_ABANDONED);
      return;
    } 
    else 
    {
      SDLNet_TCP_AddSocket(ssocket_set, client[cnum].file_sock);
      
      // check if file exists
      client[cnum].file = fopen(file , "rb");
      if (client[cnum].file)
      {
        ConOutEx(SERVER_FONT, "Server: remote file exists");
        SV_ClientPrintf(cnum, "Server: file exists on server");
        SV_ClearFileTransfer(cnum, FT_ABANDONED);
        return;
      }
      
      // open temp file for writing
      TempName(client[cnum].local_fn, tmp);
      client[cnum].file = fopen(tmp, "wb");
      if (!client[cnum].file)
      {
        ConOutEx(SERVER_FONT, "Server: couldn't open remote file for writing");
        SV_ClearFileTransfer(cnum, FT_ABANDONED);
        return;
      }
      client[cnum].zs.zalloc = (alloc_func)0;
      client[cnum].zs.zfree = (free_func)0;
      client[cnum].zs.opaque = (voidpf)0;
      
      int err = inflateInit(&client[cnum].zs);
      if (err!=Z_OK) {
        ConOutEx(SERVER_FONT, "stream initialization failed (client): %d", err);
        SV_ClearFileTransfer(cnum, FT_ABANDONED);
        return;
      }
      
      client[cnum].zs.avail_out = client[cnum].bsize;
      client[cnum].zs.next_out  = (Bytef*)client[cnum].buffer;
      client[cnum].file_status=FS_RECEIVING;
      
      client[cnum].lrec_time = SDL_GetTicks();
    }
  }
}

void DownloadRequest(int cnum, char* file, Uint16 port)
{
  IPaddress nIP;
  
  if (client[cnum].status!=CS_UNUSED)
  {
    nIP = client[cnum].msg_ip;
    nIP.port = 	SDL_SwapBE16(port);
    
    client[cnum].file_sock = SDLNet_TCP_Open(&nIP);
    if (client[cnum].file_sock==NULL) 
    {
      ConOutEx(SERVER_FONT, "Server: File transfer connection failed !");
      SV_ClearFileTransfer(cnum, FT_ABANDONED);
      return;
    } 
    else 
    {
      SDLNet_TCP_AddSocket(ssocket_set, client[cnum].file_sock);
      
      // check if file exists
      client[cnum].file = fopen(file, "rb");
      if (!client[cnum].file)
      {
        ConOutEx(SERVER_FONT, "Server: requested file doesn't exist");
        SV_ClientPrintf(cnum, "Server: requested file doesn't exist on server");
        SV_ClearFileTransfer(cnum, FT_ABANDONED);
        return;
      }
      
      client[cnum].zs.zalloc = (alloc_func)0;
      client[cnum].zs.zfree = (free_func)0;
      client[cnum].zs.opaque = (voidpf)0;
      
      int err = deflateInit(&client[cnum].zs, PWP_DOWNLOAD_COMPRESSION);
      if (err!=Z_OK) {
        ConOutEx(SERVER_FONT, "stream initialization failed (server): %d", err);
        SV_ClearFileTransfer(cnum, FT_ABANDONED);
        return;
      }
      client[cnum].zs.avail_in = 0;
      client[cnum].zs.next_in  = (Bytef*)client[cnum].buffer;
      client[cnum].file_status=FS_SENDING;
    }
  }
}

void InvitationConfirmed(net_message *msg)
{
  // WELCOME PACKET was confirmed
  // send to all players that the new player has been really connected
  Uint8 sendbuf[MSG_MAX_BODY_SIZE];
  net_msg MSG(sendbuf);
  MSG << MSGID_CHATWRITE;
  MSG.nprintf(MSG_MAX_BODY_SIZE - POS_MSG_TEXT - 1,
    "System: %s connected.", 
    client[msg->tag].name);
  
  SV_BroadcastMsg(MSG, msg_ssock, RESEND_CHAT);
  client[msg->tag].status = CS_PREPARING; 
}


void EnteringGameConfirmed(net_message *msg)
{
  // ENTERGAME PACKET was confirmed
  // set entered player as active
  client[msg->tag].status = CS_ACTIVE; 

  // mark all active object dirty for new client
  for (GAME_MAXOBJS_TYPE i=0; i<GAME_MAX_OBJS; i++) 
  {
	  if (server_info.game.objs[i]->state&OSTATE_ACTIVE)
	  {
      // optimalization
		  server_info.game.objs[i]->MakeDirty(PWP_TOTALMAX_CLIENTS + msg->tag, true); // this is special hack make dirty only nonimplicit values
	  }
  }
}


int HandleServerMessage()
{
  int n;
  int i;
  int len;
  Uint8 new_player;
  int cnum;
  char fname[_MAX_PATH];
  char buf[PWP_MAX_CLIENT_NAME];
  Uint8 sendbuf[MSG_MAX_BODY_SIZE];
  
  net_msg MSG;         
  net_msg MSG2(sendbuf);
  net_client* aclient;
  
  n = SDLNet_UDP_RecvV(msg_ssock, msg_spackets);
  while ( n-- > 0 ) 
  {
    cnum = msg_spackets[n]->channel; // client num
    if ( cnum >= 0 ) 
    {
      if ((cnum>=server_info.maxclients) || (client[cnum].status==CS_UNUSED))
      {
        ConErr("%d>Server: received bad client msg_packet ID=%d from %s !\n", server_info.ticks, msg_spackets[n]->data[POS_MSG_ID], AddrToS(&msg_spackets[n]->address));
        continue;
      }
      aclient = &client[cnum];
      
      len = MSG.Receive(&aclient->msg_pool, &aclient->stats, msg_ssock, cnum, msg_spackets[n], true);
      if (len<=0) continue;  // bad or confirming packet
      
      // now, MSG contains valid received MSG.buf
      
      switch (MSG.buf[POS_MSG_ID]) {
      case MSGID_PING:
        // ! empty
        // pong was sent by notification mechanism
        break;
        
      // chat message POST 
      case MSGID_CHATPOST: 
        SV_BroadcastPrintf("%s: %s", aclient->name, (char*)&MSG.buf[POS_MSG_TEXT]);
        break;
        
      case MSGID_CNAME: 
        strcpy(buf, aclient->name);
        memcpy(aclient->name, &MSG.buf[POS_MSG_CNAME], LEN_MSG_CNAME);
        ConOutEx(SERVER_FONT, "%s changed to %s", buf, aclient->name);
        break;
      case MSGID_CHASEPLAYERRQ:
        SV_ChasePlayer(&MSG, cnum);
        break;
      case MSGID_CREATEPLAYER: 
        SV_CreatePlayer(&MSG, cnum);
        break;
        
      case MSGID_DESTROYPLAYER: 
        SV_DestroyPlayer(&MSG, cnum);
        break;
      case MSGID_SKINPLAYER: 
        SV_SkinPlayer(&MSG, cnum);
        break;

        
      case MSGID_REMOTECMD: 
        if (s_remote.value)
        {
          ExecuteRemoteCommand(cnum, (char*)&MSG.buf[POS_MSG_CMD]);
        }
        else
        {
          SV_ClientPrintf(cnum, "Server: remote console is disabled");
          ConOutEx(SERVER_FONT, "Server: Client %d tried to execute remote command: %s", cnum, (char*)&MSG.buf[POS_MSG_CMD]);
        }
        break;
        
      case MSGID_ATTACHCON:
        if (s_remote.value && strncmp(s_acpass.string, (char*)&MSG.buf[POS_MSG_ACPASS], LEN_MSG_ACPASS)==0)
        {
          aclient->con_attach = 1;
          ConOutEx(SERVER_FONT, "%s attached server console", aclient->name);
          MSG2 << MSGID_ATTACHDONE;
          MSG2.Send(&aclient->msg_pool, &aclient->stats, msg_ssock, cnum, true, RESEND_SYSTEM);
        }
        else
        {
          if (!s_remote.value)
          {
            ConOutEx(SERVER_FONT, "%s tried attach server console, feature disabled by \"s_remote\" cvar", aclient->name);
            SV_ClientPrintf(cnum, "Server has disabled remote console access");
          }
          else
          {
            ConOutEx(SERVER_FONT, "%s tried attach server console with bad password", aclient->name);
            SV_ClientPrintf(cnum, "Password is incorrect");
          }
        }
        break;
        
      case MSGID_DETACHCON:
        aclient->con_attach = 0;
        ConOutEx(SERVER_FONT, "%s detached server console", aclient->name);
        break;
        
      case MSGID_DOWNLOAD:
        if (!s_downloading.value)
        {
          ConOutEx(SERVER_FONT, "Server: received download request, but downloading is not enabled, set \"s_downloading\" variable to 1");
        }
        else
          if (!CheckFileName((char*)&MSG.buf[POS_MSG_FN]))
          {
            ConErr("%d>Server: invalid download request %s (packet ID=%d from %s) !\n", server_info.ticks, (char*)&MSG.buf[POS_MSG_FN], msg_spackets[n]->data[POS_MSG_ID], AddrToS(&msg_spackets[n]->address));
          }
          else
          {
            DLIDtoFileName(MSG.buf[POS_MSG_DLID], (char*)&MSG.buf[POS_MSG_FN], fname);
            DownloadRequest(cnum, fname, SDLNet_Read16((Uint8*)&MSG.buf[POS_MSG_FPORT]));
            ConOutEx(SERVER_FONT, "[%s] requested download: %s", aclient->name, fname);
          }
        break;
        
      case MSGID_UPLOAD:
        if (!s_uploading.value)
        {
          ConOutEx(SERVER_FONT, "Server: received upload request, but uploading is not enabled, set \"s_uploading\" variable to 1");
        }
        else
          if (!CheckFileName((char*)&MSG.buf[POS_MSG_FN]))
          {
            ConErr("%d>Server: invalid upload request %s (packet ID=%d from %s) !\n", server_info.ticks, (char*)&MSG.buf[POS_MSG_FN], msg_spackets[n]->data[POS_MSG_ID], AddrToS(&msg_spackets[n]->address));
          }
          else
          {
            DLIDtoFileName(MSG.buf[POS_MSG_DLID], (char*)&MSG.buf[POS_MSG_FN], fname);
            UploadRequest(cnum, fname, SDLNet_Read16((Uint8*)&MSG.buf[POS_MSG_FPORT]));
            ConOutEx(SERVER_FONT, "[%s] requested upload: %s", aclient->name, fname);
          }
        break;
        
      case MSGID_CHMAPREADY: 
        // now send ENTERGAME msg to client and let's play
        ClearPool(&aclient->game_pool);
        MSG2 << MSGID_ENTERGAME;
        MSG2 << (SDL_GetTicks() - server_info.start_time); // time on server
        MSG2.Send(&aclient->msg_pool, &aclient->stats, msg_ssock, cnum, true, RESEND_ENTERINGGAME+1, EnteringGameConfirmed, cnum );
        break;
        
      case MSGID_LOGOUT:
        // clear client from server's data structures
        PurgePlayer(cnum);
        ClearServerClient(cnum);
        
        // send to all players that player has been disconected
        MSG2 << MSGID_CHATWRITE;
        MSG2.nprintf(MSG_MAX_BODY_SIZE - POS_MSG_TEXT - 1, 
          "System: %s disconnected.", 
          aclient->name);
        
        SV_BroadcastMsg(MSG2, msg_ssock, RESEND_CHAT);
        break;

        
      default:
        ConErr("%d>Server: Unresolved message packet ID=%d from %s !\n", server_info.ticks, msg_spackets[n]->data[POS_MSG_ID], AddrToS(&msg_spackets[n]->address));
      }        
    }
    else // we've got unbound packet
    {
      // UNBOUND MESSAGES ARE ALLWAYS WITHOUT MESSAGE HEADER 
      // receiving&sending in raw packet format
      
      switch (msg_spackets[n]->data[POS_MSG_ID]) {
      case MSGID_SERVERINFO:   // assemble server info packet
        {
          // 1b     server info tag
          // 2b     version
          // 1b     max num players
          // 1b     server status
          // 1b     num players
          // 1b     game type
          // string server name (MAX_SERVER_NAME)
          // string map name (MAX_MAP_NAME)
          // string script name (MAX_MAP_NAME)

          // assemble SERVERINFO packet
          UDPpacket packet;
          unsigned char body[500];
          int pos = 0;
          packet.data = body;
          packet.data[pos] = (const Uint8)PW2_RESPONSE_SERVERINFO; pos++;
          packet.data[pos] = PROTOCOL_VERSION_MAJOR; pos++;
          packet.data[pos] = PROTOCOL_VERSION_MINOR; pos++;
          packet.data[pos] = PWP_TOTALMAX_CLIENTS; pos++;
          packet.data[pos] = net_server_status; pos++;
          
          if (net_server_status == NS_RUNNING) 
          {
            // obtain # of players
            char num_players = 0;
	          for (int i = 0; i < GAME_MAX_OBJS; i++) {
		          if ((server_info.game.objs[i]->state&OSTATE_ACTIVE) && server_info.game.objs[i]->GetType() == ot_player) num_players++;
	          }
            packet.data[pos] = num_players; pos++;
            packet.data[pos] = server_info.game.game_type; pos++;
            strcpy((char*)&packet.data[pos], s_name.string); pos+=strlen(s_name.string)+1;
            strcpy((char*)&packet.data[pos], MapName); pos+=strlen(MapName)+1;
            strcpy((char*)&packet.data[pos], ScriptName); pos+=strlen(ScriptName)+1;
          }
          packet.len = pos;
          packet.channel = -1;
          packet.address = msg_spackets[n]->address;
          SDLNet_UDP_Send(msg_ssock, -1, &packet);
        }
        break;
      case MSGID_PLAYERINFO:   // assemble player info packet
        {
          // 1b     player info tag
          // 1b     n = num players

          // n times player structure:
          // signed 1b     frags
          // signed 2b     score
          // unsigned 2b   ping
          // unsigned 1b   team
          // unsigned 2b   connect time
          // string        player name
          // string        skin name

          // assemble PLAYERINFO packet
          UDPpacket packet;
          unsigned char body[1000];
          int pos = 0;
          packet.data = body;
          packet.data[pos] = (const Uint8)PW2_RESPONSE_PLAYERINFO; pos++;
          pos++;
          char num_players = 0;
          if (net_server_status == NS_RUNNING) 
          {
	          for (int i = 0; i < GAME_MAX_OBJS; i++) {
		          if ((server_info.game.objs[i]->state&OSTATE_ACTIVE) && server_info.game.objs[i]->GetType() == ot_player) 
              {
                num_players++;
                GPlayer* p = (GPlayer*)server_info.game.objs[i];

                packet.data[pos] = p->frags; pos++;
                Sint16 score = p->points;
                memcpy(&packet.data[pos], &score, 2); pos+=2;
                Uint16 ping = p->ping; 
                memcpy(&packet.data[pos], &ping, 2); pos+=2;
                packet.data[pos] = 0; pos+=1;  // TODO: teams implementation
                Uint16 time=0;  // TODO: connenction time
                memcpy(&packet.data[pos], &time, 2); pos+=2;

                strcpy((char*)&packet.data[pos], p->player_name.GetValRef()->chars); pos+=strlen(p->player_name.GetValRef()->chars)+1;
                strcpy((char*)&packet.data[pos], p->skin_name.GetValRef()->chars); pos+=strlen(p->skin_name.GetValRef()->chars)+1;
              }
	          }
          }
          packet.data[1] = num_players; 

          packet.len = pos;
          packet.channel = -1;
          packet.address = msg_spackets[n]->address;
          SDLNet_UDP_Send(msg_ssock, -1, &packet);
        }
        break;
        
      // login message 
      case MSGID_LOGIN:

        // magick test
        if (msg_spackets[n]->data[POS_MSG_ID+1]!=230 ||
            msg_spackets[n]->data[POS_MSG_ID+2]!=21 ||
            msg_spackets[n]->data[POS_MSG_ID+3]!=57 ||
            msg_spackets[n]->data[POS_MSG_ID+4]!=127)
        {
            ConErr("%d>Server: bad LOGIN MAGICK\n", server_info.ticks);
            return 0;
        }
          

 
        if (net_server_status==NS_RUNNING)
        {
          // Test multiple connection
          for (i=0; i<server_info.maxclients; i++) {
            if ((client[i].msg_ip.host==msg_spackets[n]->address.host) && (client[i].msg_ip.port==msg_spackets[n]->address.port))
            {
              if (client[i].status==CS_ACTIVE)
              {
                ConErr("%d>Server: multiple LOGIN attemp\n", server_info.ticks);
                return 0;
              }
              if (client[i].status==CS_PREPARING) 
              {
                // player requested connection and was invited before, but 
                // he didn't confirm invitation and now he requests connection again
                // => his WELCOME PACKET was lost
                goto LOSTWELCOME;
              }
              
            }
          }
          
          // Look for unconnected player slot
          for (i=0; i<server_info.maxclients; i++) {
            if (client[i].status==CS_UNUSED) break;
          }
          
          if (i==server_info.maxclients) 
          {
            // Look for inactive (not playing) player slot
            
            // Kick off one of invited players
            for (i=0; i<server_info.maxclients; i++) {
              if (client[i].status==CS_PREPARING) break;
            }
            
            // TODO: Kick off inactive player
            for (i=0; i<server_info.maxclients; i++) {
              if (client[i].status==CS_INACTIVE)
              {
              }
            }
          }
          
LOSTWELCOME:
          new_player=i;
          if ( new_player == server_info.maxclients) {
            // No more room..
            msg_spacket->data[POS_MSG_ID] = MSGID_SERVERFULL;
            msg_spacket->len = POS_MSG_ID+LEN_MSG_ID;
            net_message_pool tmp_pool;
            CreatePool(&tmp_pool, 1, 1, MSG_HEADER_SIZE+msg_spacket->len);
            SDLNet_UDP_Bind(msg_ssock, server_info.maxclients, &msg_spackets[n]->address);
            SendMsg(&tmp_pool, msg_ssock, server_info.maxclients, msg_spacket->data, msg_spacket->len, true, RESEND_SYSTEM);
            SDLNet_UDP_Unbind(msg_ssock, server_info.maxclients);
            DestroyPool(&tmp_pool);
            ConErr("%d>Server: no more room to log on server\n", server_info.ticks);
          } 
          else 
          {
            // prepare client slot
            ClearServerClient(new_player);
            memcpy(client[new_player].name, (const void*)&msg_spackets[n]->data[POS_MSG_NAME], LEN_MSG_NAME);
            client[new_player].msg_ip = msg_spackets[n]->address;
            client[new_player].game_ip = msg_spackets[n]->address;
            client[new_player].game_ip.port = SDL_SwapBE16(SDL_SwapBE16(client[new_player].msg_ip.port)+1);
            
            ClearPool(&client[new_player].msg_pool);
            ClearPool(&client[new_player].game_pool);
            client[new_player].stats.Clear();
            client[new_player].status = CS_ACTIVE;
            PrepareClientForNewGame(new_player);
            client[new_player].status = CS_PREPARING;
            
            SDLNet_UDP_Bind(game_ssock, new_player, &client[new_player].game_ip);
            SDLNet_UDP_Bind(msg_ssock, new_player, &client[new_player].msg_ip);
            
            // assemble WELCOME packet
            MSG2.pos = 0;
            MSG2 << MSGID_WELCOME;
            MSG2 << new_player;
            MSG2 << PROTOCOL_VERSION_MAJOR << PROTOCOL_VERSION_MINOR;
            MSG2.nprintf(LEN_MSG_WSNAME, "%s", s_name.string);
            MSG2.pos = POS_MSG_WSMSG;
            MSG2.nprintf(LEN_MSG_WSMSG, "%s", s_welcome_msg.string);
            MSG2.pos = POS_MSG_WMAPNAME;
            MSG2.nprintf(LEN_MSG_WMAPNAME, "%s", MapName);
            MSG2.pos = POS_MSG_WSCRIPTNAME;
            MSG2.nprintf(LEN_MSG_WSCRIPTNAME, "%s", ScriptName);
            MSG2.Send(&client[new_player].msg_pool, &client[new_player].stats, msg_ssock, new_player, true, RESEND_SYSTEM, InvitationConfirmed, new_player);
          }
        }
        else // net_server_status==SC_INITED
        {
          // there is no running game on the server
          msg_spacket->data[POS_MSG_ID] = MSGID_SERVEROFF;
          msg_spacket->len = POS_MSG_ID+LEN_MSG_ID;
          net_message_pool tmp_pool;
          CreatePool(&tmp_pool, 1, 1, MSG_HEADER_SIZE+msg_spacket->len);
          SDLNet_UDP_Bind(msg_ssock, server_info.maxclients, &msg_spackets[n]->address);
          SendMsg(&tmp_pool, msg_ssock, server_info.maxclients, msg_spacket->data, msg_spacket->len, true, RESEND_SYSTEM);
          SDLNet_UDP_Unbind(msg_ssock, server_info.maxclients);
          DestroyPool(&tmp_pool);
          ConErr("%d>Server: attempt to connection when server is not running\n", server_info.ticks);
        }
        break;
      }
    }
  }
  
  return 0;
}


int SV_SetName(char* name)
{
  Uint8 sendbuf[MSG_MAX_BODY_SIZE];
  net_msg MSG(sendbuf);
  
  MSG << MSGID_SNAME;
  MSG.nprintf(LEN_MSG_SNAME, "%s", name);
  SV_BroadcastMsg(MSG, msg_ssock, RESEND_SYSTEM);
  
  // set name in server_info
  snprintf(server_info.name, LEN_MSG_SNAME, "%s", name);
  return 0;
}


int SV_SetMap(char* mapname, char* scriptname)
{
  // dispose old game info
  server_info.game.Clear();
  server_info.game.Reset();
  server_info.game.state = GS_SERVER;  
  // load & set new map
  // TODO: check map errors
  ScriptMan->scripts[map_script_num].verbose = true;
  MapLoad(mapname, scriptname);
  if (MapLoaded==0)
  {
    ConOutEx(SERVER_FONT, "Server: Map %s loading error !", mapname);
    SV_Shutdown(SD_SHUTDOWN);
    return 1;
  }
  ScriptMan->scripts[map_script_num].verbose = false;
  server_info.game.state = GS_SERVER;
  server_info.game.map.Copy(&MapLoadedState);
  server_info.game.starttick = SDL_GetTicks();
  // init game
  server_info.game.vars.camx = 0;
  server_info.game.vars.camy = 0;
  server_info.game.map.InitAnims();
  server_info.game.map.GenerateYLookup();
  
  for (int i=0; i<server_info.maxclients; i++) PrepareClientForNewGame(i);
  
  ConOutEx(SERVER_FONT, "Server: Map changed to %s with script %s", mapname, scriptname);
  strcpy(ScriptName, scriptname);
  
  // BETTER: flush received packets from old game 
  // (should be auto done by CS_PREPARING status of clients)
  // check this !!!
  
  Uint8 sendbuf[MSG_MAX_BODY_SIZE];
  net_msg MSG(sendbuf);
  
  MSG << MSGID_CHMAP;
  MSG.nprintf(LEN_MSG_MAPNAME, "%s", mapname);
  MSG.pos = POS_MSG_SCRIPTNAME;
  MSG.nprintf(LEN_MSG_MAPNAME, "%s", scriptname);
  SV_BroadcastMsg(MSG, msg_ssock, RESEND_SYSTEM);
  
  server_info.active = true;
  server_info.ticks = 0;
  server_info.pticks = PWP_TICKS_PER_PACKET_SERVER;
  server_info.start_time = SDL_GetTicks();
  server_info.last_time = 0;
  ResetGameSettings();
  ScriptMan->RunScript(map_script_num, "ssInit");
  
  return 0;
}


int SV_Poll(Uint32 timeout)
{
  int count;
  int transfered;
  
  if (net_server_status!=NS_UNINITED)
  {
    SDLNet_CheckSockets(ssocket_set, timeout);
    if (SDLNet_SocketReady(msg_ssock)) {
      
      HandleServerMessage();
    }
    
    for (int i=0; i<server_info.maxclients; i++) {
      if (client[i].status!=CS_UNUSED)
      {
        // troggle active/inactive or drop "dead" client
        // remake this
        Uint32 time = SDL_GetTicks();
        if (client[i].status==CS_ACTIVE)        
        {
          if ((time - client[i].stats.LastRecTime)>=(Uint32)net_inactive_timeout.value)
          {
            client[i].status=CS_INACTIVE;
          }
        }
        else
          if (client[i].status==CS_INACTIVE)        
          {
            if ((time - client[i].stats.LastRecTime)>=(Uint32)net_drop_timeout.value)
            {
              SV_DropClient(i);
              continue;
            }
            if ((time - client[i].stats.LastRecTime)<=(Uint32)net_inactive_timeout.value)
            {
              client[i].status=CS_ACTIVE;
            }
          }
            
            // resend lost messages
            if (client[i].status==CS_ACTIVE) ResendMsgs(&client[i].msg_pool, msg_ssock);
          
          // manage file transfers
          if ((client[i].file_status==FS_SENDING) || (client[i].file_status==FS_RECEIVING)) 
          {
            if (client[i].file_status==FS_SENDING)
            { // SENDING
              count = LoadFileBuffer(&client[i].zs, client[i].quantum, client[i].qsize, 
                client[i].buffer, client[i].bsize, client[i].file, 
                &client[i].file_status);
              if (count)
              {
                transfered = SDLNet_TCP_Send(client[i].file_sock, (char*)client[i].quantum, count);
                if (transfered<count)
                {
                  ConOutEx(SERVER_FONT, "File sending - connection lost (server)");
                  client[i].file_status = FS_ENDED;
                }
                server_sent+=transfered;
              }
              // test for transfer result
              if ((client[i].file_status==FS_ENDED) || (client[i].file_status==FS_COMPLETED))
              {
                if (client[i].file_status==FS_COMPLETED)
                  ConOutEx(SERVER_FONT, "Server: File transfer done (%d/%d)", client[i].zs.total_out, client[i].zs.total_in);
                
                // clean up
                deflateEnd(&client[i].zs);
                SV_ClearFileTransfer(i, FT_FINISHED);
              }
            }
            else 
            { // RECEIVING
              if (SDLNet_SocketReady(client[i].file_sock))
              {
                count = SDLNet_TCP_Recv(client[i].file_sock, (char*)client[i].quantum, client[i].qsize);
                if (count<=0)
                {
                  ConOutEx(SERVER_FONT, "File receiving - connection lost (client)");
                  client[i].file_status = FS_ENDED;
                }
                else
                {
                  StoreFileBuffer(&client[i].zs, client[i].quantum, count, 
                    client[i].buffer, client[i].bsize, client[i].file, 
                    &client[i].file_status);
                  server_received+=count;
                  client[i].lrec_time = SDL_GetTicks();
                }
              }
              // check for timeouts
              if ((SDL_GetTicks() - client[i].lrec_time) >= net_file_timeout.value)
              {
                ConOutEx(SERVER_FONT, "Server: File transfer timed out (client %d)!", i);
                SV_ClearFileTransfer(i, FT_ABANDONED);
              }
              
              // test for transfer result
              if ((client[i].file_status==FS_ENDED) || (client[i].file_status==FS_COMPLETED))
              {
                if (client[i].file_status==FS_COMPLETED)
                  ConOutEx(SERVER_FONT, "Server: File transfer done (%d/%d)", client[i].zs.total_in, client[i].zs.total_out);
                
                // clean up
                inflateEnd(&client[i].zs);
                SV_ClearFileTransfer(i, FT_FINISHED_RENAME);
              }
            }
          }
      }
    }  
  }
  
  return 0;
}

// type: 0 = ConOut
//       1 = ConPar
//       2 = Con
void SV_RemoteViewer(Uint8 type, char* text)
{
  Uint8 sendbuf[MSG_MAX_BODY_SIZE];
  net_msg MSG(sendbuf);
  for (int i=0; i<server_info.maxclients; i++) {
    if ((client[i].status==CS_ACTIVE) && (client[i].con_attach))
    {
      MSG.pos = 0;
      MSG << MSGID_REMOTECON;
      MSG << type;
      MSG.nprintf(LEN_MSG_CON, "%s", text);
      MSG.Send(&client[i].msg_pool, &client[i].stats, msg_ssock, i, true, RESEND_SYSTEM);
    }
  }
}

int SV_DropClient(int cnum)
{
  // inform other clients
  SV_BroadcastPrintf("System: %s was kicked.", client[cnum].name);
  
  // send drop message to client (do not wait for confirmation)
  Uint8 sendbuf[LEN_MSG_ID];
  net_msg MSG(sendbuf);
  MSG << MSGID_YWDROPPED;
  MSG.Send(&client[cnum].msg_pool, &client[cnum].stats, msg_ssock, cnum, true, RESEND_SYSTEM);
  
  // clear client from server's data structures
  PurgePlayer(cnum);
  ClearServerClient(cnum);
  return 0;
}

char SV_ParseReplication(net_msg * msg, TICK_TYPE time, int cnum)
{
  Uint8 rep_code = REP_END+1;
  GAME_MAXOBJS_TYPE onum;
  GObj * obj;
  GPlayer * player;
  MoveVector mv;
  Uint16 oid;

  while (rep_code!=REP_END) {
    (*msg)>>rep_code;
    switch (rep_code) {
    case REP_SERVERMOVE: 
      (*msg)>>onum>>oid;
      obj = server_info.game.objs[onum];
      if (obj->oid!=oid) break;  // move was made for old player

      if (obj->GetType()!=ot_player) 
        ConErr("%d>Server: error SV_ParseReplication SERVERMOVE for non-player object", server_info.ticks);
      else
      {
        player = (GPlayer*)obj;
        mv = player->DecodeMV(msg, time);
        // do move in global game state
        //if (rand()%100==0) { ConOutEx(SERVER_FONT, "lost"); continue;}   // lost simulation
        player->ServerMove(&mv);
      }
      
      break;
    case REP_END: 
      break;
      
    case REP_REPLICATION: 
      ConErr("%d>Server: error SV_ParseReplication forbiden rep_code=%d\n", server_info.ticks, rep_code);
      break;
      
    default: 
      ConErr("%d>Server: error SV_ParseReplication unknown rep_code=%d\n", server_info.ticks, rep_code);
    }
  }

  return 1;
}

int HandleServerGame(Uint32 deltaticks)
{
  int n;
  int len;
  int cnum;
  
  net_msg GMP;         
  net_client* aclient;
  
  TICK_TYPE incoming_tick;

  n = SDLNet_UDP_RecvV(game_ssock, game_spackets);
  while ( n-- > 0 ) 
  {
    cnum = game_spackets[n]->channel; // client num
    if ( cnum >= 0 ) 
    {
      if ((cnum>=server_info.maxclients) || (client[cnum].status==CS_UNUSED))
      {
        ConErr("%d>Server: received bad client game_packet ID=%d from %s !\n", server_info.ticks, game_spackets[n]->data[POS_MSG_ID], AddrToS(&game_spackets[n]->address));
        continue;
      }
      aclient = &client[cnum];
      
      len = GMP.Receive(&aclient->game_pool, &aclient->stats, game_ssock, cnum, game_spackets[n], false);
      if (len<=0) continue;  // bad or confirming packet
      
      // now, GMP contains valid received GMP.buf
      // let's parse client's packet
      GMP>>incoming_tick;
      // momentalne zahazuju zpravy "z budoucnosti" - teorie relativity tady selhava ;))
      //if (server_info.ticks>=incoming_tick)
      {
        if (!SV_ParseReplication(&GMP, incoming_tick, cnum))
        {
          ConErr("%d>Server: error parsing client replication - incomingtick=%d !\n", server_info.ticks, incoming_tick);
          continue;
        }
      }
    }
  }
  
 	for (GAME_MAXOBJS_TYPE i = 0; i < GAME_MAX_OBJS; i++) 
  {
		if (server_info.game.objs[i]->GetType()==ot_player)
    {
      GPlayer* player = (GPlayer*)server_info.game.objs[i];
      if (player->brain_owner!=250 && (player->xpos.IsDirty(cnum) || player->ypos.IsDirty(cnum)))
      {
        client[cnum].replicator.SetLayer(1);
        // adjust position
        client[player->brain_owner].replicator<<REP_ADJUSTPOSITION<<player->slot<<player->xpos<<player->ypos<<server_info.ticks<<player->oid;
        client[player->brain_owner].replicator.Mark();
        player->xpos.MakeDirty(player->brain_owner, false);
        player->ypos.MakeDirty(player->brain_owner, false);
        //temp ConOut("SS - %d:[%d,%d]", server_info.ticks, *player->xpos.GetValRef(), *player->ypos.GetValRef());
      }

    }
	}

  return 0;
}


void SV_Move(Uint32 ticktime)
{
  int i;
  Uint32 delta = ticktime - server_info.start_time - server_info.last_time;
  Uint8 buffer[MSG_MAX_BODY_SIZE];
  net_msg GP(buffer);

  // ping all clients if possible
  for (i=0; i<server_info.maxclients; i++) 
    if (client[i].status!=CS_UNUSED) 
      if (client[i].pingreturned && ticktime-client[i].pingsent>=PING_MEASURE)
        SV_Ping(i);
  
  // ted posunu hru o delta time (v ms)
  // jeden herni tik je dlouhy FRAMETICK ms
  // jeden posilaci tik nastane za PWP_TICKS_PER_PACKET_SERVER hernich tiku
  while (delta>FRAMETICKS)
  {
    // zpracuj prichozi replikace od klientu
    // server_info.ticks a mensi
    SDLNet_CheckSockets(ssocket_set, 0);
    if (SDLNet_SocketReady(game_ssock)) {
      HandleServerGame(delta/FRAMETICKS);
    }
    // resend lost game packets
    for (i=0; i<server_info.maxclients; i++) {
      if (client[i].status==CS_ACTIVE) 
        ResendMsgs(&client[i].game_pool, game_ssock);
    }
  
    server_info.ticks++;
    
    // updatuj animace - nemaji vliv na stav hry
    server_info.game.map.UpdateAnims();
    
    // simuluj objekty
    for (int j=0; j<THINKINGS_PER_TICK; j++)
    {
      server_info.game.ServerThink(server_info.ticks);            
      ScriptMan->RunScript(map_script_num, "ssTick");   // let script do gametick stuff
    }
    server_info.game.GameThink(server_info.ticks);

    delta-=FRAMETICKS;
    
    server_info.pticks--;
    if (server_info.pticks==0)
    {
      server_info.pticks = PWP_TICKS_PER_PACKET_SERVER;
      
      // do replication process
      for (i=0; i<server_info.maxclients; i++) 
      {
        if (client[i].status==CS_ACTIVE) 
        {
          //client[i].replicator.Reset();
          //server_info.game.ServerAdjustPositions(i, client[i].replicator, server_info.ticks);
          // replicate client's data according to server game state
          server_info.game.ServerReplicate(i, client[i].replicator, server_info.ticks);
          
          if (client[i].replicator.dirty) // does client need update ?
          {
            // send game packets to client
            Uint8 more = 1;

            while (more)
            {
              GP.pos = 0;
              GP << server_info.ticks;
              more = client[i].replicator.AssembleMsg(&GP, MSG_MAX_BODY_SIZE-sizeof(server_info.ticks));
            
              GP.Send(&client[i].game_pool, &client[i].stats, game_ssock, i, false, RESEND_GAME_SERVER);
            }
            
            // reset replicator for next sending tick
            client[i].replicator.Reset();
          }
        }
      }
    }
  }
  
  server_info.last_time = ticktime - server_info.start_time - delta;
}

void SV_CreatePlayer(net_msg * MSG, int owner)
{
  GPlayer * p;
  CGame & g = server_info.game;
  GAME_MAXOBJS_TYPE i=0;

  for (i=0; i<GAME_MAX_OBJS; i++)
  {
    if ((g.objs[i]->state&OSTATE_ACTIVE) && (g.objs[i]->GetType()==ot_player))
    {
      p = (GPlayer*)g.objs[i]; 
      if (strcmp(p->player_name.GetValRef()->chars,(const char *)&MSG->buf[POS_MSG_PLNAME])==0) break;
    }
  }
  
  if (i<GAME_MAX_OBJS)
  {
    SV_ClientPrintf(owner, "Server: Player with name \"%s\" already exists in game", p->player_name.GetValRef()->chars);
    return;
  }

  
  server_info.game.IncrementLOID();
  GAME_MAXOBJS_TYPE slot = server_info.game.FindUnusedSlot();
  server_info.game.RebornObject(server_info.game.last_oid, slot, ot_player);

  p = (GPlayer*)server_info.game.objs[slot];

  Uint8 dump;
  Uint8 type;
  Uint8 index;
  (*MSG)>>dump>>type>>index;
//  p->role = ROLE_AutonomousProxy;
  p->kbindex = index;
  p->brain_type = type;
  p->brain_owner = owner;
  memcpy(p->player_name.GetValRef()->chars, &MSG->buf[POS_MSG_PLNAME], LEN_MSG_PLNAME);
  p->player_name.GetValRef()->len = strlen((const char*)&MSG->buf[POS_MSG_PLNAME])+1;
  p->player_name.MakeDirty(-1, true);
//  p->frags=0;
//  p->becher_ammo=20;
  p->Born();

  SV_BroadcastPrintf("System: Player \"%s\" has entered the game", p->player_name.GetValRef()->chars);
}

void SV_DestroyPlayer(net_msg * MSG, int owner)
{
  GPlayer * p;
  CGame & g = server_info.game;
  GAME_MAXOBJS_TYPE i=0;

  for (i=0; i<GAME_MAX_OBJS; i++)
  {
    if ((g.objs[i]->state&OSTATE_ACTIVE) && (g.objs[i]->GetType()==ot_player))
    {
      p = (GPlayer*)g.objs[i]; 
      if (strcmp(p->player_name.GetValRef()->chars,(const char *)&MSG->buf[POS_MSG_DESTROY_PLNAME])==0) break;
    }
  }
  
  if (i<GAME_MAX_OBJS)
  {
    // TODO: acces rights checks
    if (owner == p->brain_owner)
    {
      SV_BroadcastPrintf("System: Player \"%s\" has left the game", p->player_name.GetValRef()->chars);
      p->state = 0;

      // notify client for freeing keybindings
      Uint8 sendbuf[MSG_MAX_BODY_SIZE];
      net_msg MSG2(sendbuf);
      MSG2.pos = 0;
      MSG2 << MSGID_KBNOTIFY;
      MSG2 << p->kbindex;
      MSG2.Send(&client[owner].msg_pool, &client[owner].stats, msg_ssock, owner, true, RESEND_SYSTEM);
    }
    else
    {
      ConErr("%d>Server: client ID=%d tried to destroy not owned player \"%s\"!\n", server_info.ticks, owner, (const char *)&MSG->buf[POS_MSG_DESTROY_PLNAME]);
      SV_ClientPrintf(owner, "Server: you do not own player \"%s\"", p->player_name.GetValRef()->chars);
    }
  }
  else
  {
    ConErr("%d>Server: DestroyPlayer - tried to destroy non-exist player \"%s\"\n", server_info.ticks, (const char *)&MSG->buf[POS_MSG_DESTROY_PLNAME]);
    SV_ClientPrintf(owner, "Server: no such a player \"%s\"", &MSG->buf[POS_MSG_DESTROY_PLNAME]);
  }
}

void SV_SkinPlayer(net_msg * MSG, int owner)
{
  GPlayer * p;
  CGame & g = server_info.game;
  char * spritename = (char *)&MSG->buf[POS_MSG_SKIN_SPRITENAME];
  GAME_MAXOBJS_TYPE i = 0;

  for (i=0; i<GAME_MAX_OBJS; i++)
  {
    if ((g.objs[i]->state&OSTATE_ACTIVE) && (g.objs[i]->GetType()==ot_player))
    {
      p = (GPlayer*)g.objs[i]; 
      if (strcmp(p->player_name.GetValRef()->chars,(const char *)&MSG->buf[POS_MSG_SKIN_PLNAME])==0) break;
    }
  }
  
  if (i<GAME_MAX_OBJS)
  {
    // TODO: acces rights checks
    if (owner == p->brain_owner)
    {
      if (p->SetSkin(spritename))
      {
        SV_BroadcastPrintf("System: Player \"%s\" has changed skin to %s", p->player_name.GetValRef()->chars, spritename);
      }
    }
    else
    {
      ConErr("%d>Server: client ID=%d tried to skin not owned player \"%s\"!\n", server_info.ticks, owner, (const char *)&MSG->buf[POS_MSG_SKIN_PLNAME]);
      SV_ClientPrintf(owner, "Server: you do not own player \"%s\"", p->player_name.GetValRef()->chars);
    }
  }
  else
  {
    ConErr("%d>Server: SkinPlayer - tried to skin non-exist player \"%s\"\n", server_info.ticks, (const char *)&MSG->buf[POS_MSG_SKIN_PLNAME]);
    SV_ClientPrintf(owner, "Server: no such a player \"%s\"", &MSG->buf[POS_MSG_SKIN_PLNAME]);
  }
}

void SV_ChasePlayer(net_msg* MSG, int owner)
{
  GAME_MAXOBJS_TYPE i;
  Uint8 drop;
  *MSG>>drop>>i;
  if ((server_info.game.objs[i]->state&OSTATE_ACTIVE) && (server_info.game.objs[i]->GetType()==ot_player))
  {
    server_info.game.vars.chasing = i;
    Uint8 sendbuf[MSG_MAX_BODY_SIZE];
    net_msg MSG2(sendbuf);
    MSG2.pos = 0;
    MSG2 << MSGID_CHASEPLAYERCF;
    MSG2 << i;
    MSG2.Send(&client[owner].msg_pool, &client[owner].stats, msg_ssock, owner, true, RESEND_GAME);
  }
  else
  {
    ConErr("Chasing player: something went wrong ");
    server_info.game.vars.chasing = -1;
  }
}

void SV_PingReturn(net_message *msg)
{
  if (client[msg->tag].status!=CS_UNUSED)
  {
    client[msg->tag].ping = SDL_GetTicks() - client[msg->tag].pingsent;
    if (client[msg->tag].ping>999) client[msg->tag].ping = 999;
    client[msg->tag].pingreturned = true;
  }
}

// sends ping server-client[cnum]-server
// pong is processed by callback function
void SV_Ping(int cnum)
{
  if (client[cnum].pingreturned)
  {
    Uint8 buf[LEN_MSG_ID];
    net_msg MSG(buf);
    
    MSG << MSGID_PING;
    MSG.Send(&client[cnum].msg_pool, &client[cnum].stats, msg_ssock, cnum, true, RESEND_PING, SV_PingReturn, cnum);
    client[cnum].pingsent = SDL_GetTicks();
    client[cnum].pingreturned = false;
  }
  else ping=0;
}
