/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   Chat line
//## author:  Antonin Hildebrand
//## 
//## started: 3.3.2001
//## revised: ???
//## 
//## ingame chat line implementation
//## 
//###########################################################################

#ifndef _CHAT_H_
#define _CHAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL.h"
#include "const.h"
#include "cvar.h"

#define MAX_CHAT_TEXT 1024
#define MAX_CHAT_HISTORY 20
#define STD_CHAT_X "0"
#define STD_CHAT_Y "436"

extern DECLSPEC cvar_t	chat_x;
extern DECLSPEC cvar_t	chat_y;
extern DECLSPEC int chat_font;
extern DECLSPEC bool chat_on;

void DrawChat(SDL_Surface * s);
void SetChat(const char *str, ... );
void ChatEvents(SDL_Event * event);
void ResetChat();

#endif