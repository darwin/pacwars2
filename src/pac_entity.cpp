#include <string.h>
#include "SDL.h"
#include "gamedefs.h"
#include "pac.h"
#include "sampleman.h"

GEntity::GEntity(Uint16 id, CGame* igame, GAME_MAXOBJS_TYPE islot):GObj(id, igame, islot),
// implicit entity initialization

xpos        (GENTITY_ID+0 , 0         , 0),   
ypos        (GENTITY_ID+1 , 0         , 0),   
frame       (GENTITY_ID+6 , 0         , 0),   
anim        (GENTITY_ID+7 , 0         , 0),   
framedelay  (GENTITY_ID+9, 1          , 0),   
xsize       (GENTITY_ID+10, 32        , 0),   
ysize       (GENTITY_ID+11, 32        , 0)   
{
  sprite = NULL;
  SetSprite(sprite_imp.string, &sprite);
}

GEntity::~GEntity()
{
  if (sprite) SpriteMan.Unregister(sprite);
}

Uint8 GEntity::GetType()
{
  return ot_entity;
}

void GEntity::ServerThink(Uint32 time)
{
  Animate();
  curtime = time;
  
  // do test collissions, hurting players, random actions, and so on
}

void GEntity::ClientThink(Uint32 time)
{
  Animate();
  curtime = time;
}

void GEntity::BlitSurface(SDL_Surface * ssurface, SDL_Rect sr, SDL_Surface *dsurface, SDL_Rect dr, SDL_Rect *r, Uint8 alpha)
{
  // clipping
  if (dr.x<r->x) 
  {
    int dx = r->x - dr.x;
    if (dr.w<=dx) return;
    dr.w -= dx;
    sr.x += dx;
    sr.w = dr.w;
    dr.x = 0;
  }
  
  if (dr.y<r->y) 
  {
    int dy = r->y - dr.y;
    if (dr.h<=dy) return;
    dr.h -= dy;
    sr.y += dy;
    sr.h = dr.h;
    dr.y = 0;
  }
  
  if (dr.x+dr.w>r->x+r->w) 
  {
    int dx = dr.x+dr.w-(r->x+r->w);
    if (dr.w<=dx) return;
    dr.w -= dx;
    sr.w = dr.w;
  }
  
  if (dr.y+dr.h>r->y+r->h) 
  {
    int dy = dr.y+dr.h-(r->y+r->h); 
    if (dr.h<=dy) return;
    dr.h -= dy;
    sr.h = dr.h;
  }
  
  // colorkey blit to the dest surface, SDL should apply clipping
#ifdef BOUNDING_BOXES
  SDL_FillRect(dsurface, &dr, SDL_MapRGB(dsurface->format, 0, 0, 255)); //rand()));
#endif
  SDL_SetAlpha(ssurface, SDL_SRCALPHA|SDL_RLEACCEL, alpha);
  SDL_BlitSurface(ssurface, &sr, dsurface, &dr);
}

void GEntity::Draw(SDL_Surface * screen, SDL_Rect * screen_rect)
{
  if (!(state&OSTATE_VISIBLE)) return;
    
  SDL_Rect sr;
  SDL_Rect dr;
  
  Uint16 rx = xpos - game->vars.camx;
  Uint16 ry = ypos - game->vars.camy;
  
  if (anim<sprite->num_anims && 
    frame<sprite->anims[anim]->frames)
  {
    Uint16 dx = minimum(sprite->anims[anim]->w, xsize) / 2;
    Uint16 dy = minimum(sprite->anims[anim]->h, ysize) / 2;
    
    // is sprite visible ?
    if ((rx+dx>=0) && (ry+dy>=0) && (rx-dx<screen_rect->w) && (ry-dy<screen_rect->h))
    {
      // locate rectangle of source sprite
      sr.w = minimum(sprite->anims[anim]->w, xsize);
      sr.h = minimum(sprite->anims[anim]->h, ysize);
      sr.x = sr.w * frame;
      sr.y = 0;
      
      dr.x = screen_rect->x + rx - dx;
      dr.y = screen_rect->y + ry - dy;
      dr.w = sr.w;
      dr.h = sr.h;

      BlitSurface(sprite->anims[anim]->gfx, sr, screen, dr, screen_rect);
    }
  }
}

