#include <string.h>
#include "SDL.h"
#include "const.h"
#include "gamedefs.h"
#include "pac.h"
#include "client.h"

GFray::GFray(Uint16 id, CGame* igame, GAME_MAXOBJS_TYPE islot):GEntity(id, igame, islot),
// implicit rail initialization

roundings (GFRAY_ID+1 , 0             , 0),
xcenter   (GFRAY_ID+2 , 0             , 0),
ycenter   (GFRAY_ID+3 , 0             , 0),
owner     (GFRAY_ID+4 , 250           , 0)               

{
  SetSprite("fray", &sprite);
  xsize = fray_gr_sx;
  ysize = fray_gr_sy;
  timestamp = 0;
}

GFray::~GFray()
{

}

Uint8 GFray::GetType()
{
  return ot_fray;
}

/*
void GFray::Draw(SDL_Surface * screen, SDL_Rect * screen_rect)
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
      sr.w = fray_gr_sx;
      sr.h = fray_gr_sy;
      sr.x = fray_gr_sx * frame;
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
          dr.x+=fray_gr_sx;
        }
        dr.y+=fray_gr_sy;
      }

      dr.w = sr.w;
      dr.h = sr.h;
      // draw roundings
      Uint8 flags = roundings;

      if (flags&1)
      {
        if (anim==0)
        {
          dr.x = screen_rect->x + bb.right;
          dr.y = screen_rect->y + bb.top;
          BlitSurface(sprite->anims[3]->gfx, sr, screen, dr, screen_rect);
        }
        else
        {
          dr.x = screen_rect->x + bb.left;
          dr.y = screen_rect->y + bb.bottom;
          BlitSurface(sprite->anims[4]->gfx, sr, screen, dr, screen_rect);
        }
      }

      if (flags&2)
      {
        if (anim==0)
        {
          dr.x = screen_rect->x + bb.left - fray_gr_sx;
          dr.y = screen_rect->y + bb.top;
          BlitSurface(sprite->anims[5]->gfx, sr, screen, dr, screen_rect);
        }
        else
        {
          dr.x = screen_rect->x + bb.left;
          dr.y = screen_rect->y + bb.top  - fray_gr_sy;
          BlitSurface(sprite->anims[2]->gfx, sr, screen, dr, screen_rect);
        }
      }
    }
  }
}
*/

void GFray::Draw(SDL_Surface * screen, SDL_Rect * screen_rect)
{
  if (!(state&OSTATE_VISIBLE)) return;
    
  rect_t bb;
  GetBoundingBox(&bb);

  SDL_Rect dr;
  dr.y = bb.top;
  while (dr.y<=bb.bottom-fray_gr_sy)
  {
    dr.x = bb.left;
    while (dr.x<=bb.right-fray_gr_sx)
    {
      Sint16 x = dr.x + fray_gr_sx/2;
      Sint16 y = dr.y + fray_gr_sy/2;
      NormalizePos(&x, &y);
      GEntity::Draw(screen, screen_rect, x, y, fray_gr_sx, fray_gr_sy, frame, anim);
      dr.x+=fray_gr_sx;
    }
    if (dr.x<bb.right)
    {
      Sint16 x = dr.x + (bb.right-dr.x) / 2 ;
      Sint16 y = dr.y + fray_gr_sy/2;
      NormalizePos(&x, &y);
      GEntity::Draw(screen, screen_rect, x, y, bb.right-dr.x, fray_gr_sy, frame, anim);
    }
    dr.y+=fray_gr_sy;
  }
  if (dr.y<bb.bottom)
  {
    dr.x = bb.left;
    while (dr.x<=bb.right-fray_gr_sx)
    {
      Sint16 x = dr.x + fray_gr_sx/2;
      Sint16 y = dr.y + (bb.bottom-dr.y) / 2 ;
      NormalizePos(&x, &y);
      GEntity::Draw(screen, screen_rect, x, y, fray_gr_sx, bb.bottom-dr.y, frame, anim);
      dr.x+=fray_gr_sx;
    }
    if (dr.x<bb.right)
    {
      Sint16 x = dr.x + (bb.right-dr.x) / 2 ;
      Sint16 y = dr.y + (bb.bottom-dr.y) / 2 ;
      NormalizePos(&x, &y);
      GEntity::Draw(screen, screen_rect, x, y, bb.right-dr.x, bb.bottom-dr.y, frame, anim);
      dr.x+=fray_gr_sx;
    }
  }
  GEntity::Draw(screen, screen_rect, xcenter, ycenter, fray_gr_sx, fray_gr_sy, frame, 6);

  // draw roundings
  Uint8 flags = roundings;
  
  if (flags&1)
  {
    if (anim==0)
    {
      Sint16 x = bb.right + fray_gr_sx/2;
      Sint16 y = ypos;
      NormalizePos(&x, &y);
      GEntity::Draw(screen, screen_rect, x, y, fray_gr_sx, fray_gr_sy, frame, 3);
    }
    else
    {
      Sint16 x = xpos;
      Sint16 y = bb.bottom + fray_gr_sy/2;
      NormalizePos(&x, &y);
      GEntity::Draw(screen, screen_rect, x, y, fray_gr_sx, fray_gr_sy, frame, 4);
    }
  }
  
  if (flags&2)
  {
    if (anim==0)
    {
      Sint16 x = bb.left - fray_gr_sx/2;
      Sint16 y = ypos;
      NormalizePos(&x, &y);
      GEntity::Draw(screen, screen_rect, x, y, fray_gr_sx, fray_gr_sy, frame, 5);
    }
    else
    {
      Sint16 x = xpos;
      Sint16 y = bb.top - fray_gr_sy/2;
      NormalizePos(&x, &y);
      GEntity::Draw(screen, screen_rect, x, y, fray_gr_sx, fray_gr_sy, frame, 2);
    }
  }

}

