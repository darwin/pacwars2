#include <string.h>
#include "SDL.h"
#include "const.h"
#include "gamedefs.h"
#include "pac.h"
#include "client.h"

GShot::GShot(Uint16 id, CGame* igame, GAME_MAXOBJS_TYPE islot):GEntity(id, igame, islot),
// implicit shot initialization

dx           (GSHOT_ID+0 , 0             , 0),                  
owner        (GSHOT_ID+1 , 250           , 0)               
{
  bounced = false;
  SetSprite("pistolka", &sprite);
  xsize.Set(8);
  ysize.Set(8);
  anim.Set(4);
  frame.Set(0);
}

GShot::~GShot()
{
}

Uint8 GShot::GetType()
{
  return ot_shot;
}

void GShot::ServerThink(Uint32 time)
{
  GEntity::ServerThink(time);

  if (createtime && (curtime-createtime>shot_life)) { 
    state = 0; return; 
  }

  xpos+=dx;
  NormalizePos();

  rect_t r;
  GetBoundingBox(&r);
  // do test collisions, hurting players, random actions, and so on
  for (GAME_MAXOBJS_TYPE i=0; i<GAME_MAX_OBJS; i++) 
  {
    if ((bounced || i!=owner) && DetectCollision(i,&r))
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
        Bounce();
        CloneSound(SM_RSHOT, 0, 0);
        return;
      }
      if (b && b->tl1) // wall
      {
        state = 0;
        return;
      }
    }
}

void GShot::ClientThink(Uint32 time)
{
  GEntity::ClientThink(time);

  if (createtime && (curtime-createtime>shot_life)) { 
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
        GPlayer* p = (GPlayer*)game->objs[i];
        int pd = p->anim%2?-1:1;
        if (curtime<p->shield_end && pd!=SIGN(dx))  // has shield ?
        {
          Bounce();
        }
        state = state & ~OSTATE_VISIBLE;
        return;
      }
    }
  }
*/

  MapRectFromBoundingBox(&r);
  for (int my=r.top; my<=r.bottom; my++)
    for (int mx=r.left; mx<=r.right; mx++)
    {
      BLKSTR * b = game->map.GetBlock(mx, my);
      if (b && b->tr1) // bounce
      {
        Bounce();
        return;
      }
      if (b && b->tl1) // wall
      {
        state = state & ~OSTATE_VISIBLE;
        return;
      }
    }
}

void GShot::Replication(int cnum, CReplicator* rep)
{
  GEntity::Replication(cnum, rep);
  
  dx.Write(rep, cnum);
  owner.Write(rep, cnum);
}

char GShot::GetReplicated(Uint8 id, net_msg* msg, TICK_TYPE time)
{
  if (GEntity::GetReplicated(id, msg, time)) return 1;
  
  if (dx.Read(id, msg ,time)) return 1;
  if (owner.Read(id, msg ,time)) return 1;
  
  return 0;
}

void GShot::MakeDirty(int i, bool dirty)
{
  GEntity::MakeDirty(i, dirty);
  
  dx.MakeDirty(i, dirty);        
  owner.MakeDirty(i, dirty);
}

void GShot::Bounce()
{
  bounced = true;
  dx = -dx;
  xpos += dx;
}

void GShot::Animate()
{
  
}
