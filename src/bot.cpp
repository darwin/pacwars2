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
cvar_t ai_level = {"ai_level", "1", true};   // example cvar (search string "ai_level" in all sources to add more cvars)

// special heuristics function for wrapped map
int HeuristicsWrap(PED* PE, MapNode* A, MapNode* B)
{
  const int dx = abs(A->Pos.X - B->Pos.X);
  const int dy = abs(A->Pos.Y - B->Pos.Y);
  
  return PE->BaseCost*MAX(MIN(dx, PE->MapSX - dx), MIN(dy, PE->MapSY - dy)); // Max
}

static int WrapNearestDistance(int x1, int x2, int wrapsize)
{
  while(x1<0) x1+=wrapsize;
  while(x1>=wrapsize) x1-=wrapsize;
  while(x2<0) x2+=wrapsize;
  while(x2>=wrapsize) x2-=wrapsize;

  if (x1<x2)
  {
    int pd = x2 - x1;
    int nd = x1 + (wrapsize - x2);

    if (pd<nd) return pd; else return -nd;
  }
  else
  {
    int pd = x2 + (wrapsize - x1);
    int nd = x1 - x2;

    if (pd<nd) return pd; else return -nd;
  }
}

static int Map2PE(CIRect* r, PED* pe, CMapState* pmap)
{
  // assume r is normalized
  for (int y=(int)r->A.y; y<(int)r->B.y; y++)
  { 
    for (int x=(int)r->A.x; x<(int)r->B.x; x++)
    {
      BLKSTR* b = pmap->GetBlock(x,y);
      MapNode* n = peMapXY(pe,x,y);
      if (b->tl1) // wall
      {  
        n->TC = pe->MAXTC;
        continue;
      }
      // test if pacman fits
      // pacman's size is 2x2 map tiles

      // test left x right
      BLKSTR* nl = pmap->GetBlock(x-1,y);
      BLKSTR* nr = pmap->GetBlock(x+1,y);
      if (nl->tl1 && nr->tl1) 
      {  
        n->TC = pe->MAXTC;
        continue;
      }
      // test top x bottom
      BLKSTR* nt = pmap->GetBlock(x,y-1);
      BLKSTR* nb = pmap->GetBlock(x,y+1);
      if (nt->tl1 && nb->tl1) 
      {  
        n->TC = pe->MAXTC;
        continue;
      }
      // test topleft x bottomright
      BLKSTR* ntl = pmap->GetBlock(x-1,y-1);
      BLKSTR* nbr = pmap->GetBlock(x+1,y+1);
      if (ntl->tl1 && nbr->tl1) 
      {  
        n->TC = pe->MAXTC;
        continue;
      }
      // test topright x bottomleft
      BLKSTR* ntr = pmap->GetBlock(x+1,y-1);
      BLKSTR* nbl = pmap->GetBlock(x-1,y+1);
      if (ntr->tl1 && nbl->tl1) 
      {  
        n->TC = pe->MAXTC;
        continue;
      }

      if (b->tr2)  // killing tile
      {
        n->TC = pe->MAXTC-100;  // give high terrain cost (but not impossible)
        continue;
      }

      if (nt->tr2 || nb->tr2 || nl->tr2 || nr->tr2 ||
          ntr->tr2 || ntl->tr2 || nbr->tr2 || nbr->tr2)
      {
        n->TC = pe->MAXTC-150;  // keep away from killing tiles
        continue;
      }

      if (nt->tl1 || nb->tl1 || nl->tl1 || nr->tl1 ||
          ntr->tl1 || ntl->tl1 || nbr->tl1 || nbr->tl1)
      {
        n->TC = 50+nb->tl1+nl->tl1+nr->tl1+ntr->tl1+ntl->tl1+nbr->tl1+nbr->tl1; // keep away from walls
        continue;
      }

      // TODO: more sophisticated estimating (bot obtain corners, or this situation:
      //
      // W
      // W ...
      // W.
      // .WWWW
      //.


      n->TC = 0; // else the way is clear
    }
  }

  return 0;
}


//###########################################################################
//## API implementation
//###########################################################################

int bot_init(GPlayer* player, CGame* game)
{
  // init path engine
  player->pe = peInit(mapwidth, mapheight, 0, 10000, 0, 0);
  if (!player->pe) ConErr("Couldn't initialize path engine.");
  player->pe->Heuristics = HeuristicsWrap;                       // set map wrapping heuristics

  player->steps = NULL;
  player->cur_step = 0;

  return 0;
}

int bot_done(GPlayer* player, CGame* game)
{
  // done path engine
  if (player->pe) 
  {
    peDone(player->pe);
    player->pe = NULL;
  }

  return 0;
}