void GEntity::Draw(SDL_Surface * screen, SDL_Rect * screen_rect, Uint16 x, Uint16 y, Uint16 sx, Uint16 sy, Uint8 frame, Uint8 anim, CSpriteInfo* dsprite, Uint8 alpha)
{
  if (!(state&OSTATE_VISIBLE)) return;
  if (!dsprite) dsprite = sprite;
    
  SDL_Rect sr;
  SDL_Rect dr;
  
  Uint16 rx = x - game->vars.camx;
  Uint16 ry = y - game->vars.camy;
  
  if (anim<dsprite->num_anims && 
    frame<dsprite->anims[anim]->frames)
  {
    Uint16 dx = sx / 2;
    Uint16 dy = sy / 2;
    
    // is sprite visible ?
    if ((rx+dx>=0) && (ry+dy>=0) && (rx-dx<screen_rect->w) && (ry-dy<screen_rect->h))
    {
      // locate rectangle of source sprite
      sr.w = sx;
      sr.h = sy;
      sr.x = sr.w * frame;
      sr.y = 0;
      
      dr.x = screen_rect->x + rx - dx;
      dr.y = screen_rect->y + ry - dy;
      dr.w = sr.w;
      dr.h = sr.h;

      BlitSurface(dsprite->anims[anim]->gfx, sr, screen, dr, screen_rect, alpha);
    }
  }
}


void GEntity::Replication(int cnum, CReplicator* rep, bool replicate_position)
{
  GObj::Replication(cnum, rep);

  if (replicate_position) 
  {
    xpos.Write(rep, cnum);
    ypos.Write(rep, cnum);
  }
  frame.Write(rep, cnum);
  anim.Write(rep, cnum);
  framedelay.Write(rep, cnum);
  xsize.Write(rep, cnum);
  ysize.Write(rep, cnum);
}

char GEntity::GetReplicated(Uint8 id, net_msg* msg, TICK_TYPE time, bool replicate_position)
{
  if (GObj::GetReplicated(id, msg, time)) return 1;
  
  if (replicate_position) 
  {
    if (xpos.Read(id, msg ,time)) return 1;
    if (ypos.Read(id, msg ,time)) return 1;
  }

  if (frame.Read(id, msg ,time))
  {
    SetFrame(frame);
    return 1;
  }
  if (anim.Read(id, msg ,time))
  {
    SetAnim(anim);
    return 1;
  }
  if (framedelay.Read(id, msg ,time)) return 1;
  if (xsize.Read(id, msg ,time)) return 1;
  if (ysize.Read(id, msg ,time)) return 1;
  return 0;
}

void GEntity::MakeDirty(int i, bool dirty)
{
  GObj::MakeDirty(i, dirty);
  
	xpos.MakeDirty(i, dirty);
	ypos.MakeDirty(i, dirty);

  xsize.MakeDirty(i, dirty);
	ysize.MakeDirty(i, dirty);

  framedelay.MakeDirty(i, dirty);
  frame.MakeDirty(i, dirty);
  anim.MakeDirty(i, dirty);
}

void GEntity::Animate()
{
  // klasicke animate nebude posilano pres sit - zbytecne
  if (framedelay) 
  {
    framedelay--;
    framedelay.MakeDirty(-1, false);
  }
  else
  {
    frame++;
    if (frame>=sprite->anims[anim]->frames) frame = 0;
    frame.MakeDirty(-1, false);
    
    framedelay = sprite->anims[anim]->delays[frame];
    framedelay.MakeDirty(-1, false);
  }
}

bool GEntity::SetFrame(Uint8 nf)
{
  if (nf>=sprite->anims[anim]->frames) return false;
  
  frame = nf;
  framedelay = sprite->anims[anim]->delays[frame];
  return true;
}

