/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   Main program
//## author:  Antonin Hildebrand
//## 
//## started: 2.3.2000
//## revised: 2.3.2001
//## 
//## main source file
//## 
//###########################################################################

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "const.h"
#undef DrawText
#undef SendMessage

#include <gui.h>
/*
#include <sdlwidget.h>
#include <sdlapplication.h>
#include <sdlbutton.h>
#include <sdlbuttongroup.h>
#include <sdlwidgetlist.h>
#include <sdllabel.h>
#include <sdlwindow.h>
#include <sdllineedit.h>
#include <sdlscrollbar.h>
#include <sdlprogressbar.h>
#include <sdlradiobutton.h>
#include <sdlcheckbutton.h>
*/

//#include <crtdbg.h>
#include "mapman.h"
#include "smapman.h"

//###########################################################################
//## Constants & defines
//###########################################################################
#define PNG_EXT "rst"			// raist image format ;)

//###########################################################################
//## Dependent includes
//###########################################################################

#include "pw2.h"

#include "sampleman.h"

#ifdef DISABLE_BASS
#undef PW_BASS
#endif

#ifdef PW_AUDIO
#ifdef PW_BASS
#include "bass.h"
#else
#include "SDL_mixer.h"
#endif
#endif

#include "map.h"
#include "gamedefs.h"
#include "messenger.h"
#include "mouse.h"
#include "chat.h"
#include "stdconfig.h"

#undef DrawText					// must be here due to bass includes windows.h, which defines macro DrawText different from our function
#undef SendMessage				// must be here due to bass includes windows.h, which defines macro SendMessege different from our function
#include "SDL_DrawText.h"

#include "bldnum.h"

//###########################################################################
//## Globals
//###########################################################################

SDL_Color GUI_BackColor = { 0, 0, 0 };
SDL_Color GUI_ProgressColor = { 202, 49, 2 };

//###########################################################################
//## Globals
//###########################################################################


SDL_Surface *splash;
SDL_Surface *splashA;
SDL_Surface *splashB;
SDL_Surface *menu_background;
SDL_Surface *results;
SDL_Surface *credits;
SDL_Surface *help1;
SDL_Surface *help2;
SDL_Surface *help3;
SDL_Surface *udbar;

int sound_hw_enabled = 1;
int sound_3d_enabled = 1;
  

bool enable_menu_music = 1;

int MainProgram = 1;			// 0 = quitting
bool inloop = 0;
// 1 = active 
// 2 = iconified

PG_Application *pPW2app;
Uint8 video_bpp;
SDL_Surface *screen;
const SDL_VideoInfo *VideoInfo = NULL;

char PW_CONFIG_FILE_PERSONAL[255];

#ifdef PW_MUSIC
static int audio_open = 0;
char playing_music[250] = "none";

#ifdef PW_BASS
static HMUSIC music = NULL;
#else
static Mix_Music *music = NULL;
#endif
#endif

bool ConsoleDown = false;
bool NetStatsDown = false;
bool ServerView = false;
bool InfoDown = false;

int SmallFont;
int net_font;

class GUI_OKDialog1;
class GUI_OKDialog2;
class GUI_YNDialog;
GUI_OKDialog1 *OKD1;
GUI_OKDialog2 *OKD2;
GUI_YNDialog *YND;

// timing
Uint32 firsttick;
Uint32 ticktime;

bool waiting_connection = false;
bool want_quit = false;
int frames = 0, oldframes = 0;



// auto logging sequence
char auto_playername[4][PWP_MAX_PLAYER_NAME];
Uint32 time_playername;
char auto_skinname[4][MAX_SPRITE_NAME];
char auto_skinplayername[4][PWP_MAX_PLAYER_NAME];
Uint32 time_skinname;
char auto_chasename[4][PWP_MAX_PLAYER_NAME];
Uint32 time_chasename;

// temps
char tmptxt[_MAX_PATH];



//###########################################################################
//## CVARS
//###########################################################################

void CHmusic_volume(cvar_t * var, int *changed)
{
  Volume_Music(var->string);
}

void CHsound_volume(cvar_t * var, int *changed)
{
  Volume_Sound(var->string);
}

void CHgamma(cvar_t * var, int *changed)
{
  ResetGamma();
}

cvar_t last_server = { "last_server", "localhost", true };
cvar_t menu_music_file = { "menu_music_file", "pacwars2.it", true };
cvar_t music_volume =
{ "music_volume", "40", true, false, CHmusic_volume };
cvar_t sound_volume =
{ "sound_volume", "50", true, false, CHsound_volume };

cvar_t fullscreen = { "fullscreen", "1", true };
cvar_t pw2_gamma_r = { "gamma_r", "1", true, false, CHgamma };
cvar_t pw2_gamma_b = { "gamma_b", "1", true, false, CHgamma };
cvar_t pw2_gamma_g = { "gamma_g", "1", true, false, CHgamma };
cvar_t doublebuf = { "doublebuf", "1", true };
cvar_t glblit = { "glblit", "0", true };
cvar_t alphamenu = { "alphamenu", "0", true };
cvar_t autoserver = {"autosever", "0", true };
cvar_t theme = {"theme", DEFAULT_THEME, true };

cvar_t autolog = { "autolog", "0", true };	// automaticke rozjeti serveru a vytvoreni hrace po spusteni

void ResetGamma()
{
  ConOut("Reseting gamma");
  SDL_SetGamma(pw2_gamma_r.value, pw2_gamma_g.value, pw2_gamma_b.value);
}


void CEndClient(char *s);
void CEndServer(char *s);

void App_Quit()
{
  //  quitting sequence
  CEndClient(NULL);
  CEndServer(NULL);
  enable_menu_music = 0;
  MainProgram = 0;
  smPlayVoice(SM_BYEBYE, 200);
}

void App_Activate()
{
  if (MainProgram == 2) {
    //Play_Music("");
#ifdef PW_BASS
    BASS_Start();
#endif
    if (MapLoaded)
      MapRestore();
    SkinMan.Restore();
    SpriteMan.Restore();
    MainProgram = 1;
  }
}

void App_Deactivate()
{
  if (MainProgram == 1) {
    //Pause_Music("");
#ifdef PW_BASS
    BASS_Pause();
#endif
    MainProgram = 2;
  }
}

#define NS_PX 0
#define NS_PY 0

void DrawNetStats()
{
  
  char line[1024];
  char out[1024];
  
  int y = NS_PY;
  int i;
  int c;
  int d;
  
  // draw server part
  
  if (net_server_status == NS_RUNNING) {
    sprintf(out, " %-79s", "");
    DrawText(out, screen, net_font, NS_PX, y);
    y += 13;
    for (i = 0, c = 0, d = 0; i < server_info.maxclients; i++) {
      if (client[i].status != CS_UNUSED)
        c += UnconfirmedMsgs(&client[i].msg_pool);
      if (client[i].status != CS_UNUSED)
        d += UnconfirmedMsgs(&client[i].game_pool);
    }
    sprintf(line,
      "SERVER[%05d]: msgUCM=%d, gameUCM=%d, fsent=%d, freceived=%d",
      server_info.ticks, c, d, server_sent, server_received);
    sprintf(out, " %-79s", line);
    DrawText(out, screen, net_font, NS_PX, y);
    y += 13;
    sprintf(line,
      "NUM  NAME         STATUS RECEIVED     SENT   ERR  DROP   DUP LAST TIME");
    sprintf(out, " %-79s", line);
    DrawText(out, screen, net_font, NS_PX, y);
    y += 13;
    for (i = 0, c = 0; i < server_info.maxclients; i++) {
      if (client[i].status != CS_UNUSED) {
        sprintf(line, "[%02d] %-12s %6d %8d %8d %5d %5d %5d %9d",
          i, client[i].name, client[i].status,
          client[i].stats.PacketsReceived,
          client[i].stats.PacketsSent,
          client[i].stats.errCRC, client[i].stats.errDropped,
          client[i].stats.errDuplicity,
          client[i].stats.LastRecTime);
        sprintf(out, " %-79s", line);
        DrawText(out, screen, net_font, NS_PX, y);
      } else {
        sprintf(line, "[%02d]", i, client[i].name,
          client[i].status, client[i].stats.PacketsReceived,
          client[i].stats.PacketsSent,
          client[i].stats.errCRC, client[i].stats.errDropped,
          client[i].stats.errDuplicity,
          client[i].stats.LastRecTime);
        sprintf(out, " %-79s", line);
        DrawText(out, screen, net_font, NS_PX, y);
      }
      y += 13;
    }
  }
  
  if (net_client_status == NS_CONNECTED) {
    sprintf(out, " %-79s", "");
    DrawText(out, screen, net_font, NS_PX, y);
    y += 13;
    
    // print #packets waiting confirmation (client)
    c = UnconfirmedMsgs(&server.msg_pool);
    d = UnconfirmedMsgs(&server.game_pool);
    sprintf(line,
      "CLIENT[%05d]: msgUCM=%d, gameUCM=%d, fsent=%d, freceived=%d",
      client_info.ticks, c, d, client_sent, client_received);
    sprintf(out, " %-79s", line);
    DrawText(out, screen, net_font, NS_PX, y);
    y += 13;
    sprintf(line, "Server name: %s", s_name.string);
    sprintf(out, " %-79s", line);
    DrawText(out, screen, net_font, NS_PX, y);
    y += 13;
    sprintf(line,
      "PING              STATUS RECEIVED     SENT   ERR  DROP   DUP LAST TIME");
    sprintf(out, " %-79s", line);
    DrawText(out, screen, net_font, NS_PX, y);
    y += 13;
    
    sprintf(line, "%-17d %6d %8d %8d %5d %5d %5d %9d", ping,
      server.status, server.stats.PacketsReceived,
      server.stats.PacketsSent, server.stats.errCRC,
      server.stats.errDropped, server.stats.errDuplicity,
      server.stats.LastRecTime);
    sprintf(out, " %-79s", line);
    DrawText(out, screen, net_font, NS_PX, y);
    y += 13;
  }
  
}

/////////////////////////////////////////////////////////////////////////////
// temporary ...
/////////////////////////////////////////////////////////////////////////////

SDL_Surface *LoadPic(char *fname)
{
  SDL_Surface *Temp = IMG_Load(fname);
  if (!Temp) {
    fprintf(stderr, "Image %s couldn't be loaded.\n", fname);
    // try to use empty bitmap
    return SDL_AllocSurface(SDL_SWSURFACE | SDL_SRCCOLORKEY, 10, 10,
      video_bpp, 0, 0, 0, 0);
  }
//  SDL_Surface *Converted = SDL_DisplayFormat(Temp);
//  SDL_FreeSurface(Temp);
//  return Converted;
  return Temp;
}

void DrawGameBack(SDL_Surface * screen)
{
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, GUI_BackColor.r, GUI_BackColor.g, GUI_BackColor.b));
}

void DrawGUIBack(SDL_Surface * screen)
{
  SDL_Rect r;
  r.x = 0;
  r.y = 0;
  r.h = menu_background->h;
  r.w = menu_background->w;
  SDL_BlitSurface(menu_background, NULL, screen, &r);
}

void DrawResultsBack(SDL_Surface * screen)
{
  SDL_Rect r;
  r.x = 0;
  r.y = 0;
  r.h = results->h;
  r.w = results->w;
  SDL_BlitSurface(results, NULL, screen, &r);
}

void UpdateSplash(SDL_Surface * screen, int progress)
{
  //DrawGUIBack(screen);
  
  // draw splash screen
  SDL_Rect r;
  int sx = (screen->w - splash->w) / 2;
  int sy = (screen->h - splash->h) / 2;
  
  r.x = sx;
  r.y = sy;
  r.h = splash->h;
  r.w = splash->w;
  SDL_BlitSurface(splash, NULL, screen, &r);
  
  if (progress == -1) {
    r.x = sx + 158;
    r.y = sy + 59;
    r.w = splashA->w;
    r.h = splashA->h;
    if ((SDL_GetTicks() / 200) % 2 == 0)
      SDL_BlitSurface(splashA, NULL, screen, &r);
    else
      SDL_BlitSurface(splashB, NULL, screen, &r);
    progress = 100;
  }
  
  
  {
    r.x = sx + 334;
    r.y = sy + 458;
    r.w = (unsigned short) ((298 / 100.0) * progress);
    r.h = 13;
    SDL_FillRect(screen, &r,
					 SDL_MapRGB(screen->format, GUI_ProgressColor.r,
           GUI_ProgressColor.g, GUI_ProgressColor.b));
  }
  SDL_Flip(screen);
}

void RenderMapScreen(CGame & game)
{
  SDL_Rect view;
  
  if (game.map.DrawBG(screen, game.vars.camx, game.vars.camy) == 1) {
    //    MapRestore();
  }
  // drawsprites
  view.x = MMOX;
  view.y = MMOY;
  view.w = MSCRW;
  view.h = MSCRH;
  for (GAME_MAXOBJS_TYPE i = 0; i < GAME_MAX_OBJS; i++) {
    if (game.objs[i]->state & OSTATE_ACTIVE)
      game.objs[i]->Draw(screen, &view);
  }
  
  game.map.DrawFG(screen, game.vars.camx, game.vars.camy);
  {
    //    MapRestore();
  }
}


//###########################################################################
//## Console commands
//## 
//## 
//###########################################################################


