/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   top network engine layer
//## author:  Antonin Hildebrand
//##         
//## started: 13.3.2000
//## revised: ???
//## 
//##          Based on low level multiplatform SDL_net mod for SDL lib
//## 
//###########################################################################

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL.h"
#include "SDL_image.h"
#include "zlib.h"
#include "const.h"
#include "SDL_Console.h"
#include "SDL_net.h"
#include "net.h"
#include "server.h"
#include "client.h"

//###########################################################################
//## Globals
//###########################################################################

int net_status;

static char ipnamebuf[128];

UDPpacket *confirm_packet;

// cvars
cvar_t	net_inactive_timeout = {"inactive_timeout",STD_INACTIVE_TIMEOUT,true,true};
cvar_t	net_drop_timeout = {"drop_timeout",STD_DROP_TIMEOUT,true,true};
cvar_t	net_file_timeout = {"file_timeout",STD_FILE_TIMEOUT,true,true};
cvar_t	net_ping_timeout = {"ping_timeout",STD_PING_TIMEOUT,true,true};

//###########################################################################
//## General func
//###########################################################################

// CRC funkce ze zlibu
Uint16 CRC(void* buffer, int length)
{
  uLong adler = adler32(0L, Z_NULL, 0);
  return (Uint16)(adler32(adler, (const unsigned char*)buffer, length)&0xFFFF);
}

bool CheckFileName(char* fname)
{
  for (unsigned int i=0; i<strlen(fname); i++)
  {
    if (fname[i]=='/' || fname[i]=='\\')
      return false;
  }
  return true;
}

bool DLIDtoFileName(Uint8 DLID, char* fname, char* full_name)
{
  switch (DLID) {
  case DLID_MAP:
    sprintf(full_name, "%s%s", map_dir.string, fname);
    break;
  case DLID_SKIN:
    sprintf(full_name, "%s%s", skin_dir.string, fname);
    break;
  case DLID_SCRIPT:
    sprintf(full_name, "%s%s", script_dir.string, fname);
    break;
  case DLID_SPRITE:
    sprintf(full_name, "%s%s", sprite_dir.string, fname);
    break;
  case DLID_SOUND:
    sprintf(full_name, "%s%s", snd_dir.string, fname);
    break;
  default:
    return false;
  }
  return true;
}

//###########################################################################
//## NET STATS
//###########################################################################

void net_stats::Clear()
{
  errCRC = 0;
  errDuplicity = 0;
  errDropped = 0;

  PacketsSent = 0;
  PacketsReceived = 0;

  LastRecTime = SDL_GetTicks();
}


//###########################################################################
//## MSG functions
//###########################################################################

net_msg::net_msg()
{
  pos = 0;
  buf = NULL;
}

net_msg::net_msg(Uint16 ipos)
{
  pos = ipos;
  buf = NULL;
}

net_msg::net_msg(Uint8* ibuf)
{
  pos = 0;
  buf = ibuf;
}

net_msg::net_msg(Uint16 ipos, Uint8* ibuf)
{
  pos = ipos;
  buf = ibuf;
}

net_msg::~net_msg()
{
}

void net_msg::W8(Uint8 val)
{
  buf[pos] = val;
  pos++;
#ifdef PARANOID
  if (pos>MSG_MAX_BODY_SIZE) fprintf(stderr, "MSG: Write8 overflow");
#endif
}

void net_msg::W16(Uint16 val)
{
  SDLNet_Write16(val, buf+pos);
  pos+=2;
#ifdef PARANOID
  if (pos>MSG_MAX_BODY_SIZE) fprintf(stderr, "MSG: Write16 overflow");
#endif
}

void net_msg::W32(Uint32 val)
{
  SDLNet_Write32(val, buf+pos);
  pos+=4;
#ifdef PARANOID
  if (pos>MSG_MAX_BODY_SIZE) fprintf(stderr, "MSG: Write32 overflow");
#endif
}

void net_msg::WStr(char* s)
{
  memcpy(buf+pos, s, strlen(s)+1);
  pos+=strlen(s)+1;
#ifdef PARANOID
  if (pos>MSG_MAX_BODY_SIZE) fprintf(stderr, "MSG: WriteString overflow");
#endif
}

