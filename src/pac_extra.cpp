#include <string.h>
#include "SDL.h"
#include "const.h"
#include "gamedefs.h"
#include "pac.h"
#include "client.h"

GExtra::GExtra(Uint16 id, CGame* igame, GAME_MAXOBJS_TYPE islot):GEntity(id, igame, islot),
// implicit shot initialization

type     (GEXTRA_ID+0 , 0             , 0),
life     (GEXTRA_ID+1 , 0             , 0),
modifier (GEXTRA_ID+2 , 0             , 0),
data     (GEXTRA_ID+3 , 0             , 0)
{
  spriteZ = NULL;
  spriteB = NULL;
  spriteP = NULL;
  spriteM = NULL;
  spriteN = NULL;
  spriteA = NULL;
  SetSprite("ez", &spriteZ);
  SetSprite("eb", &spriteB); 
  SetSprite("ep", &spriteP); 
  SetSprite("en", &spriteN); 
  SetSprite("em", &spriteM); 
  SetSprite("bp", &spriteA); 
  sprite = spriteZ;
  timestamp = 0;
}

GExtra::~GExtra()
{
  if (spriteZ!=sprite) SpriteMan.Unregister(spriteZ);
  if (spriteB!=sprite) SpriteMan.Unregister(spriteB);
  if (spriteN!=sprite) SpriteMan.Unregister(spriteN);
  if (spriteM!=sprite) SpriteMan.Unregister(spriteM);
  if (spriteP!=sprite) SpriteMan.Unregister(spriteP);
  if (spriteA!=sprite) SpriteMan.Unregister(spriteA);
}

Uint8 GExtra::GetType()
{
  return ot_extra;
}

int GExtra::TranslateAnim(Uint8 anim)
{
  switch (anim/30) {
  case 0:
    sprite = spriteZ;     
     if (anim-EX_WEP_BASE>=sprite->num_anims) return 0;
    return anim-EX_WEP_BASE;
  case 1:
    sprite = spriteM;     
    if (anim-EX_AMM_BASE>=sprite->num_anims) return 0;
    return anim-EX_AMM_BASE;
  case 2:
    sprite = spriteP;     
    if (anim-EX_POS_BASE>=sprite->num_anims) return 0;
    return anim-EX_POS_BASE;
  case 3:
    sprite = spriteN;     
    if (anim-EX_NEG_BASE>=sprite->num_anims) return 0;
    return anim-EX_NEG_BASE;
  case 4:
    sprite = spriteB;     
    if (anim-EX_PTS_BASE>=sprite->num_anims) return 0;
    return anim-EX_PTS_BASE;
  case 5:
    sprite = spriteA;     
    if (anim-EX_SUP_BASE>=sprite->num_anims) return 0;
    return anim-EX_SUP_BASE;
  }
  return 0;
}

void GExtra::Draw(SDL_Surface * screen, SDL_Rect * screen_rect)
{
  if (!(state&OSTATE_VISIBLE)) return;

  int sanim = TranslateAnim(anim);
  SDL_Rect sr;
  SDL_Rect dr;
  
  Uint16 rx = xpos - game->vars.camx;
  Uint16 ry = ypos - game->vars.camy;
  
  if (sanim<sprite->num_anims && 
    frame<sprite->anims[sanim]->frames)
  {
    Uint16 dx = minimum(sprite->anims[sanim]->w, xsize) / 2;
    Uint16 dy = minimum(sprite->anims[sanim]->h, ysize) / 2;
    
    // is sprite visible ?
    if ((rx+dx>=0) && (ry+dy>=0) && (rx-dx<screen_rect->w) && (ry-dy<screen_rect->h))
    {
      // locate rectangle of source sprite
      sr.w = minimum(sprite->anims[sanim]->w, xsize);
      sr.h = minimum(sprite->anims[sanim]->h, ysize);
      sr.x = sr.w * frame;
      sr.y = 0;
      
      dr.x = screen_rect->x + rx - dx;
      dr.y = screen_rect->y + ry - dy;
      dr.w = sr.w;
      dr.h = sr.h;

      BlitSurface(sprite->anims[sanim]->gfx, sr, screen, dr, screen_rect);
    }
  }
}