/////////////////////////////////////////////////////////////////////////////
// help function
/////////////////////////////////////////////////////////////////////////////
void Help(char *string)
{
  char param[MAX_WORD_INPUTED];
  
  // detect input format & load values into string variables
  if (sscanf(string, "%s", param) < 1) {
    ConOut("Usage: help <command>");
    ConOutEx(CHAT_FONT, "To list all commands available type \"list\"");
    ConOutEx(CHAT_FONT, "<param> means param, which must be explicitly set");
    ConOutEx(CHAT_FONT, "(param) means param, which needn't be explicitly set, help describes implicit value");
  } else {
    if (strcmp(param, "quit") == 0) {
      ConOut("Usage: quit");
      ConOutEx(CHAT_FONT, "Desc: quits game immediatelly");
    } else if (strcmp(param, "list") == 0) {
      ConOut("Usage: list");
      ConOutEx(CHAT_FONT, "Desc: outputs list of all commands available");
    } else if (strcmp(param, "video") == 0) {
      ConOut("Usage: video");
      ConOutEx(CHAT_FONT, "Desc: writes info about video hardware");
    } else if (strcmp(param, "map") == 0) {
      ConOut("Usage: map <map filename without extension> (script name without extension)");
      ConOutEx(CHAT_FONT, "Desc: loads and inits new game map with given script, old map and script are released");
    } else if (strcmp(param, "mapinfo") == 0) {
      ConOut("Usage: mapinfo");
      ConOutEx(CHAT_FONT, "Desc: writes info about currently loaded map");
    } else if (strcmp(param, "sprite_scan") == 0) {
      ConOut("Usage: sprite_scan");
      ConOutEx(CHAT_FONT, "Desc: searches sprites directory for new sprites");
      ConOutEx(CHAT_FONT, "      directory is stored in cvar \"sprite_dir\"");
    } else if (strcmp(param, "sprite_list") == 0) {
      ConOut("Usage: sprite_list");
      ConOutEx(CHAT_FONT, "Desc: shows list of all sprites available");
    } else if (strcmp(param, "sprite_load") == 0) {
      ConOut("Usage: sprite_load <name>");
      ConOutEx(CHAT_FONT, "Desc: loads sprite <name> in memory");
    } else if (strcmp(param, "sprite_scan") == 0) {
      ConOut("Usage: sprite_free <name>");
      ConOutEx(CHAT_FONT, "Desc: free sprite <name> from memory");
    } else if (strcmp(param, "skin_scan") == 0) {
      ConOut("Usage: skin_scan");
      ConOutEx(CHAT_FONT, "Desc: searches skins directory for new skins");
      ConOutEx(CHAT_FONT, "      directory is stored in cvar \"skin_dir\"");
    } else if (strcmp(param, "skin_list") == 0) {
      ConOut("Usage: skin_list");
      ConOutEx(CHAT_FONT, "Desc: shows list of all skins available");
    } else if (strcmp(param, "sc") == 0) {
      ConOut("Usage: sc");
      ConOutEx(CHAT_FONT, "Desc: inits game client");
    } else if (strcmp(param, "ec") == 0) {
      ConOut("Usage: ec");
      ConOutEx(CHAT_FONT, "Desc: ends game client");
    } else if (strcmp(param, "ss") == 0) {
      ConOut("Usage: ss");
      ConOutEx(CHAT_FONT, "Desc: inits game server");
    } else if (strcmp(param, "ec") == 0) {
      ConOut("Usage: es");
      ConOutEx(CHAT_FONT, "Desc: ends game server");
    } else if (strcmp(param, "rs") == 0) {
      ConOut("Usage: rs");
      ConOutEx(CHAT_FONT, "Desc: runs game server");
      ConOutEx(CHAT_FONT, "      cvar \"s_next_map\" defines first map");
      ConOutEx(CHAT_FONT, "      cvar \"s_next_script\" defines first script");
      ConOutEx(CHAT_FONT, "      cvar \"s_next_gt\" defines first gametype");
    } else if (strcmp(param, "ds") == 0) {
      ConOut("Usage: ds");
      ConOutEx(CHAT_FONT, "Desc: shutdowns game server");
    } else if (strcmp(param, "connect") == 0) {
      ConOut("Usage: connect <IP> or connect <URL>");
      ConOutEx(CHAT_FONT, "Desc: attempts to connect to the server at <IP> or <URL>");
      ConOutEx(CHAT_FONT, "      before calling game client must be started via \"sc\"");
    } else if (strcmp(param, "reconnect") == 0) {
      ConOut("Usage: reconnect");
      ConOutEx(CHAT_FONT, "Desc: attempts to reconnect to the last server");
      ConOutEx(CHAT_FONT, "      last server is stored in cvar \"last_server\"");
    } else if (strcmp(param, "disconnect") == 0) {
      ConOut("Usage: disconnect");
      ConOutEx(CHAT_FONT, "Desc: disconnects from server");
    } else if (strcmp(param, "attach") == 0) {
      ConOut("Usage: attach <pass>");
      ConOutEx(CHAT_FONT, "Desc: tries to attach remote server console to the client's console");
      ConOutEx(CHAT_FONT, "      <pass> is password chosen by the server admin");
      ConOutEx(CHAT_FONT, "      if success, when you type \"!<command>\", command will be sent across net and executed in server console");
      ConOutEx(CHAT_FONT, "      because of packet switching server output could be unordered");
    } else if (strcmp(param, "detach") == 0) {
      ConOut("Usage: detach");
      ConOutEx(CHAT_FONT, "Desc: detaches server console (previously attached by \"attach\" command");
    } else if (strcmp(param, "dlmap") == 0) {
      ConOut("Usage: dlmap <mapname>");
      ConOutEx(CHAT_FONT, "Desc: sends download reqest for map <mapname>");
      ConOutEx(CHAT_FONT, "      due to security only alphanumeric names are supported");
      ConOutEx(CHAT_FONT, "      map will be downloaded into directory defined by cvar \"map_dir\"");
      ConOutEx(CHAT_FONT, "      server must have downloading enabled");
    } else if (strcmp(param, "dlsprite") == 0) {
      ConOut("Usage: dlsprite <spritename>");
      ConOutEx(CHAT_FONT, "Desc: sends download reqest for sprite <spritename>");
      ConOutEx(CHAT_FONT, "      due to security only alphanumeric names are supported");
      ConOutEx(CHAT_FONT, "      sprite will be downloaded into directory defined by cvar \"sprite_dir\"");
      ConOutEx(CHAT_FONT, "      server must have downloading enabled");
    } else if (strcmp(param, "dlskin") == 0) {
      ConOut("Usage: dlskin <skinname>");
      ConOutEx(CHAT_FONT, "Desc: sends download reqest for skin <skinname>");
      ConOutEx(CHAT_FONT, "      due to security only alphanumeric names are supported");
      ConOutEx(CHAT_FONT, "      skin will be downloaded into directory defined by cvar \"skin_dir\"");
      ConOutEx(CHAT_FONT, "      server must have downloading enabled");
    } else if (strcmp(param, "dlsound") == 0) {
      ConOut("Usage: dlsound <soundname>");
      ConOutEx(CHAT_FONT, "Desc: sends download reqest for sound <soundname>");
      ConOutEx(CHAT_FONT, "      due to security only alphanumeric names are supported");
      ConOutEx(CHAT_FONT, "      sound will be downloaded into directory defined by cvar \"snd_dir\"");
      ConOutEx(CHAT_FONT, "      server must have downloading enabled");
    } else if (strcmp(param, "ulmap") == 0) {
      ConOut("Usage: ulmap <mapname>");
      ConOutEx(CHAT_FONT, "Desc: sends upload reqest for map <mapname>");
      ConOutEx(CHAT_FONT, "      due to security only alphanumeric names are supported");
      ConOutEx(CHAT_FONT, "      map will be searched in directory defined by cvar \"map_dir\"");
      ConOutEx(CHAT_FONT, "      server must have uploading enabled");
    } else if (strcmp(param, "ulsprite") == 0) {
      ConOut("Usage: ulsprite <spritename>");
      ConOutEx(CHAT_FONT, "Desc: sends upload reqest for sprite <spritename>");
      ConOutEx(CHAT_FONT, "      due to security only alphanumeric names are supported");
      ConOutEx(CHAT_FONT, "      sprite will be searched in directory defined by cvar \"sprite_dir\"");
      ConOutEx(CHAT_FONT, "      server must have uploading enabled");
    } else if (strcmp(param, "ulskin") == 0) {
      ConOut("Usage: ulskin <skinname>");
      ConOutEx(CHAT_FONT, "Desc: sends upload reqest for skin <skinname>");
      ConOutEx(CHAT_FONT, "      due to security only alphanumeric names are supported");
      ConOutEx(CHAT_FONT, "      skin will be searched in directory defined by cvar \"skin_dir\"");
      ConOutEx(CHAT_FONT, "      server must have uploading enabled");
    } else if (strcmp(param, "ulsound") == 0) {
      ConOut("Usage: ulsound <soundname>");
      ConOutEx(CHAT_FONT, "Desc: sends upload reqest for sound <soundname>");
      ConOutEx(CHAT_FONT, "      due to security only alphanumeric names are supported");
      ConOutEx(CHAT_FONT, "      sound will be searched in directory defined by cvar \"snd_dir\"");
      ConOutEx(CHAT_FONT, "      server must have uploading enabled");
    } else if (strcmp(param, "cp") == 0) {
      ConOut("Usage: cp <playername> (keybindingsnum)");
      ConOutEx(CHAT_FONT, "Desc: creates player with name <playername>");
      ConOutEx(CHAT_FONT, "      keybindingsnum is number 1-4 and says, which keyboard layout controls this player");
      ConOutEx(CHAT_FONT, "      note: after creation, you can skin player with \"sp\" command");
      ConOutEx(CHAT_FONT, "      one client could create more than one player");
    } else if (strcmp(param, "dp") == 0) {
      ConOut("Usage: dp <playername>");
      ConOutEx(CHAT_FONT, "Desc: deletes player with name <playername>");
      ConOutEx(CHAT_FONT, "      <playername> must be player created by this client");
    } else if (strcmp(param, "sp") == 0) {
      ConOut("Usage: sp <playername> (skin)");
      ConOutEx(CHAT_FONT, "Desc: skins player with name <playername> with given skin");
      ConOutEx(CHAT_FONT, "      to list all skins use \"list_skins\"");
      ConOutEx(CHAT_FONT, "      <playername> must be player created by this client");
    } else if (strcmp(param, "chase") == 0) {
      ConOut("Usage: chase <playername>");
      ConOutEx(CHAT_FONT, "Desc: camera will be observing player <playername>");
    } else if (strcmp(param, "kick") == 0) {
      ConOut("Usage: kick <clientname>");
      ConOutEx(CHAT_FONT, "Desc: kicks client <clientname> from the server (admin)");
    } else if (strcmp(param, "skin_load") == 0) {
      ConOut("Usage: skin_load <name>");
      ConOutEx(CHAT_FONT, "Desc: loads skin <name> in memory");
    } else if (strcmp(param, "skin_scan") == 0) {
      ConOut("Usage: skin_free <name>");
      ConOutEx(CHAT_FONT, "Desc: free skin <name> from memory");
    } else if (strcmp(param, "map_scan") == 0) {
      ConOut("Usage: map_scan");
      ConOutEx(CHAT_FONT, "Desc: searches maps directory for new maps");
      ConOutEx(CHAT_FONT, "      directory is stored in cvar \"map_dir\"");
    } else if (strcmp(param, "map_list") == 0) {
      ConOut("Usage: map_list");
      ConOutEx(CHAT_FONT, "Desc: shows list of all maps available");
    } else if (strcmp(param, "script_scan") == 0) {
      ConOut("Usage: script_scan");
      ConOutEx(CHAT_FONT, "Desc: searches scripts directory for new scripts");
      ConOutEx(CHAT_FONT, "      directory is stored in cvar \"script_dir\"");
    } else if (strcmp(param, "script_list") == 0) {
      ConOut("Usage: script_list");
      ConOutEx(CHAT_FONT, "Desc: shows list of all scripts available");
    } else if (strcmp(param, "say") == 0) {
      ConOut("Usage: say <text>");
      ConOutEx(CHAT_FONT, "Desc: displays message from admin");
    } else if (strcmp(param, "blitinfo") == 0) {
      ConOut("Usage: blitinfo (tilenum)");
      ConOutEx(CHAT_FONT, "Desc: writes some map tile info (blitting) for tile tilenum, implicit value is 0");
    } else
#ifdef PW_MUSIC
      if (strcmp(param, "mus_play") == 0) {
        ConOut("Usage: mus_play (song filename with extension)");
        ConOutEx(CHAT_FONT, "Desc: loads and starts to play requested song, ");
        ConOutEx(CHAT_FONT, "      if none parameter is given, music is resumed if it was previously stopped.");
      } else if (strcmp(param, "mus_stop") == 0) {
        ConOut("Usage: mus_stop");
        ConOutEx(CHAT_FONT, "Desc: stops playing music");
      } else if (strcmp(param, "mus_release") == 0) {
        ConOut("Usage: mus_release");
        ConOutEx(CHAT_FONT, "Desc: stops playing and releases music from memory");
      } else if (strcmp(param, "mus_volume") == 0) {
        ConOut("Usage: mus_volume <volume>");
        ConOutEx(CHAT_FONT, "Desc: sets music volume in range 0-128");
      } else
#endif
    if (strcmp(param, "sc_list") == 0) {
      ConOut("Usage: sc_list");
      ConOutEx(CHAT_FONT, "Desc: writes state of script manager, seleced script is pointed by arrow");
      ConOutEx(CHAT_FONT, "      for understanding scripting, please read SeeR documentation (doc/seer)");
    } else if (strcmp(param, "sc_sel") == 0) {
      ConOut("Usage: sc_sel <script>");
      ConOutEx(CHAT_FONT, "Desc: selects script <script> as active script");
    } else if (strcmp(param, "sc_add") == 0) {
      ConOut("Usage: sc_add <script filename without extension>");
      ConOutEx(CHAT_FONT, "Desc: adds script to next unused slot in script manager");
      ConOutEx(CHAT_FONT, "      note: before you run it, you must instance it");
    } else if (strcmp(param, "sc_instance") == 0) {
      ConOut("Usage: sc_instance (script)");
      ConOutEx(CHAT_FONT, "Desc: instances script (script) or selected script");
    } else if (strcmp(param, "sc_run") == 0) {
      ConOut("Usage: sc_run (script) <funcname>");
      ConOutEx(CHAT_FONT, "Desc: calls function in script (script) or in selected script");
    } else if (strcmp(param, "sc_make") == 0) {
      ConOut("Usage: sc_make <script filename without extension>");
      ConOutEx(CHAT_FONT, "Desc: adds and instances given script, See sc_add and sc_instance help.");
    } else if (strcmp(param, "sc_delete") == 0) {
      ConOut("Usage: sc_delete (script)");
      ConOutEx(CHAT_FONT, "Desc: frees instance and removes script from manager");
    } else if (strcmp(param, "sc_delinst") == 0) {
      ConOut("Usage: sc_delinst (script)");
      ConOutEx(CHAT_FONT, "Desc: frees instance of script");
    } else if (strcmp(param, "help") == 0) {
      ConOutEx(CHAT_FONT, "Are you crazy ? Type \"help\" instead");
    } else if (strcmp(param, "?") == 0) {
      ConOut("Usage: ? (script) <symbol_name> (type_id)");
      ConOutEx(CHAT_FONT, "Desc: shows a value of given symbol interpreted as a type of type_id");
      ConOutEx(CHAT_FONT, "      type_ids are extension to types in formating strings in printf or scanf functions");
      ConOutEx(CHAT_FONT, "      please see ex_debug example script for more info");
      ConOutEx(CMD_FONT,  "Warning: incorrect type_id could crash program !!!");
    } else if (strcmp(param, "=") == 0) {
      ConOut("Usage: ? (script) <symbol_name> <new_value> (type_id)");
      ConOutEx(CHAT_FONT, "Desc: sets the value of given symbol interpreted as type of type_id");
      ConOutEx(CHAT_FONT, "      type_ids are extension to types in formating strings in printf or scanf functions");
      ConOutEx(CHAT_FONT, "      please see ex_debug example script for more info");
      ConOutEx(CMD_FONT,  "Warning: incorrect type_id could crash program !!!");
    } else {
      ConOut("Command %s has no help describtion", param);
    }
  }
}


/////////////////////////////////////////////////////////////////////////////
// call this to end the main loop 
/////////////////////////////////////////////////////////////////////////////
void KillProgram(char *String)
{
  want_quit = true;
}

/////////////////////////////////////////////////////////////////////////////
// adds a border to the bottom
/////////////////////////////////////////////////////////////////////////////
void ListConCmds(char *string)
{
  ListCommands();
}

/////////////////////////////////////////////////////////////////////////////
// sets camera x y
/////////////////////////////////////////////////////////////////////////////
/*
void SetCam(char *string)
{
  int camx;
  int camy;
  
  if (!MapLoaded) {
    ConOut("Map is not loaded !");
    return;
  }
  sscanf(string, "%d %d", &camx, &camy);
  // input correction
  if (camx < 0)
    camx = 0;
  if (camy < 0)
    camy = 0;
  if (camx > ((mapwidth * mapblockwidth) - MSCRW))
    camx = (mapwidth * mapblockwidth) - MSCRW;
  if (camy > ((mapheight * mapblockheight) - MSCRH))
    camy = (mapheight * mapblockheight) - MSCRH;
  // let know values set
  ConOut("Camera at [%d, %d]", camx, camy);
  
  client_info.game.vars.camx = camx;
  client_info.game.vars.camy = camy;
}
*/

/////////////////////////////////////////////////////////////////////////////
// some tiles graphics info (blitting)
// use blitinfo (tilenum)
/////////////////////////////////////////////////////////////////////////////
void BlitInfo(char *string)
{
  int vid_tiles = 0;
  int last_vid_tile = -1;
  int tilenum = 0;
  
  if (!MapLoaded) {
    ConOut("Map is not loaded !");
    return;
  }
  if ((string) && (*string != 0)) {
    if (sscanf(string, "%d", &tilenum) < 1) {
      ConOut("Error in parameter: use blitinfo (tilenum)");
      return;
    }
    if ((tilenum > mapnumblockgfx) || (tilenum < 0)) {
      ConOut
        ("Error in parameter: %d is out of range. Current map has tiles from %d to %d.",
        tilenum, 0, mapnumblockgfx);
      return;
    }
    
  }
  
  for (int i = 0; i < mapnumblockgfx; i++) {
    if (mapTiles[i]->flags & SDL_HWSURFACE) {
      last_vid_tile = i;
      vid_tiles++;
    }
  }
  
  ConOut("");
  ConOut("Blit info:");
  ConOut("Map tiles stored in videomem: %d", vid_tiles);
  ConOut("Last tile stored in videomem: %d", last_vid_tile);
  ConOut
    ("Tile[%d]: flags=0x%X, w=%d, h=%d, pitch=%d, RGBA mask=[0x%X,0x%X,0x%X,0x%X]",
    tilenum, mapTiles[0]->flags, mapTiles[0]->w, mapTiles[0]->h,
    mapTiles[0]->pitch, mapTiles[0]->format->Rmask,
    mapTiles[0]->format->Gmask, mapTiles[0]->format->Bmask,
    mapTiles[0]->format->Amask);
}