void net_msg::WMem(void* src, int len)
{
  memcpy(buf+pos, src, len);
  pos+=len;
#ifdef PARANOID
  if (pos>MSG_MAX_BODY_SIZE) fprintf(stderr, "MSG: WriteMem overflow");
#endif
}

Uint8  net_msg::R8(void)
{
  Uint8 r = buf[pos];
  pos++;
#ifdef PARANOID
  if (pos>MSG_MAX_BODY_SIZE) fprintf(stderr, "MSG: Read8 overflow");
#endif
  return r;
}

Uint16 net_msg::R16(void)
{
  Uint16 r = SDLNet_Read16(buf+pos);
  pos+=2;
#ifdef PARANOID
  if (pos>MSG_MAX_BODY_SIZE) fprintf(stderr, "MSG: Read16 overflow");
#endif
  return r;
}

Uint32 net_msg::R32(void)
{
  Uint32 r = SDLNet_Read32(buf+pos);
  pos+=4;
#ifdef PARANOID
  if (pos>MSG_MAX_BODY_SIZE) fprintf(stderr, "MSG: Read32 overflow");
#endif
  return r;
}

char*  net_msg::RStr(void)
{
  char* r = (char*)(buf+pos);
  pos+=strlen(r)+1;
#ifdef PARANOID
  if (pos>MSG_MAX_BODY_SIZE) fprintf(stderr, "MSG: ReadString overflow");
#endif
  return r;
}

void   net_msg::RMem(void* dest, int len)
{
  memcpy(dest, buf+pos, len);
  pos+=len;
#ifdef PARANOID
  if (pos>MSG_MAX_BODY_SIZE) fprintf(stderr, "MSG: ReadMem overflow");
#endif
}

net_msg& net_msg::operator<<(net_msg& right)
{
  memcpy(buf+pos, right.buf, right.pos);
  pos+=right.pos;
#ifdef PARANOID
  if (pos>MSG_MAX_BODY_SIZE) fprintf(stderr, "MSG: msg copy overflow");
#endif
  return *this;
}

net_msg& net_msg::operator<<(Uint8 val)
{
  W8(val);
  return *this;
}

net_msg& net_msg::operator<<(Uint16 val)
{
  W16(val);
  return *this;
}

net_msg& net_msg::operator<<(Uint32 val)
{
  W32(val);
  return *this;
}
net_msg& net_msg::operator<<(Sint8 val)
{
  W8((Uint8)val);
  return *this;
}

net_msg& net_msg::operator<<(Sint16 val)
{
  W16((Uint16)val);
  return *this;
}

net_msg& net_msg::operator<<(Sint32 val)
{
  W32((Uint32)val);
  return *this;
}

net_msg& net_msg::operator<<(char * str)
{
  WStr(str);
  return *this;
}

net_msg& net_msg::operator<<(PString ps)
{
  W8(ps.len);
  WMem(ps.chars, ps.len);
  return *this;
}

net_msg& net_msg::operator>>(net_msg& right)
{
  memcpy(right.buf+right.pos, buf, pos);
  right.pos+=pos;
#ifdef PARANOID
  if (right.pos>MSG_MAX_BODY_SIZE) fprintf(stderr, "MSG: msg copy overflow");
#endif
  return *this;
}

net_msg& net_msg::operator>>(Uint8& val)
{
  val = R8();
  return *this;
}

net_msg& net_msg::operator>>(Uint16& val)
{
  val = R16();
  return *this;
}

net_msg& net_msg::operator>>(Uint32& val)
{
  val = R32();
  return *this;
}

net_msg& net_msg::operator>>(Sint8& val)
{
  val = (Sint8)R8();
  return *this;
}

net_msg& net_msg::operator>>(Sint16& val)
{
  val = (Sint16)R16();
  return *this;
}

net_msg& net_msg::operator>>(Sint32& val)
{
  val = (Sint32)R32();
  return *this;
}

net_msg& net_msg::operator>>(char*& str)
{
  str = RStr();
  return *this;
}

