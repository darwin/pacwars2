/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   client side code
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
#include "SDL_Console.h"
#include "SDL_net.h"
#include "net.h"
#include "client.h"
#include "messenger.h"
#include "scripting.h"
#include "pw2.h"

//###########################################################################
//## Globals
//###########################################################################
// temp
Uint32 cltime = 0;
Uint32 csum = 0;
Uint32 cxtime = 0;
Uint32 cnum = 0;

int net_client_status;

int ping;
Uint32 pingsent;
bool pingreturned;

int client_sent;				// file data: bytes sent     
int client_received;			// file data: bytes received
Uint32 lcreCL_time;				// last time value when client received file data

Uint32 entersent;


int CL_SetName(char *);

void CHc_name(cvar_t * var, int *changed)
{
  CL_SetName(var->string);
}

// cvars
cvar_t c_name = { "c_name", STD_CLIENT_NAME, true, false, CHc_name };
cvar_t c_desc = { "c_desc", STD_CLIENT_DESC, true };

cvar_t c_deletetmps = { "c_deltmps", "1", true };
cvar_t c_downloading = { "c_downloading", "0", true };
cvar_t c_uploading = { "c_uploading", "0", true };


// player1 settings

void CL_p1_KeyChanged1(cvar_t * var, int *changed)
{
  Cvar_Set("p1_keyn_up", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p1_KeyChanged2(cvar_t * var, int *changed)
{
  Cvar_Set("p1_keyn_down", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p1_KeyChanged3(cvar_t * var, int *changed)
{
  Cvar_Set("p1_keyn_left", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p1_KeyChanged4(cvar_t * var, int *changed)
{
  Cvar_Set("p1_keyn_right", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p1_KeyChanged5(cvar_t * var, int *changed)
{
  Cvar_Set("p1_keyn_fire", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p1_KeyChanged6(cvar_t * var, int *changed)
{
  Cvar_Set("p1_keyn_shield", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p1_KeyChanged7(cvar_t * var, int *changed)
{
  Cvar_Set("p1_keyn_warp", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p1_KeyChanged8(cvar_t * var, int *changed)
{
  Cvar_Set("p1_keyn_weapon1",
			 SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p1_KeyChanged9(cvar_t * var, int *changed)
{
  Cvar_Set("p1_keyn_weapon2",
			 SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p1_KeyChanged10(cvar_t * var, int *changed)
{
  Cvar_Set("p1_keyn_weapon3",
			 SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p1_KeyChanged11(cvar_t * var, int *changed)
{
  Cvar_Set("p1_keyn_weapon4",
			 SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p1_KeyChanged12(cvar_t * var, int *changed)
{
  Cvar_Set("p1_keyn_weapon5",
			 SDL_GetKeyName((SDLKey) atoi(var->string)));
}

cvar_t p1_on = { "p1_on", "0", false };
cvar_t p1_name = { "p1_name", "unknown1", true };
cvar_t p1_skin = { "p1_skin", "implicit", true };

cvar_t p1_key_up =
{ "p1_key_up", "", true, false, CL_p1_KeyChanged1, STD_KEY_UP };
cvar_t p1_key_down =
{ "p1_key_down", "", true, false, CL_p1_KeyChanged2, STD_KEY_DOWN };
cvar_t p1_key_left =
{ "p1_key_left", "", true, false, CL_p1_KeyChanged3, STD_KEY_LEFT };
cvar_t p1_key_right =
{ "p1_key_right", "", true, false, CL_p1_KeyChanged4, STD_KEY_RIGHT };
cvar_t p1_key_fire =
{ "p1_key_fire", "", true, false, CL_p1_KeyChanged5, STD_KEY_FIRE };
cvar_t p1_key_shield =
{ "p1_key_shield", "", true, false, CL_p1_KeyChanged6,
STD_KEY_SHIELD
};
cvar_t p1_key_warp =
{ "p1_key_warp", "", true, false, CL_p1_KeyChanged7, STD_KEY_WARP };
cvar_t p1_key_weapon1 =
{ "p1_key_weapon1", "", true, false, CL_p1_KeyChanged8,
STD_KEY_WEAPON1
};
cvar_t p1_key_weapon2 =
{ "p1_key_weapon2", "", true, false, CL_p1_KeyChanged9,
STD_KEY_WEAPON2
};
cvar_t p1_key_weapon3 =
{ "p1_key_weapon3", "", true, false, CL_p1_KeyChanged10,
STD_KEY_WEAPON3
};
cvar_t p1_key_weapon4 =
{ "p1_key_weapon4", "", true, false, CL_p1_KeyChanged11,
STD_KEY_WEAPON4
};
cvar_t p1_key_weapon5 =
{ "p1_key_weapon5", "", true, false, CL_p1_KeyChanged12,
STD_KEY_WEAPON5
};

cvar_t p1_keyn_up = { "p1_keyn_up", "", true };
cvar_t p1_keyn_down = { "p1_keyn_down", "", true };
cvar_t p1_keyn_left = { "p1_keyn_left", "", true };
cvar_t p1_keyn_right = { "p1_keyn_right", "", true };
cvar_t p1_keyn_fire = { "p1_keyn_fire", "", true };
cvar_t p1_keyn_shield = { "p1_keyn_shield", "", true };
cvar_t p1_keyn_warp = { "p1_keyn_warp", "", true };
cvar_t p1_keyn_weapon1 = { "p1_keyn_weapon1", "", true };
cvar_t p1_keyn_weapon2 = { "p1_keyn_weapon2", "", true };
cvar_t p1_keyn_weapon3 = { "p1_keyn_weapon3", "", true };
cvar_t p1_keyn_weapon4 = { "p1_keyn_weapon4", "", true };
cvar_t p1_keyn_weapon5 = { "p1_keyn_weapon5", "", true };

// player2 settings

void CL_p2_KeyChanged1(cvar_t * var, int *changed)
{
  Cvar_Set("p2_keyn_up", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p2_KeyChanged2(cvar_t * var, int *changed)
{
  Cvar_Set("p2_keyn_down", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p2_KeyChanged3(cvar_t * var, int *changed)
{
  Cvar_Set("p2_keyn_left", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p2_KeyChanged4(cvar_t * var, int *changed)
{
  Cvar_Set("p2_keyn_right", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p2_KeyChanged5(cvar_t * var, int *changed)
{
  Cvar_Set("p2_keyn_fire", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p2_KeyChanged6(cvar_t * var, int *changed)
{
  Cvar_Set("p2_keyn_shield", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p2_KeyChanged7(cvar_t * var, int *changed)
{
  Cvar_Set("p2_keyn_warp", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p2_KeyChanged8(cvar_t * var, int *changed)
{
  Cvar_Set("p2_keyn_weapon1",
			 SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p2_KeyChanged9(cvar_t * var, int *changed)
{
  Cvar_Set("p2_keyn_weapon2",
			 SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p2_KeyChanged10(cvar_t * var, int *changed)
{
  Cvar_Set("p2_keyn_weapon3",
			 SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p2_KeyChanged11(cvar_t * var, int *changed)
{
  Cvar_Set("p2_keyn_weapon4",
			 SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p2_KeyChanged12(cvar_t * var, int *changed)
{
  Cvar_Set("p2_keyn_weapon5",
			 SDL_GetKeyName((SDLKey) atoi(var->string)));
}

cvar_t p2_on = { "p2_on", "0", false };
cvar_t p2_name = { "p2_name", "unknown2", true };
cvar_t p2_skin = { "p2_skin", "implicit", true };

cvar_t p2_key_up =
{ "p2_key_up", "", true, false, CL_p2_KeyChanged1, STD_KEY_UP };
cvar_t p2_key_down =
{ "p2_key_down", "", true, false, CL_p2_KeyChanged2, STD_KEY_DOWN };
cvar_t p2_key_left =
{ "p2_key_left", "", true, false, CL_p2_KeyChanged3, STD_KEY_LEFT };
cvar_t p2_key_right =
{ "p2_key_right", "", true, false, CL_p2_KeyChanged4, STD_KEY_RIGHT };
cvar_t p2_key_fire =
{ "p2_key_fire", "", true, false, CL_p2_KeyChanged5, STD_KEY_FIRE };
cvar_t p2_key_shield =
{ "p2_key_shield", "", true, false, CL_p2_KeyChanged6,
STD_KEY_SHIELD
};
cvar_t p2_key_warp =
{ "p2_key_warp", "", true, false, CL_p2_KeyChanged7, STD_KEY_WARP };
cvar_t p2_key_weapon1 =
{ "p2_key_weapon1", "", true, false, CL_p2_KeyChanged8,
STD_KEY_WEAPON1
};
cvar_t p2_key_weapon2 =
{ "p2_key_weapon2", "", true, false, CL_p2_KeyChanged9,
STD_KEY_WEAPON2
};
cvar_t p2_key_weapon3 =
{ "p2_key_weapon3", "", true, false, CL_p2_KeyChanged10,
STD_KEY_WEAPON3
};
cvar_t p2_key_weapon4 =
{ "p2_key_weapon4", "", true, false, CL_p2_KeyChanged11,
STD_KEY_WEAPON4
};
cvar_t p2_key_weapon5 =
{ "p2_key_weapon5", "", true, false, CL_p2_KeyChanged12,
STD_KEY_WEAPON5
};

cvar_t p2_keyn_up = { "p2_keyn_up", "", true };
cvar_t p2_keyn_down = { "p2_keyn_down", "", true };
cvar_t p2_keyn_left = { "p2_keyn_left", "", true };
cvar_t p2_keyn_right = { "p2_keyn_right", "", true };
cvar_t p2_keyn_fire = { "p2_keyn_fire", "", true };
cvar_t p2_keyn_shield = { "p2_keyn_shield", "", true };
cvar_t p2_keyn_warp = { "p2_keyn_warp", "", true };
cvar_t p2_keyn_weapon1 = { "p2_keyn_weapon1", "", true };
cvar_t p2_keyn_weapon2 = { "p2_keyn_weapon2", "", true };
cvar_t p2_keyn_weapon3 = { "p2_keyn_weapon3", "", true };
cvar_t p2_keyn_weapon4 = { "p2_keyn_weapon4", "", true };
cvar_t p2_keyn_weapon5 = { "p2_keyn_weapon5", "", true };


// player4 settings

void CL_p3_KeyChanged1(cvar_t * var, int *changed)
{
  Cvar_Set("p3_keyn_up", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p3_KeyChanged2(cvar_t * var, int *changed)
{
  Cvar_Set("p3_keyn_down", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p3_KeyChanged3(cvar_t * var, int *changed)
{
  Cvar_Set("p3_keyn_left", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p3_KeyChanged4(cvar_t * var, int *changed)
{
  Cvar_Set("p3_keyn_right", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p3_KeyChanged5(cvar_t * var, int *changed)
{
  Cvar_Set("p3_keyn_fire", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p3_KeyChanged6(cvar_t * var, int *changed)
{
  Cvar_Set("p3_keyn_shield", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p3_KeyChanged7(cvar_t * var, int *changed)
{
  Cvar_Set("p3_keyn_warp", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p3_KeyChanged8(cvar_t * var, int *changed)
{
  Cvar_Set("p3_keyn_weapon1",
			 SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p3_KeyChanged9(cvar_t * var, int *changed)
{
  Cvar_Set("p3_keyn_weapon2",
			 SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p3_KeyChanged10(cvar_t * var, int *changed)
{
  Cvar_Set("p3_keyn_weapon3",
			 SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p3_KeyChanged11(cvar_t * var, int *changed)
{
  Cvar_Set("p3_keyn_weapon4",
			 SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p3_KeyChanged12(cvar_t * var, int *changed)
{
  Cvar_Set("p3_keyn_weapon5",
			 SDL_GetKeyName((SDLKey) atoi(var->string)));
}

cvar_t p3_on = { "p3_on", "0", false };
cvar_t p3_name = { "p3_name", "unknown3", true };
cvar_t p3_skin = { "p3_skin", "implicit", true };

cvar_t p3_key_up =
{ "p3_key_up", "", true, false, CL_p3_KeyChanged1, STD_KEY_UP };
cvar_t p3_key_down =
{ "p3_key_down", "", true, false, CL_p3_KeyChanged2, STD_KEY_DOWN };
cvar_t p3_key_left =
{ "p3_key_left", "", true, false, CL_p3_KeyChanged3, STD_KEY_LEFT };
cvar_t p3_key_right =
{ "p3_key_right", "", true, false, CL_p3_KeyChanged4, STD_KEY_RIGHT };
cvar_t p3_key_fire =
{ "p3_key_fire", "", true, false, CL_p3_KeyChanged5, STD_KEY_FIRE };
cvar_t p3_key_shield =
{ "p3_key_shield", "", true, false, CL_p3_KeyChanged6,
STD_KEY_SHIELD
};
cvar_t p3_key_warp =
{ "p3_key_warp", "", true, false, CL_p3_KeyChanged7, STD_KEY_WARP };
cvar_t p3_key_weapon1 =
{ "p3_key_weapon1", "", true, false, CL_p3_KeyChanged8,
STD_KEY_WEAPON1
};
cvar_t p3_key_weapon2 =
{ "p3_key_weapon2", "", true, false, CL_p3_KeyChanged9,
STD_KEY_WEAPON2
};
cvar_t p3_key_weapon3 =
{ "p3_key_weapon3", "", true, false, CL_p3_KeyChanged10,
STD_KEY_WEAPON3
};
cvar_t p3_key_weapon4 =
{ "p3_key_weapon4", "", true, false, CL_p3_KeyChanged11,
STD_KEY_WEAPON4
};
cvar_t p3_key_weapon5 =
{ "p3_key_weapon5", "", true, false, CL_p3_KeyChanged12,
STD_KEY_WEAPON5
};

cvar_t p3_keyn_up = { "p3_keyn_up", "", true };
cvar_t p3_keyn_down = { "p3_keyn_down", "", true };
cvar_t p3_keyn_left = { "p3_keyn_left", "", true };
cvar_t p3_keyn_right = { "p3_keyn_right", "", true };
cvar_t p3_keyn_fire = { "p3_keyn_fire", "", true };
cvar_t p3_keyn_shield = { "p3_keyn_shield", "", true };
cvar_t p3_keyn_warp = { "p3_keyn_warp", "", true };
cvar_t p3_keyn_weapon1 = { "p3_keyn_weapon1", "", true };
cvar_t p3_keyn_weapon2 = { "p3_keyn_weapon2", "", true };
cvar_t p3_keyn_weapon3 = { "p3_keyn_weapon3", "", true };
cvar_t p3_keyn_weapon4 = { "p3_keyn_weapon4", "", true };
cvar_t p3_keyn_weapon5 = { "p3_keyn_weapon5", "", true };


// player3 settings

void CL_p4_KeyChanged1(cvar_t * var, int *changed)
{
  Cvar_Set("p4_keyn_up", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p4_KeyChanged2(cvar_t * var, int *changed)
{
  Cvar_Set("p4_keyn_down", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p4_KeyChanged3(cvar_t * var, int *changed)
{
  Cvar_Set("p4_keyn_left", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p4_KeyChanged4(cvar_t * var, int *changed)
{
  Cvar_Set("p4_keyn_right", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p4_KeyChanged5(cvar_t * var, int *changed)
{
  Cvar_Set("p4_keyn_fire", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p4_KeyChanged6(cvar_t * var, int *changed)
{
  Cvar_Set("p4_keyn_shield", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p4_KeyChanged7(cvar_t * var, int *changed)
{
  Cvar_Set("p4_keyn_warp", SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p4_KeyChanged8(cvar_t * var, int *changed)
{
  Cvar_Set("p4_keyn_weapon1",
			 SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p4_KeyChanged9(cvar_t * var, int *changed)
{
  Cvar_Set("p4_keyn_weapon2",
			 SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p4_KeyChanged10(cvar_t * var, int *changed)
{
  Cvar_Set("p4_keyn_weapon3",
			 SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p4_KeyChanged11(cvar_t * var, int *changed)
{
  Cvar_Set("p4_keyn_weapon4",
			 SDL_GetKeyName((SDLKey) atoi(var->string)));
}
void CL_p4_KeyChanged12(cvar_t * var, int *changed)
{
  Cvar_Set("p4_keyn_weapon5",
			 SDL_GetKeyName((SDLKey) atoi(var->string)));
}

cvar_t p4_on = { "p4_on", "0", false };
cvar_t p4_name = { "p4_name", "unknown4", true };
cvar_t p4_skin = { "p4_skin", "implicit", true };

cvar_t p4_key_up =
{ "p4_key_up", "", true, false, CL_p4_KeyChanged1, STD_KEY_UP };
cvar_t p4_key_down =
{ "p4_key_down", "", true, false, CL_p4_KeyChanged2, STD_KEY_DOWN };
cvar_t p4_key_left =
{ "p4_key_left", "", true, false, CL_p4_KeyChanged3, STD_KEY_LEFT };
cvar_t p4_key_right =
{ "p4_key_right", "", true, false, CL_p4_KeyChanged4, STD_KEY_RIGHT };
cvar_t p4_key_fire =
{ "p4_key_fire", "", true, false, CL_p4_KeyChanged5, STD_KEY_FIRE };
cvar_t p4_key_shield =
{ "p4_key_shield", "", true, false, CL_p4_KeyChanged6,
STD_KEY_SHIELD
};
cvar_t p4_key_warp =
{ "p4_key_warp", "", true, false, CL_p4_KeyChanged7, STD_KEY_WARP };
cvar_t p4_key_weapon1 =
{ "p4_key_weapon1", "", true, false, CL_p4_KeyChanged8,
STD_KEY_WEAPON1
};
cvar_t p4_key_weapon2 =
{ "p4_key_weapon2", "", true, false, CL_p4_KeyChanged9,
STD_KEY_WEAPON2
};
cvar_t p4_key_weapon3 =
{ "p4_key_weapon3", "", true, false, CL_p4_KeyChanged10,
STD_KEY_WEAPON3
};
cvar_t p4_key_weapon4 =
{ "p4_key_weapon4", "", true, false, CL_p4_KeyChanged11,
STD_KEY_WEAPON4
};
cvar_t p4_key_weapon5 =
{ "p4_key_weapon5", "", true, false, CL_p4_KeyChanged12,
STD_KEY_WEAPON5
};

cvar_t p4_keyn_up = { "p4_keyn_up", "", true };
cvar_t p4_keyn_down = { "p4_keyn_down", "", true };
cvar_t p4_keyn_left = { "p4_keyn_left", "", true };
cvar_t p4_keyn_right = { "p4_keyn_right", "", true };
cvar_t p4_keyn_fire = { "p4_keyn_fire", "", true };
cvar_t p4_keyn_shield = { "p4_keyn_shield", "", true };
cvar_t p4_keyn_warp = { "p4_keyn_warp", "", true };
cvar_t p4_keyn_weapon1 = { "p4_keyn_weapon1", "", true };
cvar_t p4_keyn_weapon2 = { "p4_keyn_weapon2", "", true };
cvar_t p4_keyn_weapon3 = { "p4_keyn_weapon3", "", true };
cvar_t p4_keyn_weapon4 = { "p4_keyn_weapon4", "", true };
cvar_t p4_keyn_weapon5 = { "p4_keyn_weapon5", "", true };

keybindings_t keybindings[PWP_MAX_PLAYERS_PER_CLIENT];	// keybindings

void CL_ClearKBSlots()
{
  for (int i = 0; i < PWP_MAX_PLAYERS_PER_CLIENT; i++)
    keybindings[i].used = false;
}

int CL_FindUnusedKBSlot()
{
  int i = 0;
  for (i = 0; i < PWP_MAX_PLAYERS_PER_CLIENT; i++)
    if (!keybindings[i].used)
      break;
    if (i == PWP_MAX_PLAYERS_PER_CLIENT) {
      ConOutEx(CLIENT_FONT, "All keybinding slots are in use - using slot 0");
      return 0;
    }
    return i;
}

void CL_FreeKBSlot(Uint8 slot)
{
  keybindings[slot].used = false;
}

//###########################################################################
//## Client part
//###########################################################################

SDLNet_SocketSet csocket_set = NULL;

UDPpacket **game_cpackets = NULL;	// packets for receiving
UDPpacket *game_cpacket = NULL;	// packet for sending
UDPpacket **msg_cpackets = NULL;	// packets for receiving
UDPpacket *msg_cpacket = NULL;	// packet for sending

UDPsocket game_csock = NULL;	// comunicating only on channel 0
UDPsocket msg_csock = NULL;		// comunicating only on channel 0
TCPsocket serv_sock = NULL;
Uint16 serv_port;

net_server server;
net_client_info client_info;

void net_client_info::AddDlRequest(Uint8 id, char *fname)
{
  if (net_client_status == NS_CONNECTED) {
    CFileRequest *fr = new CFileRequest();
    fr->Set(id, fname);
    
    CFileRequest **i = &dlreqs;
    while (*i) {
      if ((**i).id == id && strcmp((**i).filename, fname) == 0)
        return;
      i = &(*i)->next;
    }
    *i = fr;
    
    ConOutEx(CLIENT_FONT, "Added request for downloading %s", fname);
    
    if (server.file_status == FS_READY)
      CL_Download(dlreqs->id, dlreqs->filename);
  }
}

void net_client_info::ClearDlRequests()
{
  CFileRequest *i = dlreqs;
  CFileRequest *j;
  while (i) {
    j = i;
    i = i->next;
    delete j;
  }
  dlreqs = NULL;
}

void net_client_info::ScheduleNewDlRequest()
{
  int len;
  CSpriteInfo *si;
  CFileRequest *i = dlreqs;
  if (i) {
    dlreqs = i->next;
    switch (i->id) {
    case DLID_MAP:
    case DLID_SCRIPT:
      break;
    case DLID_SPRITE:
      // reload sprite
      SpriteMan.Scan(sprite_dir.string, sprite_ext.string);
      len = strlen(i->filename) - (1 + strlen(sprite_ext.string));
      if (len > (MAX_SPRITE_NAME - 1))
        len = MAX_SPRITE_NAME - 1;
      i->filename[len] = 0;
      si = SpriteMan.Find(i->filename);
      if (si) {
        SpriteMan.Free(si);
        SpriteMan.Load(si);
      } else
        SpriteMan.Register(i->filename);
      
      break;
    case DLID_SKIN:
      // reload skin
      SkinMan.Scan(skin_dir.string, skin_ext.string);
      len = strlen(i->filename) - (1 + strlen(skin_ext.string));
      if (len > (MAX_SPRITE_NAME - 1))
        len = MAX_SPRITE_NAME - 1;
      i->filename[len] = 0;
      si = SkinMan.Find(i->filename);
      if (si) {
        SkinMan.Free(si);
        SkinMan.Load(si);
      } else
        SkinMan.Register(i->filename);
      break;
    case DLID_SOUND:
      // TODO:
      break;
    }
  }
  
  if (dlreqs) {
    ConOutEx(CLIENT_FONT, "Asking for downloading %s", dlreqs->filename);
    CL_Download(dlreqs->id, dlreqs->filename);
  } else						// finished downloading
  {
    if (i) {
      switch (i->id) {
      case DLID_MAP:
      case DLID_SCRIPT:
        CL_Disconnect();
        SDL_Delay(2000);
        CL_Connect(last_server.string);
        break;
      case DLID_SPRITE:
        break;
      case DLID_SKIN:
        break;
      case DLID_SOUND:
        // TODO:
        break;
      }
    }
  }
  if (i)
    delete i;
}

int CL_ClearFileTransfer(int how)
{
  char tmp[_MAX_PATH];
  
  server.file_status = FS_READY;
  client_received = 0;
  client_sent = 0;
  
  if (server.file_sock) {
    SDLNet_TCP_DelSocket(csocket_set, server.file_sock);
    SDLNet_TCP_Close(server.file_sock);
    server.file_sock = NULL;
    
  }
  
  switch (how) {
  case FT_FINISHED_RENAME:
    if (server.file) {
      fclose(server.file);
      server.file = NULL;
    }
    // rename tmp file
    TempName(server.local_fn, tmp);
    if (rename(tmp, server.local_fn)) {
      ConOutEx(CLIENT_FONT, "Client: error renaming received file !");
    }
    break;
  case FT_FINISHED:
    if (server.file) {
      fclose(server.file);
      server.file = NULL;
    }
    break;
  case FT_ABANDONED:
    if (server.file) {
      fclose(server.file);
      server.file = NULL;
      
      // delete broken tmp file ?
      if (c_deletetmps.value) {
        TempName(server.local_fn, tmp);
        if (remove(tmp) != 0) {
          ConOutEx(CLIENT_FONT, "Client: error removing broken tmp file !");
        }
      }
    }
    break;
  }
  return 0;
}

int CL_Clear()
{
  if (net_client_status != NS_UNINITED) {
    net_client_status = NS_INITED;
    client_info.active = false;
    client_received = 0;
    client_sent = 0;
    
    // unbind all sockets
    SDLNet_UDP_Unbind(msg_csock, 0);
    SDLNet_UDP_Unbind(game_csock, 0);
    
    // clear server structure
    server.status = SS_UNUSED;
    server.stats.Clear();
    
    server.file_status = FS_READY;
    if (server.file)
      fclose(server.file);
    server.file = NULL;
    
    ClearPool(&server.msg_pool);
    ClearPool(&server.game_pool);
    
    // clear client_info structure
    client_info.con_attach = 0;
    client_info.game.Clear();
    client_info.ClearDlRequests();
    
    enable_menu_music = 1;
    
    MapLoaded = 0;
    
    CL_ClearFileTransfer(FT_FINISHED);
    
    CL_ClearKBSlots();
    pingreturned = true;
  }
  
  GUI_Refresh();
  return 0;
}

int CL_Start(char *station_name)
{
  int i;
  if (net_client_status != NS_UNINITED) {
    SDLNet_SetError("Client allready started");
    return 1;
  }
  strcpy(client_info.name, station_name);
  
  // prepare server's slots
  server.status = SS_UNUSED;
  server.file_sock = NULL;
  if (CreatePool
    (&server.msg_pool, MSG_SPOOL_SIZE, MSG_RPOOL_SIZE, PWP_MMP_SIZE)) {
    SDLNet_SetError
      ("Couldn't create msg pool (client): Out of memory\n");
    return 1;
  }
  if (CreatePool
    (&server.game_pool, GAME_SPOOL_SIZE, GAME_RPOOL_SIZE,
    PWP_MGP_SIZE)) {
    SDLNet_SetError
      ("Couldn't create game pool (client): Out of memory\n");
    return 1;
  }
  server.buffer = (Uint8 *) malloc(PWP_FBUFFER_SIZE);
  if (!server.buffer) {
    SDLNet_SetError
      ("Couldn't create file buffer (client): Out of memory\n");
    return 1;
  }
  server.bsize = PWP_FBUFFER_SIZE;
  server.quantum = (Uint8 *) malloc(PWP_FQUANTUM_SIZE);
  if (!server.quantum) {
    SDLNet_SetError
      ("Couldn't create quantum buffer (client): Out of memory\n");
    return 1;
  }
  server.qsize = PWP_FQUANTUM_SIZE;
  CL_Clear();
  
  // allocate net structs
  game_cpackets = SDLNet_AllocPacketV(PWP_MGP_CCOUNT, PWP_MGP_SIZE);
  if (game_cpackets == NULL) {
    SDLNet_SetError("Couldn't allocate packets: Out of memory\n");
    return 1;
  }
  
  game_cpacket = SDLNet_AllocPacket(PWP_MGP_SIZE);
  if (game_cpacket == NULL) {
    SDLNet_SetError("Couldn't allocate packets: Out of memory\n");
    return 1;
  }
  
  msg_cpackets = SDLNet_AllocPacketV(PWP_MMP_CCOUNT, PWP_MMP_SIZE);
  if (msg_cpackets == NULL) {
    SDLNet_SetError("Couldn't allocate packets: Out of memory\n");
    return 1;
  }
  
  msg_cpacket = SDLNet_AllocPacket(PWP_MMP_SIZE);
  if (msg_cpacket == NULL) {
    SDLNet_SetError("Couldn't allocate packets: Out of memory\n");
    return 1;
  }
  
  for (i = 0; (msg_csock == NULL || game_csock == NULL)
    && i < PWP_PORT_STEP * PWP_SOCKET_RANGE; i += PWP_PORT_STEP) {
    msg_csock = SDLNet_UDP_Open(PWP_MSG_CPORT + i);
    game_csock = SDLNet_UDP_Open(PWP_MSG_CPORT + i + 1);
  }
  if (game_csock == NULL) {
    SDLNet_SetError("Couldn't create UDP endpoint (game)\n");
    return 1;
  }
  
  if (msg_csock == NULL) {
    SDLNet_SetError("Couldn't create UDP endpoint (msg)\n");
    return 1;
  }
  
  IPaddress serverIP;
  for (i = 0; (serv_sock == NULL) && i < PWP_SOCKET_RANGE; i++) {
    SDLNet_ResolveHost(&serverIP, NULL, PWP_FILE_PORT + i);
    serv_sock = SDLNet_TCP_Open(&serverIP);
  }
  if (serv_sock == NULL) {
    SDLNet_SetError("Couldn't create server socket: %s\n",
						SDLNet_GetError());
    return 2;
  }
  serv_port = PWP_FILE_PORT + i - 1;
  
  csocket_set = SDLNet_AllocSocketSet(1 + 1 + 1 + 1);
  if (csocket_set == NULL) {
    SDLNet_SetError("Couldn't create socket set: %s\n",
						SDLNet_GetError());
    return 3;
  }
  
  SDLNet_UDP_AddSocket(csocket_set, game_csock);
  SDLNet_UDP_AddSocket(csocket_set, msg_csock);
  SDLNet_TCP_AddSocket(csocket_set, serv_sock);
  
  net_client_status = NS_INITED;
  client_info.active = false;
  return 0;
}

int CL_End()
{
  if (net_client_status == NS_UNINITED)
    return 0;
  if (net_client_status == NS_CONNECTED)
    CL_Disconnect();
  
  server.status = CS_UNUSED;
  if (server.file_sock) {
    SDLNet_TCP_Close(server.file_sock);
    server.file_sock = NULL;
  }
  free(server.buffer);
  server.buffer = NULL;
  free(server.quantum);
  server.quantum = NULL;
  DestroyPool(&server.msg_pool);
  DestroyPool(&server.game_pool);
  
  if (game_csock != NULL) {
    SDLNet_UDP_Close(game_csock);
    game_csock = NULL;
  }
  if (msg_csock != NULL) {
    SDLNet_UDP_Close(msg_csock);
    msg_csock = NULL;
  }
  if (serv_sock != NULL) {
    SDLNet_TCP_Close(serv_sock);
    serv_sock = NULL;
  }
  if (csocket_set != NULL) {
    SDLNet_FreeSocketSet(csocket_set);
    csocket_set = NULL;
  }
  if (game_cpackets != NULL) {
    SDLNet_FreePacketV(game_cpackets);
    game_cpackets = NULL;
  }
  if (game_cpacket != NULL) {
    SDLNet_FreePacket(game_cpacket);
    game_cpacket = NULL;
  }
  if (msg_cpackets != NULL) {
    SDLNet_FreePacketV(msg_cpackets);
    msg_cpackets = NULL;
  }
  if (msg_cpacket != NULL) {
    SDLNet_FreePacket(msg_cpacket);
    msg_cpacket = NULL;
  }
  
  net_client_status = NS_UNINITED;
  client_info.active = false;
  return 0;
}

int HandleClientMessage()
{
  int n;
  int len;
  net_msg MSG;
  Uint8 msgID;
  Uint8 slot;
  Uint8 ui8;
  Uint8 id;
  Uint8 volume;
  Uint8 priority;
  Uint32 time;
  Uint8 major, minor;
  
  n = SDLNet_UDP_RecvV(msg_csock, msg_cpackets);
  while (n-- > 0) {
    len =
      MSG.Receive(&server.msg_pool, &server.stats, msg_csock, 0,
						msg_cpackets[n], true);
    if (len <= 0)
      continue;			// bad or confirming packet
    
    // now, MSG contains valid received data
    if (msg_cpackets[n]->channel >= 0) {
      MSG >> msgID;
      switch (msgID) {
      case MSGID_PING:
        // ! empty
        // pong was sent by notification mechanism
        break;
        // WRITE chat message  
      case MSGID_CHATWRITE:
        // diassemble CHATWRITE packet
        client_info.game.UpdateGamebarSlots();
        ConOutEx(2,(char *) &MSG.buf[POS_MSG_TEXT]);
        SetMessenger((char *) &MSG.buf[POS_MSG_TEXT]);
        break;
        
      case MSGID_REMOTECON:
        switch (MSG.buf[POS_MSG_CONTYPE]) {
        case 0:
          ConOutEx(CLIENT_FONT, "$%s", &MSG.buf[POS_MSG_CON]);
          break;
        case 1:
          ConPar("$%s", &MSG.buf[POS_MSG_CON]);
          break;
        case 2:
          ConPut(1, "$%s", &MSG.buf[POS_MSG_CON]);
          break;
        }
        break;
        
        case MSGID_CHMAP:
          CL_ChangeMap((char *) &MSG.buf[POS_MSG_MAPNAME],
            (char *) &MSG.buf[POS_MSG_SCRIPTNAME]);
          break;
          
        case MSGID_ENTERGAME:
          MSG >> time;
          CL_EnterGame(time);
          break;
          
        case MSGID_SERVERFULL:
          if (net_client_status == NS_INITED) {
            ConOutEx(CLIENT_FONT, "Client: rejected - server is full");
            CL_Clear();
          } else
            ConErr(
            "%d>Client: unexpected SERVERFULL message from %s",
            client_info.ticks,
            AddrToS(&msg_cpackets[n]->address));
          break;
        case MSGID_YWDROPPED:
          if (net_client_status == NS_CONNECTED) {
            ConOutEx(CLIENT_FONT, "Client: you were kicked.");
            CL_Clear();
          } else
            ConErr(
            "%d>Client: unexpected YWDROPPED message from %s",
            client_info.ticks,
            AddrToS(&msg_cpackets[n]->address));
          break;
        case MSGID_KBNOTIFY:
          if (net_client_status == NS_CONNECTED) {
            MSG >> slot;
            CL_FreeKBSlot(slot);
          } else
            ConErr(
            "%d>Client: unexpected KBNOTIFY message from %s",
            client_info.ticks,
            AddrToS(&msg_cpackets[n]->address));
          break;
          
        case MSGID_CHASEPLAYERCF:
          if (net_client_status == NS_CONNECTED) {
            MSG >> slot;
            CL_ChasePlayerConf(slot);
          }
        case MSGID_SOUND:
          MSG >> id >> volume;
          smPlaySample(id, volume);
          break;
        case MSGID_VOICE:
          MSG >> id >> volume >> priority;
          smPlayVoice(id, volume, priority);
          break;
        case MSGID_SERVEROFF:
          if (net_client_status == NS_INITED) {
            ConOutEx(CLIENT_FONT, "Client: rejected - server is not running");
            CL_Clear();
          } else
            ConErr(
            "%d>Client: unexpected SERVEROFF message from %s",
            client_info.ticks,
            AddrToS(&msg_cpackets[n]->address));
          break;
        case MSGID_ATTACHDONE:
          client_info.con_attach = 1;
          ConOutEx(CLIENT_FONT, "Server console attached");
          break;
        case MSGID_WELCOME:
          if (net_client_status == NS_INITED) {
            
            MSG >> client_info.client_num >> major >> minor;
            ConOutEx(CLIENT_FONT, "Welcome to %s", (char *) &MSG.buf[POS_MSG_WSNAME]);
            ConOutEx(CLIENT_FONT, "Server version: %d.%02d", major, minor);
            ConOutEx(CLIENT_FONT, "Server message: %s", (char *) &MSG.buf[POS_MSG_WSMSG]);
            if (major > VERSION_MAJOR || (major == VERSION_MAJOR && minor > VERSION_MINOR)) 
            {
              ConOutEx(CLIENT_FONT, "Server version %d.%02d is greater than version of your program %d.%02d.\n");
              ConOutEx(CLIENT_FONT, "Please download latest version of PacWars2. (%s)\n", PW_WEBADDRESS);
            } 
            else 
            {
              net_client_status = NS_CONNECTED;
              server.status = SS_ACTIVE;
              smPlayVoice(SM_PREPARE, 100, 150);
              CL_ChangeMap((char *) &MSG.buf[POS_MSG_WMAPNAME], (char *) &MSG.buf[POS_MSG_WSCRIPTNAME]);
            }
          } 
          else
            ConErr( "%d>Client: unexpected WELCOME message from %s", client_info.ticks, AddrToS(&msg_cpackets[n]->address));
          break;
          
        case MSGID_SERVERDOWN:
          if (net_client_status == NS_CONNECTED) {
            MSG >> ui8;
            switch (ui8) {
            case SD_SHUTDOWN:
            case SD_OFFLINE:
              ConOutEx(CLIENT_FONT, "Client: server is down");
              net_client_status = NS_INITED;
              client_info.active = false;
              CL_Clear();
              break;
            case SD_CHANGING_GAME:
              ConOutEx(CLIENT_FONT, "Client: game ended");
              net_client_status = NS_VIEWING_RESULTS;
              break;
            case SD_CHANGING_MAP:
              ConOutEx(CLIENT_FONT, "Client: admin changed the map");
              net_client_status = NS_INITED;
              client_info.active = false;
              CL_Clear();
              CL_Reconnect();
              break;
            }
          } else
            ConErr(
            "%d>Client: unexpected SERVERDOWN message from %s",
            client_info.ticks,
            AddrToS(&msg_cpackets[n]->address));
          
          break;
            case MSGID_SNAME:
              if (net_client_status == NS_CONNECTED) {
                memcpy(server.name, &MSG.buf[POS_MSG_SNAME], LEN_MSG_SNAME);
                ConOutEx(CLIENT_FONT, "Server name changed to \"%s\"", server.name);
              }
              break;
              
            default:
              ConErr(
                "%d>Client: Unresolved message packet (client) ID=%d from %s!\n",
                client_info.ticks,
                msg_cpackets[n]->data[POS_MSG_ID],
                AddrToS(&msg_cpackets[n]->address));
      }
    } else					//we've got unbound message
    {
      ConErr(
        "%d>Client: Unresolved unbound message packet (client) ID=%d from %s!\n",
        client_info.ticks, msg_cpackets[n]->data[POS_MSG_ID],
        AddrToS(&msg_cpackets[n]->address));
    }
  }
  
  
  return 0;
}

int CL_Connect(char *server)
{
  if (net_client_status == NS_CONNECTED) CL_Disconnect();
  
  IPaddress game_serverIP;
  IPaddress msg_serverIP;
  
  if (net_client_status != NS_UNINITED) 
  {
    SDLNet_ResolveHost(&game_serverIP, server, PWP_GAME_SPORT);
    SDLNet_ResolveHost(&msg_serverIP, server, PWP_MSG_SPORT);
    
    if (msg_serverIP.host == INADDR_NONE) {
      SDLNet_SetError("Couldn't resolve hostname\n");
      return 1;
    } else {
      char *remote_host_name = SDLNet_ResolveIP(&msg_serverIP);
      
      if (remote_host_name)
        ConOutEx(CLIENT_FONT, "Connecting to %s [%s]", /*_strlwr(*/ remote_host_name	/*) */, AddrToS(&msg_serverIP));
      else
        ConOutEx(CLIENT_FONT, "Connecting to [%s]", AddrToS(&msg_serverIP));
      
      // Bind the address to the UDP socket
      CL_Clear();
      SDLNet_UDP_Bind(msg_csock, 0, &msg_serverIP);
      SDLNet_UDP_Bind(game_csock, 0, &game_serverIP);
      
      // assemble LOGIN message
      msg_cpacket->data[POS_MSG_ID] = MSGID_LOGIN;
      msg_cpacket->data[POS_MSG_ID + 1] = 230;
      msg_cpacket->data[POS_MSG_ID + 2] = 21;
      msg_cpacket->data[POS_MSG_ID + 3] = 57;
      msg_cpacket->data[POS_MSG_ID + 4] = 127;
      strncpy((char*)&msg_cpacket->data[POS_MSG_NAME], c_name.string, PWP_MAX_CLIENT_NAME);
      msg_cpacket->data[POS_MSG_NAME + PWP_MAX_CLIENT_NAME - 1] = 0;
      msg_cpacket->len = POS_MSG_NAME + PWP_MAX_CLIENT_NAME;
      SDLNet_UDP_Send(msg_csock, 0, msg_cpacket);
      Cvar_Set("last_server", server);
    }
    return 0;
  } 
  else 
  {
    SDLNet_SetError("Client isn't started");
    return 1;
  }
}

int CL_Reconnect()
{
  return CL_Connect(last_server.string);
}

int CL_Disconnect()
{
  if (net_client_status == NS_CONNECTED) 
  {
    Uint8 buf[LEN_MSG_ID];
    net_msg MSG(buf);
    
    MSG << MSGID_LOGOUT;
    MSG.Send(&server.msg_pool, &server.stats, msg_csock, 0, true, RESEND_SYSTEM);
  }
  return CL_Clear();
}

int ChatMessage(char *m)
{
  net_msg MSG((Uint8 *) msg_cpacket->data);
  
  MSG << MSGID_CHATPOST;
  MSG.nprintf(MSG_MAX_BODY_SIZE - LEN_MSG_ID, "%s", m);
  MSG.Send(&server.msg_pool, &server.stats, msg_csock, 0, true,
			 RESEND_CHAT);
  
  return 0;
}

void PingReturn(net_message * msg)
{
  ping = SDL_GetTicks() - pingsent;
  pingreturned = true;
  
}

// sends ping client-server-client
// pong is processed by callback function
void CL_Ping()
{
  if ((net_client_status == NS_CONNECTED) && pingreturned) {
    Uint8 buf[LEN_MSG_ID];
    net_msg MSG(buf);
    
    MSG << MSGID_PING;
    MSG.Send(&server.msg_pool, &server.stats, msg_csock, 0, true,
      RESEND_PING, PingReturn);
    pingsent = SDL_GetTicks();
    pingreturned = false;
  } else
    ping = 0;
}

int CL_SetName(char *name)
{
  Uint8 sendbuf[MSG_MAX_BODY_SIZE];
  net_msg MSG(sendbuf);
  if (net_client_status == NS_CONNECTED) {
    MSG << MSGID_CNAME;
    MSG.nprintf(LEN_MSG_CNAME, "%s", name);
    MSG.Send(&server.msg_pool, &server.stats, msg_csock, 0, true,
      RESEND_SYSTEM);
  }
  // set name in server_info
  snprintf(client_info.name, LEN_MSG_CNAME, "%s", name);
  return 0;
}

int CL_Attach(char* pass)
{
  if (!client_info.con_attach) {
    Uint8 sendbuf[LEN_MSG_ID];
    net_msg MSG(sendbuf);
    
    MSG << MSGID_ATTACHCON;
    MSG.nprintf(LEN_MSG_ACPASS, "%s", pass);
    MSG.Send(&server.msg_pool, &server.stats, msg_csock, 0, true, RESEND_SYSTEM);
  } else
    ConOutEx(CLIENT_FONT, "Server console already attached");
  return 0;
}

void DetachNotify(net_message * msg)
{
  ConOutEx(CLIENT_FONT, "Server console detached");
}

int CL_Detach()
{
  if (client_info.con_attach) {
    Uint8 sendbuf[LEN_MSG_ID];
    net_msg MSG(sendbuf);
    
    MSG << MSGID_DETACHCON;
    MSG.Send(&server.msg_pool, &server.stats, msg_csock, 0, true,
      RESEND_SYSTEM, DetachNotify);
    client_info.con_attach = 0;
  } else
    ConOutEx(CLIENT_FONT, "Server console isn't attached");
  
  return 0;
}

int HandleFileResponse()
{
  char tmp[_MAX_PATH];
  
  if ((server.file_status != FS_WAIT_RECEIVING)
    && (server.file_status != FS_WAIT_SENDING)) {
    ConErr( "%d>unexpected file transfer response !!",
      client_info.ticks);
    return 1;
  }
  
  server.file_sock = SDLNet_TCP_Accept(serv_sock);
  if (server.file_sock == NULL)
    return 3;
  
  SDLNet_TCP_AddSocket(csocket_set, server.file_sock);
  if (server.file_status == FS_WAIT_RECEIVING) {	// RECEIVING = DOWNLOADING from server
    
    // check if file exists
    server.file = fopen(server.local_fn, "rb");
    if (server.file) {
      ConOutEx(CLIENT_FONT, "Client: local file %s exists\n", server.local_fn);
      // TODO: report error to client
      CL_ClearFileTransfer(FT_ABANDONED);
      return 2;
    }
    // open temp file for writing
    TempName(server.local_fn, tmp);
    server.file = fopen(tmp, "wb");
    if (!server.file) {
      ConOutEx(CLIENT_FONT, "Client: couldn't open local file for writing");
      CL_ClearFileTransfer(FT_ABANDONED);
      return 1;
    }
    server.zs.zalloc = (alloc_func) 0;
    server.zs.zfree = (free_func) 0;
    server.zs.opaque = (voidpf) 0;
    
    int err = inflateInit(&server.zs);
    if (err != Z_OK) {
      ConOutEx(CLIENT_FONT, "stream initialization failed (client): %d", err);
      CL_ClearFileTransfer(FT_ABANDONED);
      return 5;
    }
    
    server.zs.avail_out = server.bsize;
    server.zs.next_out = (Bytef *) server.buffer;
    server.file_status = FS_RECEIVING;
    
    lcreCL_time = SDL_GetTicks();
  } else {					// SENDING = UPLOADING to server
    
    // check if file exists
    server.file = fopen(server.local_fn, "rb");
    if (!server.file) {
      ConOutEx(CLIENT_FONT, "Client: local file doesn't exist");
      CL_ClearFileTransfer(FT_ABANDONED);
      return 1;
    }
    
    server.zs.zalloc = (alloc_func) 0;
    server.zs.zfree = (free_func) 0;
    server.zs.opaque = (voidpf) 0;
    
    int err = deflateInit(&server.zs, PWP_UPLOAD_COMPRESSION);
    if (err != Z_OK) {
      ConOutEx(CLIENT_FONT, "stream initialization failed (client): %d", err);
      CL_ClearFileTransfer(FT_ABANDONED);
      return 2;
    }
    server.zs.avail_in = 0;
    server.zs.next_in = (Bytef *) server.buffer;
    server.file_status = FS_SENDING;
  }
  return 0;
}

int CL_Poll()
{
  int count;
  int transfered;
  
  if (net_client_status != NS_UNINITED) {
    SDLNet_CheckSockets(csocket_set, 0);
    if (SDLNet_SocketReady(msg_csock)) {
      HandleClientMessage();
    }
    
    if (net_client_status == NS_CONNECTED) {
      if (server.status != SS_UNUSED) {
        // resend lost messages
        ResendMsgs(&server.msg_pool, msg_csock);
        
        // check for file transfer connection
        if (SDLNet_SocketReady(serv_sock))
          HandleFileResponse();
        
        Uint32 time = SDL_GetTicks();
        if (!pingreturned
          && ((time - pingsent) >=
          (Uint32) net_ping_timeout.value)) {
          CL_ServerCrashed();
        }
        // manage file transfers
        if ((server.file_status == FS_SENDING)
          || (server.file_status == FS_RECEIVING)) {
          if (server.file_status == FS_SENDING) {	// SENDING
            count =
              LoadFileBuffer(&server.zs, server.quantum,
              server.qsize, server.buffer,
              server.bsize, server.file,
              &server.file_status);
            if (count) {
              transfered =
                SDLNet_TCP_Send(server.file_sock,
                (char *) server.quantum,
                count);
              if (transfered < count) {
                ConOut
                  ("File sending - connection lost (server)");
                server.file_status = FS_ENDED;
              }
              client_sent += transfered;
              
            }
            // test for transfer result
            if ((server.file_status == FS_ENDED)
              || (server.file_status == FS_COMPLETED)) {
              if (server.file_status == FS_COMPLETED)
                ConOut
                ("Client: File transfer done (%d/%d)",
                server.zs.total_out,
                server.zs.total_in);
              
              // clean up
              deflateEnd(&server.zs);
              CL_ClearFileTransfer(FT_FINISHED);
            }
            
          } else {	// RECEIVING
            if (SDLNet_SocketReady(server.file_sock)) {
              count =
                SDLNet_TCP_Recv(server.file_sock,
                (char *) server.quantum,
                server.qsize);
              if (count <= 0) {
                ConOut
                  ("File receiving - connection lost (client)");
                server.file_status = FS_ENDED;
              } else {
                StoreFileBuffer(&server.zs, server.quantum,
                  count, server.buffer,
                  server.bsize, server.file,
                  &server.file_status);
                client_received += count;
                lcreCL_time = SDL_GetTicks();
              }
            }
            // check for timeouts
            if ((SDL_GetTicks() - lcreCL_time) >=
              net_file_timeout.value) {
              ConOutEx(CLIENT_FONT, "Client: File transfer timed out !");
              CL_ClearFileTransfer(FT_ABANDONED);
            }
            // test for transfer result
            if ((server.file_status == FS_ENDED)
              || (server.file_status == FS_COMPLETED)) {
              if (server.file_status == FS_COMPLETED)
                ConOut
                ("Client: File transfer done (%d/%d)",
                server.zs.total_in,
                server.zs.total_out);
              
              // clean up
              inflateEnd(&server.zs);
              CL_ClearFileTransfer(FT_FINISHED_RENAME);
              client_info.ScheduleNewDlRequest();
            }
          }
        }
      }
    }
  }
  return 0;
}

int CL_Upload(Uint8 ft_id, char *fname)
{
  if (!c_uploading.value) {
    ConOut
      ("Client: uploading is not enabled, set \"c_uploading\" variable to 1");
    return 3;
  }
  
  if (server.file_status == FS_READY) {
    if (!CheckFileName(fname)) {
      ConOutEx(CLIENT_FONT, "Filename must be simple <filename.ext>");
      return 2;
    }
    
    Uint8 sendbuf[MSG_MAX_BODY_SIZE];
    net_msg MSG(sendbuf);
    MSG << MSGID_UPLOAD;
    MSG << serv_port;
    MSG << ft_id;
    MSG.nprintf(LEN_MSG_FN, "%s", fname);
    MSG.Send(&server.msg_pool, &server.stats, msg_csock, 0, true,
      RESEND_SYSTEM);
    
    DLIDtoFileName(ft_id, fname, server.local_fn);
    server.file_status = FS_WAIT_SENDING;
    return 0;
  } else
    return 1;
}

int CL_Download(Uint8 ft_id, char *fname)
{
  if (!c_downloading.value) {
    ConOut
      ("Client: downloading is not enabled, set \"c_downloading\" variable to 1");
    return 3;
  }
  
  if (server.file_status == FS_READY) {
    if (!CheckFileName(fname)) {
      ConOutEx(CLIENT_FONT, "Client: filename must be simple <filename.ext>");
      return 2;
    }
    
    Uint8 sendbuf[MSG_MAX_BODY_SIZE];
    net_msg MSG(sendbuf);
    MSG << MSGID_DOWNLOAD;
    MSG << serv_port;
    MSG << ft_id;
    MSG.nprintf(LEN_MSG_FN, "%s", fname);
    MSG.Send(&server.msg_pool, &server.stats, msg_csock, 0, true,
      RESEND_SYSTEM);
    
    DLIDtoFileName(ft_id, fname, server.local_fn);
    server.file_status = FS_WAIT_RECEIVING;
    return 0;
  } else
    return 1;
}

// called after 
void CL_EnterGame(Uint32 time)
{
  Uint32 atime = SDL_GetTicks();
  // -- woid, no delta works better, server is one step before all clients
  // seems there is a bug with client timer which runs before server's one -> causes unconsistent game state
  Uint32 delta = 0; //(atime - entersent) / 2; 
  
  ClearPool(&server.game_pool);
  
  // active client
  client_info.active = true;
  client_info.ticks = 0;
  client_info.pticks = PWP_TICKS_PER_PACKET_CLIENT;
  client_info.start_time = atime - (time + delta);
  
  // simulate all map animations from server start
  Uint32 ctime = time + delta;
  while (ctime > FRAMETICKS) {
    client_info.ticks++;
    client_info.game.map.UpdateAnims();
    ctime -= FRAMETICKS;
    client_info.pticks--;
    if (client_info.pticks == 0)
      client_info.pticks = PWP_TICKS_PER_PACKET_CLIENT;
  }
  
  // synchronize time with server
  client_info.last_time = time + delta - ctime;
}
 
int CL_ChangeMap(char *mapname, char *scriptname)
{
  // dispose old game info
  client_info.game.Clear();
  client_info.game.Reset();
  client_info.game.state = GS_CLIENT;
  
  // load & set new map
  int res = MapLoad(mapname, scriptname);
  if (MapLoaded == 0) 
  {
    if (res != -10) 
    {
      ConOutEx(CLIENT_FONT, "Client: map %s not found !", mapname);
      ConOutEx(CLIENT_FONT, "Ask for downloading map by typing \"dlmap %s\" in console", mapname);
      
      char s[100];
      sprintf(s, "%s.%s", mapname, map_ext.string);
      client_info.AddDlRequest(DLID_MAP, s);
    } 
    else 
    {
      ConOutEx(CLIENT_FONT, "Client: script %s not found or not correct !", scriptname);
      ConOutEx(CLIENT_FONT, "Ask for downloading script by typing \"dlscript %s\" in console", scriptname);
      
      char s[100];
      sprintf(s, "%s.%s", scriptname, smap_ext.string);
      client_info.AddDlRequest(DLID_SCRIPT, s);
    }
    //CL_Disconnect();
    return 1;
  }
  client_info.game.map.Copy(&MapLoadedState);
  // init game
  client_info.game.vars.camx = 0;
  client_info.game.vars.camy = 0;
  client_info.game.state = GS_CLIENT;
  MapLoaded = 2;
  client_info.game.map.InitAnims();
  client_info.game.map.GenerateYLookup();
  ScriptMan->RunScript(map_script_num, "csInit");
  
  // send accomplished message
  Uint8 sendbuf[LEN_MSG_ID];
  net_msg MSG(sendbuf);
  MSG << MSGID_CHMAPREADY;
  MSG.Send(&server.msg_pool, &server.stats, msg_csock, 0, true, RESEND_ENTERINGGAME);
  entersent = SDL_GetTicks();
  
  return 0;
}

char CL_ParseReplication(net_msg * msg, TICK_TYPE time, CGame & game, int client)
{
  Uint8 rep_code = REP_END + 1;
  
  GObj *obj;
  GPlayer *player;
  Uint16 oid;
  
  GAME_MAXOBJS_TYPE slot;
  TICK_TYPE tick;
  
  while (rep_code != REP_END) {
    (*msg) >> rep_code;
GOT_REPCODE_IN_REPLICATION_CYCLE:
    switch (rep_code) {
      
    case REP_ADJUSTPOSITION:
      Uint16 x, y;
      (*msg) >> slot >> x >> y >> tick >> oid;
      
      obj = game.objs[slot];
      if (obj->GetType() != ot_player || oid != game.objs[slot]->oid)	// is the replication for new object ?
      {
        ConErr("%d>Client: error CL_ParseReplication ADJUSTPOSITION called for non-player object slot=%d\n", client_info.ticks, slot);
        // creation of object
        game.RebornObject(oid, slot, ot_player);	// reborn was made by ServerReplicate 
      }

      obj = game.objs[slot];
      player = (GPlayer *) obj;
      // adjust position + prediction
      player->AdjustPosition(x, y, tick);
      break;
      
    case REP_REPLICATION:
      // force object to get replicated
      Uint8 ot;
      (*msg) >> slot >> oid >> ot;
      
      if (oid != game.objs[slot]->oid)	// is the replication for new object ?
      {
        // creation of object
        game.RebornObject(oid, slot, ot);	// reborn was made by ServerReplicate 
      }
      
      obj = game.objs[slot];
      if (obj->GetType() != ot)
        ConErr(
        "%d>Client: error CL_ParseReplication REP_REPLICATION: nonconsistent object types %d",
        client_info.ticks, ot);
      
      (*msg) >> rep_code;
      while (rep_code < REP_CODEBASE) {
        obj->GetReplicated(rep_code, msg, time);
        (*msg) >> rep_code;
      }
      goto GOT_REPCODE_IN_REPLICATION_CYCLE;
      
      break;
      
    case REP_END:			//skipping
      break;
      
    case REP_SERVERMOVE:
      ConErr(
        "%d>Client: error CL_ParseReplication forbiden rep_code=%d\n",
        client_info.ticks, rep_code);
      break;
      
    default:
      ConErr(
        "%d>Client: error CL_ParseReplication unknown rep_code=%d\n",
        client_info.ticks, rep_code);
    }
  }
  return 1;
}


int HandleClientGame(Uint32 deltaticks)
{
  int n;
  int len;
  net_msg GMP;
  TICK_TYPE incoming_tick;
  
  n = SDLNet_UDP_RecvV(game_csock, game_cpackets);
  while (n-- > 0) {
    len =
      GMP.Receive(&server.game_pool, &server.stats, game_csock, 0,
						game_cpackets[n], false);
    if (len <= 0)
      continue;			// bad or confirming packet
    
    // now, GMP contains valid received data
    // let's parse server's packet
    GMP >> incoming_tick;
    //    if (client_info.ticks+deltaticks>=incoming_tick)
    {
      if (!CL_ParseReplication
        (&GMP, incoming_tick, client_info.game,
        client_info.client_num)) {
        ConErr(
          "%d>Client: error parsing server replication - incomingtick=%d !\n",
          client_info.ticks, incoming_tick);
        continue;
      }
    }
    //    else 
    {
      //TODO: vratit message do fronty
      // to se nevi jiste
    }
  }
  
  return n;
}


void CL_Move(Uint32 ticktime)
{
  Uint32 delta =
    ticktime - client_info.start_time - client_info.last_time;
  Uint8 buffer[MSG_MAX_BODY_SIZE];
  net_msg GP(buffer);
  
  if (pingreturned && ticktime - pingsent >= PING_MEASURE)
    CL_Ping();
  
  while (delta > FRAMETICKS) {
    SDLNet_CheckSockets(csocket_set, 0);
    if (SDLNet_SocketReady(game_csock)) {
      HandleClientGame(delta / FRAMETICKS);
    }
    // resend lost messages
    ResendMsgs(&server.game_pool, game_csock);
    
    client_info.ticks++;
    // TODO: zpracuj gamestate a replication, ktere nalezi do tohoto ticku
    // (kazda promenna ma u sebe tick, kdy byla naposledy replikovana)
    // To bylo provedeno uz v HandleClientGame
    
    // updatuj animaci
    client_info.game.map.UpdateAnims();
    
    // simuluj objekty (kazdy objekt ma u sebe tick, kdy byl naposledy simulovan)
    // ovladani pacove replikuji svoje pohybove vektory
    // (=zpracuj vstup od uzivatele / pust MOVEAUTONOMOUS na jeho postavicku)
    for (int j = 0; j < THINKINGS_PER_TICK; j++) {
      client_info.game.ClientThink(client_info.ticks);
      ScriptMan->RunScript(map_script_num, "csTick");	// let script do gametick stuff
    }
    
    // update camera position
    ScriptMan->scripts[map_script_num].RunScript("csCamera");
    
    delta -= FRAMETICKS;
    
    client_info.pticks--;
    if (client_info.pticks == 0) {
      client_info.pticks = PWP_TICKS_PER_PACKET_CLIENT;
      
      // do replication process
      // replicate servers's data according to server game state
      client_info.game.ClientReplicate(client_info.ticks);
      
      if (client_info.game.replicator.dirty)	// does server need my update ?
      {
        // send game packets to server
        Uint8 more = 1;
        
        while (more) {
          GP.pos = 0;
          GP << client_info.ticks;
          more =
            client_info.game.replicator.AssembleMsg(&GP,
            MSG_MAX_BODY_SIZE
            -
            sizeof
            (client_info.ticks));
          GP.Send(&server.game_pool, &server.stats, game_csock,
            0, false);
        }
        
        // reset replicator
        client_info.game.replicator.Reset();
      }
    }
  }
  
  client_info.last_time = ticktime - client_info.start_time - delta;
}

int CL_CreatePlayer(char *name, Uint8 keybindings)
{
  Uint8 sendbuf[200];
  net_msg MSG(sendbuf);
  
  MSG << MSGID_CREATEPLAYER;
  MSG << (Uint8) bt_client;
  MSG << keybindings;
  MSG.nprintf(LEN_MSG_PLNAME, "%s", name);
  
  MSG.Send(&server.msg_pool, &server.stats, msg_csock, 0, true,
			 RESEND_SYSTEM);
  
  smPlayVoice(SM_FIGHT1 + rand() % 2, 100, 5);
  return 0;
}

int CL_DestroyPlayer(char *name)
{
  Uint8 sendbuf[200];
  net_msg MSG(sendbuf);
  
  MSG << MSGID_DESTROYPLAYER;
  MSG.nprintf(PWP_MAX_PLAYER_NAME, "%s", name);
  
  MSG.Send(&server.msg_pool, &server.stats, msg_csock, 0, true,
			 RESEND_SYSTEM);
  
  return 0;
}

int CL_SkinPlayer(char *name, char *spritename)
{
  Uint8 sendbuf[200];
  net_msg MSG(sendbuf);
  
  MSG << MSGID_SKINPLAYER;
  MSG.nprintf(PWP_MAX_PLAYER_NAME, "%s", name);
  MSG.pos = POS_MSG_SKIN_SPRITENAME;
  MSG.nprintf(MAX_SPRITE_NAME, "%s", spritename);
  
  MSG.Send(&server.msg_pool, &server.stats, msg_csock, 0, true,
			 RESEND_SYSTEM);
  
  return 0;
}

int CL_ChasePlayerConf(int i)
{
  if ((client_info.game.objs[i]->state & OSTATE_ACTIVE)
    && (client_info.game.objs[i]->GetType() == ot_player)) {
    client_info.game.vars.chasing = i;
    ConOutEx(CLIENT_FONT, "Chasing player \"%s\"",
			   ((GPlayer *) client_info.game.objs[i])->
         player_name.GetValRef()->chars);
  } else {
    ConErr("Chasing player: something went wrong ");
  }
  return 0;
}

int CL_ChasePlayer(char *name)
{
  GPlayer *p;
  CGame & g = client_info.game;
  GAME_MAXOBJS_TYPE i = 0;
  
  for (i = 0; i < GAME_MAX_OBJS; i++) {
    if ((g.objs[i]->state & OSTATE_ACTIVE)
      && (g.objs[i]->GetType() == ot_player)) {
      p = (GPlayer *) g.objs[i];
      if (strcmp(p->player_name.GetValRef()->chars, name) == 0)
        break;
    }
  }
  
  if (i < GAME_MAX_OBJS) {
    Uint8 sendbuf[200];
    net_msg MSG(sendbuf);
    MSG << MSGID_CHASEPLAYERRQ << i;
    MSG.Send(&server.msg_pool, &server.stats, msg_csock, 0, true,
      RESEND_GAME);
  } else {
    ConOutEx(CLIENT_FONT, "Player \"%s\" not found", name);
  }
  
  return 0;
}


void CL_ServerCrashed()
{
  ConOutEx(CLIENT_FONT, "Client: Server crashed");
  CL_Clear();
}
