#include <string.h>
#include "SDL.h"
#include "const.h"
#include "gamedefs.h"
#include "pac.h"
#include "client.h"

GGrenade::GGrenade(Uint16 id, CGame* igame, GAME_MAXOBJS_TYPE islot):GEntity(id, igame, islot),
// implicit shot initialization

owner        (GGRENADE_ID+0 , 250           , 0) 
{
  SetSprite("grenade", &sprite);
  xsize.Set(32);
  ysize.Set(32);
  SetAnim(2);
  movementx=0;
  movementy=0;
}

GGrenade::~GGrenade()
{
}

Uint8 GGrenade::GetType()
{
  return ot_grenade;
}

void GGrenade::ServerThink(Uint32 time)
{
  GEntity::ServerThink(time);

  rect_t r1;
  GetBoundingBox(&r1);

  if (movementx)
  {
    MoveBoundingBox(&r1, movementx, 0);
    if (!GlobalCollision(&r1))
    {
      xpos = xpos + movementx;
      NormalizePos();
    }
    else
      Explode();
  }
  
  if (movementy)
  {
    MoveBoundingBox(&r1, 0, movementy);
    if (!GlobalCollision(&r1))
    {
      ypos = ypos + movementy;
      NormalizePos();
    }
    else
      Explode();
  }

  // do test collisions, hurting players, random actions, and so on
  if (anim==3)
  {

    rect_t r;
    GetBoundingBox(&r);
    for (GAME_MAXOBJS_TYPE i=0; i<GAME_MAX_OBJS; i++) 
    {
      if (DetectCollision(i,&r))
      {
        if (game->objs[i]->GetType()==ot_bomb)
        {
          ((GBomb*)game->objs[i])->Explode();
        }
        if (game->objs[i]->GetType()==ot_grenade)
        {
          ((GGrenade*)game->objs[i])->Explode();
        }
      }
    }

    if (frame==sprite->anims[anim]->frames-1)
    {
      state = 0;
    }
  }
    
  if (anim==2 && createtime && (curtime-createtime>grenade_timeout)) 
    Explode();
}

void GGrenade::ClientThink(Uint32 time)
{
  GEntity::ClientThink(time);
}

void GGrenade::Explode()
{
  if (anim==3) return; // allready exploding

  SetAnim(3);

  xsize = 128;
  ysize = 128;
  
  CloneSound(SM_XPLO1, 0, 0);
  
  movementx = 0;
  movementy = 0;
}

void GGrenade::Replication(int cnum, CReplicator* rep)
{
  GEntity::Replication(cnum, rep);
  
  owner.Write(rep, cnum);
}

char GGrenade::GetReplicated(Uint8 id, net_msg* msg, TICK_TYPE time)
{
  if (GEntity::GetReplicated(id, msg, time)) return 1;
  
  if (owner.Read(id, msg ,time)) return 1;
  
  return 0;
}

void GGrenade::MakeDirty(int i, bool dirty)
{
  GEntity::MakeDirty(i, dirty);
  
  owner.MakeDirty(i, dirty);
}