net_msg& net_msg::operator>>(PString& ps)
{
  ps.len = R8();
  RMem(ps.chars, ps.len);
  ps.chars[ps.len]=0; 
  return *this;
}

void net_msg::nprintf(size_t count, const char *fmt, ... )
{
	va_list		argptr;
	va_start (argptr,fmt);
	int written=vsnprintf ((char*)buf+pos, count, fmt, argptr);
	va_end (argptr);
  pos+=(written+1);
}

int net_msg::Receive(net_message_pool *pool, net_stats* stats, UDPsocket sock, int channel, UDPpacket *packet, bool confirm)
{
  int len = ReceiveMsg(pool, sock, channel, packet, confirm);
  buf = (Uint8*)&packet->data[MSG_HEADER_SIZE];
  pos = 0;
  // stats
  stats->LastRecTime = SDL_GetTicks();
  stats->PacketsReceived++;
  switch (len) {
    case -1:
      stats->errCRC++;
      break;
    case -2:
      stats->errDuplicity++;
      break;
    case -4:
      stats->errDropped++;
      break;
    case 0:
      break;
  }  
  
  return len;
}

int net_msg::Send(net_message_pool *pool, net_stats* stats, UDPsocket sock, int channel,
           bool reliable, Uint32 resend, net_msg_callback *msgcb, Uint16 tag)
{
  // stats
  stats->PacketsSent++;
  return SendMsg(pool, sock, channel, buf, pos, reliable, resend, msgcb, tag);
}



//###########################################################################
//## Message pool
//###########################################################################
//
//
//  Posilani zprav pres UDP
//
//  Zpravy jsou urceny pro nesynchronizovanou komunikaci mezi klientem a serverem
//  tento system obstarava zaruceny prenos zprav pomoci potvrzeni prijeti
//  (jedinou vyjimkou je prihlaseni (LOGIN packet))
//  
//  negarantuje se poradi prislych zprav !!!
//  urceno pro nastavovani udaju, synchronizaci prenosu souboru a paketu hry
//  urceno pro CHAT a dalkovou spravu konzole
//  urceno pro jednorazove zpravy, ktere nejsou posilany prilis casto

//  takze: musis mit datovou strukturu net_message_pool, kterou predavas funkcim ...Message
//  "bazenek zprav" obsahuje seznam nedavno poslanych zprav a nedavno prijatych zprav, take se stara o cislovani zprav
//  jde o jednoducha pole: rmessages jsou cisla naposledy prislych zprav (funguje jako ring, aktualni pozice je rpos)
//                         smessages jsou net_messages, ktere si pamatuji naposledy poslane pakety a jsou schopny z nich paket opet odeslat


//  po poslani zpravy:
//   je urcen jeji cas a je umistena do bazenku, kde ceka na potvrzeni prijeti
//    je mozno definovat callback funkci, ktera bude zavolana po prijeti potvrzujici zpravy + moznost zadat znacku


//  po prijmuti zpravy
//   pokud je to potvrzujici zprava, (cislo zpravy==0)
//     tak je to zaneseno do bazenku zprav cekajicich na potvrzeni
//   pokud to neni potvrzujici zprava, 
//     provede se test na CRC a duplicitu
//     zaradi se mezi nedavno prisle zpravy
//     posle se potvrzujici zprava o prijeti
//
 
//  pro znovuposlani zprav, ktere uz dlouho nebyly potvrzeny 
//  se vola ResendMessages

