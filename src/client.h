/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   client side code
//## author:  Antonin Hildebrand
//##         
//## started: 1.4.2000
//## revised: ???
//## 
//###########################################################################

#ifndef _PACCLIENT_H_
#define _PACCLIENT_H_

#include "SDL_net.h"
#include "protocol.h"
#include "net.h"
#include "client.h"
#include "game.h"
#include "pac_player.h"

//###########################################################################
//## Constants & defines
//###########################################################################

//###########################################################################
//## Data structures
//###########################################################################
class CFileRequest 
{
public:
  CFileRequest() { next = NULL; }
  
  void Set(Uint8 nid, char* nname) { id = nid; strncpy(filename, nname, PWP_MAX_FILE_NAME); }

  Uint8 id;
  char filename[PWP_MAX_FILE_NAME];
  
  CFileRequest* next;
};

class net_client_info 
{
public:
    net_client_info() { dlreqs = NULL; }

    char name[PWP_MAX_CLIENT_NAME];
    int con_attach;
    
    bool active;
    
    Uint32 start_time;  // time when client started
    Uint32 last_time;   // last tick time
    TICK_TYPE ticks;       // number of frame ticks
    Uint16    pticks;       // how many tick remains to send new game packet
    
    CGame game;    // CLIENT GAME STATE

    CFileRequest* dlreqs;

    void AddDlRequest(Uint8 id, char* fname);
    void ClearDlRequests();
    void ScheduleNewDlRequest();
    
    Uint8 client_num;    // client number on server
};

typedef struct {
    int status;
    char name[PWP_MAX_SERVER_NAME];
    
    // GAME related
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
    int       qsize;     // ammount of data to send/receive in one polling step
    Uint8*    quantum;
    z_stream  zs;
    
    //CGame game;    // FAKE SERVER GAME STATE
    
    // statistics
    net_stats stats;
} net_server;



//###########################################################################
//## Globals
//###########################################################################

extern DECLSPEC int net_client_status;
extern DECLSPEC int ping;

extern DECLSPEC int client_sent;            
extern DECLSPEC int client_received; 

extern DECLSPEC net_server server;
extern DECLSPEC net_client_info client_info;

extern DECLSPEC UDPsocket game_csock;    // comunicating only on channel 0
extern DECLSPEC UDPsocket msg_csock;     // comunicating only on channel 0
extern DECLSPEC TCPsocket serv_sock;

extern DECLSPEC net_server server;
extern DECLSPEC net_client_info client_info;

extern DECLSPEC Uint32 cltime;
extern DECLSPEC Uint32 csum;
extern DECLSPEC Uint32 cxtime;
extern DECLSPEC Uint32 cnum;

extern DECLSPEC keybindings_t keybindings[PWP_MAX_PLAYERS_PER_CLIENT]; // keybindings

// cvars
extern DECLSPEC cvar_t	c_deletetmps;
extern DECLSPEC cvar_t	c_name;
extern DECLSPEC cvar_t	c_desc;
extern DECLSPEC cvar_t	c_downloading;
extern DECLSPEC cvar_t	c_uploading;


extern DECLSPEC cvar_t	p1_on;
extern DECLSPEC cvar_t	p1_name;
extern DECLSPEC cvar_t	p1_skin;
extern DECLSPEC cvar_t	p1_key_up;
extern DECLSPEC cvar_t	p1_key_down;
extern DECLSPEC cvar_t	p1_key_left;
extern DECLSPEC cvar_t	p1_key_right;
extern DECLSPEC cvar_t	p1_key_fire;
extern DECLSPEC cvar_t	p1_key_shield;
extern DECLSPEC cvar_t	p1_key_warp;
extern DECLSPEC cvar_t	p1_key_weapon1;
extern DECLSPEC cvar_t	p1_key_weapon2;
extern DECLSPEC cvar_t	p1_key_weapon3;
extern DECLSPEC cvar_t	p1_key_weapon4;
extern DECLSPEC cvar_t	p1_key_weapon5;
extern DECLSPEC cvar_t	p1_keyn_up;
extern DECLSPEC cvar_t	p1_keyn_down;
extern DECLSPEC cvar_t	p1_keyn_left;
extern DECLSPEC cvar_t	p1_keyn_right;
extern DECLSPEC cvar_t	p1_keyn_fire;
extern DECLSPEC cvar_t	p1_keyn_shield;
extern DECLSPEC cvar_t	p1_keyn_warp;
extern DECLSPEC cvar_t	p1_keyn_weapon1;
extern DECLSPEC cvar_t	p1_keyn_weapon2;
extern DECLSPEC cvar_t	p1_keyn_weapon3;
extern DECLSPEC cvar_t	p1_keyn_weapon4;
extern DECLSPEC cvar_t	p1_keyn_weapon5;

extern DECLSPEC cvar_t	p2_on;
extern DECLSPEC cvar_t	p2_name;
extern DECLSPEC cvar_t	p2_skin;
extern DECLSPEC cvar_t	p2_key_up;
extern DECLSPEC cvar_t	p2_key_down;
extern DECLSPEC cvar_t	p2_key_left;
extern DECLSPEC cvar_t	p2_key_right;
extern DECLSPEC cvar_t	p2_key_fire;
extern DECLSPEC cvar_t	p2_key_shield;
extern DECLSPEC cvar_t	p2_key_warp;
extern DECLSPEC cvar_t	p2_key_weapon1;
extern DECLSPEC cvar_t	p2_key_weapon2;
extern DECLSPEC cvar_t	p2_key_weapon3;
extern DECLSPEC cvar_t	p2_key_weapon4;
extern DECLSPEC cvar_t	p2_key_weapon5;
extern DECLSPEC cvar_t	p2_keyn_up;
extern DECLSPEC cvar_t	p2_keyn_down;
extern DECLSPEC cvar_t	p2_keyn_left;
extern DECLSPEC cvar_t	p2_keyn_right;
extern DECLSPEC cvar_t	p2_keyn_fire;
extern DECLSPEC cvar_t	p2_keyn_shield;
extern DECLSPEC cvar_t	p2_keyn_warp;
extern DECLSPEC cvar_t	p2_keyn_weapon1;
extern DECLSPEC cvar_t	p2_keyn_weapon2;
extern DECLSPEC cvar_t	p2_keyn_weapon3;
extern DECLSPEC cvar_t	p2_keyn_weapon4;
extern DECLSPEC cvar_t	p2_keyn_weapon5;

