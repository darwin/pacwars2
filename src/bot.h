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

// NOTE: you can add the bot into the game by typing (in console) :
// 
// cb botname
// chase botname

// first command creates bot
// second sets camera to chase him


//###########################################################################
//## Constants & defines
//###########################################################################


//###########################################################################
//## API
//###########################################################################

int bot_think(GPlayer* player, CGame* game, MoveVector* mv);


#endif