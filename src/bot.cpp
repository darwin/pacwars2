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

#include "bot.h"

// TODO: here should go game AI cvars ...
// cvar_t	g_results_x = {"g_results_x", "100", true};

//###########################################################################
//## API implementation
//###########################################################################

int bot_think(GPlayer* player, CGame* game, MoveVector* mv) 
{ 
  // see AI state machine stored in player_object 
  // look around in map 
  // look after other objects around me 
  // prepare move_vector 


  // game->map => game map object (see CMapState map.h)
  // game->objs[i] => i-th object of the game

  // in future here should be
  // game->events => event queue

  // typical iteration through objects:
  //
  // GAME_MAXOBJS_TYPE slot;
  // for (slot=0; slot<GAME_MAX_OBJS; slot++) 
  // {
  //   switch (game->objs[slot]->GetType()) {
  //   case ot_bomb:
  //     {
  //       GBomb* bomb = (GBomb*)game->objs[slot];
  //       do something with bomb ...
  //     }
  //     break;
  //   case ot_player:
  //     {
  //       GPlayer* enemy = (GPlayer*)game->objs[slot];
  //       do something with enemy ...
  //     }
  //     break;
  //   ...
  //
  //   }
  // }


  // stupid random move vector
  static int horizontal = 0;
  static int vertical = 0;
  
  memset(mv, 0, sizeof(MoveVector)); // clear vector

  if (rand()%20==0) horizontal = rand()%3-1; // random horizontal move
  if (rand()%20==0) vertical = rand()%3-1; // random vertical move
  if (rand()%30==0) mv->FIRE = 1; // random fire
  if (rand()%90==0) mv->WEAPON[rand()%5] = 1; // random weapon change

  // keep direction
  switch (horizontal) {
    case 1: mv->RIGHT = 1; break;
    case -1: mv->LEFT = 1; break;
  }
  switch (vertical) {
    case 1: mv->UP = 1; break;
    case -1: mv->DOWN = 1; break;
  }

  return 0; // ok
} 