extern DECLSPEC cvar_t	p3_on;
extern DECLSPEC cvar_t	p3_name;
extern DECLSPEC cvar_t	p3_skin;
extern DECLSPEC cvar_t	p3_key_up;
extern DECLSPEC cvar_t	p3_key_down;
extern DECLSPEC cvar_t	p3_key_left;
extern DECLSPEC cvar_t	p3_key_right;
extern DECLSPEC cvar_t	p3_key_fire;
extern DECLSPEC cvar_t	p3_key_shield;
extern DECLSPEC cvar_t	p3_key_warp;
extern DECLSPEC cvar_t	p3_key_weapon1;
extern DECLSPEC cvar_t	p3_key_weapon2;
extern DECLSPEC cvar_t	p3_key_weapon3;
extern DECLSPEC cvar_t	p3_key_weapon4;
extern DECLSPEC cvar_t	p3_key_weapon5;
extern DECLSPEC cvar_t	p3_keyn_up;
extern DECLSPEC cvar_t	p3_keyn_down;
extern DECLSPEC cvar_t	p3_keyn_left;
extern DECLSPEC cvar_t	p3_keyn_right;
extern DECLSPEC cvar_t	p3_keyn_fire;
extern DECLSPEC cvar_t	p3_keyn_shield;
extern DECLSPEC cvar_t	p3_keyn_warp;
extern DECLSPEC cvar_t	p3_keyn_weapon1;
extern DECLSPEC cvar_t	p3_keyn_weapon2;
extern DECLSPEC cvar_t	p3_keyn_weapon3;
extern DECLSPEC cvar_t	p3_keyn_weapon4;
extern DECLSPEC cvar_t	p3_keyn_weapon5;

extern DECLSPEC cvar_t	p4_on;
extern DECLSPEC cvar_t	p4_name;
extern DECLSPEC cvar_t	p4_skin;
extern DECLSPEC cvar_t	p4_key_up;
extern DECLSPEC cvar_t	p4_key_down;
extern DECLSPEC cvar_t	p4_key_left;
extern DECLSPEC cvar_t	p4_key_right;
extern DECLSPEC cvar_t	p4_key_fire;
extern DECLSPEC cvar_t	p4_key_shield;
extern DECLSPEC cvar_t	p4_key_warp;
extern DECLSPEC cvar_t	p4_key_weapon1;
extern DECLSPEC cvar_t	p4_key_weapon2;
extern DECLSPEC cvar_t	p4_key_weapon3;
extern DECLSPEC cvar_t	p4_key_weapon4;
extern DECLSPEC cvar_t	p4_key_weapon5;
extern DECLSPEC cvar_t	p4_keyn_up;
extern DECLSPEC cvar_t	p4_keyn_down;
extern DECLSPEC cvar_t	p4_keyn_left;
extern DECLSPEC cvar_t	p4_keyn_right;
extern DECLSPEC cvar_t	p4_keyn_fire;
extern DECLSPEC cvar_t	p4_keyn_shield;
extern DECLSPEC cvar_t	p4_keyn_warp;
extern DECLSPEC cvar_t	p4_keyn_weapon1;
extern DECLSPEC cvar_t	p4_keyn_weapon2;
extern DECLSPEC cvar_t	p4_keyn_weapon3;
extern DECLSPEC cvar_t	p4_keyn_weapon4;
extern DECLSPEC cvar_t	p4_keyn_weapon5;


//###########################################################################
//## Exported functions
//###########################################################################

extern DECLSPEC int CL_Start(char * station_name);
extern DECLSPEC int CL_End();
extern DECLSPEC int CL_Clear();

extern DECLSPEC int CL_Connect(char* server);
extern DECLSPEC int CL_Disconnect();
extern DECLSPEC void CL_Ping();

extern DECLSPEC int CL_Attach();
extern DECLSPEC int CL_Detach();

extern DECLSPEC int CL_Upload(Uint8 ft_id, char* fname);
extern DECLSPEC int CL_Download(Uint8 ft_id, char* fname);

extern DECLSPEC int CL_Poll();
extern DECLSPEC void CL_Move(Uint32 ticktime);
extern DECLSPEC void CL_EnterGame(Uint32 time);

extern DECLSPEC int ChatMessage(char* m);
extern DECLSPEC void CL_ServerCrashed();

extern DECLSPEC int CL_FindUnusedKBSlot();

// do not call directly
extern DECLSPEC int CL_ChangeMap(char* mapname, char* scriptname);

extern DECLSPEC char CL_ParseReplication(net_msg * msg, TICK_TYPE time, CGame& game, bool mirror, int client);

extern DECLSPEC int CL_CreatePlayer(char* name, Uint8 keybindings);
extern DECLSPEC int CL_DestroyPlayer(char* name);
extern DECLSPEC int CL_SkinPlayer(char* name, char* spritename);
extern DECLSPEC int CL_ChasePlayer(char* name);
extern DECLSPEC int CL_ChasePlayerConf(int i);



// Special callbacks
//int CL_CB_changed_player_brain(GPlayer* player);


#endif