/////////////////////////////////////////////////////////////////////////////
// video command writes video information to console
/////////////////////////////////////////////////////////////////////////////
void Video(char *string)
{
  if (!VideoInfo) {
    ConOut("Video information not available !");
    return;
  }
  ConOut("");
  ConOut("Video information:");
  ConOut
    ("Video surface: flags=0x%X, w=%d, h=%d, pitch=%d, RGBA mask=[0x%X,0x%X,0x%X,0x%X]",
    screen->flags, screen->w, screen->h, screen->pitch,
    screen->format->Rmask, screen->format->Gmask,
    screen->format->Bmask, screen->format->Amask);
  ConOut("VideoRAM: %dKB (do not believe ;)", VideoInfo->video_mem);
  ConOut("Hardware surfaces: %s",
    VideoInfo->hw_available ? "yes" : "no");
  ConOut("Window manager: %s", VideoInfo->wm_available ? "yes" : "no");
  ConOut("Accelerated blits HW --> HW: %s",
    VideoInfo->blit_hw ? "yes" : "no");
  ConOut("Accelerated blits with colorkey: %s",
    VideoInfo->blit_hw_CC ? "yes" : "no");
  ConOut("Accelerated blits with alpha: %s",
    VideoInfo->blit_hw_A ? "yes" : "no");
  ConOut("Accelerated blits SW --> HW: %s",
    VideoInfo->blit_sw ? "yes" : "no");
  ConOut("Accelerated blits with colorkey: %s",
    VideoInfo->blit_sw_CC ? "yes" : "no");
  ConOut("Accelerated blits with alpha: %s",
    VideoInfo->blit_sw_A ? "yes" : "no");
  ConOut("Accelerated color fill: %s",
    VideoInfo->blit_fill ? "yes" : "no");
}


/////////////////////////////////////////////////////////////////////////////
// Loads & inits game map
/////////////////////////////////////////////////////////////////////////////
void SetMap(char *string)
{
  char mname[MAX_WORD_INPUTED];
  char sname[MAX_WORD_INPUTED];
  
  if (sscanf(string, "%s %s", mname, sname) < 2) {
    if (sscanf(string, "%s", mname) < 1) {
      ConOut("Error: usage: map <mapname> (scriptname)");
      return;
    }
    sprintf(sname, "%s00", mname);
  }

  SV_Shutdown(SD_CHANGING_MAP);
  Cvar_Set("s_next_map", mname);
  Cvar_Set("s_next_script", sname);
  SV_Run();
}

/////////////////////////////////////////////////////////////////////////////
// Writes info about loaded map
/////////////////////////////////////////////////////////////////////////////
void MapInfo(char *String)
{
  if (MapLoaded) {
    ConOut("Map info (%s): ", MapName);
    ConOut("Map size %dx%d", mapwidth, mapheight);
    ConOut("Tile size %dx%d", mapblockwidth, mapblockheight);
    ConOut("Map depth: %d", mapdepth);
    ConOut("numstr=%d, numgfx=%d", mapnumblockstr, mapnumblockgfx);
  } else
    ConOut("Map was not loaded", exp);
}

#ifdef PW_MUSIC
/////////////////////////////////////////////////////////////////////////////
// mus_pause command pauses playing music
/////////////////////////////////////////////////////////////////////////////
void Pause_Music(char *string)
{
  if (sound_hw_enabled)
  {
#ifdef PW_BASS
    BASS_Pause();
#else
    if (music)
      Mix_PauseMusic();
#endif
    ConOut("Music: music paused.");
  }
}

/////////////////////////////////////////////////////////////////////////////
// mus_play (song_file_name) command loads music and starts playing
// if no parameter is given, tries to resume previously paused music
/////////////////////////////////////////////////////////////////////////////
void Play_Music(char *string)
{
  if (sound_hw_enabled)
  {
    if ((!string) || (*string == 0)) {
      if (music) {
#ifdef PW_BASS
        BASS_Start();
#else
        Mix_ResumeMusic();
#endif
        ConOut("Music: music resumed.");
      } else
        ConOut("Music: no music loaded !");
      return;
    }
  
    if (strcmp(playing_music, string) == 0)
      return;
    else
      strcpy(playing_music, string);
  
#ifdef PW_BASS
    BASS_MusicFree(music);
    music = NULL;
  
    FileNameConversion(snd_dir.string, string, "", tmptxt);
    // Load a music from "file" and make it use ramping
    music =
      BASS_MusicLoad(FALSE, tmptxt, 0, 0,
      BASS_MUSIC_LOOP | BASS_MUSIC_FT2MOD |
      BASS_MUSIC_RAMP);
    if (!music) {
      ConErr("Music: Couldn't load %s: %d\n",
			     tmptxt, BASS_ErrorGetCode());
      return;
    }
  
    ConOut("Music: music %s loaded, playing ...", tmptxt);
  
    if (!BASS_MusicPlay(music))
      ConErr("Can't play music");
  
#else
    if (music)
      Mix_FadeOutMusic(1000);
    if (music) {
      Mix_FreeMusic(music);
      music = NULL;
    }
  
    FileNameConversion(snd_dir.string, string, "", tmptxt);
    music = Mix_LoadMUS(tmptxt);
    if (music == NULL) {
      ConErr("Music: Couldn't load %s: %s\n", tmptxt, SDL_GetError());
      return;
    }
  
    ConOut("Music: music %s loaded, playing ...", tmptxt);
  
    // Play and then exit
    Mix_FadeInMusic(music, -1, 1000);
#endif
  }
}

/////////////////////////////////////////////////////////////////////////////
// mus_release command stops play and frees music 
/////////////////////////////////////////////////////////////////////////////
void Release_Music(char *string)
{
  if (sound_hw_enabled)
  {
    if (!music) {
      ConOut("Music: no music loaded !");
      return;
    }
#ifdef PW_BASS
    BASS_MusicFree(music);
    music = NULL;
#else
    if (music)
      Mix_FadeOutMusic(2000);
    if (music) {
      Mix_FreeMusic(music);
      music = NULL;
    }
#endif
    ConOut("Music: music released.");
  }
}

/////////////////////////////////////////////////////////////////////////////
// mus_volume command sets music volume
/////////////////////////////////////////////////////////////////////////////
void Volume_Music(char *string)
{
  if (sound_hw_enabled)
  {
    if (!music) {
      ConOut("Music: no music loaded !");
      return;
    }
  
    int param = 0;
    if (sscanf(string, "%d", &param) < 1) {
      ConOut("Could not resolve prameter %s !", string);
      return;
    }
  
    char val[128];
    sprintf(val, "%d", param);
    free(music_volume.string);	// free the old value string
    music_volume.string = (char *) malloc(strlen(val) + 1);
    strcpy(music_volume.string, val);
    music_volume.value = (float) param;
  
#ifdef PW_BASS
    BASS_SetGlobalVolumes((int) music_volume.value,
      (int) (sound_volume.value * (100.0 / 64)), 0);
    //  BASS_MusicSetAmplify(music, (int)(param*(100.0/64)));
#else
    Mix_VolumeMusic(param);
#endif
    ConOut("Music: volume = %d", param);
  }
}

/////////////////////////////////////////////////////////////////////////////
// snd_volume command sets sound volume
/////////////////////////////////////////////////////////////////////////////
void Volume_Sound(char *string)
{
  if (sound_hw_enabled)
  {
    static bool in = false;
  
    int param = 0;
    if (sscanf(string, "%d", &param) < 1) {
      ConOut("Could not resolve prameter %s !", string);
      return;
    }
  
    char val[128];
    sprintf(val, "%d", param);
    free(sound_volume.string);	// free the old value string
    sound_volume.string = (char *) malloc(strlen(val) + 1);
    strcpy(sound_volume.string, val);
    sound_volume.value = (float) param;
  
#ifdef PW_BASS
    BASS_SetGlobalVolumes((int) music_volume.value,
      (int) (sound_volume.value * (100.0 / 64)), 0);
#else
    Mix_Volume(-1, param);
#endif
    ConOut("Sound: volume = %d", param);
  }
}
#endif

/////////////////////////////////////////////////////////////////////////////
// sc_add <script> 
/////////////////////////////////////////////////////////////////////////////
void AddScript(char *string)
{
  char *name, *ext;
  name = string;
  
  // check param
  for (unsigned int i = 0; i < strlen(string); i++) {
    if (string[i] == '.') {
      ext = &string[i + 1];
      string[i] = 0;
      goto XXX;
    }
  }
  ConOut("Error: usage: sc_add <script.ext>");
  return;
XXX:
  ScriptMan->NewScript(name, ext);
}

/////////////////////////////////////////////////////////////////////////////
// sc_instance (script) 
/////////////////////////////////////////////////////////////////////////////
void InstanceScript(char *string)
{
  char param[MAX_WORD_INPUTED];
  char *selected;
  
  // detect input format & load values into string variables
  if (sscanf(string, "%s", param) < 1) {
    if (ScriptMan->GetActiveScript(&selected) == 0)
      strcpy(param, selected);
    else {
      ConOut
        ("Scripting: trying to use selected script, but script slot is unused !");
      return;
    }
  }
  
  ScriptMan->InstanceScript(param);
}

/////////////////////////////////////////////////////////////////////////////
// sc_run (script) <funcname>
/////////////////////////////////////////////////////////////////////////////
void RunScript(char *string)
{
  char func[MAX_WORD_INPUTED];
  char param[MAX_WORD_INPUTED];
  char *selected;
  
  // detect input format & load values into string variables
  if (sscanf(string, "%s %s", param, func) < 2) {
    if (sscanf(string, "%s", func) < 1) {
      ConOut("Error: usage: sc_run (script) <funcname>");
      return;
    }
    
    if (ScriptMan->GetActiveScript(&selected) == 0)
      strcpy(param, selected);
    else {
      ConOut
        ("Scripting: trying to use selected script, but script slot is unused !");
      return;
    }
  }
  
  ScriptMan->RunScript(param, func);
}

/////////////////////////////////////////////////////////////////////////////
// sc_delinst (script) 
/////////////////////////////////////////////////////////////////////////////
void DelInstanceScript(char *string)
{
  char param[MAX_WORD_INPUTED];
  char *selected;
  
  // detect input format & load values into string variables
  if (sscanf(string, "%s", param) < 1) {
    if (ScriptMan->GetActiveScript(&selected) == 0)
      strcpy(param, selected);
    else {
      ConOut
        ("Scripting: trying to use selected script, but script slot is unused !");
      return;
    }
  }
  
  ScriptMan->DoneInstanceScript(param);
}

/////////////////////////////////////////////////////////////////////////////
// sc_delete (script)
/////////////////////////////////////////////////////////////////////////////
void DeleteScript(char *string)
{
  char param[MAX_WORD_INPUTED];
  char *selected;
  
  // detect input format & load values into string variables
  if (sscanf(string, "%s", param) < 1) {
    if (ScriptMan->GetActiveScript(&selected) == 0)
      strcpy(param, selected);
    else {
      ConOut
        ("Scripting: trying to use selected script, but script slot is unused !");
      return;
    }
  }
  
  ScriptMan->DeleteScript(param);
}

/////////////////////////////////////////////////////////////////////////////
// sc_list
/////////////////////////////////////////////////////////////////////////////
void ListScripts(char *string)
{
  ScriptMan->ListScripts();
}

/////////////////////////////////////////////////////////////////////////////
// sc_make <script>
/////////////////////////////////////////////////////////////////////////////
void MakeScript(char *string)
{
  // check param
  if ((!string) || (*string == 0)) {
    ConOut("Error: usage: sc_make <script>");
    return;
  }
  AddScript(string);
  InstanceScript(string);
}

/////////////////////////////////////////////////////////////////////////////
// sc_sel <script>
/////////////////////////////////////////////////////////////////////////////
void SelScript(char *string)
{
  // check param
  if ((!string) || (*string == 0)) {
    ConOut("Error: usage: sc_sel <script>");
    return;
  }
  
  ScriptMan->SetActiveScript(string);
}

// help function for testing type shortcut given in type
// returns 0 if an error occurs !
// returns 1 if normal type (non asterixed id)
// returns 2 in case of "*id" (means value is pointer to that type i.e *i means pointer to i)
// returns 3 in case of "id*" (opposite to 2, means value dereferenced value to be returned as reference, 
//                             i.e. char a[50] we want return as string "s*" )

int CheckTypeId(char *type, char *type_id)
{
  char fp;
  int res;
  
  if (strlen(type) == 0)
    return 0;				// I'd like to check type[1]
  
  if (type[0] == '*') {
    res = 2;
    fp = 1;
  } else {
    if (type[1] == '*') {
      res = 3;
      fp = 0;
    } else {
      fp = 0;
      res = 1;
    }
  }
  
  // fp = type_id position
  // res = syntax type of type (1,2,3)
  switch (type[fp]) {
  case 'c':
  case 'C':
  case 'd':
  case 'i':
  case 'o':
  case 'u':
  case 'x':
  case 'X':
  case 'e':
  case 'E':
  case 'f':
  case 'g':
  case 'G':
  case 'p':
  case 'n':
  case 'S':
  case 's':
    // ok printf&scanf know this id, that's right
    break;
  default:
    // type+id is not recognized
    ConOut("Scripting: %s is unknown type shortcut !", type);
    return 0;
  }
  
  *type_id = type[fp];
  return res;
}

