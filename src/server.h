/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   server side code
//## author:  Antonin Hildebrand
//##         
//## started: 1.4.2000
//## revised: ???
//## 
//###########################################################################

#ifndef _PACSERVER_H_
#define _PACSERVER_H_

#include "SDL_net.h"
#include "protocol.h"
#include "net.h"
#include "server.h"
#include "game.h"


//###########################################################################
//## Constants & defines
//###########################################################################


//###########################################################################
//## Data structures
//###########################################################################
typedef struct {
  char name[PWP_MAX_SERVER_NAME];
  Uint16 version;
  bool active;
  
  Uint32      start_time;   // time when server started
  Uint32      last_time;    // last tick time
  TICK_TYPE   ticks;        // number of frame ticks
  Uint16      pticks;       // how many tick remains to send new game packet
  
  CGame game;  // SERVER GAME STATE - this is the only reality

  int maxclients;
} net_server_info;

typedef struct {
  int status;
  char name[PWP_MAX_CLIENT_NAME];
	IPaddress msg_ip;
	IPaddress game_ip;
  
  int con_attach;

  // GAME related
//  CGame game;    // CLIENT GAME STATE ... mirroring
  CReplicator replicator;
  net_message_pool game_pool;

  // MSG related
  net_message_pool msg_pool;

  // FILE transfer
  char      local_fn[PWP_MAX_FILE_NAME];
  int       file_status;
  TCPsocket file_sock;
  FILE*     file;
  int       bsize;
  Uint8*    buffer;
  int       qsize;     // amount of data to send/receive in one step
  Uint8*    quantum;
  z_stream  zs;
  Uint32    lrec_time;     // last time value when server received file data from this client

  
  // statistics
  net_stats stats;

  // ping
  int ping;
  Uint32 pingsent;
  bool pingreturned;
} net_client;

//###########################################################################
//## Globals
//###########################################################################
extern DECLSPEC int net_server_status;

extern DECLSPEC int server_sent;            
extern DECLSPEC int server_received; 

extern DECLSPEC net_client client[PWP_TOTALMAX_CLIENTS];
extern DECLSPEC net_server_info server_info;

extern DECLSPEC Uint32 sltime;
extern DECLSPEC Uint32 ssum;
extern DECLSPEC Uint32 servertime;
extern DECLSPEC Uint32 snum;


//###########################################################################
//## Cvars
//###########################################################################

// cvars
extern DECLSPEC cvar_t	s_deletetmps;
extern DECLSPEC cvar_t	s_downloading;
extern DECLSPEC cvar_t	s_uploading;
extern DECLSPEC cvar_t	s_remote;
extern DECLSPEC cvar_t	s_acpass;

extern DECLSPEC cvar_t	s_maxclients;
extern DECLSPEC cvar_t	s_name;
extern DECLSPEC cvar_t	s_welcome_msg;

extern DECLSPEC cvar_t	s_next_map;
extern DECLSPEC cvar_t	s_next_script;
extern DECLSPEC cvar_t	s_next_gt;
extern DECLSPEC cvar_t	s_timelimit;
extern DECLSPEC cvar_t	s_fraglimit;


extern DECLSPEC SDLNet_SocketSet ssocket_set; // docasne

//###########################################################################
//## Exported functions
//###########################################################################
extern DECLSPEC int SV_BroadcastMsg(net_msg& MSG, UDPsocket sock, Uint32 resend);
extern DECLSPEC void SV_BroadcastPrintf(char *fmt, ...);
extern DECLSPEC void SV_ClientPrintf(int c, char *fmt, ...);


extern DECLSPEC int SV_Start();
extern DECLSPEC int SV_End();
extern DECLSPEC int SV_Clear();

extern DECLSPEC int SV_DropClient(int cnum);

extern DECLSPEC int SV_Shutdown(Uint8 type);
extern DECLSPEC int SV_Run();
extern DECLSPEC int SV_ChangeGame();

extern DECLSPEC int SV_SetMap(char* mapname, char* scriptname);

extern DECLSPEC int SV_Poll();
extern DECLSPEC void SV_Move(Uint32 ticktime);
extern DECLSPEC char SV_ParseReplication(net_msg * msg, TICK_TYPE time);
extern DECLSPEC void SV_CreatePlayer(net_msg * MSG, int owner);
extern DECLSPEC void SV_DestroyPlayer(net_msg * MSG, int owner);
extern DECLSPEC void SV_SkinPlayer(net_msg * MSG, int owner);
extern DECLSPEC void SV_ChasePlayer(net_msg * MSG, int owner);

extern DECLSPEC void SV_BroadcastSound(Uint8 id, Uint8 volume = 100);
extern DECLSPEC void SV_ClientSound(int c,Uint8 id, Uint8 volume = 100);
extern DECLSPEC void SV_BroadcastVoice(Uint8 id, Uint8 volume = 100, Uint8 priority = 1);
extern DECLSPEC void SV_ClientVoice(int c,Uint8 id, Uint8 volume = 100, Uint8 priority = 1);
extern DECLSPEC void SV_Ping(int cnum);






#endif