#include <string.h>
#include "SDL.h"
#include "const.h"
#include "gamedefs.h"
#include "pac.h"
#include "client.h"

GBShot::GBShot(Uint16 id, CGame* igame, GAME_MAXOBJS_TYPE islot):GEntity(id, igame, islot),
// implicit shot initialization

dx           (GBSHOT_ID+0 , 0             , 0),                  
owner        (GBSHOT_ID+1 , 250           , 0)               
{
  bounced = false;
  SetSprite("becher", &sprite);
  xsize.Set(32);
  ysize.Set(16);
  anim.Set(4);
  frame.Set(0);
}

GBShot::~GBShot()
{
}

Uint8 GBShot::GetType()
{
  return ot_bshot;
}

void GBShot::ServerThink(Uint32 time)
{
  GEntity::ServerThink(time);

  if (createtime && (curtime-createtime>bshot_life)) { 
    state = 0; return; 
  }

  xpos+=dx;
  NormalizePos();

  // do test collisions, hurting players, random actions, and so on
  rect_t r;
  GetBoundingBox(&r);
  for (GAME_MAXOBJS_TYPE i=0; i<GAME_MAX_OBJS; i++) 
  {
    if (DetectCollision(i,&r))
    {
      if (game->objs[i]->GetType()==ot_bomb)
      {
        ((GBomb*)game->objs[i])->Explode();
        state = 0;
        return;
      }
      if (game->objs[i]->GetType()==ot_grenade)
      {
        ((GGrenade*)game->objs[i])->Explode();
        state = 0;
        return;
      }
    }
  }

  MapRectFromBoundingBox(&r);
  for (int my=r.top; my<=r.bottom; my++)
    for (int mx=r.left; mx<=r.right; mx++)
    {
      BLKSTR * b = game->map.GetBlock(mx, my);
      if (b && b->tr1) // bounce
      {
        state = 0;
        
        // TODO sound
        return;
      }
      if (b && b->tl1) // wall
      {
        state = 0;
        return;
      }
    }
}

void GBShot::ClientThink(Uint32 time)
{
  GEntity::ClientThink(time);

  if (createtime && (curtime-createtime>bshot_life)) { 
    state = 0; return; 
  }

  xpos+=dx;
  NormalizePos();

  rect_t r;
  GetBoundingBox(&r);
/*  for (GAME_MAXOBJS_TYPE i=0; i<GAME_MAX_OBJS; i++) 
  {
    if ((bounced || i!=owner) && DetectCollision(i,&r))
    {
      if (game->objs[i]->GetType()==ot_player)
      {
        state = state & ~OSTATE_VISIBLE;
        return;
      }
    }
  }*/
  MapRectFromBoundingBox(&r);
  for (int my=r.top; my<=r.bottom; my++)
    for (int mx=r.left; mx<=r.right; mx++)
    {
      BLKSTR * b = game->map.GetBlock(mx, my);
      if (b && b->tr1) // bounce
      {
        state = state & ~OSTATE_VISIBLE;
        
        // TODO sound
        return;
      }
      if (b && b->tl1) // wall
      {
        state = state & ~OSTATE_VISIBLE;
        return;
      }
    }
}



void GBShot::Replication(int cnum, CReplicator* rep)
{
  GEntity::Replication(cnum, rep);
  
  dx.Write(rep, cnum);
  owner.Write(rep, cnum);
}

char GBShot::GetReplicated(Uint8 id, net_msg* msg, TICK_TYPE time)
{
  if (GEntity::GetReplicated(id, msg, time)) return 1;
  
  if (dx.Read(id, msg ,time)) return 1;
  if (owner.Read(id, msg ,time)) return 1;
  
  return 0;
}

void GBShot::MakeDirty(int i, bool dirty)
{
  GEntity::MakeDirty(i, dirty);
  
  dx.MakeDirty(i, dirty);        
  owner.MakeDirty(i, dirty);
}

void GBShot::Animate()
{
  
}