// sends message packet with data of length len
int SendMsg(net_message_pool *pool, UDPsocket sock, int channel, void *data, int len, 
            bool reliable, Uint32 resend, net_msg_callback *msgcb, Uint16 tag)
{
	int i;
  net_message *msg;
  net_message *oldest_msg;
  Uint32 oldest_time = ~0;
  
  pool->msg_num++;
 
  // find unused message slot
  msg = pool->smessages;
  for (i=0; i<pool->ssize; i++)
  {
    if (msg->state==PS_UNUSED) break;
    if (msg->sended<oldest_time)
    {
      oldest_msg = msg;
      oldest_time = msg->sended;
    }
    msg++;
  }
  
  // if there is no room, kick off oldest message
  if (i==pool->ssize)
  {
    msg = oldest_msg;
    fprintf(stderr, "Net: no room for sent message, dropping oldest message\n");
  }
  
  msg->state = PS_USED;
  msg->id = pool->msg_num;
  
  msg->msgcb = msgcb;
  msg->tag = tag;
  msg->times = 0;

  // fill packet header
  SDLNet_Write16(CRC(data, len), PO(msg->packet, POS_CRC)); 
  SDLNet_Write16(msg->id, PO(msg->packet, POS_NUM)); 
  
  // random error
//  if (rand()%100==0)
//    ((Uint8*)data)[rand()%len] = rand()%256;
  
  // fill message body
  memcpy(PO(msg->packet, MSG_HEADER_SIZE), data, len);
  msg->packet->len = MSG_HEADER_SIZE+len;
  msg->sended = SDL_GetTicks();
  msg->timeout = msg->sended+resend;
  
#ifdef NET_LOGGING
  char tbuf[100];
  for (i=0; i<server_info.maxclients; i++)
  {
    if (pool==&client[i].game_pool)
      sprintf(tbuf,"SERVER: client[%d].game_pool", i);
    if (pool==&client[i].msg_pool)
      sprintf(tbuf,"SERVER: client[%d].msg_pool", i);
  }
  if (pool==&server.game_pool)
    sprintf(tbuf,"CLIENT: server.game_pool");
  if (pool==&server.msg_pool)
    sprintf(tbuf,"CLIENT: server.msg_pool");
  
  fprintf(stderr, "%s: \tSend[%d] \t\t->%d \t\t(len=%d, resend=%d)\n", tbuf, msg->id, channel, len, resend);
#endif
  int retcode = SDLNet_UDP_Send(sock, channel, msg->packet);

  if (!reliable) // if this is not reliable packet, confirm automatically
  {
    msg->state = PS_CONFIRMED;  
    if (msg->msgcb) msg->msgcb(msg);  // call msg callback
    msg->state = PS_UNUSED;  // clear confirmed message in smessages
  }
  
  return retcode;
}

int SendMsg(net_message_pool *pool, UDPsocket sock, int channel, net_msg msg, 
            bool reliable, Uint32 resend, net_msg_callback *msgcb, Uint16 tag)
{
  return SendMsg(pool, sock, channel, msg.buf, msg.pos, reliable, resend, msgcb, tag);
}


// returns (length of message data) if packet is valid message packet
//          0 was confirmation message packet
//         -1 CRC error
//         -2 duplicity error
//         -4 stupid error

