#include <string.h>
#include "SDL.h"
#include "const.h"
#include "gamedefs.h"
#include "pac.h"
#include "client.h"

GBomb::GBomb(Uint16 id, CGame* igame, GAME_MAXOBJS_TYPE islot):GEntity(id, igame, islot),
// implicit shot initialization

owner        (GBOMB_ID+0 , 250           , 0) 
{
  SetSprite("bomb", &sprite);
  xsize.Set(32);
  ysize.Set(32);
  anim.Set(0);
  frame.Set(0);
  movementx=0;
  movementy=0;
  moved = false;
  explode_time;
}

GBomb::~GBomb()
{
}

Uint8 GBomb::GetType()
{
  return ot_bomb;
}

static GBomb* me;
static bool GBombObjColB(GObj* x)
{
  switch (x->GetType()) {
  case ot_bomb:
    return true;
  case ot_player:
    if (x->slot!=me->owner)
      return true;
    else
      return false;

  }
  return false;
}


void GBomb::ServerThink(Uint32 time)
{
  GEntity::ServerThink(time);

  rect_t r1;
  GetBoundingBox(&r1);

  me = this;
  if (movementx)
  {
    MoveBoundingBox(&r1, movementx, 0);
    if (!GlobalCollision(&r1, GBombObjColB))
    {
      xpos = xpos + movementx;
      NormalizePos();
      moved = true;
    }
    else
    {
      movementx=0;
      movementy=0;
      if (moved) CloneSound(SM_NARAZ, 0, 0);
      moved = false;
    }
  }
  
  if (movementy)
  {
    MoveBoundingBox(&r1, 0, movementy);
    if (!GlobalCollision(&r1, GBombObjColB))
    {
      ypos = ypos + movementy;
      NormalizePos();
      moved = true;
    }
    else
    {
      movementx=0;
      movementy=0;
      if (moved) CloneSound(SM_NARAZ, 0, 0);
      moved = false;
    }
  }

  // do test collisions, hurting players, random actions, and so on
  if (anim==1 && frame==sprite->anims[anim]->frames-1)
  {
    state = 0;
  }

  if (anim==0 && curtime>=explode_time)
    Explode();
}

void GBomb::ClientThink(Uint32 time)
{
  GEntity::ClientThink(time);
}


void GBomb::Explode()
{
  if (anim==1) return; // allready exploding

  SetAnim(1);
  
  GAME_MAXOBJS_TYPE fray_slot;
  
  game->IncrementLOID();
  fray_slot = game->FindUnusedSlot();
  game->RebornObject(game->last_oid, fray_slot, ot_fray);
  
  GFray * r1 = (GFray*)game->objs[fray_slot];
  
  r1->owner.Set(owner);
  r1->xpos.Set(xpos);
  r1->ypos.Set(ypos);
  r1->createtime.Set(curtime);
  r1->ExpandFrayX(power);
  r1->SetAnim(0);
  r1->xcenter.Set(xpos);
  r1->ycenter.Set(ypos);
  
  game->IncrementLOID();
  fray_slot = game->FindUnusedSlot();
  game->RebornObject(game->last_oid, fray_slot, ot_fray);
  
  GFray * r2 = (GFray*)game->objs[fray_slot];
  
  r2->owner.Set(owner);
  r2->xpos.Set(xpos);
  r2->ypos.Set(ypos);
  r2->createtime.Set(curtime);
  r2->ExpandFrayY(power);
  r2->SetAnim(1);
  r2->xcenter.Set(xpos);
  r2->ycenter.Set(ypos);
  
  CloneSound(SM_XPLO2, 0, 0);
  
  movementx = 0;
  movementy = 0;
  state = 0;
}

void GBomb::Replication(int cnum, CReplicator* rep)
{
  GEntity::Replication(cnum, rep);
  
  owner.Write(rep, cnum);
}

char GBomb::GetReplicated(Uint8 id, net_msg* msg, TICK_TYPE time)
{
  if (GEntity::GetReplicated(id, msg, time)) return 1;
  
  if (owner.Read(id, msg ,time)) return 1;
  
  return 0;
}

void GBomb::MakeDirty(int i, bool dirty)
{
  GEntity::MakeDirty(i, dirty);
  
  owner.MakeDirty(i, dirty);
}

