#ifndef _PW2_H_
#define _PW2_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "protocol.h"

#include "cvar.h"
#include "gui.h"

extern DECLSPEC Uint32 firsttick;
extern DECLSPEC Uint32 ticktime;
extern DECLSPEC bool enable_menu_music;

extern DECLSPEC cvar_t	last_server;
extern DECLSPEC cvar_t	menu_music_file;
extern DECLSPEC cvar_t	music_volume;

extern DECLSPEC cvar_t	autolog;

extern DECLSPEC cvar_t	fullscreen;
extern DECLSPEC cvar_t	pw2_gamma_r;
extern DECLSPEC cvar_t	pw2_gamma_b;
extern DECLSPEC cvar_t	pw2_gamma_g;
extern DECLSPEC cvar_t	doublebuf;
extern DECLSPEC cvar_t	glblit;

extern DECLSPEC cvar_t	alphamenu;

extern SDL_Surface *credits;
extern SDL_Surface *help1;
extern SDL_Surface *help2;

extern char auto_playername[4][PWP_MAX_PLAYER_NAME];
extern Uint32 time_playername;
extern char auto_skinname[4][MAX_SPRITE_NAME];
extern char auto_skinplayername[4][PWP_MAX_PLAYER_NAME];
extern Uint32 time_skinname;
extern char auto_chasename[4][PWP_MAX_PLAYER_NAME];
extern Uint32 time_chasename;

extern cvar_t sound_volume;
extern bool inloop;

extern DECLSPEC int LoadConfig(char* cfgname);
extern DECLSPEC int SaveConfig(char* cfgname);
extern DECLSPEC void AddConsoleVars();
extern DECLSPEC void AddConsoleCommands();

extern DECLSPEC void Pause_Music(char*);
extern DECLSPEC void Play_Music(char*);
extern DECLSPEC void Volume_Music(char*);
extern DECLSPEC void Volume_Sound(char*);

//extern DECLSPEC void RenderResults(SDL_Surface *screen);

//extern DECLSPEC void GUI_Refresh();
extern DECLSPEC void ResetGamma();

void SetKeybindings(int kbindex, int set);

#endif