void GFray::ServerThink(Uint32 time)
{
  GEntity::ServerThink(time);

  if (frame==sprite->anims[anim]->frames-1) 
  { 
    state = 0; 
    return; 
  }

  // do test collisions, hurting players, random actions, and so on
  if (state&OSTATE_VISIBLE)
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

void GFray::ClientThink(Uint32 time)
{
  GEntity::ClientThink(time);
  if (frame==sprite->anims[anim]->frames-1) 
  { 
//    state = state & ~OSTATE_VISIBLE; 
    return; 
  }
}


void GFray::Replication(int cnum, CReplicator* rep)
{
  GEntity::Replication(cnum, rep);
  
  roundings.Write(rep, cnum);
  owner.Write(rep, cnum);
  xcenter.Write(rep, cnum);
  ycenter.Write(rep, cnum);
}

char GFray::GetReplicated(Uint8 id, net_msg* msg, TICK_TYPE time)
{
  if (GEntity::GetReplicated(id, msg, time)) return 1;
  
  if (roundings.Read(id, msg ,time)) return 1;
  if (owner.Read(id, msg ,time)) return 1;
  if (xcenter.Read(id, msg ,time)) return 1;
  if (ycenter.Read(id, msg ,time)) return 1;
  
  return 0;
}

void GFray::MakeDirty(int i, bool dirty)
{
  GEntity::MakeDirty(i, dirty);
  
  roundings.MakeDirty(i, dirty);
  owner.MakeDirty(i, dirty);
  xcenter.MakeDirty(i, dirty);
  ycenter.MakeDirty(i, dirty);
}

bool GFrayObjCol(GObj* obj)
{
  return false;
}

void GFray::ExpandFrayY(int length)
{
  rect_t box1;
  GetBoundingBox(&box1);
  rect_t box2 = box1;

  int counter;

  Uint8 flags = 0;
  counter = 0;
  while (!GlobalCollision(&box1, &GFrayObjCol)) 
  {
    if (counter++>length) 
    {
      flags|=1;
      goto ENDLOOP1;
    }
    MoveBoundingBox(&box1, 0, 8);
  }
  // collision - do per pixel accuracy
  MoveBoundingBox(&box1, 0, -8);
  while (!GlobalCollision(&box1, &GFrayObjCol)) 
  {
    MoveBoundingBox(&box1, 0, 1);
  }

ENDLOOP1:

  counter = 0;
  while (!GlobalCollision(&box2, &GFrayObjCol)) 
  {
    if (counter++>length) 
    {
      flags|=2;
      goto ENDLOOP2;
    }
    MoveBoundingBox(&box2, 0, -8);
  }
  // collision - do per pixel accuracy
  MoveBoundingBox(&box2, 0, 8);
  while (!GlobalCollision(&box2, &GFrayObjCol)) 
  {
    MoveBoundingBox(&box2, 0, -1);
  }

ENDLOOP2:

  roundings = flags;

  box1.bottom = MAX(box2.bottom, box1.bottom);
  box1.left = MIN(box2.left, box1.left);
  box1.right = MAX(box2.right, box1.right);
  box1.top = MIN(box2.top, box1.top);

  xsize = box1.right - box1.left;
  ysize = box1.bottom - box1.top; 

  xpos = box1.left + xsize / 2;
  ypos = box1.top + ysize / 2;

  NormalizePos();
}

void GFray::ExpandFrayX(int length)
{
  rect_t box1;
  GetBoundingBox(&box1);
  rect_t box2 = box1;

  int counter;
  Uint8 flags = 0;

  roundings = 0;
  counter = 0;
  while (!GlobalCollision(&box1, &GFrayObjCol)) 
  {
    if (counter++>length) 
    {  
      flags|=1;
      goto ENDLOOP1;
    }
    MoveBoundingBox(&box1, 8, 0);
  }
  // collision - do per pixel accuracy
  MoveBoundingBox(&box1, -8, 0);
  while (!GlobalCollision(&box1, &GFrayObjCol)) 
  {
    MoveBoundingBox(&box1, 1, 0);
  }

ENDLOOP1:

  counter = 0;
  while (!GlobalCollision(&box2, &GFrayObjCol)) 
  {
    if (counter++>length) 
    {
      flags|=2;
      goto ENDLOOP2;
    }
    MoveBoundingBox(&box2, -8, 0);
  }
  // collision - do per pixel accuracy
  MoveBoundingBox(&box2, 8, 0);
  while (!GlobalCollision(&box2, &GFrayObjCol)) 
  {
    MoveBoundingBox(&box2, -1, 0);
  }
  
ENDLOOP2:

  roundings = flags;
  box1.bottom = MAX(box2.bottom, box1.bottom);
  box1.left = MIN(box2.left, box1.left);
  box1.right = MAX(box2.right, box1.right);
  box1.top = MIN(box2.top, box1.top);

  xsize = box1.right - box1.left;
  ysize = box1.bottom - box1.top; 

  xpos = box1.left + xsize / 2;
  ypos = box1.top + ysize / 2;

  NormalizePos();
}