/////////////////////////////////////////////////////////////////////////////
// ? (script) <symbol> (type_id)
/////////////////////////////////////////////////////////////////////////////
void SGet_data(char *string)
{
  char name[MAX_WORD_INPUTED], var[MAX_WORD_INPUTED];
  char fmt[256];
  char type[MAX_WORD_INPUTED];
  char type_id;
  int asterix;
  void *val;
  char *selected;
  
  // detect input format & load values into string variables
  if (sscanf(string, "%s %s %s", name, var, type) < 3) {
    if (sscanf(string, "%s %s", var, type) < 2) {
      if (sscanf(string, "%s", var) < 1) {
        ConOut("Error: usage: ? (script) <symbol> (type_id)");
        return;
      }
      // implicit type_id
      type[0] = 'p';
      type[1] = 0;
    }
    
    if (ScriptMan->GetActiveScript(&selected) == 0)
      strcpy(name, selected);
    else {
      ConOut
        ("Scripting: trying to use selected script, but script slot is unused !");
      return;
    }
  }
  
  if (!ScriptMan->Get_data(name, var, &val)) {
    asterix = CheckTypeId(type, &type_id);
    if (asterix == 0)
      return;				// type error ?
    sprintf(fmt, "%%s=%%%c", type_id);
    switch (asterix) {
    case 1:				// normal type "id" get val
      switch (type_id) {
      case 'c':
      case 'C':
      case 'd':
      case 'i':
      case 'o':
      case 'u':
      case 'x':
      case 'X':
      case 'p':
      case 'n':
        ConOut(fmt, var, *(int *) val);
        break;
        
      case 'e':
      case 'E':
      case 'f':
      case 'g':
      case 'G':
        ConOut(fmt, var, *(double *) val);
        break;
        
      case 'S':
      case 's':
        ConOut(fmt, var, *(char **) val);
        break;
      }
      break;
      case 2:				// "*id" get val & derefernce it
        switch (type_id) {
        case 'c':
        case 'C':
        case 'd':
        case 'i':
        case 'o':
        case 'u':
        case 'x':
        case 'X':
        case 'p':
        case 'n':
          ConOut(fmt, var, **(int **) val);
          break;
          
        case 'e':
        case 'E':
        case 'f':
        case 'g':
        case 'G':
          ConOut(fmt, var, **(double **) val);
          break;
          
        case 'S':
        case 's':
          ConOut(fmt, var, **(char ***) val);
          break;
        }
        break;
        case 3:				// "id*" get val & refernce it
          switch (type_id) {
          case 'c':
          case 'C':
          case 'd':
          case 'i':
          case 'o':
          case 'u':
          case 'x':
          case 'X':
          case 'p':
          case 'n':
            ConOut(fmt, var, (int *) val);
            break;
            
          case 'e':
          case 'E':
          case 'f':
          case 'g':
          case 'G':
            ConOut(fmt, var, (double *) val);
            break;
            
          case 'S':
          case 's':
            ConOut(fmt, var, (char *) val);
            break;
          }
          break;
          
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
// = (script) <symbol> <value> (type_id)
/////////////////////////////////////////////////////////////////////////////
void SSet_data(char *string)
{
  char name[MAX_WORD_INPUTED], var[MAX_WORD_INPUTED],
    value[MAX_WORD_INPUTED];
  char type[MAX_WORD_INPUTED];
  char fmt[256];
  void *oldval;
  char *selected;
  int size;
  int asterix;
  char type_id;
  float f;
  
  char buffer1[100];
  char buffer2[100];
  
  // detect input format & load values into string variables
  if (sscanf(string, "%s %s %s %s", name, var, value, type) < 4) {
    if (sscanf(string, "%s %s %s", var, value, type) < 3) {
      if (sscanf(string, "%s %s", var, value) < 2) {
        ConOut
          ("Error: usage: = (script) <symbol> <value> (type_id)");
        return;
      }
      // implicit type_id
      type[0] = 'p';
      type[1] = 0;
    }
    
    if (ScriptMan->GetActiveScript(&selected) == 0)
      strcpy(name, selected);
    else {
      ConOut
        ("Scripting: trying to use selected script, but script slot is unused !");
      return;
    }
  }
  
  asterix = CheckTypeId(type, &type_id);
  if (asterix == 0)
    return;					// type error ?
  
  sprintf(fmt, "%%%c", type_id);
  sscanf(value, fmt, buffer1);
  
  // type size switching
  switch (type_id) {
  case 'c':
  case 'C':
  case 'd':
  case 'i':
  case 'o':
  case 'u':
  case 'x':
  case 'X':
    size = sizeof(int);
    break;
    
  case 'e':
  case 'E':
  case 'f':
  case 'g':
  case 'G':
    size = sizeof(double);
    // due to uncompatibility e,E,f,g,G between printf and scanf
    f = *(float *) buffer1;
    *(double *) buffer1 = (double) f;
    break;
    
  case 'p':
  case 'n':
    size = sizeof(void *);
    break;
    
  case 'S':
  case 's':
    // strings are processed specially
    asterix += 3;
    size = strlen(value) + 1;
    break;
  }
  
  if (!ScriptMan->Get_data(name, var, &oldval)) {
    switch (asterix) {
    case 1:				// normal type "id" get val
      memcpy(buffer2, oldval, size);
      break;
    case 2:				// "*id" get val & derefernce it
      memcpy(buffer2, *(void **) oldval, size);
      break;
    case 3:				// "id*" get val & refernce it
      memcpy(buffer2, &oldval, size);
      break;
    case 4:				// "s" 1.special string manipulation
      strcpy(buffer2, *(char **) oldval);
      break;
    case 5:				// "*s" 2.special string manipulation
      strcpy(buffer2, **(char ***) oldval);
      break;
    case 6:				// "s*" 3.special string manipulation
      strcpy(buffer2, (char *) oldval);
      break;
    }
    sprintf(fmt, "%%s=%%%c -> %%s=%%%c", type_id, type_id);
    switch (asterix) {
    case 1:				// normal type "id" get val
      memcpy(oldval, buffer1, size);
      switch (type_id) {
      case 'c':
      case 'C':
      case 'd':
      case 'i':
      case 'o':
      case 'u':
      case 'x':
      case 'X':
      case 'p':
      case 'n':
        ConOut(fmt, var, *(int *) buffer2, var, *(int *) buffer1);
        break;
        
      case 'e':
      case 'E':
      case 'f':
      case 'g':
      case 'G':
        ConOut(fmt, var, *(double *) buffer2, var,
          *(double *) buffer1);
        break;
      }
      break;
      case 2:				// "*id" get val & derefernce it
        memcpy(*(void **) oldval, buffer1, size);
        switch (type_id) {
        case 'c':
        case 'C':
        case 'd':
        case 'i':
        case 'o':
        case 'u':
        case 'x':
        case 'X':
        case 'p':
        case 'n':
          ConOut(fmt, var, *(int *) buffer2, var, *(int *) buffer1);
          break;
          
        case 'e':
        case 'E':
        case 'f':
        case 'g':
        case 'G':
          ConOut(fmt, var, *(double *) buffer2, var,
            *(double *) buffer1);
          break;
        }
        break;
        case 3:				// "id*" get val & refernce it
          memcpy(&oldval, buffer1, size);
          switch (type_id) {
          case 'c':
          case 'C':
          case 'd':
          case 'i':
          case 'o':
          case 'u':
          case 'x':
          case 'X':
          case 'p':
          case 'n':
            ConOut(fmt, var, *(int *) buffer2, var, *(int *) buffer1);
            break;
            
          case 'e':
          case 'E':
          case 'f':
          case 'g':
          case 'G':
            ConOut(fmt, var, *(double *) buffer2, var,
              *(double *) buffer1);
            break;
          }
          break;
          case 4:				// "s" 1. special string manipulation
            memcpy(*(void **) oldval, buffer1, size);
            // type_id is "s" or "S"  
            ConOut(fmt, var, (char *) buffer2, var, (char *) buffer1);
            break;
          case 5:				// "*s" 2. special string manipulation
            memcpy(**(void ***) oldval, buffer1, size);
            // type_id is "s" or "S"  
            ConOut(fmt, var, (char *) buffer2, var, (char *) buffer1);
            break;
          case 6:				// "s*" 3. special string manipulation
            memcpy(oldval, buffer1, size);
            // type_id is "s" or "S"  
            ConOut(fmt, var, (char *) buffer2, var, (char *) buffer1);
            break;
    }
  }
}


/////////////////////////////////////////////////////////////////////////////
// say <message> 
/////////////////////////////////////////////////////////////////////////////
void Say(char *string)
{
  // check param
  if ((!string) || (*string == 0)) {
    ConOut("Error: usage: say <message>");
    return;
  }
  if (client_info.active)
    ChatMessage(string);
  else if (server_info.active)
    SV_BroadcastPrintf("Server operator: %s", string);
}


/////////////////////////////////////////////////////////////////////////////
// start_server
/////////////////////////////////////////////////////////////////////////////
void CStartServer(char *string)
{
  int r;
#ifdef PW_NETSERVER
  r = SV_Start();
  if (!r)
    ConOutEx(SERVER_FONT, "Server: server started");
  else
    ConOutEx(SERVER_FONT, "Server: error starting server: %s", SDL_GetError());
#else
  ConOutEx(SERVER_FONT, "Server: server support isn't compiled !");
#endif
}

void CEndServer(char *string)
{
#ifdef PW_NETSERVER
  int r;
  r = SV_End();
  if (!r)
    ConOutEx(SERVER_FONT, "Server: server ended");
  else
    ConOutEx(SERVER_FONT, "Server: error ending server: %s", SDL_GetError());
#else
  ConOutEx(SERVER_FONT, "Server: server support isn't compiled !");
#endif
}

void CStartClient(char *string)
{
  int r;
#ifdef PW_NETCLIENT
  r = CL_Start(STD_CLIENT_NAME);
  if (!r)
    ConOutEx(CLIENT_FONT, "Client: client started");
  else
    ConOutEx(CLIENT_FONT, "Client: error starting client: %s", SDL_GetError());
#else
  ConOutEx(CLIENT_FONT, "Client: client support isn't compiled !");
#endif
}

void CEndClient(char *string)
{
  int r;
#ifdef PW_NETCLIENT
  r = CL_End();
  if (!r)
    ConOutEx(CLIENT_FONT, "Client: client ended");
  else
    ConOutEx(CLIENT_FONT, "Client: error ending client: %s", SDL_GetError());
#else
  ConOutEx(CLIENT_FONT, "Client: client support isn't compiled !");
#endif
}

void CRunServer(char *string)
{
  int r;
#ifdef PW_NETSERVER
  r = SV_Run();
  if (!r)
    ConOutEx(SERVER_FONT, "Server: server is running");
  else
    ConOutEx(SERVER_FONT, "Server: error running server: %s", SDL_GetError());
#else
  ConOutEx(SERVER_FONT, "Server: server support isn't compiled !");
#endif
}

void CShutdownServer(char *string)
{
  int r;
#ifdef PW_NETSERVER
  r = SV_Shutdown(SD_SHUTDOWN);
  
  if (!r)
    ConOutEx(SERVER_FONT, "Server: server is down");
  else
    ConOutEx(SERVER_FONT, "Server: error shutdowning server: %s", SDL_GetError());
#else
  ConOutEx(SERVER_FONT, "Server: server support isn't compiled !");
#endif
}

void CConnectClient(char *string)
{
  int r;
#ifdef PW_NETCLIENT
  r = CL_Connect(string);
  enable_menu_music = 0;
  if (r)
    ConOutEx(CLIENT_FONT, "Client: %s", SDL_GetError());
#else
  ConOutEx(CLIENT_FONT, "Client: client support isn't compiled !");
#endif
}

void CReconnectClient(char *string)
{
  int r;
#ifdef PW_NETCLIENT
  r = CL_Reconnect();
  enable_menu_music = 0;
  if (r)
    ConOutEx(CLIENT_FONT, "Client: %s", SDL_GetError());
#else
  ConOutEx(CLIENT_FONT, "Client: client support isn't compiled !");
#endif
}

void CDisconnectClient(char *string)
{
  int r;
#ifdef PW_NETCLIENT
  r = CL_Disconnect();
  if (!r)
    ConOutEx(CLIENT_FONT, "Client: disconnected");
  else
    ConOutEx(CLIENT_FONT, "Client: %s", SDL_GetError());
#else
  ConOutEx(CLIENT_FONT, "Client: client support isn't compiled !");
#endif
  
  Play_Music(menu_music_file.string);
  Volume_Music(music_volume.string);
}

/*
void CSetServerName(char *string)
{
int r;
#ifdef PW_NETSERVER
if ((!string) || (*string==0))
{
if (net_server_status==NS_INITED)
ConOut("Server name is %s", server_info.name);
else
ConOut("Server is not runnig");
}
else
{
r=SV_SetName(string);
if (r) ConOut("Net: %s", SDL_GetError());
}
#else
ConOut("Net: server support isn't compiled !");
#endif
}
*/
/*
void CSetClientName(char *string)
{
int r;
#ifdef PW_NETCLIENT
if ((!string) || (*string==0))
{
ConOut("Your name is %s", client_info.name);
}
else
{
r=CL_SetName(string);
if (r) ConOut("Net: %s", SDL_GetError());
}
#else
ConOut("Net: client support isn't compiled !");
#endif
}
*/
void CAttachCon(char *string)
{
  if (net_server_status != NS_UNINITED) {
    ConOut("You are directly accesing server console.");
    return;
  }
#ifdef PW_NETCLIENT
  if (net_client_status == NS_CONNECTED) {
    if ((!string) || (*string==0)) 
    {
      ConOut("Usage: attach <pass>");
    }
    else
    {
      CL_Attach(string);
    }
  } else
    ConOut("Connect to the server first");
  
#else
  ConOut("Net: client support isn't compiled !");
#endif
}

void CDetachCon(char *string)
{
#ifdef PW_NETCLIENT
  if (net_client_status == NS_CONNECTED) {
    CL_Detach();
  } else
    ConOut("Connect to the server first");
#else
  ConOut("Net: client support isn't compiled !");
#endif
}

void CDownloadMap(char *string)
{
#ifdef PW_NETCLIENT
  if (net_client_status == NS_CONNECTED) {
    CL_Download(DLID_MAP, string);
  } else
    ConOut("Connect to the server first");
#else
  ConOut("Net: client support isn't compiled !");
#endif
}


void CUploadMap(char *string)
{
#ifdef PW_NETCLIENT
  if (net_client_status == NS_CONNECTED) {
    CL_Upload(DLID_MAP, string);
  } else
    ConOut("Connect to the server first");
#else
  ConOut("Net: client support isn't compiled !");
#endif
}


void CDownloadSkin(char *string)
{
#ifdef PW_NETCLIENT
  if (net_client_status == NS_CONNECTED) {
    CL_Download(DLID_SKIN, string);
  } else
    ConOut("Connect to the server first");
#else
  ConOut("Net: client support isn't compiled !");
#endif
}


void CUploadSkin(char *string)
{
#ifdef PW_NETCLIENT
  if (net_client_status == NS_CONNECTED) {
    CL_Upload(DLID_SKIN, string);
  } else
    ConOut("Connect to the server first");
#else
  ConOut("Net: client support isn't compiled !");
#endif
}

void CDownloadSound(char *string)
{
#ifdef PW_NETCLIENT
  if (net_client_status == NS_CONNECTED) {
    CL_Download(DLID_SOUND, string);
  } else
    ConOut("Connect to the server first");
#else
  ConOut("Net: client support isn't compiled !");
#endif
}


void CUploadSound(char *string)
{
#ifdef PW_NETCLIENT
  if (net_client_status == NS_CONNECTED) {
    CL_Upload(DLID_SOUND, string);
  } else
    ConOut("Connect to the server first");
#else
  ConOut("Net: client support isn't compiled !");
#endif
}

void CDownloadSprite(char *string)
{
#ifdef PW_NETCLIENT
  if (net_client_status == NS_CONNECTED) {
    CL_Download(DLID_SPRITE, string);
  } else
    ConOut("Connect to the server first");
#else
  ConOut("Net: client support isn't compiled !");
#endif
}


void CUploadSprite(char *string)
{
#ifdef PW_NETCLIENT
  if (net_client_status == NS_CONNECTED) {
    CL_Upload(DLID_SPRITE, string);
  } else
    ConOut("Connect to the server first");
#else
  ConOut("Net: client support isn't compiled !");
#endif
}


int SaveConfig(char *cfgname)
{
  FILE *f;
  f = fopen(cfgname, "wt");
  if (!f) {
    fprintf(stderr, "Couldn't write config %s\n", cfgname);
    return 1;
  }
  fprintf(f, "#-PW2-config-file------------------------\n");
  Cvar_WriteVariables(f);
  fprintf(f, "#-end-of-cfg-----------------------------");
  fclose(f);
  
  return 0;
}

int LoadConfig(char *cfgname)
{
  FILE *f;
  f = fopen(cfgname, "rt");
  if (!f) {
    fprintf(stderr, "Couldn't open config %s\n", cfgname);
    return 1;
  }
  
  int line = 0;
  char buffer[1024];
  char name[1024];
  char *val;
  cvar_t *var;
  
  while (!feof(f)) {
    line++;
    fgets(buffer, 1024, f);
    if (buffer[0] == '#')
      continue;
    // THIS REMOVES CR/LF, FIX WHEN PORTING TO UNIX
    *(buffer + strlen(buffer) - 1) = 0;
    //    *(buffer+strlen(buffer)-1)=0;
    if (sscanf(buffer, "%s", name) < 1)
      ConOut("Loader: error in config file %s on line %d.", cfgname,
      line);
    val = buffer + strlen(name);
    while (*val == ' ')
      val++;
    
    var = Cvar_FindVar(name);
    if (!var)
      ConOut
      ("Loader: error in config file %s on line %d. variable %s not found",
      cfgname, line, name);
    else
      Cvar_Set(name, val, false);
  }
  fclose(f);
  ConOut("Config loaded.");
  
  return 0;
}

void CDropClient(char *string)
{
  int i;
#ifdef PW_NETSERVER
  if (net_server_status == NS_RUNNING) {
    for (i = 0; i < server_info.maxclients; i++) {
      if (stricmp(string, client[i].name) == 0)
        break;
    }
    
    if (i == server_info.maxclients) {
      ConOut("Bad player %s", string);
      return;
    }
    SV_DropClient(i);
  } else
    ConOut("Server is not running");
#else
  ConOut("Net: server support isn't compiled !");
#endif
}

void MapScan(char *string)
{
  int c = MapMan.Scan(map_dir.string, map_ext.string);
  if (c)
    ConOut("MapMan: %d new map(s) found", c);
  else
    ConOut("MapMan: no new maps found", c);
}

void MapList(char *string)
{
  MapMan.List("Maps:");
}

void MapScriptScan(char *string)
{
  int c = SMapMan.Scan(script_dir.string, smap_ext.string);
  if (c)
    ConOut("SMapMan: %d new map script(s) found", c);
  else
    ConOut("SMapMan: no new map scripts found", c);
}

void MapScriptList(char *string)
{
  SMapMan.List("Map scripts:");
}


void SkinScan(char *string)
{
  int c = SkinMan.Scan(skin_dir.string, skin_ext.string);
  if (c)
    ConOut("SkinMan: %d new Skin(s) found", c);
  else
    ConOut("SkinMan: no new Skins found", c);
}

void SkinList(char *string)
{
  SkinMan.List();
}

void SkinLoad(char *string)
{
  CSpriteInfo *si = SkinMan.Find(string);
  if (SkinMan.Load(si) == 1)
    ConOut("SkinMan: %s not found", string);
}

void SkinFree(char *string)
{
  CSpriteInfo *si = SkinMan.Find(string);
  if (SkinMan.Free(si) == 1)
    ConOut("SkinMan: %s not found", string);
}

void SpriteScan(char *string)
{
  int c = SpriteMan.Scan(sprite_dir.string, sprite_ext.string);
  if (c)
    ConOut("SpriteMan: %d new sprite(s) found", c);
  else
    ConOut("SpriteMan: no new sprites found", c);
}

void SpriteList(char *string)
{
  SpriteMan.List();
}

void SpriteLoad(char *string)
{
  CSpriteInfo *si = SpriteMan.Find(string);
  if (SpriteMan.Load(si) == 1)
    ConOut("SpriteMan: %s not found", string);
}

void SpriteFree(char *string)
{
  CSpriteInfo *si = SpriteMan.Find(string);
  if (SpriteMan.Free(si) == 1)
    ConOut("SpriteMan: %s not found", string);
}

void SetKeybindings(int kbindex, int set)
{
  // eat keybindings from keybinding struct
  switch (set) {
  case 0:
    keybindings[kbindex].key_up = (Uint16) p1_key_up.value;
    keybindings[kbindex].key_down = (Uint16) p1_key_down.value;
    keybindings[kbindex].key_left = (Uint16) p1_key_left.value;
    keybindings[kbindex].key_right = (Uint16) p1_key_right.value;
    keybindings[kbindex].key_fire = (Uint16) p1_key_fire.value;
    keybindings[kbindex].key_shield = (Uint16) p1_key_shield.value;
    keybindings[kbindex].key_warp = (Uint16) p1_key_warp.value;
    keybindings[kbindex].key_weapon[0] = (Uint16) p1_key_weapon1.value;
    keybindings[kbindex].key_weapon[1] = (Uint16) p1_key_weapon2.value;
    keybindings[kbindex].key_weapon[2] = (Uint16) p1_key_weapon3.value;
    keybindings[kbindex].key_weapon[3] = (Uint16) p1_key_weapon4.value;
    keybindings[kbindex].key_weapon[4] = (Uint16) p1_key_weapon5.value;
    break;
  case 1:
    keybindings[kbindex].key_up = (Uint16) p2_key_up.value;
    keybindings[kbindex].key_down = (Uint16) p2_key_down.value;
    keybindings[kbindex].key_left = (Uint16) p2_key_left.value;
    keybindings[kbindex].key_right = (Uint16) p2_key_right.value;
    keybindings[kbindex].key_fire = (Uint16) p2_key_fire.value;
    keybindings[kbindex].key_shield = (Uint16) p2_key_shield.value;
    keybindings[kbindex].key_warp = (Uint16) p2_key_warp.value;
    keybindings[kbindex].key_weapon[0] = (Uint16) p2_key_weapon1.value;
    keybindings[kbindex].key_weapon[1] = (Uint16) p2_key_weapon2.value;
    keybindings[kbindex].key_weapon[2] = (Uint16) p2_key_weapon3.value;
    keybindings[kbindex].key_weapon[3] = (Uint16) p2_key_weapon4.value;
    keybindings[kbindex].key_weapon[4] = (Uint16) p2_key_weapon5.value;
    break;
  case 2:
    keybindings[kbindex].key_up = (Uint16) p3_key_up.value;
    keybindings[kbindex].key_down = (Uint16) p3_key_down.value;
    keybindings[kbindex].key_left = (Uint16) p3_key_left.value;
    keybindings[kbindex].key_right = (Uint16) p3_key_right.value;
    keybindings[kbindex].key_fire = (Uint16) p3_key_fire.value;
    keybindings[kbindex].key_shield = (Uint16) p3_key_shield.value;
    keybindings[kbindex].key_warp = (Uint16) p3_key_warp.value;
    keybindings[kbindex].key_weapon[0] = (Uint16) p3_key_weapon1.value;
    keybindings[kbindex].key_weapon[1] = (Uint16) p3_key_weapon2.value;
    keybindings[kbindex].key_weapon[2] = (Uint16) p3_key_weapon3.value;
    keybindings[kbindex].key_weapon[3] = (Uint16) p3_key_weapon4.value;
    keybindings[kbindex].key_weapon[4] = (Uint16) p3_key_weapon5.value;
    break;
  case 3:
    keybindings[kbindex].key_up = (Uint16) p4_key_up.value;
    keybindings[kbindex].key_down = (Uint16) p4_key_down.value;
    keybindings[kbindex].key_left = (Uint16) p4_key_left.value;
    keybindings[kbindex].key_right = (Uint16) p4_key_right.value;
    keybindings[kbindex].key_fire = (Uint16) p4_key_fire.value;
    keybindings[kbindex].key_shield = (Uint16) p4_key_shield.value;
    keybindings[kbindex].key_warp = (Uint16) p4_key_warp.value;
    keybindings[kbindex].key_weapon[0] = (Uint16) p4_key_weapon1.value;
    keybindings[kbindex].key_weapon[1] = (Uint16) p4_key_weapon2.value;
    keybindings[kbindex].key_weapon[2] = (Uint16) p4_key_weapon3.value;
    keybindings[kbindex].key_weapon[3] = (Uint16) p4_key_weapon4.value;
    keybindings[kbindex].key_weapon[4] = (Uint16) p4_key_weapon5.value;
    break;
  }
}

void CCreatePlayer(char *string)
{
  int r;
  int index = -1;
  char s[MAX_WORD_INPUTED];
#ifdef PW_NETCLIENT
  if (sscanf(string, "%s %d", s, &index) < 2) {
    if (sscanf(string, "%s", s) < 1) {
      ConOut("Error: usage: cp <player_name> (player_slot)");
      return;
    }
    index = -1;
  }
  index--;
  
  if (net_client_status == NS_CONNECTED) {
    if (index < 0 || index > 3)
      index = CL_FindUnusedKBSlot();
    keybindings[index].used = true;
    SetKeybindings(index, index);
    switch (index) {
    case 0:
      Cvar_SetValue("p1_on", 1);
      ConOutEx(CLIENT_FONT, "Client: creating player %s as player1", s);
      break;
    case 1:
      Cvar_SetValue("p2_on", 1);
      ConOutEx(CLIENT_FONT, "Client: creating player %s as player2", s);
      break;
    case 2:
      Cvar_SetValue("p3_on", 1);
      ConOutEx(CLIENT_FONT, "Client: creating player %s as player3", s);
      break;
    case 3:
      Cvar_SetValue("p4_on", 1);
      ConOutEx(CLIENT_FONT, "Client: creating player %s as player4", s);
      break;
    }
    r = CL_CreatePlayer(s, index);
    if (r)
      ConOutEx(CLIENT_FONT, "client: %s", SDL_GetError());
  } else
    ConOutEx(CMD_FONT, "Connect to the server first", SDL_GetError());
#else
  ConOut("Net: client support isn't compiled !");
#endif
}

void CDestroyPlayer(char *string)
{
  int r;
  GPlayer *p = NULL;
  CGame & g = client_info.game;
  GAME_MAXOBJS_TYPE i;
  int kbindex;
  
#ifdef PW_NETCLIENT
  kbindex = -1;
  if (net_client_status == NS_CONNECTED) {
    for (i = 0; i < GAME_MAX_OBJS; i++) {
      if ((g.objs[i]->state & OSTATE_ACTIVE)
        && (g.objs[i]->GetType() == ot_player)) {
        p = (GPlayer *) g.objs[i];
        if (strcmp(p->player_name.GetValRef()->chars, string) == 0) {
          kbindex = p->kbindex;
          break;
        }
      }
    }
    switch (kbindex) {
    case 0:
      Cvar_SetValue("p1_on", 0);
      break;
    case 1:
      Cvar_SetValue("p2_on", 0);
      break;
    case 2:
      Cvar_SetValue("p3_on", 0);
      break;
    case 3:
      Cvar_SetValue("p4_on", 0);
      break;
    }
    
    r = CL_DestroyPlayer(string);
    if (r)
      ConOutEx(CLIENT_FONT, "Client: %s", SDL_GetError());
  } else
    ConOutEx(CMD_FONT, "Connect to the server first", SDL_GetError());
#else
  ConOut("Net: client support isn't compiled !");
#endif
}

void CSkinPlayer(char *string)
{
  int r;
  char pname[MAX_WORD_INPUTED], sname[MAX_WORD_INPUTED];
  
#ifdef PW_NETCLIENT
  if (net_client_status == NS_CONNECTED) {
    if (sscanf(string, "%s %s", pname, sname) < 2) {
      ConOut("Error: usage: sp <player_name> <sprite_name>");
      return;
    }
    r = CL_SkinPlayer(pname, sname);
    if (r)
      ConOutEx(CLIENT_FONT, "Client: %s", SDL_GetError());
  } else
    ConOutEx(CMD_FONT, "Connect to the server first", SDL_GetError());
#else
  ConOut("Net: client support isn't compiled !");
#endif
}

void CChasePlayer(char *string)
{
  int r;
#ifdef PW_NETCLIENT
  if (net_client_status == NS_CONNECTED) {
    r = CL_ChasePlayer(string);
    if (r)
      ConOutEx(CLIENT_FONT, "Client: %s", SDL_GetError());
  } else
    ConOutEx(CMD_FONT, "Connect to the server first", SDL_GetError());
#else
  ConOut("Net: client support isn't compiled !");
#endif
}

void AddConsoleVars()
{
  // game settings
  Cvar_RegisterVariable(&relevant_destination);
  Cvar_RegisterVariable(&killing_time);
  Cvar_RegisterVariable(&banzaiing_time);
  Cvar_RegisterVariable(&menu_volume);
  Cvar_RegisterVariable(&player_sa_pistolka);
  Cvar_RegisterVariable(&player_sa_becheromet);
  Cvar_RegisterVariable(&player_sa_railgun);
  Cvar_RegisterVariable(&player_sa_bombs);
  Cvar_RegisterVariable(&player_sa_grenades);
  Cvar_RegisterVariable(&player_sa_witems);
  Cvar_RegisterVariable(&player_sa_weapon);
  Cvar_RegisterVariable(&player_sa_shields);
  Cvar_RegisterVariable(&player_sa_warps);
  Cvar_RegisterVariable(&player_sa_power);
  Cvar_RegisterVariable(&sound_shiftx);
  Cvar_RegisterVariable(&pistolka_reload_time);
  Cvar_RegisterVariable(&becheromet_reload_time);
  Cvar_RegisterVariable(&railgun_reload_time);
  Cvar_RegisterVariable(&bomb_reload_time);
  Cvar_RegisterVariable(&grenade_reload_time);
  Cvar_RegisterVariable(&warp_reload_time);
  Cvar_RegisterVariable(&born_framedelay);
  Cvar_RegisterVariable(&bornin_time);
  Cvar_RegisterVariable(&warp_framedelay);
  Cvar_RegisterVariable(&warpin_time);
  Cvar_RegisterVariable(&warpout_time);
  Cvar_RegisterVariable(&bombhit_timeout);
  Cvar_RegisterVariable(&shot_life);
  Cvar_RegisterVariable(&bshot_life);
  Cvar_RegisterVariable(&shot_step);
  Cvar_RegisterVariable(&bshot_step);
  Cvar_RegisterVariable(&bomb_timeout);
  Cvar_RegisterVariable(&grenade_timeout);
  Cvar_RegisterVariable(&pistolka_fire_end_anim);
  Cvar_RegisterVariable(&becheromet_fire_end_anim);
  Cvar_RegisterVariable(&railgun_fire_end_anim);
  Cvar_RegisterVariable(&bombs_fire_end_anim);
  Cvar_RegisterVariable(&grenades_fire_end_anim);
  Cvar_RegisterVariable(&shieldtime);
  Cvar_RegisterVariable(&invisalpha);
  Cvar_RegisterVariable(&player_rail_delay);
  Cvar_RegisterVariable(&player_rail_shiftx);
  Cvar_RegisterVariable(&player_rail_shifty);
  Cvar_RegisterVariable(&player_becheromet_shiftx);
  Cvar_RegisterVariable(&player_becheromet_shifty);
  Cvar_RegisterVariable(&player_pistolka_shiftx);
  Cvar_RegisterVariable(&player_pistolka_shifty);
  Cvar_RegisterVariable(&player_grenade_shiftx);
  Cvar_RegisterVariable(&player_grenade_shifty);
  Cvar_RegisterVariable(&player_grenade_movement);
  Cvar_RegisterVariable(&bomb_mv_speed);
  Cvar_RegisterVariable(&fray_gr_sx);
  Cvar_RegisterVariable(&fray_gr_sy);
  Cvar_RegisterVariable(&max_fray);
  Cvar_RegisterVariable(&skin_gr_sx);
  Cvar_RegisterVariable(&skin_gr_sy);
  Cvar_RegisterVariable(&rail_gr_sx);
  Cvar_RegisterVariable(&rail_gr_sy);
  Cvar_RegisterVariable(&max_rail);
  Cvar_RegisterVariable(&rail_life);
  Cvar_RegisterVariable(&max_ammo_bombs);
  Cvar_RegisterVariable(&max_ammo_grenades);
  Cvar_RegisterVariable(&max_ammo_railgun);
  Cvar_RegisterVariable(&max_ammo_pistolka);
  Cvar_RegisterVariable(&max_ammo_becheromet);
  Cvar_RegisterVariable(&spec_ammo_bombs);
  Cvar_RegisterVariable(&spec_ammo_grenades);
  Cvar_RegisterVariable(&spec_ammo_railgun);
  Cvar_RegisterVariable(&spec_ammo_becheromet);
  Cvar_RegisterVariable(&spec_ammo_pistolka);
  Cvar_RegisterVariable(&spec_infra_time);
  Cvar_RegisterVariable(&spec_reverse_time);
  Cvar_RegisterVariable(&spec_slow_time);
  Cvar_RegisterVariable(&spec_speed_time);
  Cvar_RegisterVariable(&spec_glue_time);
  Cvar_RegisterVariable(&spec_invis_time);
  Cvar_RegisterVariable(&spec_noweapon_time);
  Cvar_RegisterVariable(&spec_berserk_time);
  Cvar_RegisterVariable(&max_warps);
  Cvar_RegisterVariable(&max_shields);
  Cvar_RegisterVariable(&spec_add_warps);
  Cvar_RegisterVariable(&spec_add_shields);
  Cvar_RegisterVariable(&extra_power);
  Cvar_RegisterVariable(&points_becher);
  Cvar_RegisterVariable(&points_rapid);
  Cvar_RegisterVariable(&points_coct1);
  Cvar_RegisterVariable(&points_coct2);
  Cvar_RegisterVariable(&points_gold1);
  Cvar_RegisterVariable(&points_gold2);
  Cvar_RegisterVariable(&points_frag);
  Cvar_RegisterVariable(&points_mfrag);
  Cvar_RegisterVariable(&points_superbecher);
  Cvar_RegisterVariable(&points_superbecherdestroy);
  
  // local
  Cvar_RegisterVariable(&script_dir);
  Cvar_RegisterVariable(&map_dir);
  Cvar_RegisterVariable(&gfx_dir);
  Cvar_RegisterVariable(&skin_dir);
  Cvar_RegisterVariable(&snd_dir);
  Cvar_RegisterVariable(&gui_dir);
  Cvar_RegisterVariable(&sprite_dir);
  
  Cvar_RegisterVariable(&map_ext);
  Cvar_RegisterVariable(&script_ext);
  Cvar_RegisterVariable(&smap_ext);
  Cvar_RegisterVariable(&skin_ext);
  Cvar_RegisterVariable(&tmp_ext);
  
  Cvar_RegisterVariable(&last_server);
  Cvar_RegisterVariable(&menu_music_file);
  Cvar_RegisterVariable(&music_volume);
  Cvar_RegisterVariable(&sound_volume);
  Cvar_RegisterVariable(&autolog);
  
  Cvar_RegisterVariable(&g_results_x);
  Cvar_RegisterVariable(&g_results_y);
  Cvar_RegisterVariable(&g_results_time);
  
  // client
  Cvar_RegisterVariable(&c_deletetmps);
  Cvar_RegisterVariable(&c_downloading);
  Cvar_RegisterVariable(&c_uploading);
  
  Cvar_RegisterVariable(&c_name);
  Cvar_RegisterVariable(&c_desc);
  
  Cvar_RegisterVariable(&p1_on);
  Cvar_RegisterVariable(&p1_name);
  Cvar_RegisterVariable(&p1_skin);
  Cvar_RegisterVariable(&p1_key_up);
  Cvar_RegisterVariable(&p1_key_down);
  Cvar_RegisterVariable(&p1_key_left);
  Cvar_RegisterVariable(&p1_key_right);
  Cvar_RegisterVariable(&p1_key_fire);
  Cvar_RegisterVariable(&p1_key_shield);
  Cvar_RegisterVariable(&p1_key_warp);
  Cvar_RegisterVariable(&p1_key_weapon1);
  Cvar_RegisterVariable(&p1_key_weapon2);
  Cvar_RegisterVariable(&p1_key_weapon3);
  Cvar_RegisterVariable(&p1_key_weapon4);
  Cvar_RegisterVariable(&p1_key_weapon5);
  Cvar_RegisterVariable(&p1_keyn_up);
  Cvar_RegisterVariable(&p1_keyn_down);
  Cvar_RegisterVariable(&p1_keyn_left);
  Cvar_RegisterVariable(&p1_keyn_right);
  Cvar_RegisterVariable(&p1_keyn_fire);
  Cvar_RegisterVariable(&p1_keyn_shield);
  Cvar_RegisterVariable(&p1_keyn_warp);
  Cvar_RegisterVariable(&p1_keyn_weapon1);
  Cvar_RegisterVariable(&p1_keyn_weapon2);
  Cvar_RegisterVariable(&p1_keyn_weapon3);
  Cvar_RegisterVariable(&p1_keyn_weapon4);
  Cvar_RegisterVariable(&p1_keyn_weapon5);
  
  Cvar_RegisterVariable(&p2_on);
  Cvar_RegisterVariable(&p2_name);
  Cvar_RegisterVariable(&p2_skin);
  Cvar_RegisterVariable(&p2_key_up);
  Cvar_RegisterVariable(&p2_key_down);
  Cvar_RegisterVariable(&p2_key_left);
  Cvar_RegisterVariable(&p2_key_right);
  Cvar_RegisterVariable(&p2_key_fire);
  Cvar_RegisterVariable(&p2_key_shield);
  Cvar_RegisterVariable(&p2_key_warp);
  Cvar_RegisterVariable(&p2_key_weapon1);
  Cvar_RegisterVariable(&p2_key_weapon2);
  Cvar_RegisterVariable(&p2_key_weapon3);
  Cvar_RegisterVariable(&p2_key_weapon4);
  Cvar_RegisterVariable(&p2_key_weapon5);
  Cvar_RegisterVariable(&p2_keyn_up);
  Cvar_RegisterVariable(&p2_keyn_down);
  Cvar_RegisterVariable(&p2_keyn_left);
  Cvar_RegisterVariable(&p2_keyn_right);
  Cvar_RegisterVariable(&p2_keyn_fire);
  Cvar_RegisterVariable(&p2_keyn_shield);
  Cvar_RegisterVariable(&p2_keyn_warp);
  Cvar_RegisterVariable(&p2_keyn_weapon1);
  Cvar_RegisterVariable(&p2_keyn_weapon2);
  Cvar_RegisterVariable(&p2_keyn_weapon3);
  Cvar_RegisterVariable(&p2_keyn_weapon4);
  Cvar_RegisterVariable(&p2_keyn_weapon5);
  
  Cvar_RegisterVariable(&p3_on);
  Cvar_RegisterVariable(&p3_name);
  Cvar_RegisterVariable(&p3_skin);
  Cvar_RegisterVariable(&p3_key_up);
  Cvar_RegisterVariable(&p3_key_down);
  Cvar_RegisterVariable(&p3_key_left);
  Cvar_RegisterVariable(&p3_key_right);
  Cvar_RegisterVariable(&p3_key_fire);
  Cvar_RegisterVariable(&p3_key_shield);
  Cvar_RegisterVariable(&p3_key_warp);
  Cvar_RegisterVariable(&p3_key_weapon1);
  Cvar_RegisterVariable(&p3_key_weapon2);
  Cvar_RegisterVariable(&p3_key_weapon3);
  Cvar_RegisterVariable(&p3_key_weapon4);
  Cvar_RegisterVariable(&p3_key_weapon5);
  Cvar_RegisterVariable(&p3_keyn_up);
  Cvar_RegisterVariable(&p3_keyn_down);
  Cvar_RegisterVariable(&p3_keyn_left);
  Cvar_RegisterVariable(&p3_keyn_right);
  Cvar_RegisterVariable(&p3_keyn_fire);
  Cvar_RegisterVariable(&p3_keyn_shield);
  Cvar_RegisterVariable(&p3_keyn_warp);
  Cvar_RegisterVariable(&p3_keyn_weapon1);
  Cvar_RegisterVariable(&p3_keyn_weapon2);
  Cvar_RegisterVariable(&p3_keyn_weapon3);
  Cvar_RegisterVariable(&p3_keyn_weapon4);
  Cvar_RegisterVariable(&p3_keyn_weapon5);
  
  Cvar_RegisterVariable(&p4_on);
  Cvar_RegisterVariable(&p4_name);
  Cvar_RegisterVariable(&p4_skin);
  Cvar_RegisterVariable(&p4_key_up);
  Cvar_RegisterVariable(&p4_key_down);
  Cvar_RegisterVariable(&p4_key_left);
  Cvar_RegisterVariable(&p4_key_right);
  Cvar_RegisterVariable(&p4_key_fire);
  Cvar_RegisterVariable(&p4_key_shield);
  Cvar_RegisterVariable(&p4_key_warp);
  Cvar_RegisterVariable(&p4_key_weapon1);
  Cvar_RegisterVariable(&p4_key_weapon2);
  Cvar_RegisterVariable(&p4_key_weapon3);
  Cvar_RegisterVariable(&p4_key_weapon4);
  Cvar_RegisterVariable(&p4_key_weapon5);
  Cvar_RegisterVariable(&p4_keyn_up);
  Cvar_RegisterVariable(&p4_keyn_down);
  Cvar_RegisterVariable(&p4_keyn_left);
  Cvar_RegisterVariable(&p4_keyn_right);
  Cvar_RegisterVariable(&p4_keyn_fire);
  Cvar_RegisterVariable(&p4_keyn_shield);
  Cvar_RegisterVariable(&p4_keyn_warp);
  Cvar_RegisterVariable(&p4_keyn_weapon1);
  Cvar_RegisterVariable(&p4_keyn_weapon2);
  Cvar_RegisterVariable(&p4_keyn_weapon3);
  Cvar_RegisterVariable(&p4_keyn_weapon4);
  Cvar_RegisterVariable(&p4_keyn_weapon5);
  
  // server
  Cvar_RegisterVariable(&s_deletetmps);
  Cvar_RegisterVariable(&s_downloading);
  Cvar_RegisterVariable(&s_uploading);
  Cvar_RegisterVariable(&s_remote);
  Cvar_RegisterVariable(&s_acpass);
  
  Cvar_RegisterVariable(&s_maxclients);
  Cvar_RegisterVariable(&s_name);
  Cvar_RegisterVariable(&s_welcome_msg);
  Cvar_RegisterVariable(&s_next_map);
  Cvar_RegisterVariable(&s_next_script);
  Cvar_RegisterVariable(&s_next_gt);
  Cvar_RegisterVariable(&s_fraglimit);
  Cvar_RegisterVariable(&s_timelimit);
  // net
  Cvar_RegisterVariable(&net_inactive_timeout);
  Cvar_RegisterVariable(&net_drop_timeout);
  Cvar_RegisterVariable(&net_file_timeout);
  Cvar_RegisterVariable(&net_ping_timeout);
  
  // messenger
  Cvar_RegisterVariable(&messenger_x);
  Cvar_RegisterVariable(&messenger_y);
  Cvar_RegisterVariable(&messenger_time);
  
  // chat
  Cvar_RegisterVariable(&chat_x);
  Cvar_RegisterVariable(&chat_y);

  // sound
  Cvar_RegisterVariable(&snd_swap);
  
  Cvar_RegisterVariable(&snd_dist);
  Cvar_RegisterVariable(&snd_roll);
  Cvar_RegisterVariable(&snd_dopp);
  Cvar_RegisterVariable(&snd_3don);
  
  Cvar_RegisterVariable(&fullscreen);
  Cvar_RegisterVariable(&doublebuf);
  Cvar_RegisterVariable(&glblit);
  Cvar_RegisterVariable(&pw2_gamma_r);
  Cvar_RegisterVariable(&pw2_gamma_g);
  Cvar_RegisterVariable(&pw2_gamma_b);
  Cvar_RegisterVariable(&alphamenu);
  Cvar_RegisterVariable(&autoserver);
  Cvar_RegisterVariable(&theme);
}

void AddConsoleCommands()
{
  // commands available only on local machine
  AddCommand(&Help, "help");
  AddCommand(&KillProgram, "quit");
  AddCommand(&ListConCmds, "list");
  AddCommand(&Video, "video");
  AddCommand(&MapInfo, "mapinfo");
  AddCommand(&BlitInfo, "blitinfo");
  
  AddCommand(&SpriteScan, "sprite_scan");
  AddCommand(&SpriteList, "sprite_list");
  AddCommand(&SpriteLoad, "sprite_load");
  AddCommand(&SpriteFree, "sprite_free");
  AddCommand(&SkinScan, "skin_scan");
  AddCommand(&SkinList, "skin_list");
  AddCommand(&SkinLoad, "skin_load");
  AddCommand(&SkinFree, "skin_free");
  AddCommand(&MapScan, "map_scan");
  AddCommand(&MapList, "map_list");
  AddCommand(&MapScriptScan, "script_scan");
  AddCommand(&MapScriptList, "script_list");
#ifdef PW_MUSIC
  AddCommand(&Pause_Music, "mus_pause");
  AddCommand(&Play_Music, "mus_play");
  AddCommand(&Release_Music, "mus_release");
  AddCommand(&Volume_Music, "mus_volume");
  AddCommand(&Volume_Sound, "snd_volume");
#endif
  AddCommand(&Say, "say");
  
  // commands available only on client
  AddCommand(&CStartClient, "sc", CMD_CLIENT);
  AddCommand(&CEndClient, "ec", CMD_CLIENT);
  AddCommand(&CConnectClient, "connect", CMD_CLIENT);
  AddCommand(&CReconnectClient, "reconnect", CMD_CLIENT);
  AddCommand(&CDisconnectClient, "disconnect", CMD_CLIENT);
  //  AddCommand( &CSetClientName,      "name",           CMD_CLIENT);
  AddCommand(&CAttachCon, "attach", CMD_CLIENT);
  AddCommand(&CDetachCon, "detach", CMD_CLIENT);
  AddCommand(&CDownloadMap, "dlmap", CMD_CLIENT);
  AddCommand(&CUploadMap, "ulmap", CMD_CLIENT);
  AddCommand(&CDownloadSprite, "dlsprite", CMD_CLIENT);
  AddCommand(&CUploadSprite, "ulsprite", CMD_CLIENT);
  AddCommand(&CDownloadSkin, "dlskin", CMD_CLIENT);
  AddCommand(&CUploadSkin, "ulskin", CMD_CLIENT);
  AddCommand(&CDownloadSound, "dlsound", CMD_CLIENT);
  AddCommand(&CUploadSound, "ulsound", CMD_CLIENT);
  AddCommand(&CCreatePlayer, "cp", CMD_CLIENT);
  AddCommand(&CDestroyPlayer, "dp", CMD_CLIENT);
  AddCommand(&CSkinPlayer, "sp", CMD_CLIENT);
  AddCommand(&CChasePlayer, "chase", CMD_CLIENT);
  
  // commands available only on server and remote from clients
  AddCommand(&AddScript, "sc_add", CMD_SERVER);
  AddCommand(&InstanceScript, "sc_instance", CMD_SERVER);
  AddCommand(&RunScript, "sc_run", CMD_SERVER);
  AddCommand(&DelInstanceScript, "sc_delinst", CMD_SERVER);
  AddCommand(&DeleteScript, "sc_delete", CMD_SERVER);
  AddCommand(&MakeScript, "sc_make", CMD_SERVER);
  AddCommand(&ListScripts, "sc_list", CMD_SERVER);
  AddCommand(&SelScript, "sc_sel", CMD_SERVER);
  AddCommand(&SGet_data, "?", CMD_SERVER);
  AddCommand(&SSet_data, "=", CMD_SERVER);
  
  AddCommand(&CStartServer, "ss", CMD_SERVER);
  AddCommand(&CEndServer, "es", CMD_SERVER);
  AddCommand(&CRunServer, "rs", CMD_SERVER);
  AddCommand(&CShutdownServer, "ds", CMD_SERVER);
  //  AddCommand( &CSetServerName,      "server_name",    CMD_SERVER);
  AddCommand(&SetMap, "map", CMD_SERVER);
  AddCommand(&CDropClient, "kick", CMD_SERVER);
}

#include "gui.h"


void RecreatePlayers()
{
  if (p1_on.value) {
    strcpy(auto_playername[0], p1_name.string);
    time_playername = ticktime + 2000;
    strcpy(auto_skinname[0], p1_skin.string);
    strcpy(auto_skinplayername[0], p1_name.string);
    time_skinname = ticktime + 3000;
    strcpy(auto_chasename[0], p1_name.string);
    time_chasename = ticktime + 2500;
  } else {
    auto_playername[0][0] = 0;
    auto_skinname[0][0] = 0;
    auto_skinplayername[0][0] = 0;
    auto_chasename[0][0] = 0;
  }
  if (p2_on.value) {
    strcpy(auto_playername[1], p2_name.string);
    time_playername = ticktime + 2000;
    strcpy(auto_skinname[1], p2_skin.string);
    strcpy(auto_skinplayername[1], p2_name.string);
    time_skinname = ticktime + 3000;
    strcpy(auto_chasename[1], p2_name.string);
    time_chasename = ticktime + 2500;
  } else {
    auto_playername[1][0] = 0;
    auto_skinname[1][0] = 0;
    auto_skinplayername[1][0] = 0;
    auto_chasename[1][0] = 0;
  }
  if (p3_on.value) {
    strcpy(auto_playername[2], p3_name.string);
    time_playername = ticktime + 2000;
    strcpy(auto_skinname[2], p3_skin.string);
    strcpy(auto_skinplayername[2], p3_name.string);
    time_skinname = ticktime + 3000;
    strcpy(auto_chasename[2], p3_name.string);
    time_chasename = ticktime + 2500;
  } else {
    auto_playername[2][0] = 0;
    auto_skinname[2][0] = 0;
    auto_skinplayername[2][0] = 0;
    auto_chasename[2][0] = 0;
  }
  if (p4_on.value) {
    strcpy(auto_playername[3], p4_name.string);
    time_playername = ticktime + 3000;
    strcpy(auto_skinname[3], p4_skin.string);
    strcpy(auto_skinplayername[3], p4_name.string);
    time_skinname = ticktime + 2500;
    strcpy(auto_chasename[3], p4_name.string);
    time_chasename = ticktime + 2500;
  } else {
    auto_playername[3][0] = 0;
    auto_skinname[3][0] = 0;
    auto_skinplayername[3][0] = 0;
    auto_chasename[3][0] = 0;
  }
  
}

//###########################################################################
//## Event hook
//## 
//## Processes all the incoming events 
//## Put the console down if the user hits the '`' key
//## and EnableUNICODE when the console is down so that the events sent 
//## to the console can be translated into text
//## 
//###########################################################################

void WaitForKeypress()
{
  SDL_Event event;
  
  while (1) {
    UpdateSplash(screen, -1);
    
    while (SDL_PollEvent(&event))
      switch (event.type) {
      case SDL_QUIT:
        App_Quit();
        return;
      case SDL_KEYDOWN:
        return;
    }
  }
}

void ProcessEvents()
{
  SDL_Event event;
  if (net_client_status != NS_CONNECTED) ResetChat();
  
  if (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
        App_Quit();
      break;
    case SDL_ACTIVEEVENT:
      if (event.active.state & SDL_APPINPUTFOCUS) {
        if (event.active.gain) {
          ConOut("App activated");
          App_Activate();
          ConOut("... lost surfaces restored");
        } else {
          ConOut("App deactivated");
          App_Deactivate();
        }
      }
      break;
    case SDL_KEYDOWN:
      if (chat_on)
      {
        ChatEvents(&event);
      }
      else
      {
        switch (event.key.keysym.sym) {
        case SDLK_BACKQUOTE:
          if (ConsoleDown) {
            ConsoleDown = false;
            blocked_inputs = 0;
          } else {
            ConsoleDown = true;
            blocked_inputs = 1;
          }
			break;
          //continue;
        case SDLK_F1:
          if (!NetStatsDown) {
            NetStatsDown = true;
          } else {
            NetStatsDown = false;
          }
          break;
        case SDLK_F2:
          if (!ServerView) {
            if (net_server_status == NS_RUNNING)
              ServerView = true;
          } else {
            ServerView = false;
          }
          break;
        case SDLK_F3:
          if (!InfoDown) {
            InfoDown = true;
          } else {
            InfoDown = false;
          }
          break;
        case SDLK_ESCAPE:
          if (!ConsoleDown)
          {
            if (!GUI_id) {
              GUI_OpenMenu(GUI_MAINMENU);
            } else {
              if (GUI_id != GUI_MAINMENU || client_info.active)
                GUI_Return();
            }
          }
          break;
        case SDLK_SPACE:
          if (net_client_status == NS_VIEWING_RESULTS)	// user wants shutdown results and proceed to next map
          {
            GUI_Return();
          
            // do clearing
            client_info.active = false;
            CL_Clear();
            enable_menu_music = 0;
          
          
            // now client is disconnected
          
            // reconnect sequence
            CommandExecuteOut("connect %s", last_server.string);
            waiting_connection = true;
          
            // recreate players
            RecreatePlayers();
          
            // fight
          }
          break;
        case SDLK_t:
          if (net_client_status == NS_CONNECTED) chat_on=true;
          break;
        }
      }
    }
    
    
    
    // If the console is down send events that were not
    // filtered out yet to the console 
    if (ConsoleDown) {
      ConsoleEvents(&event);
      //continue;
    }
    
    if (GUI_id) {
      PG_MessageObject::PumpIntoEventQueue(&event);
      //continue;
    }
    
    if (ServerView)
      switch (event.type) {
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_LEFT:
          if (server_info.game.vars.camx - CSX >= 0)
            server_info.game.vars.camx -= CSX;
          break;
        case SDLK_RIGHT:
          if (server_info.game.vars.camx + CSX <=
            (mapwidth * mapblockwidth) - MSCRW)
            server_info.game.vars.camx += CSX;
          break;
        case SDLK_UP:
          if (server_info.game.vars.camy - CSY >= 0)
            server_info.game.vars.camy -= CSY;
          break;
        case SDLK_DOWN:
          if (server_info.game.vars.camy + CSY <=
            (mapheight * mapblockheight) - MSCRH)
            server_info.game.vars.camy += CSY;
          break;
        case SDLK_F12:
          App_Quit();
          break;
        }
    }
  }
  while(SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_KEYDOWNMASK | SDL_MOUSEMOTIONMASK | SDL_SYSWMEVENTMASK) > 0);

}