void GExtra::ServerCHS(Uint32 time)
{
  lifetime = time + life*10;
  if (modifier&EX_FLAG_FIXED) 
  {
    lifetime = ~1;
  }
  else
  {
    if (timestamp!=0) state = 0;
    return;
  }

  if (modifier&EX_FLAG_RANDOMPLACE) 
  {
    rect_t r;
    RandomPlace(32, 32, &r);
    xpos = r.left + (r.right - r.left)/2;
    ypos = r.top + (r.bottom - r.top)/2;
    NormalizePos();
  }

  bool randomized = false;
  if (modifier&EX_FLAG_RANDOM_WEP) 
  {
    if (!randomized || (rand()%5==0))
    {
      type = EX_WEP_BASE + 3*(rand()%EX_WEP_NUM);
      randomized = true;
    }
  }
  if (modifier&EX_FLAG_RANDOM_AMM) 
  {
    if (!randomized || (rand()%5==0))
    {
      type = EX_AMM_BASE + 3*(rand()%EX_AMM_NUM);
      randomized = true;
    }
  }
  if (modifier&EX_FLAG_RANDOM_PTS) 
  {
    if (!randomized || (rand()%5==0))
    {
      type = EX_PTS_BASE + 3*(rand()%EX_PTS_NUM);
      randomized = true;
    }
  }
  if (modifier&EX_FLAG_RANDOM_POS) 
  {
    if (!randomized || (rand()%5==0))
    {
      type = EX_POS_BASE + 3*(rand()%EX_POS_NUM);
      randomized = true;
    }
  }
  if (modifier&EX_FLAG_RANDOM_NEG) 
  {
    if (!randomized || (rand()%5==0))
    {
      type = EX_NEG_BASE + 3*(rand()%EX_NEG_NUM);
      randomized = true;
    }
  }

  if (modifier&EX_FLAG_RESPAWN) 
  {
    state = state & ~OSTATE_VISIBLE; // respawn after data time
    timestamp = time + data*10;  // respawn time
    SetAnim(type);  
    return;
  }

  SetAnim(type+1);  // generate at another special
}