// message data starts at &packet->data[MSG_HEADER_SIZE]
int ReceiveMsg(net_message_pool *pool, UDPsocket sock, int channel, UDPpacket *packet, bool confirm)
{
  int msglen = packet->len - MSG_HEADER_SIZE;
  if (msglen<=0) return -4;

  int i;
  Uint16 *recnum;
  net_message *msg;
  
  Uint16 mNUM = SDLNet_Read16(PO(packet, POS_NUM));
  Uint16 mCRC = SDLNet_Read16(PO(packet, POS_CRC));
#ifdef NET_LOGGING
  char tbuf[100];
  for (i=0; i<server_info.maxclients; i++)
  {
    if (pool==&client[i].game_pool)
      sprintf(tbuf,"SERVER: client[%d].game_pool", i);
    if (pool==&client[i].msg_pool)
      sprintf(tbuf,"SERVER: client[%d].msg_pool", i);
  }
  if (pool==&server.game_pool)
    sprintf(tbuf,"CLIENT: server.game_pool");
  if (pool==&server.msg_pool)
    sprintf(tbuf,"CLIENT: server.msg_pool");
#endif
  // check if packet is confirming message
  if (mNUM==0)
  {
#ifdef NET_LOGGING
    fprintf(stderr, "%s: \tConfirmation[%d] \t->%d\n", tbuf, mCRC, channel);
#endif
    msg = pool->smessages;
    for (i=0; i<pool->ssize; i++)
    {
      if ((msg->state==PS_USED) && (msg->id==mCRC))
      {
        msg->state = PS_CONFIRMED;  
        if (msg->msgcb) msg->msgcb(msg);  // call msg callback
        msg->state = PS_UNUSED;  // clear confirmed message in smessages
        return 0;
      }
      msg++;
    }
    fprintf(stderr, "Net: duplicit confirmation packet(%d)\n", mCRC);
    return 0;
  }

  // check CRC
  if (CRC(PO(packet, MSG_HEADER_SIZE), msglen)!=mCRC)
    return -1;
  
  // check duplicity
  recnum = pool->rmessages;
  for (i=0; i<pool->rsize; i++)
  {
    if ((*recnum)==mNUM)
    {
      msglen = -2;
      goto CONFIRM;
    }
    recnum++;
  }
  
  // write received msg number to the "received queue" (=rmessages)
  pool->rpos++;
  if (pool->rpos==pool->rsize) pool->rpos=0;
  pool->rmessages[pool->rpos] = mNUM;

#ifdef NET_LOGGING
  fprintf(stderr, "%s: \tReceive[%d] \t\t<-%d\n", tbuf, mNUM, channel);
#endif
CONFIRM:
  if (confirm)
  {
    // send confirmation packet
    SDLNet_Write16(mNUM, PO(confirm_packet, POS_CRC)); // (CRC=NUM of confirmated packet)
    SDLNet_Write16(0, PO(confirm_packet, POS_NUM));  // 0==NUM (confirmation packet)
    confirm_packet->len = MSG_HEADER_SIZE+1;
    SDLNet_UDP_Send(sock, channel, confirm_packet);
  }

  return msglen;
}

int ResendMsgs(net_message_pool *pool, UDPsocket sock)
{
  Uint32 time = SDL_GetTicks();
  net_message *msg = pool->smessages;
  for (int i=0; i<pool->ssize; i++)
  {
    if ((msg->state==PS_USED) && (msg->timeout<time)) // is time to resend ?
    {
      //beep(1000, 10);
      msg->times++;
      if (msg->times>=PWP_MAX_RESENDS)
      {
        msg->state=PS_UNUSED;
		    fprintf(stderr, "Net: max_resends exceeds - packet(%d) was lost\n", msg->id);
        continue;
      }
      msg->timeout -= msg->sended;
      msg->sended = SDL_GetTicks();
      msg->timeout += msg->sended;
      SDLNet_UDP_Send(sock, msg->packet->channel, msg->packet);
      fprintf(stderr, "Net: resend packet(%d) to %d\n", msg->id, msg->packet->channel);
    }
    msg++;
  }
  return 0;
}

int UnconfirmedMsgs(net_message_pool *pool)
{
  int r = 0;
  net_message *msg = pool->smessages;
  for (int i=0; i<pool->ssize; i++)
  {
    if ((msg->state!=PS_UNUSED)) r++;
    msg++;
  }
  return r;
}


int CreatePool(net_message_pool *pool, int send_size, int rec_size, int packet_size)
{
  pool->msg_num = 1; 
  pool->rsize = rec_size;
  pool->ssize = send_size;
  pool->rmessages = (Uint16*)malloc(rec_size*sizeof(Uint16));  
  if (!pool->rmessages) {
    fprintf(stderr, "Net: Couldn't allocate rmessages: Out of memory\n");
    return 1;
  }
  pool->smessages = (net_message*)malloc(send_size*sizeof(net_message));
  if (!pool->smessages) {
    fprintf(stderr, "Net: Couldn't allocate smessages: Out of memory\n");
    return 1;
  }
  pool->rpos=0;
  for (int i=0; i<pool->ssize; i++)
  {
    pool->smessages[i].packet = SDLNet_AllocPacket(packet_size);
	  if (!pool->smessages[i].packet) {
		  fprintf(stderr, "Net: Couldn't allocate packets (pool): Out of memory\n");
      return 1;
	  }
  }
  
  return ClearPool(pool);
}

