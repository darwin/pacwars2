#include <string.h>
#include "SDL.h"
#include "const.h"
#include "gamedefs.h"
#include "pac.h"
#include "client.h"

GRail::GRail(Uint16 id, CGame* igame, GAME_MAXOBJS_TYPE islot):GEntity(id, igame, islot),
// implicit rail initialization

owner     (GRAIL_ID+0 , 250           , 0)               

{
  SetSprite("railgun", &sprite);
  xsize = rail_gr_sx;
  ysize = rail_gr_sy;
  SetAnim(4);
  SetFrame(0);
  timestamp = 0;
}

GRail::~GRail()
{

}

Uint8 GRail::GetType()
{
  return ot_rail;
}

/*
void GRail::Draw(SDL_Surface * screen, SDL_Rect * screen_rect)
{
  if (!(state&OSTATE_VISIBLE)) return;
    
  SDL_Rect dr;
  SDL_Rect sr;
  SDL_Rect cr;
  
  if (anim<sprite->num_anims && 
    frame<sprite->anims[anim]->frames)
  {
    Uint16 rx = xpos - game->vars.camx;
    Uint16 ry = ypos - game->vars.camy;

    rect_t bb;
    GetBoundingBox(&bb);
    MoveBoundingBox(&bb, -game->vars.camx, -game->vars.camy);
  
    // is sprite visible ?
    if ((bb.left<screen_rect->w && bb.right>0) && (bb.top<screen_rect->h && bb.bottom>0))
    {
      // locate rectangle of source sprite
      sr.w = rail_gr_sx;
      sr.h = rail_gr_sy;
      sr.x = rail_gr_sx * frame;
      sr.y = 0;
      
      dr.x = screen_rect->x + bb.left;
      dr.y = screen_rect->y + bb.top;
      dr.w = sr.w;
      dr.h = sr.h;

      cr.x = MIN(MAX(screen_rect->x + bb.left, screen_rect->x), screen_rect->x + bb.right );
      cr.y = MIN(MAX(screen_rect->y + bb.top , screen_rect->y), screen_rect->y + bb.bottom);
      cr.w = MIN(bb.right - bb.left, screen_rect->w - (cr.x - screen_rect->x)) ;
      cr.h = MIN(bb.bottom - bb.top, screen_rect->h - (cr.y - screen_rect->y)) ;

      while (dr.y<bb.bottom)
      {
        dr.x = screen_rect->x + bb.left;
        while (dr.x<bb.right)
        {
          BlitSurface(sprite->anims[anim]->gfx, sr, screen, dr, &cr);
          dr.x+=rail_gr_sx;
        }
        dr.y+=rail_gr_sy;
      }
    }
  }
}
*/

void GRail::Draw(SDL_Surface * screen, SDL_Rect * screen_rect)
{
  if (!(state&OSTATE_VISIBLE)) return;
    
  rect_t bb;
  GetBoundingBox(&bb);

  SDL_Rect dr;
  dr.y = bb.top;
  while (dr.y<bb.bottom)
  {
    dr.x = bb.left;
    while (dr.x<=bb.right-rail_gr_sx)
    {
      Sint16 x = dr.x + rail_gr_sx/2;
      Sint16 y = dr.y + rail_gr_sy/2;
      NormalizePos(&x, &y);
      GEntity::Draw(screen, screen_rect, x, y, rail_gr_sx, rail_gr_sy, frame, anim);
      dr.x+=rail_gr_sx;
    }
    if (dr.x<bb.right)
    {
      Sint16 x = dr.x + (bb.right-dr.x) / 2 ;
      Sint16 y = dr.y + rail_gr_sy/2;
      NormalizePos(&x, &y);
      GEntity::Draw(screen, screen_rect, x, y, bb.right-dr.x, rail_gr_sy, frame, anim);
      dr.x+=rail_gr_sx;
    }
    dr.y+=rail_gr_sy;
  }
   
}

void GRail::ServerThink(Uint32 time)
{
  GRail::ClientThink(time);

  if (createtime && (curtime-createtime>rail_life)) 
  { 
    state = 0; 
    return; 
  }

  // do test collisions, hurting players, random actions, and so on
  if (state&OSTATE_VISIBLE && (time-createtime<4))
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
  }
}

void GRail::ClientThink(Uint32 time)
{
  GEntity::ClientThink(time);

  if (createtime && (curtime-createtime>rail_life)) 
  { 
    state = state & ~OSTATE_VISIBLE; 
    return; 
  }
}

void GRail::Replication(int cnum, CReplicator* rep)
{
  GEntity::Replication(cnum, rep);
  
  owner.Write(rep, cnum);
}

char GRail::GetReplicated(Uint8 id, net_msg* msg, TICK_TYPE time)
{
  if (GEntity::GetReplicated(id, msg, time)) return 1;
  
  if (owner.Read(id, msg ,time)) return 1;
  
  return 0;
}

void GRail::MakeDirty(int i, bool dirty)
{
  GEntity::MakeDirty(i, dirty);
  
  owner.MakeDirty(i, dirty);
}

void GRail::ExpandRail(Uint8 direction)
{
  rect_t box1;
  GetBoundingBox(&box1);
  rect_t box2 = box1;

  int dx;
  int dy;
       
  switch (direction) {
  case 1:
    dx=8;
    dy=0;
    break;
  case 3:
    dx=-8;
    dy=0;
    break;
  }

  int counter = 0;
  while (!GlobalCollision(&box1)) 
  {
    if (counter++>max_rail) goto ENDLOOP;
    MoveBoundingBox(&box1, dx, dy);
  }
  if (counter==0) goto ENDLOOP;

  // collision - do per pixel accuracy
  MoveBoundingBox(&box1, -dx, -dy);
  while (!GlobalCollision(&box1)) 
  {
    MoveBoundingBox(&box1, SIGN(dx), SIGN(dy));
  }

ENDLOOP:
  box1.bottom = MAX(box2.bottom, box1.bottom);
  box1.left = MIN(box2.left, box1.left);
  box1.right = MAX(box2.right, box1.right);
  box1.top = MIN(box2.top, box1.top);

  xsize = box1.right - box1.left;// - rail_gr_sx/2;
  ysize = box1.bottom - box1.top; 

  xpos = box1.left + xsize / 2;
  ypos = box1.top + ysize / 2;

  NormalizePos();
}