void Renderscreen(SDL_Surface * screen)
{
  static int counter = 0;
  SDL_Rect r;
  
  if (net_client_status == NS_VIEWING_RESULTS) {
    DrawResultsBack(screen);
    if (!GUI_id) {
      GUI_OpenMenu(GUI_RESULTS);
    }
    RenderResults(screen);
  } else {
    if (MapLoaded != 2)		//(client_info.game.state!=GS_INITED)
    {
      DrawGUIBack(screen);
    } else {
      r.x = 0;
      r.y = 0;
      r.h = 480;
      r.w = 480;
      DrawGameBack(screen);
      
      r.x = 480;
      r.y = 0;
      r.h = gamebar_bkg->h;
      r.w = gamebar_bkg->w;
      SDL_BlitSurface(gamebar_bkg, NULL, screen, &r);
      {
        if (!ServerView) {
          RenderMapScreen(client_info.game);
          client_info.game.RenderStatusBar(screen, SmallFont, 0,
            480 - 32);
          client_info.game.RenderBecherBar(screen, 467, 448 + 4);
          client_info.game.RenderGameBar(screen, SmallFont,
            481 + 3, 4);
        } else {
          RenderMapScreen(server_info.game);
        }
      }
#define DS_POS 28
      
      SDL_Rect s;
      char genstr[100];
      if (net_client_status != NS_UNINITED) {
        r.x = 480 + 9;
        r.y = 426;
        r.w = udbar->w;
        r.h = 10;
        s.x = 0;
        s.w = udbar->w;
        s.h = 10;
        if (c_uploading.value) {
          s.y = 0;
          SDL_BlitSurface(udbar, &s, screen, &r);
        } else {
          s.y = 10;
          SDL_BlitSurface(udbar, &s, screen, &r);
        }
        
        r.y = 426 + 10;
        if (c_downloading.value) {
          s.y = 20;
          SDL_BlitSurface(udbar, &s, screen, &r);
        } else {
          s.y = 30;
          SDL_BlitSurface(udbar, &s, screen, &r);
        }
        // print client file transfer info
        if (server.file_status == FS_SENDING) {
          sprintf(genstr, "file %s", server.local_fn);
          DrawText(genstr, screen, 1, screen->w - 150, 396);	// consolefont should be 0
          sprintf(genstr, "progress %d/%d", server.zs.total_in,
            server.zs.total_out);
          DrawText(genstr, screen, 1, screen->w - 150, 409);	// consolefont should be 0
        } else {
          if (server.file_status == FS_RECEIVING) {
            sprintf(genstr, "file %s", server.local_fn);
            DrawText(genstr, screen, 1, screen->w - 150, 396);	// consolefont should be 0
            sprintf(genstr, "progress %d/%d",
              server.zs.total_in, server.zs.total_out);
            DrawText(genstr, screen, 1, screen->w - 150, 409);	// consolefont should be 0
          } else {
            int x = client_info.game.tick / FPS;
            
            if (client_info.game.tick & 8)
              sprintf(genstr, "%d:%02d", x / 60, x % 60);
            else
              sprintf(genstr, "%d %02d", x / 60, x % 60);
            DrawText(genstr, screen, 1,
              screen->w - strlen(genstr) * 6 - 4, 409);	// consolefont should be 0
          }
        }
      }
      
      
      
    }
    
  }
  
  
  counter++;
}