void GExtra::Take(GPlayer* p, TICK_TYPE time)
{
  if (type>=EX_SUP_BASE)
  {
    int owner = -1;
    if (type==EX_SUP_BECHERX)
    {
      p->points += points_superbecher;
      SV_BroadcastPrintf("%s collected diamond", p->player_name.GetValRef()->chars);
      //SV_BroadcastVoice(SM_BECHERC1, 100, 120);
      type = EX_SUP_BECHER;
      SetAnim(type);
      ServerCHS(time);
    }
    else
    {
      if (time>p->warpout_end)
      {
        p->warpout_end = time + warpout_time;
        //if (p->brain_type==bt_client)
          //SV_ClientVoice(p->brain_owner, SM_OHNO, 100, 80);
      }
    }
  }
  else
  {
    int sanim = TranslateAnim(anim);
    if (sanim%3==0)
    {
      // perform action on player p
      switch (type) {
        // weapons
      case EX_WEP_BOMB:
        if (p->witems&WI_BOMBS) goto NEXT;
        p->witems = p->witems|WI_BOMBS;
        CloneSound(SM_GPREMIE, 0, 0);
        break;
      case EX_WEP_GRENADE:
        if (p->witems&WI_GRENADES) goto NEXT;
        p->witems = p->witems|WI_GRENADES;
        CloneSound(SM_GPREMIE, 0, 0);
        break;
      case EX_WEP_RAILGUN:
        if (p->witems&WI_RAILGUN) goto NEXT;
        p->witems = p->witems|WI_RAILGUN;
        CloneSound(SM_GPREMIE, 0, 0);
        break;
      case EX_WEP_BECHER:
        if (p->witems&WI_BECHEROMET) goto NEXT;
        p->witems = p->witems|WI_BECHEROMET;
        CloneSound(SM_GPREMIE, 0, 0);
        break;
      
        // positive
      case EX_POS_PWBM:
        p->power = extra_power;
        CloneSound(SM_GPREMIE, 0, 0);
        break;
      case EX_POS_INFRA:
        p->infra_end = time + spec_infra_time;
        CloneSound(SM_GPREMIE, 0, 0);
        break;
      case EX_POS_MVBM:
        p->witems = p->witems|SI_PUNCH;
        CloneSound(SM_GPREMIE, 0, 0);
        break;
      case EX_POS_BERSERK:
        p->berserk_end = time + spec_berserk_time;
        SV_BroadcastVoice(SM_BERSERK, 100, 5);
        CloneSound(SM_GPREMIE, 0, 0);
        break;
      case EX_POS_INVIS:
        p->invis_end = time + spec_invis_time;
        CloneSound(SM_GPREMIE, 0, 0);
        break;
      case EX_POS_SPEED:
        p->speed_end = time + spec_speed_time;
        CloneSound(SM_GPREMIE, 0, 0);
        break;
      
        // negative
      case EX_NEG_GLUE:
        p->glue_end = time + spec_glue_time;
        CloneSound(SM_GPREMIE, 0, 0);
        break;
      case EX_NEG_REVERSE:
        p->reverse_end = time + spec_reverse_time;
        CloneSound(SM_GPREMIE, 0, 0);
        break;
      case EX_NEG_SLOW:
        p->slow_end = time + spec_slow_time;
        CloneSound(SM_GPREMIE, 0, 0);
        break;
      case EX_NEG_NOWEAPON:
        p->noweapon_end = time + spec_noweapon_time;
        CloneSound(SM_GPREMIE, 0, 0);
        break;
      
        // ammo
      case EX_AMM_BOMB:
        if (p->bombs_ammo<max_ammo_bombs) p->bombs_ammo += spec_ammo_bombs;
        CloneSound(SM_GAMMO, 0, 0);
        break;
      case EX_AMM_GREN:
        if (p->grenades_ammo<max_ammo_grenades) p->grenades_ammo += spec_ammo_grenades;
        CloneSound(SM_GAMMO, 0, 0);
        break;
      case EX_AMM_WARP:
        if (p->warps<max_warps) p->warps += spec_add_warps;
        CloneSound(SM_GPREMIE, 0, 0);
        break;
      case EX_AMM_SHIELD:
        if (p->shields<max_shields) p->shields += spec_add_shields;
        CloneSound(SM_GPREMIE, 0, 0);
        break;
      case EX_AMM_BECHER:
        if (p->becheromet_ammo<max_ammo_becheromet) p->becheromet_ammo += spec_ammo_becheromet;
        CloneSound(SM_GAMMO, 0, 0);
        break;
      case EX_AMM_RAIL:
        if (p->railgun_ammo<max_ammo_railgun) p->railgun_ammo += spec_ammo_railgun;
        CloneSound(SM_GAMMO, 0, 0);
        break;
      
        // points
      case EX_PTS_COCT1:
        p->AddPoints(points_coct1);
        CloneSound(SM_GPREMIE, 0, 0);
        break;
      case EX_PTS_COCT2:
        p->AddPoints(points_coct2);
        CloneSound(SM_GPREMIE, 0, 0);
        break;
      case EX_PTS_GOLD1:
        p->AddPoints(points_gold1);
        CloneSound(SM_GPREMIE, 0, 0);
        break;
      case EX_PTS_GOLD2:
        p->AddPoints(points_gold2);
        CloneSound(SM_GPREMIE, 0, 0);
        break;
      case EX_PTS_ENERGY:
        p->AddPoints(points_rapid);
        CloneSound(SM_GPREMIE, 0, 0);
        break;
      }
      ServerCHS(time);
    }
  }
NEXT:;
}

