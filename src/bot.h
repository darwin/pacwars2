/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   bots AI implementation
//## author:  
//##         
//## started: 28.5.2001
//## revised: ???
//## 
//## 
//###########################################################################

#ifndef _PACBOT_H_
#define _PACBOT_H_

#include <stdio.h>
#include <stdlib.h>
#include "const.h"
#include "game.h"
#include "console/SDL_DrawText.h"
#include "server.h"
#include "client.h"
#include "scripting.h"
#include "messenger.h"
#include "pw2.h"
#include "pac.h"

#include "aibase.h"

extern cvar_t	ai_level;

// NOTE: you can add the bot into the game by typing (in console) :
// 
// cb botname
// chase botname

// first command creates bot
// second sets camera to chase him


//###########################################################################
//## Constants & defines
//###########################################################################

// These define the bot types (less agresive to more agressive)
#define BOT_DUMMY			0	
#define BOT_GRUNT			1
#define BOT_SOLDIER		2
#define BOT_HUNTER		3
#define BOT_TERMINATOR	4

// This structure is the essense of the bot's personality traits.
// The numbers are between 0 and 1. The closer to 0, the less probable
// something is (hitting target, responding fast enought). The closer to
// 1, the more probable something is.
//
typedef struct
{
	float accuracy;
	float confidence;
	float reflex;
} s_botpersonality;


//###########################################################################
//## API
//###########################################################################

extern int bot_init(GPlayer* player, CGame* game);
extern int bot_command(GPlayer* player, CGame* game, char* cmd, char* params);
extern int bot_think(GPlayer* player, CGame* game, MoveVector* mv);
extern int bot_done(GPlayer* player, CGame* game);
extern int orig_bot_think(GPlayer* player, CGame* game, MoveVector* mv);
extern int bot_set_destination(GPlayer *player, CGame *game, Uint16 dst_x, Uint16 dst_y);

#endif