bool GEntity::SetAnim(Uint8 na)
{
  if (na>=sprite->num_anims) return false;
  
  anim = na;
  return SetFrame(0);
}

bool GEntity::SetSprite(char* ns, CSpriteInfo ** sprite)
{
  if (*sprite) SpriteMan.Unregister(*sprite);
  *sprite = SpriteMan.Register(ns);
  return true;
}


bool GEntity::IsPassable(Uint16 px, Uint16 py) 
{ 
  if (point_in_rect(xpos-xsize/2, ypos-ysize/2, xsize, ysize, px, py))
    return false; 
  else 
    return true;
};


bool GEntity::DetectCollision(GAME_MAXOBJS_TYPE islot, rect_t * bbox)
{
  if (slot==islot) return false;   // objects are not in collision with themselves
  if (game->objs[islot]->state&OSTATE_ACTIVE)
  {
    if (game->objs[islot]->GetType()==ot_sound) return false;

    GEntity * target = (GEntity*)game->objs[islot];

    // rectangular vs rectangular
    rect_t r2;
    target->GetBoundingBox(&r2);
    
    rect_t a1[4];
    int i1 = DecomposeBoundingBox(&r2, a1);
    rect_t a2[4];
    int i2 = DecomposeBoundingBox(bbox, a2);
    
    for (int ii1=0; ii1<i1; ii1++)
      for (int ii2=0; ii2<i2; ii2++)
        if (RectOverlap(&a1[ii1], &a2[ii2])) return true;
        
    return false;
        
    // TODO: pixel-level collision detection
  }
  else
    return false;  // non-entity objects don't colide
}

bool GEntity::GlobalCollision(rect_t * bbox, ObjColCB* ocb, MapColCB* mcb)
{
  GAME_MAXOBJS_TYPE slot;
  for (slot=0; slot<GAME_MAX_OBJS; slot++) 
  {
    if (DetectCollision(slot, bbox))
    {
      if (ocb)
      {
        if ((*ocb)(game->objs[slot])) return true;
      }
      else
      {
        // normal behaviour
        switch (game->objs[slot]->GetType()) {
        case ot_bomb:
          return true;
        }
      }
    }
  }
  int mx, my;

  rect_t mr = *bbox;
  MapRectFromBoundingBox(&mr);
  for (my=mr.top; my<=mr.bottom; my++)
  {
    for (mx=mr.left; mx<=mr.right; mx++)
    {
      BLKSTR * b = game->map.GetBlock(mx, my);
      if (mcb)
      {
        if ((*mcb)(b, mx, my)) return true;
      }
      else
      {
        // normal behaviour
        if (b)
        {
          if (b->tl1) // wall
          {
            return true;
          }
        }
      }
    }
  }
  return false;
}


static bool GEntityObjColAll(GObj* x)
{
  return true;
}

void GEntity::RandomPlace(Uint16 sx, Uint16 sy, rect_t * r)
{
  int c=0;
  while (c<10) 
  {
    r->left = rand() % mappxwidth;
    r->top = rand() % mappxheight;
    r->right = r->left + sx;
    r->bottom = r->top + sy;
    if (!GlobalCollision(r, GEntityObjColAll)) break;
  }
}

void GEntity::GetMapRect(rect_t * r)
{
  GetBoundingBox(r);
  MapRectFromBoundingBox(r);
}

void GEntity::MoveBoundingBox(rect_t * r, int dx, int dy)
{
  r->left += dx;
  r->right += dx;
  r->top += dy;
  r->bottom += dy;
}

void GEntity::MapRectFromBoundingBox(rect_t * r)
{
  r->left = r->left / MBSW;
  r->right = (r->right-1) / MBSW;
  r->top = r->top / MBSH;
  r->bottom = (r->bottom-1) / MBSH;
}

void GEntity::GetBoundingBox(rect_t * r)
{
  r->left = xpos - xsize/2;
  r->right = xpos + xsize/2;
  r->top = ypos - ysize/2;
  r->bottom = ypos + ysize/2;
} 