int bot_command(GPlayer* player, CGame* game, char* cmd, char* params)
{
  if (strcmp(cmd, "go")==0)
  {
    int dst_x, dst_y;
    if (sscanf(params, "%d %d", &dst_x, &dst_y) < 2) return 2;

    Uint16 cur_x = player->xpos;
    Uint16 cur_y = player->ypos;

    // per pixel position -> map block coordinates
    cur_x>>=4;
    cur_y>>=4;

    MapNode* start = peMapXY(player->pe, cur_x, cur_y);
    MapNode* dest = peMapXY(player->pe, dst_x, dst_y);

	CIRect rect(CIPoint(0,0), CIPoint(mapwidth, mapheight));
    Map2PE(&rect, player->pe, &game->map);
    int length = peFindPath(player->pe, start, dest, 0, 1, &player->steps);
    if (length) 
      player->cur_step = 0; 
    else 
      ConOut("bot \"%s\": I don't know how to get there", player->player_name.GetValRef()->chars);

  
    return 0;
  }

  return 1;
}

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

  // we have two modes of bot behaviour
  if (!player->steps)
  {
    // mode: random moving
    if (rand()%20==0) horizontal = rand()%3-1; // random horizontal move
    if (rand()%20==0) vertical = rand()%3-1; // random vertical move

  }
  else 
  {
    // mode: bot has planed way (via console command "bc <botname> go <x> <y>")
    Uint16 cur_x = player->xpos;
    Uint16 cur_y = player->ypos;

    int bdx = WrapNearestDistance(cur_x, (player->steps[player->cur_step].Pos.X*16)+8+8*player->last_horizontal,mappxwidth);
    int bdy = WrapNearestDistance(cur_y, (player->steps[player->cur_step].Pos.Y*16)+8+8*player->last_vertical, mappxheight);

    // test if bot has reached the waypoint
    if (ABS(bdx)<=2*player->last_speed && ABS(bdy)<=2*player->last_speed)
    {
      if (player->steps[player->cur_step].Type&PST_END) 
      {
        peRemovePath(player->pe, player->steps);
        player->steps = NULL;
        player->cur_step = 0;
        ConOut("bot \"%s\": path done", player->player_name.GetValRef()->chars, player->cur_step);
      }
      else
      {
        ConOut("bot \"%s\": step %d reached", player->player_name.GetValRef()->chars, player->cur_step);
        player->cur_step+=1;
      }
    }

    // extract the direction to move
    horizontal = SIGN(bdx);
    vertical = SIGN(bdy);

    if (horizontal) player->last_horizontal = horizontal;
    if (vertical) player->last_vertical = vertical;

    // test if there is a wall in the moving direction, if yes disable that direction 
    // (against blocking player in narrow tunnels)
    rect_t mr;
    player->GetBoundingBox(&mr);

    int mx,my;

    // horizontal test
    rect_t mbx = mr;
    player->MoveBoundingBox(&mbx, horizontal, 0);
    player->MapRectFromBoundingBox(&mbx);
    for (my=mbx.top; horizontal && my<=mbx.bottom; my++)
    {
      for (mx=mbx.left; mx<=mbx.right; mx++)
      {
        BLKSTR * b = game->map.GetBlock(mx, my);
        if (b)
        {
          if (b->tl1) // wall
          {
            horizontal = 0;
            break;
          }
        }
      }
    }

    // vertical test
    rect_t mby = mr;
    player->MoveBoundingBox(&mby, 0, vertical);
    player->MapRectFromBoundingBox(&mby);
    for (my=mby.top; vertical && my<=mby.bottom; my++)
    {
      for (mx=mby.left; mx<=mby.right; mx++)
      {
        BLKSTR * b = game->map.GetBlock(mx, my);
        if (b)
        {
          if (b->tl1) // wall
          {
            vertical = 0;
            break;
          }
        }
      }
    }

    // do not oscilate about waypoint
    if (horizontal && abs(bdx)<=player->last_speed/2) horizontal = 0;
    if (vertical && abs(bdy)<=player->last_speed/2) vertical = 0;
  }

//  if (rand()%30==0) mv->FIRE = 1; // random fire
  if (rand()%90==0) mv->WEAPON[rand()%5] = 1; // random weapon change

  // keep direction
  switch (horizontal) {
  case -1: mv->LEFT = 1; break;
  case 1: mv->RIGHT = 1; break;
  }
  switch (vertical) {
  case -1: mv->UP = 1; break;
  case 1: mv->DOWN = 1; break;
  }
  
  // example cvar code
  if (ai_level.value!=1)
  {
    // just notify by sound
#ifdef WIN32
    _beep(400, 400);
#endif
    Cvar_SetValue("ai_level", 1);
  }

  return 0; // ok
}