int DestroyPool(net_message_pool *pool)
{
  free(pool->rmessages);
  for (int i=0; i<pool->ssize; i++)
    SDLNet_FreePacket(pool->smessages[i].packet);
  free(pool->smessages);
  return 0;
}

int ClearPool(net_message_pool *pool)
{
  memset(pool->rmessages, 0, pool->rsize*sizeof(Uint16));
  for (int i=0; i<pool->ssize; i++)
    pool->smessages[i].state = PS_UNUSED;
  return 0;
}

//###########################################################################
//## File transfer
//###########################################################################

int LoadFileBuffer(z_streamp zs, Bytef* quantum, int qsize, Bytef* buffer, int bsize, FILE* file, int* file_status)
{
  int count;
  int err;
  

  zs->next_out = quantum;
  zs->avail_out = qsize;
  while (zs->avail_out)
  {
     if (zs->avail_in) 
     {
       err = deflate(zs, Z_NO_FLUSH);
       if (err!=Z_OK) 
       {
         ConOut("Net: error deflating stream: %d", err);
         *file_status = FS_ENDED;
         return 0;
       }
     }
     else
     {
       count = fread(buffer, 1, bsize, file);
       if (count==0)
       {
         err = deflate(zs, Z_FINISH);
         if (err==Z_STREAM_END)
           *file_status = FS_COMPLETED;
         return (qsize - zs->avail_out);
       }
       zs->avail_in = count;
       zs->next_in  = buffer;
     }
  }
  return (qsize - zs->avail_out);
}


int StoreFileBuffer(z_streamp zs, Bytef* quantum, int qsize, Bytef* buffer, int bsize, FILE* file, int* file_status)
{
  zs->next_in = quantum;
  zs->avail_in = qsize;
  while (zs->avail_in)
  {
     if (zs->avail_out)
     {
       int err = inflate(zs, Z_NO_FLUSH);
       if (err==Z_OK) continue;
       if (err==Z_STREAM_END)
       {
         fwrite(buffer, 1, bsize - zs->avail_out, file);
         *file_status = FS_COMPLETED;
         return 0;
       }
       else
       {
         ConOut("Net: error inflating stream: %d", err);
         fwrite(buffer, 1, bsize - zs->avail_out, file);
         *file_status = FS_ENDED;
         return 1;
       }
     }
     else
     {
       fwrite(buffer, 1, bsize, file);
       zs->avail_out = bsize;
       zs->next_out  = buffer;
     }
  }
  return 0;
}


//###########################################################################
//## Independent part
//###########################################################################

char *AddrToS(IPaddress *ip)
{
  snprintf(ipnamebuf, 128, "%d.%d.%d.%d:%d",
             ((Uint8*)&ip->host)[0],((Uint8*)&ip->host)[1],
             ((Uint8*)&ip->host)[2],((Uint8*)&ip->host)[3], 
             SDL_SwapBE16(ip->port));
  return ipnamebuf;
}


int PollNet()
{
  SV_Poll();
  CL_Poll();
  return 0;
}


void NetCleanup(void)
{
  if (net_client_status != NS_UNINITED)
  {
    CL_End();
  }
  
  if (net_server_status != NS_UNINITED)
  {
    SV_End();
  }

  if (net_status != NS_UNINITED)
  {
    SDLNet_Quit();
    net_status = NS_UNINITED;
  }
	if ( confirm_packet != NULL ) {
		SDLNet_FreePacket(confirm_packet);
		confirm_packet = NULL;
	}
}


int InitNet()
{
	if ( SDLNet_Init() < 0 ) {
		fprintf(stderr, "Couldn't initialize net: %s\n",
						SDLNet_GetError());
		return 1;
	}
	atexit(NetCleanup);

  confirm_packet = SDLNet_AllocPacket(MSG_HEADER_SIZE+1);
	if ( confirm_packet == NULL ) {
		fprintf(stderr, "Couldn't allocate packet (InitNet): Out of memory\n");
    return 1;
	}

  net_status = NS_INITED;
  net_client_status = NS_UNINITED;
  net_server_status = NS_UNINITED;
   
  return 0;
}

int DoneNet()
{
  NetCleanup(); 
  return 0;
}