int GEntity::DecomposeBoundingBox(rect_t * r, rect_t *q)
{
  int x1, x2, x3, x4; bool x;
  int y1, y2, y3, y4; bool y;

  // x coordinates
  if (r->left < 0) 
  { 
    x1 = r->left + mappxwidth;
    x2 = mappxwidth;
    x3 = 0;
    x4 = r->right;
    x = true;
  }
  else
  if (r->right > mappxwidth)
  {
    x1 = r->left;
    x2 = mappxwidth;
    x3 = 0;
    x4 = r->right - mappxwidth;
    x = true;
  }
  else
  {
    x1 = r->left;
    x2 = r->right;
    x = false;
  }

  // y coordinates
  if (r->top < 0) 
  { 
    y1 = r->top + mappxheight;
    y2 = mappxheight;
    y3 = 0;
    y4 = r->bottom;
    y = true;
  }
  else
  if (r->bottom > mappxheight)
  {
    y1 = r->top;
    y2 = mappxheight;
    y3 = 0;
    y4 = r->bottom - mappxheight;
    y = true;
  }
  else
  {
    y1 = r->top;
    y2 = r->bottom;
    y = false;
  }

  if (x)
  {
    if (y)
    {
      q[0].left = x1;
      q[0].right = x2;
      q[0].top = y1;
      q[0].bottom = y2;
      q[1].left = x1;
      q[1].right = x2;
      q[1].top = y3;
      q[1].bottom = y4;
      q[2].left = x3;
      q[2].right = x4;
      q[2].top = y1;
      q[2].bottom = y2;
      q[3].left = x3;
      q[3].right = x4;
      q[3].top = y3;
      q[3].bottom = y4;
      return 4;
    }
    q[0].left = x1;
    q[0].right = x2;
    q[0].top = y1;
    q[0].bottom = y2;
    q[1].left = x3;
    q[1].right = x4;
    q[1].top = y1;
    q[1].bottom = y2;
    return 2;
  }
  if (y)
  {
    q[0].left = x1;
    q[0].right = x2;
    q[0].top = y1;
    q[0].bottom = y2;
    q[1].left = x1;
    q[1].right = x2;
    q[1].top = y3;
    q[1].bottom = y4;
    return 2;
  }
  
  q[0].left = x1;
  q[0].right = x2;
  q[0].top = y1;
  q[0].bottom = y2;
  return 1;
}

void GEntity::NormalizePos()
{
  if (xpos<0) xpos+=mappxwidth;
  else
    if (xpos>=mappxwidth) xpos-=mappxwidth;

  if (ypos<0) ypos+=mappxheight;
  else
    if (ypos>=mappxheight) ypos-=mappxheight;
}

void GEntity::NormalizePos(Sint16* x, Sint16* y)
{
  if (*x<0) *x+=mappxwidth;
  else
    if (*x>=mappxwidth) *x-=mappxwidth;

  if (*y<0) *y+=mappxheight;
  else
    if (*y>=mappxheight) *y-=mappxheight;
}

void GEntity::CloneSound(Uint8 id, int dx, int dy, Uint8 volume, TICK_TYPE loop)
{
  game->IncrementLOID();
  GAME_MAXOBJS_TYPE sound_slot = game->FindUnusedSlot();
  game->RebornObject(game->last_oid, sound_slot, ot_sound);
  
  GSound * s = (GSound*)game->objs[sound_slot];

  s->xpos.Set(xpos+dx);
  s->ypos.Set(ypos+dy);
  s->NormalizePos();
  
  s->loop.Set(loop);
  s->sampleid.Set(id);
  s->volume.Set(volume);
  s->createtime.Set(curtime);
}


int GEntity::Destination(GEntity* e)
{
  Sint16 dx = ABS(xpos - e->xpos);
  Sint16 dy = ABS(ypos - e->ypos);

  NormalizePos(&dx, &dy);

  if (dx>mappxwidth/2) dx=mappxwidth-dx;
  if (dy>mappxheight/2) dy=mappxheight-dy;

  return MAX(dx, dy);
}