void DoneMap()
{
  MapFreeMem();
}

//extern "C" HWND ReturnSDLHWND();  //woid

//###########################################################################
//## Program entry point
//## 
//## 
//###########################################################################
#define CI_POS 20

main(int argc, char *argv[])
{
  
/*
#ifdef _DEBUG
{
// Get current flag
int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );

		// Turn on leak-checking bit
    tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
    
      // Set flag to the new value
      _CrtSetDbgFlag( tmpFlag );
      }
      #endif
  */
  char fname[_MAX_PATH];
  
  srand((unsigned) time(NULL));
  // Add console commands
  AddConsoleCommands();
  AddConsoleVars();
  
#ifndef WIN32
  sprintf(PW_CONFIG_FILE_PERSONAL, "%s/.pw2config", getenv("HOME"));
  if(LoadConfig(PW_CONFIG_FILE_PERSONAL) != 0) {
    fprintf(stderr, "trying global config: %s\n", PW_CONFIG_FILE);
    if(LoadConfig(PW_CONFIG_FILE) == 0) {
      fprintf(stderr, "saving personal config: %s\n", PW_CONFIG_FILE_PERSONAL);
      SaveConfig(PW_CONFIG_FILE_PERSONAL);
    }
  }
#else
  LoadConfig(PW_CONFIG_FILE);
#endif
  
  PG_Application app;
  strcpy(fname, gui_dir.string);
  if (fname[strlen(fname) - 1] == '/')
    fname[strlen(fname) - 1] = 0;
  app.SetApplicationPath(fname);
  if (!app.LoadTheme(theme.string, true, fname))
  {
    fprintf(stderr, "Couldn't load menu theme %s from %s\nCheck config.cfg for \"gui_dir\" and \"theme\"\n", theme.string, gui_dir.string);
    if (!app.LoadTheme(DEFAULT_THEME, true, fname)) {
      fprintf(stderr, "Couldn't load default menu theme %s from %s\n", DEFAULT_THEME, gui_dir.string);
      exit(2);
    }
  }
  
  Uint32 videoflags;
  
  int ticks = 0, oldFPSticks = 0, oldPINGticks = 0;
  char framerate[30] = "";
  char pingstr[30] = "";
  char genstr[60] = "";
  char tempstr[60] = "";
  
#ifdef PW_MUSIC
  int audio_rate;
  Uint16 audio_format;
  int audio_channels;
  int audio_buffers;
  int looping = 0;
  int interactive = 0;
  
#ifdef PW_BASS
  // Initialize audio variables 
  audio_rate = 44050;
  audio_format = AUDIO_S16;
  audio_channels = 2;
  audio_buffers = 8 * 4096;
#else
  // Initialize with SDL_mixer defaults
  audio_rate = MIX_DEFAULT_FREQUENCY;
  audio_format = MIX_DEFAULT_FORMAT;
  audio_channels = 2;
#ifdef __BEOS__
  audio_buffers = 4 * 4096;
#else
  audio_buffers = 1024;
#endif
#endif
  
#endif
  
  char curr_date[20];
  char curr_time[20];
  
  strcpy(curr_date, "");
  strcpy(curr_time, "");
  
#ifdef WIN32

  _strdate(curr_date);
  _strtime(curr_time);

#else

  time_t curtime;
  struct tm *loctime;

  curtime = time (NULL);
  loctime = localtime (&curtime);

  strftime(curr_date, sizeof(curr_date), "%d.%m.%y", loctime);
  strftime(curr_time, sizeof(curr_time), "%H:%M:%S", loctime);

#endif
  
  fprintf(stderr, "+-----------------------------------------+\n");
  fprintf(stderr, "| PacWars2 log file                       |\n");
  fprintf(stderr, "|   version %2d.%02d                         |\n",
    VERSION_MAJOR, VERSION_MINOR);
  fprintf(stderr, "|   build %04d                            |\n",
    build_number);
  fprintf(stderr, "+-----------------------------------------+\n");
  fprintf(stderr, "executed: %s on %s, %s\n\n", argv[0], curr_time, curr_date);
  
  // parse and process program's arguments
  videoflags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN;	//|SDL_ASYNCBLIT|SDL_OPENGLBLIT;
  if (!fullscreen.value)
    videoflags &= ~SDL_FULLSCREEN;
  if (glblit.value) {
    videoflags |= SDL_OPENGLBLIT;
    videoflags &= ~SDL_DOUBLEBUF;
  }
  if (!doublebuf.value)
    videoflags &= ~SDL_DOUBLEBUF;
  
  video_bpp = 16;
  while (argc > 1) {
    --argc;
    if (strcmp(argv[argc - 1], "-bpp") == 0) {
      video_bpp = atoi(argv[argc]);
      --argc;
    } else if (strcmp(argv[argc], "-sw") == 0) {
      videoflags |= SDL_SWSURFACE;
      videoflags &= ~SDL_HWSURFACE;
    } else if (strcmp(argv[argc], "-warp") == 0) {
      videoflags |= SDL_HWPALETTE;
    } else if (strcmp(argv[argc], "-gl") == 0) {
      videoflags |= SDL_OPENGLBLIT;
      videoflags &= ~SDL_DOUBLEBUF;
    } else if (strcmp(argv[argc], "-nosound") == 0) {
      sound_hw_enabled = 0;
    } else if (strcmp(argv[argc], "-no3dsound") == 0) {
      sound_3d_enabled = 0;
    } else if (strcmp(argv[argc], "-windowed") == 0) {
      videoflags &= ~SDL_FULLSCREEN;
    } else {
      if (((strcmp(argv[argc], "?") == 0)
        || (strcmp(argv[argc], "/?") == 0))) {
        fprintf(stderr,
          "Usage: %s [-bpp N] [-warp] [-sw] [-windowed] [-nosound] [-gl]\n",
          argv[0]);
        exit(1);
      }
    }
  }
  if (snd_3don.value != 1.0f)
    sound_3d_enabled = false;
  
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_NOPARACHUTE) < 0) {
    fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
    exit(1);
  }
  atexit(SDL_Quit);
  
  // Intialize network
  if (InitNet() < 0) {
    fprintf(stderr, "Couldn't inititalize network: %s\n",
      SDL_GetError());
    exit(2);
  }
  
  SDL_WM_SetCaption("PacWars2", NULL);
  FileNameConversion(gfx_dir.string, "icon", "bmp", tmptxt);
  SDL_WM_SetIcon(SDL_LoadBMP(tmptxt), NULL);
  // Set video mode
  if (!app.InitScreen(640, 480, video_bpp, videoflags)) {
    fprintf(stderr, "Couldn't set 640x480x%d video mode: %s\n",
      video_bpp, SDL_GetError());
    exit(2);
  }
  SDL_ShowCursor(false);
  
  screen = app.GetScreen();
  
  SDL_Surface *fake_screen = SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_SRCCOLORKEY|SDL_SRCALPHA, 640, 480, video_bpp, 0, 0, 0, 0);
  SDL_FillRect(fake_screen, NULL, 0x0);
  app.SetScreen(fake_screen);
  
  ResetGamma();
  
