/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   Messenger
//## author:  Antonin Hildebrand
//## 
//## started: 23.6.2000
//## revised: ???
//## 
//## ingame message title implementation
//## 
//###########################################################################

#ifndef _MESSENGER_H_
#define _MESSENGER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL.h"
#include "const.h"
#include "cvar.h"

#define MAX_MESSENGER_TEXT 1024
#define STD_MESSENGER_X "0"
#define STD_MESSENGER_Y "0"
#define STD_MESSENGER_TIME "5000"

extern DECLSPEC cvar_t	messenger_x;
extern DECLSPEC cvar_t	messenger_y;
extern DECLSPEC cvar_t	messenger_time;
extern DECLSPEC int messenger_font;

void DrawMessenger(SDL_Surface * s);
void SetMessenger(const char *str, ... );

#endif