void GExtra::ServerThink(Uint32 time)
{
  if (type==EX_SUP_BECHER && ysize!=64) ysize=64;
  GEntity::ServerThink(time);
  int sanim = TranslateAnim(anim);
  
  if (timestamp==0) 
  {
    ServerCHS(time);
    timestamp = 1;
  }

  Uint8 subanim = sanim%3;

  if (subanim==1)
    if (frame == sprite->anims[sanim]->frames-1) SetAnim(type);

  if (subanim==2)
    if (frame == sprite->anims[sanim]->frames-1) 
    {
      timestamp = 1;
      ServerCHS(time);
      return;
    }

  if (type==EX_SUP_BECHER)
  {
    if ((subanim==0) && (state&OSTATE_VISIBLE))
      if (frame == sprite->anims[sanim]->frames-1) 
      {
        type=EX_SUP_BECHERX;
        SetAnim(type);
        return;
      }
  }

  if (time==timestamp)
  {
    // respawn time
    if (type==EX_SUP_BECHER)
    {
      //SV_BroadcastVoice(SM_BECHERA1, 100, 120);
      SV_BroadcastPrintf("Diamond appeared");
    }
    state = state | OSTATE_VISIBLE;
    SetAnim(type+1);
    lifetime = time + life*10;
  }
  
  if (type<EX_SUP_BASE)
    if (time==lifetime) { 
      SetAnim(type+2);  // disappier
      lifetime = 0;
    }

}

void GExtra::ClientThink(Uint32 time)
{
  if (type==EX_SUP_BECHER && ysize!=64) ysize=64;
  GEntity::ClientThink(time);
  int sanim = TranslateAnim(anim);
  if (sanim%3==1)
      if (frame == sprite->anims[sanim]->frames-1) SetAnim(type);
}

void GExtra::Replication(int cnum, CReplicator* rep)
{
  GEntity::Replication(cnum, rep);
  
  type.Write(rep, cnum);
  life.Write(rep, cnum);
  modifier.Write(rep, cnum);
  data.Write(rep, cnum);
}

char GExtra::GetReplicated(Uint8 id, net_msg* msg, TICK_TYPE time)
{
  if (GEntity::GetReplicated(id, msg, time)) return 1;
  
  if (type.Read(id, msg ,time)) return 1;
  if (life.Read(id, msg ,time)) return 1;
  if (modifier.Read(id, msg ,time)) return 1;
  if (data.Read(id, msg ,time)) return 1;
  
  return 0;
}

void GExtra::MakeDirty(int i, bool dirty)
{
  GEntity::MakeDirty(i, dirty);
  
  type.MakeDirty(i, dirty);
  life.MakeDirty(i, dirty);
  modifier.MakeDirty(i, dirty);   
  data.MakeDirty(i, dirty);  
}

bool GExtra::SetAnim(Uint8 na)
{
  int sanim = TranslateAnim(anim);
  anim = na;
  return SetFrame(0);
}

bool GExtra::SetFrame(Uint8 nf)
{
  int sanim = TranslateAnim(anim);
  if (nf>=sprite->anims[sanim]->frames) return false;

  frame = nf;
  framedelay = sprite->anims[sanim]->delays[frame];
  return true;
}

void GExtra::Animate()
{
  // klasicke animate nebude posilano pres sit - zbytecne
  int sanim = TranslateAnim(anim);
  if (framedelay) 
  {
    framedelay--;
    framedelay.MakeDirty(-1, false);
  }
  else
  {
    frame++;
    if (frame>=sprite->anims[sanim]->frames) frame = 0;
    frame.MakeDirty(-1, false);
    
    framedelay = sprite->anims[sanim]->delays[frame];
    framedelay.MakeDirty(-1, false);
  }
}