#ifdef PW_AUDIO
#ifdef PW_BASS
  // try initializing the default device, at 44100hz stereo 16 bits
  if (sound_hw_enabled) {
    int flags = 0;
    if (sound_3d_enabled)
      flags |= BASS_DEVICE_3D;
    if (!BASS_Init(-1, 44100, flags, GetForegroundWindow())) {
      // couldn't initialize device, so use no sound
      BASS_Init(-2, 44100, flags, GetForegroundWindow());
      fprintf(stderr, "Couldn't open audio: %s\n, using nosound",
        SDL_GetError());
      audio_open = 0;
    } else {
      printf("Opened audio at 44100\n");
      audio_open = 1;
      if (sound_3d_enabled)
        printf("3D sound enabled\n");
      else
        printf("3D sound disabled\n");
    }
  }
#else
  // Open the audio device
  if (sound_hw_enabled) {
    if (Mix_OpenAudio
      (audio_rate, audio_format, audio_channels, audio_buffers) < 0) {
      fprintf(stderr, "Couldn't open audio: %s\n, using nosound",
        SDL_GetError());
      audio_open = 0;
    } else {
      Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
      printf
        ("Opened audio at %d Hz %d bit %s, %d bytes audio buffer\n",
        audio_rate, (audio_format & 0xFF),
        (audio_channels > 1) ? "stereo" : "mono", audio_buffers);
      audio_open = 1;
    }
  }
#endif
#endif
  
  SDL_EnableUNICODE(1);

  ConOut("PacWars2 - version %d.%02d", VERSION_MAJOR, VERSION_MINOR);
  ConOut("");
  // Inform about previous initializations
#ifdef PW_AUDIO
#ifdef PW_BASS
  BASS_Start();				// Start digital output
  ConOut("BASS sound sytem initialized.");
#else
  ConOut("MikMod sound system initialized.");
#endif
#endif
  ConOut("Video hardware initialized.");
  
  
  ConOut("Network engine initialized.");
  
#ifdef PW_MUSIC
#ifdef PW_BASS
  Play_Music(menu_music_file.string);
  Volume_Music(music_volume.string);
  enable_menu_music = 0;
#else
  // Set the external music player, if any
  Mix_SetMusicCMD(getenv("MUSIC_CMD"));
#endif
  // Load the requested music file
#endif
  
#ifdef PW_SOUND
  smLoadSamples();
  // Load the requested music file
  Volume_Sound(sound_volume.string);
#endif
  
  
  FileNameConversion(gfx_dir.string, "mainscr", PNG_EXT, tmptxt);
  menu_background = LoadPic(tmptxt);
  
  // load&show splash screen
  FileNameConversion(gfx_dir.string, "drzloduch", PNG_EXT, tmptxt);
  splash = LoadPic(tmptxt);
  UpdateSplash(screen, 1);
  FileNameConversion(gfx_dir.string, "drzloduchkey", PNG_EXT, tmptxt);
  splashA = LoadPic(tmptxt);
  UpdateSplash(screen, 1);
  FileNameConversion(gfx_dir.string, "drzloduchkeyn", PNG_EXT, tmptxt);
  splashB = LoadPic(tmptxt);
  UpdateSplash(screen, 1);
  
  FileNameConversion(gfx_dir.string, "gamebar_bkg", PNG_EXT, tmptxt);
  gamebar_bkg = LoadPic(tmptxt);
  UpdateSplash(screen, 3);
  FileNameConversion(gfx_dir.string, "status", PNG_EXT, tmptxt);
  statusbar = LoadPic(tmptxt);
  UpdateSplash(screen, 4);
  FileNameConversion(gfx_dir.string, "statusbar_bkg", PNG_EXT, tmptxt);
  statusbar_bkg = LoadPic(tmptxt);
  UpdateSplash(screen, 5);
  FileNameConversion(gfx_dir.string, "gamebar_playerbar", PNG_EXT,
    tmptxt);
  gamebar_playerbar = LoadPic(tmptxt);
  UpdateSplash(screen, 6);
  FileNameConversion(gfx_dir.string, "bechbar", PNG_EXT, tmptxt);
  becherbar = LoadPic(tmptxt);
  UpdateSplash(screen, 7);
  FileNameConversion(gfx_dir.string, "udbar", PNG_EXT, tmptxt);
  udbar = LoadPic(tmptxt);
  UpdateSplash(screen, 9);
  
  FileNameConversion(gfx_dir.string, "credits2", PNG_EXT, tmptxt);
  credits = LoadPic(tmptxt);
  
  FileNameConversion(gfx_dir.string, "helpscr01", PNG_EXT, tmptxt);
  help1 = LoadPic(tmptxt);
  FileNameConversion(gfx_dir.string, "helpscr02", PNG_EXT, tmptxt);
  help2 = LoadPic(tmptxt);
  
  FileNameConversion(gfx_dir.string, "results", PNG_EXT, tmptxt);
  results = LoadPic(tmptxt);
  
  // Get video info
  if ((VideoInfo = SDL_GetVideoInfo()) == NULL) {
    fprintf(stderr, "Couldn't retrieve video info: %s\n",
      SDL_GetError());
    exit(2);
  }
  UpdateSplash(screen, 10);
  
  // Init the console
  int tmp_font;
  FileNameConversion(gfx_dir.string, "cfont0", PNG_EXT, tmptxt);
  if (InitConsole(tmptxt, screen, 100)) {
    fprintf(stderr, "Couldn't init console: %s\n", SDL_GetError());
    exit(2);
  }
  FileNameConversion(gfx_dir.string, "cfont1", PNG_EXT, tmptxt);
  tmp_font = LoadFont(tmptxt, TRANS_FONT);
  if (tmp_font == -1) {
    fprintf(stderr, "Couldn't load font: %s\n", tmptxt);
    exit(2);
  }
  FileNameConversion(gfx_dir.string, "cfont2", PNG_EXT, tmptxt);
  tmp_font = LoadFont(tmptxt, TRANS_FONT);
  if (tmp_font == -1) {
    fprintf(stderr, "Couldn't load font: %s\n", tmptxt);
    exit(2);
  }
  FileNameConversion(gfx_dir.string, "cfont3", PNG_EXT, tmptxt);
  tmp_font = LoadFont(tmptxt, TRANS_FONT);
  if (tmp_font == -1) {
    fprintf(stderr, "Couldn't load font: %s\n", tmptxt);
    exit(2);
  }
  FileNameConversion(gfx_dir.string, "cfont4", PNG_EXT, tmptxt);
  tmp_font = LoadFont(tmptxt, TRANS_FONT);
  if (tmp_font == -1) {
    fprintf(stderr, "Couldn't load font: %s\n", tmptxt);
    exit(2);
  }
  FileNameConversion(gfx_dir.string, "cfont5", PNG_EXT, tmptxt);
  tmp_font = LoadFont(tmptxt, TRANS_FONT);
  if (tmp_font == -1) {
    fprintf(stderr, "Couldn't load font: %s\n", tmptxt);
    exit(2);
  }
  FileNameConversion(gfx_dir.string, "cfont6", PNG_EXT, tmptxt);
  tmp_font = LoadFont(tmptxt, TRANS_FONT);
  if (tmp_font == -1) {
    fprintf(stderr, "Couldn't load font: %s\n", tmptxt);
    exit(2);
  }
  UpdateSplash(screen, 13);
  
  
  FileNameConversion(gfx_dir.string, "smallfont", PNG_EXT, tmptxt);
  SmallFont = LoadFont(tmptxt, TRANS_FONT);
  if (SmallFont == -1) {
    fprintf(stderr, "Couldn't load font: %s\n", tmptxt);
    exit(2);
  }
  UpdateSplash(screen, 16);
  
  FileNameConversion(gfx_dir.string, "mesfont", PNG_EXT, tmptxt);
  messenger_font = LoadFont(tmptxt, TRANS_FONT);
  if (messenger_font == -1) {
    fprintf(stderr, "Couldn't load font: %s\n", tmptxt);
    exit(2);
  }
  UpdateSplash(screen, 20);
  chat_font = messenger_font;
  
  FileNameConversion(gfx_dir.string, "netfont", PNG_EXT, tmptxt);
  net_font = LoadFont(tmptxt, 0);
  if (net_font == -1) {
    fprintf(stderr, "Couldn't load font: %s\n", tmptxt);
    exit(2);
  }
  UpdateSplash(screen, 25);
  
  
  // Additional console settings
  FileNameConversion(gfx_dir.string, "conlogo", PNG_EXT, tmptxt);
  ConsoleBackground(tmptxt, screen->w - 32, 0);
  UpdateSplash(screen, 30);
  FileNameConversion(gfx_dir.string, "border", PNG_EXT, tmptxt);
  SetConsoleBorder(tmptxt);
  UpdateSplash(screen, 34);
  
  UpdateSplash(screen, 45);
  
  // Enable the scripting engine
  InitScripting();
  UpdateSplash(screen, 60);
  
  // Init mouse
  InitMouse(0, 0);
  FileNameConversion(gui_dir.string, "mouse", PNG_EXT, fname);
  SDL_RWops *src = SDL_RWFromFile(fname, "rb");
  if ((!src) || (!LoadMouse(src))) {
    fprintf(stderr, "File: Couldn't load Mouse Sprite %s", fname);
    return 2;
  }
  SDL_RWclose(src);
  HideMouse();
  ConOut("Mouse initialized.");
  UpdateSplash(screen, 65);
  
  // Scan for sprites
  ConOut("%d sprite(s) found",
    SpriteMan.Scan(sprite_dir.string, sprite_ext.string));
  UpdateSplash(screen, 70);
  // Scan for skins
  ConOut("%d skin(s) found",
    SkinMan.Scan(skin_dir.string, skin_ext.string));
  UpdateSplash(screen, 75);
  // Scan for map
  ConOut("%d map(s) found", MapMan.Scan(map_dir.string, map_ext.string));
  UpdateSplash(screen, 80);
  // Scan for scripts
  ConOut("%d scripts(s) found",
    SMapMan.Scan(script_dir.string, smap_ext.string));
  UpdateSplash(screen, 85);
  
  // Init GUI system
  if (!GUI_Init(screen))
    return 2;
  GUI_OKDialog1 OKDialog1("");
  OKD1 = &OKDialog1;
  GUI_System = &OKDialog1;
  GUI_OKDialog2 OKDialog2("");
  OKDialog1.next = &OKDialog2;
  OKD2 = &OKDialog2;
  GUI_YNDialog YNDialog;
  OKDialog2.next = &YNDialog;
  YND = &YNDialog;
  GUI_MainMenu MainMenu;
  YNDialog.next = &MainMenu;
  GUI_NewGameMenu NewGameMenu;
  MainMenu.next = &NewGameMenu;
  GUI_JoinGameMenu JoinGameMenu;
  NewGameMenu.next = &JoinGameMenu;
  GUI_OptionsMenu OptionsMenu;
  JoinGameMenu.next = &OptionsMenu;
  GUI_PathsMenu PathsMenu;
  OptionsMenu.next = &PathsMenu;
  GUI_CreatePlayerSelMenu CreatePlayerMenu;
  PathsMenu.next = &CreatePlayerMenu;
  GUI_CreatePlayerMenu CreatePlayerMenu1(GUI_CREATEPLAYER1, 1);
  CreatePlayerMenu.next = &CreatePlayerMenu1;
  GUI_CreatePlayerMenu CreatePlayerMenu2(GUI_CREATEPLAYER2, 2);
  CreatePlayerMenu1.next = &CreatePlayerMenu2;
  GUI_CreatePlayerMenu CreatePlayerMenu3(GUI_CREATEPLAYER3, 3);
  CreatePlayerMenu2.next = &CreatePlayerMenu3;
  GUI_CreatePlayerMenu CreatePlayerMenu4(GUI_CREATEPLAYER4, 4);
  CreatePlayerMenu3.next = &CreatePlayerMenu4;
  GUI_CreditsMenu CreditsMenu;
  CreatePlayerMenu4.next = &CreditsMenu;
  GUI_SoundMenu SoundMenu;
  CreditsMenu.next = &SoundMenu;
  GUI_ResultsMenu ResultsMenu;
  SoundMenu.next = &ResultsMenu;
  GUI_Help1Menu Help1Menu;
  ResultsMenu.next = &Help1Menu;
  GUI_Help2Menu Help2Menu;
  Help1Menu.next = &Help2Menu;
  GUI_DeletePlayerMenu DeletePlayerMenu;
  Help2Menu.next = &DeletePlayerMenu;
  GUI_ServerMenu ServerMenu;
  DeletePlayerMenu.next = &ServerMenu;
  GUI_VideoMenu VideoMenu;
  ServerMenu.next = &VideoMenu;
  GUI_ClientMenu ClientMenu;
  VideoMenu.next = &ClientMenu;
  
  ConOut("GUI initialized.");
  UpdateSplash(screen, 90);
  
  
  // Console's inviting message
  ConOut("----------------------------------------------------");
  ConOut("At your service, my Lord ...");
  ConOut("");
  
  UpdateSplash(screen, 95);
  server_info.active = false;
  net_server_status = NS_UNINITED;
  
  client_info.active = false;
  net_client_status = NS_UNINITED;
  
  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,
    SDL_DEFAULT_REPEAT_INTERVAL);
  
  // Main execution loop
  firsttick = SDL_GetTicks();
  
  for (int uui = 0; uui < 4; uui++) {
    auto_playername[uui][0] = 0;
    auto_skinname[uui][0] = 0;
    auto_skinplayername[uui][0] = 0;
    auto_chasename[uui][0] = 0;
  }
  UpdateSplash(screen, 100);
  WaitForKeypress();
  
  if (autolog.value) {
    ConOutEx(MISC_FONT, "> automatic starting command sequence: <");
    CommandExecuteOut("ss");
    CommandExecuteOut("rs");
    CommandExecuteOut("sc");
    CommandExecuteOut("connect localhost");
    ConOut("");
    
    CommandExecuteOut("p1_on 1");
    
    RecreatePlayers();
  }
  else
  if (autoserver.value) {
    ConOutEx(MISC_FONT, "> \"autoserver\" command sequence: <");
    CommandExecuteOut("ss");
  }
  
  inloop = true;
  while (MainProgram) {
    PollNet();
    
    // ====== new tick ======
    ticktime = SDL_GetTicks();
    if (server_info.active) {
      SV_Move(ticktime);
    }
    ticktime = SDL_GetTicks();
    if (client_info.active) {
      CL_Move(ticktime);
      client_info.game.UpdateGamebarSlots();
      waiting_connection = false;
    } else {
      if (net_client_status != NS_VIEWING_RESULTS)
        if (!GUI_menu && !waiting_connection) {
          GUI_OpenMenu(GUI_MAINMENU);
        }
    }
    
    // automatic scheduler 1
    if (auto_playername[0][0] && (ticktime > time_playername)) {
      CommandExecuteOut("cp %s 1", auto_playername[0]);
      auto_playername[0][0] = 0;
    }
    if (auto_skinname[0][0] && (ticktime > time_skinname)) {
      CommandExecuteOut("sp %s %s", auto_skinplayername[0],
        auto_skinname[0]);
      auto_skinname[0][0] = 0;
    }
    if (auto_chasename[0][0] && (ticktime > time_chasename)) {
      CommandExecuteOut("chase %s", auto_chasename[0]);
      auto_chasename[0][0] = 0;
    }
    // automatic scheduler 2
    if (auto_playername[1][0] && (ticktime > time_playername)) {
      CommandExecuteOut("cp %s 2", auto_playername[1]);
      auto_playername[1][0] = 0;
    }
    if (auto_skinname[1][0] && (ticktime > time_skinname)) {
      CommandExecuteOut("sp %s %s", auto_skinplayername[1],
        auto_skinname[1]);
      auto_skinname[1][0] = 0;
    }
    if (auto_chasename[1][0] && (ticktime > time_chasename)) {
      CommandExecuteOut("chase %s", auto_chasename[1]);
      auto_chasename[1][0] = 0;
    }
    // automatic scheduler 3
    if (auto_playername[2][0] && (ticktime > time_playername)) {
      CommandExecuteOut("cp %s 3", auto_playername[2]);
      auto_playername[2][0] = 0;
    }
    if (auto_skinname[2][0] && (ticktime > time_skinname)) {
      CommandExecuteOut("sp %s %s", auto_skinplayername[2],
        auto_skinname[2]);
      auto_skinname[2][0] = 0;
    }
    if (auto_chasename[2][0] && (ticktime > time_chasename)) {
      CommandExecuteOut("chase %s", auto_chasename[2]);
      auto_chasename[2][0] = 0;
    }
    // automatic scheduler 4
    if (auto_playername[3][0] && (ticktime > time_playername)) {
      CommandExecuteOut("cp %s 4", auto_playername[3]);
      auto_playername[3][0] = 0;
    }
    if (auto_skinname[3][0] && (ticktime > time_skinname)) {
      CommandExecuteOut("sp %s %s", auto_skinplayername[3],
        auto_skinname[3]);
      auto_skinname[3][0] = 0;
    }
    if (auto_chasename[3][0] && (ticktime > time_chasename)) {
      CommandExecuteOut("chase %s", auto_chasename[3]);
      auto_chasename[3][0] = 0;
    }
    
    ProcessEvents();
    if (MainProgram)
      Renderscreen(screen);
    
    if (InfoDown) {
      // print the framerate
      ticks = SDL_GetTicks();
      frames++;
      if (ticks - oldFPSticks >= FPS_MEASURE) {
        sprintf(framerate, "%.2f FPS", float (1000 * (frames - oldframes)) / (ticks - oldFPSticks));
        oldframes = frames;
        oldFPSticks = ticks;
      }
      
      
      if (ServerView)
        DrawText("Server view", screen, SmallFont, INFO_BASEX + screen->w - 160,
        INFO_BASEY + 0 * 13);
      else
        DrawText("Client view", screen, SmallFont, INFO_BASEX + screen->w - 160,
        INFO_BASEY + 0 * 13);
      
      DrawText(framerate, screen, SmallFont, INFO_BASEX + screen->w - 160, INFO_BASEY + 1 * 13);
      sprintf(genstr, "time %d", ticktime);
      DrawText(genstr, screen, SmallFont, INFO_BASEX + screen->w - 160, INFO_BASEY + 2 * 13);
      
      DrawText(pingstr, screen, SmallFont, INFO_BASEX + screen->w - 160, INFO_BASEY + 3 * 13);
      sprintf(genstr, "server ticks %d", server_info.game.tick);
      DrawText(genstr, screen, SmallFont, INFO_BASEX + screen->w - 160, INFO_BASEY + 4 * 13);
      sprintf(genstr, "client ticks %d", client_info.game.tick);
      DrawText(genstr, screen, SmallFont, INFO_BASEX + screen->w - 160, INFO_BASEY + 5 * 13);
      
      sprintf(genstr, "transfer %d / %d", server.zs.total_in,
        server.zs.total_out);
      DrawText(genstr, screen, SmallFont, INFO_BASEX + screen->w - 160, INFO_BASEY + 8 * 13);
    }
    
    if (MapLoaded != 2) {
      sprintf(genstr, "PW2 v%d.%02d build %04d", VERSION_MAJOR,
        VERSION_MINOR, build_number);
      DrawText(genstr, screen, SmallFont, 480 - 6 * strlen(genstr),
        screen->h - 1 * 13);
    }
    
    if (NetStatsDown)
      DrawNetStats();
    
    if (GUI_menu) {
      if (enable_menu_music) {
        Play_Music(menu_music_file.string);
        Volume_Music(music_volume.string);
        enable_menu_music = 0;
      }
      
      PG_Rect r1, r2;
      GUI_menu->GetClipRects(r1, r2);
      
      if (alphamenu.value) {
        SDL_SetAlpha(app.GetScreen(), SDL_SRCALPHA, MENU_ALPHA);
      } else {
        SDL_SetAlpha(app.GetScreen(), 0, 0);
      }
      
      SDL_SetColorKey(app.GetScreen(), SDL_SRCCOLORKEY, 0x0);
      SDL_BlitSurface(app.GetScreen(), &r2, screen, &r2);

		// will change that again -- Alex
      /*if (GUI_id != GUI_MAINMENU) {
        GUI_menu->DrawHLine(1, 1, r2.w + 1, 255, 255, 255, screen);
        GUI_menu->DrawHLine(r2.x - 1, r2.y + r2.h, r2.w + 2, 255,
          255, 255, screen);
        GUI_menu->DrawVLine(r2.x - 1, r2.y - 1, r2.h + 1, 255, 255,
          255, screen);
        GUI_menu->DrawVLine(r2.x + r2.w, r2.y - 1, r2.h + 1, 255,
          255, 255, screen);
      }*/
    } else {
      if (MapLoaded != 2)
        DrawText("Press ESCAPE to open menu", screen, SmallFont, 200,
        screen->h - 1 * 13);
    }
    
    if (ConsoleDown)
      DrawConsole();
    else {
      if (MapLoaded != 2) {
        DrawText("Press ` to drop down the console", screen, SmallFont, 0,
          screen->h - 1 * 13);
      }
      DrawMessenger(screen);
      DrawChat(screen);
    }
    
    UpdateMouse();
    DrawMouse(screen);
    SDL_Flip(screen);
    if (want_quit) App_Quit();
  }
  inloop = false;
  
  SDL_Delay(1000);			// wait for byebye sound
  MapFreeMem();
  
  GUI_Done();
  
  DoneMouse();
  
  DoneConsole();
  
  DoneScripting();			// Done scripting engine
  
  SDL_FreeSurface(app.GetScreen());
  
#ifdef PW_SOUND
  smFreeSamples();
#endif
  
#ifdef PW_AUDIO
#ifdef PW_BASS
  BASS_Stop();				// Stop digital output
  BASS_Free();				// Close digital sound system
#else
  // Done music
  Mix_CloseAudio();
#endif
#endif
  
  // Done network
  DoneNet();
  
#ifdef WIN32
  SaveConfig(PW_CONFIG_FILE);
#else
  SaveConfig(PW_CONFIG_FILE_PERSONAL);
#endif
  
  Cvar_Free();
  
  FreeCommands();

  printf("PacWars2 (c) 2000-2001 Woid, Raist, Gameover, Dusty, Braindead\n");
  printf("\n");
  return 0;
}
