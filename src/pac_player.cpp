#include <string.h>
#include "SDL.h"
#include "const.h"
#include "gamedefs.h"
#include "pac.h"
#include "client.h"
#include "server.h"
#include "sampleman.h"
#include "bot.h"

int blocked_inputs = 0;

GPlayer::GPlayer(Uint16 id, CGame* igame, GAME_MAXOBJS_TYPE islot):GEntity(id, igame, islot),
// implicit entity initialization

player_name     (GPLAYER_ID+0 , ""            , 0),
frags           (GPLAYER_ID+1 , 0             , 0),                  
points          (GPLAYER_ID+2 , 0             , 0),                  
brain_type      (GPLAYER_ID+3 , bt_soulfree   , 0),   
brain_owner     (GPLAYER_ID+4 , 250           , 0),           
pistolka_ammo   (GPLAYER_ID+5 , player_sa_pistolka             , 0),   
becheromet_ammo (GPLAYER_ID+6 , player_sa_becheromet           , 0),
railgun_ammo    (GPLAYER_ID+7 , player_sa_railgun              , 0),   
bombs_ammo      (GPLAYER_ID+8 , player_sa_bombs                , 0),   
grenades_ammo   (GPLAYER_ID+9 , player_sa_grenades             , 0),   
kbindex         (GPLAYER_ID+10 , 0             , 0),
skin_name       (GPLAYER_ID+11 , ""            , 0),
special         (GPLAYER_ID+12 , 0             , 0),
weapon          (GPLAYER_ID+13 , player_sa_weapon , 0),
witems          (GPLAYER_ID+14 , player_sa_witems , 0),
shields         (GPLAYER_ID+15 , player_sa_shields, 0),
warps           (GPLAYER_ID+16 , player_sa_warps, 0),
power           (GPLAYER_ID+17 , player_sa_power, 0),
speed_end       (GPLAYER_ID+18 , 0             , 0),
slow_end        (GPLAYER_ID+19 , 0             , 0),
shield_end      (GPLAYER_ID+20 , 0             , 0),
glue_end        (GPLAYER_ID+21 , 0             , 0),
infra_end       (GPLAYER_ID+22 , 0             , 0),
reverse_end     (GPLAYER_ID+23 , 0             , 0),
noweapon_end    (GPLAYER_ID+24 , 0             , 0),
invis_end       (GPLAYER_ID+25 , 0             , 0),
fire_anim_end   (GPLAYER_ID+26 , 0             , 0),
warpin_end      (GPLAYER_ID+27 , 0             , 0),
warpout_end     (GPLAYER_ID+28 , 0             , 0),
berserk_end     (GPLAYER_ID+29 , 0             , 0),
bornin_end      (GPLAYER_ID+30 , 0             , 0),
ping            (GPLAYER_ID+31 , 0             , 0)
{
  direction=0;

  last_pistolka_time = 0;
  last_becheromet_time = 0;
  last_railgun_time = 0;
  last_bomb_time = 0;
  last_grenade_time = 0;
  last_warp_time = 0;

  rail_scheduler = 0;

  adjusttime=0;
  skin = NULL;
  SetSkin(sprite_imp.string);

  pistolka = NULL;
  xbomb = NULL;
  railgun = NULL;
  grenade = NULL;
  becheromet = NULL;
  addons = NULL;

  SetSprite("pistolka", &pistolka);
  SetSprite("xbomb", &xbomb);
  SetSprite("railgun", &railgun);
  SetSprite("grenade", &grenade);
  SetSprite("becher", &becheromet);
  SetSprite("addons", &addons);
  
  last_x_tick;
  last_y_tick;

  steps = NULL; 
  pe = NULL;

  last_speed = 8;
}

GPlayer::~GPlayer()
{
  if (brain_type==bt_bot) bot_done(this, game);

  if (skin) SkinMan.Unregister(skin);

  if (pistolka) SpriteMan.Unregister(pistolka);
  if (xbomb) SpriteMan.Unregister(xbomb);
  if (railgun) SpriteMan.Unregister(railgun);
  if (grenade) SpriteMan.Unregister(grenade);
  if (becheromet) SpriteMan.Unregister(becheromet);
  if (addons) SpriteMan.Unregister(addons);
}

Uint8 GPlayer::GetType()
{
  return ot_player;
}

bool GPlayer::SetSkin(char* ns)
{
  if (skin) SkinMan.Unregister(skin);
  skin = SkinMan.Register(ns);
  skin_name = ns;
  return true;
}

void GPlayer::Draw(SDL_Surface * screen, SDL_Rect * screen_rect)
{
  if (!(state&OSTATE_VISIBLE)) return;
  Uint8 alpha;

  if (curtime<invis_end) 
  {  
    if (brain_owner==client_info.client_num || curtime<infra_end)
      alpha=invisalpha; 
    else 
      return;
  }
  else
    alpha=255;

  GEntity::Draw(screen, screen_rect, xpos, ypos, skin_gr_sx, skin_gr_sy, frame, anim, skin, alpha);
  if (curtime<berserk_end) 
  {  
    GEntity::Draw(screen, screen_rect, xpos, ypos, skin_gr_sx, skin_gr_sy, 0, 4, addons, 255);
  }

  int dir = anim%2?0:1;
  Sint16 x;
  Sint16 y;

  if (curtime>=noweapon_end)
    switch (weapon) {
    case 0:
      if (curtime<fire_anim_end)
        GEntity::Draw(screen, screen_rect, xpos, ypos, skin_gr_sx, skin_gr_sy, 0, dir, pistolka, alpha);
      else
        GEntity::Draw(screen, screen_rect, xpos, ypos, skin_gr_sx, skin_gr_sy, 0, 2+dir, pistolka, alpha);
      break;
    case 1:
      if (dir) x = xpos + 16; else x = xpos - 16; 
      y=0;
      NormalizePos(&x, &y);
      if (curtime<fire_anim_end)
        GEntity::Draw(screen, screen_rect, x, ypos, 2*skin_gr_sx, skin_gr_sy, 0, dir, becheromet, alpha);
      else
        GEntity::Draw(screen, screen_rect, x, ypos, 2*skin_gr_sx, skin_gr_sy, 0, 2+dir, becheromet, alpha);
      break;
    case 2:
      if (curtime<fire_anim_end)
        GEntity::Draw(screen, screen_rect, xpos, ypos, skin_gr_sx, skin_gr_sy, 0, dir, railgun, alpha);
      else
        GEntity::Draw(screen, screen_rect, xpos, ypos, skin_gr_sx, skin_gr_sy, 0, 2+dir, railgun, alpha);
      break;
    case 3:
      if (curtime<fire_anim_end)
        GEntity::Draw(screen, screen_rect, xpos, ypos, skin_gr_sx, skin_gr_sy, 0, dir, xbomb, alpha);
      else
        GEntity::Draw(screen, screen_rect, xpos, ypos, skin_gr_sx, skin_gr_sy, 0, 2+dir, xbomb, alpha);
      break;
    case 4:
      GEntity::Draw(screen, screen_rect, xpos, ypos, skin_gr_sx, skin_gr_sy, 0, dir, grenade, alpha);
      break;
    }

  if (curtime<shield_end)
  {
    GEntity::Draw(screen, screen_rect, xpos, ypos, skin_gr_sx, skin_gr_sy, 0, 0+dir, addons, alpha);
  }

  if (curtime<bornin_end) 
  {  
    int f=addons->anims[5]->frames-(bornin_end-curtime)/born_framedelay;
    if (f<0) f=0;
    GEntity::Draw(screen, screen_rect, xpos, ypos, skin_gr_sx, skin_gr_sy, f, 5, addons, 255);
  }
  if (curtime<warpin_end) 
  {  
    int f=addons->anims[2]->frames-(warpin_end-curtime)/warp_framedelay;
    if (f<0) f=0;
    GEntity::Draw(screen, screen_rect, xpos, ypos, skin_gr_sx, skin_gr_sy, f, 3, addons, 255);
  }
  if (curtime<warpout_end) 
  {  
    int f=addons->anims[3]->frames-(warpout_end-curtime)/warp_framedelay;
    if (f<0) f=0;
    GEntity::Draw(screen, screen_rect, xpos, ypos, skin_gr_sx, skin_gr_sy, f, 2, addons, 255);
  }

  if (steps)
  {
    int i=0;
    do
    {
      Sint16 x = steps[i].Pos.X * 16 + 8;
      Sint16 y = steps[i].Pos.Y * 16 + 8;
      NormalizePos(&x, &y);
      GEntity::Draw(screen, screen_rect, x, y, 8, 8, 0, 4, pistolka, 255);
      i++;
    } 
    while (!(steps[i].Type&PST_END));
  }
}


void GPlayer::ClientThink(Uint32 time)
{
  int i;

  GEntity::ClientThink(time);
//  if (anim>=2 && skin->anims[anim]->frames-1==frame) state = state & ~OSTATE_VISIBLE;

  if (anim>=2) return;  // player is going to die

  // only owner
  // get user/brain input and create move vector
  if ((game->state==GS_CLIENT) && (role == ROLE_AutonomousProxy))
  {
    MoveVector mv;
    Uint8 *keys;
    
    switch (brain_type) {
    // human user input -> move vector
    case bt_client:
      keys = SDL_GetKeyState(NULL);
      if (keys[keybindings[kbindex].key_down] == SDL_PRESSED) mv.DOWN = 1; else  mv.DOWN = 0;
      if (keys[keybindings[kbindex].key_up] == SDL_PRESSED) mv.UP = 1; else  mv.UP = 0;
      if (keys[keybindings[kbindex].key_left] == SDL_PRESSED) mv.LEFT = 1; else  mv.LEFT = 0;
      if (keys[keybindings[kbindex].key_right] == SDL_PRESSED) mv.RIGHT = 1; else  mv.RIGHT = 0;
      if (keys[keybindings[kbindex].key_fire] == SDL_PRESSED) mv.FIRE = 1; else  mv.FIRE = 0;
      if (keys[keybindings[kbindex].key_shield] == SDL_PRESSED) mv.SHIELD = 1; else  mv.SHIELD = 0;
      if (keys[keybindings[kbindex].key_warp] == SDL_PRESSED) mv.WARP = 1; else  mv.WARP = 0;
      for (i=0; i<MAX_WEAPONS; i++) {
        if (keys[keybindings[kbindex].key_weapon[i]] == SDL_PRESSED) mv.WEAPON[i] = 1; else  mv.WEAPON[i] = 0;
      }
      
      break;
    
    // bot AI -> move vector
    case bt_bot:
      // call AI to obtain bot's move vector
      bot_think(this, game, &mv);
      break;
    }
    
    mv.tick = time;
    if (time<reverse_end)
    {
      Uint8 s;
      s = mv.RIGHT;
      mv.RIGHT = mv.LEFT;
      mv.LEFT = s;
      s = mv.UP;
      mv.UP = mv.DOWN;
      mv.DOWN = s;
    }
    
    if (brain_type == bt_soulfree || blocked_inputs)
    {
      mv.DOWN = 0;
      mv.UP = 0;
      mv.LEFT = 0;
      mv.RIGHT = 0;
      mv.FIRE = 0;
      mv.SHIELD = 0;
      mv.WARP = 0;
      for (i=0; i<MAX_WEAPONS; i++)
        mv.WEAPON[i] = 0;
    }
    
    // make server_move packet
    Uint16 c=0;
    c = (mv.WARP<<6) + (mv.SHIELD<<5) + (mv.FIRE<<4) + (mv.DOWN<<3) + (mv.UP<<2) + (mv.LEFT<<1) + mv.RIGHT;
    for (i=0; i<MAX_WEAPONS; i++)
      c = (c<<1) + mv.WEAPON[i];
    
    if (c)  // send only non-empty move vector
    {
      game->replicator.SetLayer(LAY_MOVEVECTOR);
      game->replicator<<REP_SERVERMOVE<<slot<<oid<<mv.tick<<c;
      game->replicator.Mark();
      
      MoveAutonomous(&mv, time);
      if (dbg_prediction.value) ConOut("sending mv -> ctime=%d cpos=[%d,%d]", time, *xpos.GetValRef(), *ypos.GetValRef());
      predictor.Add(&mv);
    }
  }

}

// called from server's replication process
// currently this is the only way how client could change server's game state
void GPlayer::ServerMove(MoveVector * mv)
{
  // TODO: here should be real server movement (collisions, ph. forces, etc.)
  if (anim>=2) return;  // player is going to die
  MoveAutonomous(mv, mv->tick);
}

MoveVector GPlayer::DecodeMV(net_msg* msg, TICK_TYPE time)
{
  int i;
  MoveVector mv;
  Uint16 c=0;
  
  (*msg)>>mv.tick;
  (*msg)>>c;
  
  for (i=MAX_WEAPONS-1; i>=0; i--)
  {
    mv.WEAPON[i] = c&1;
    c = c>>1;
  }

  mv.WARP = (c>>6)&1;
  mv.SHIELD = (c>>5)&1;
  mv.FIRE = (c>>4)&1;
  mv.DOWN = (c>>3)&1;
  mv.UP = (c>>2)&1;
  mv.LEFT = (c>>1)&1;
  mv.RIGHT = (c)&1;
  
  return mv;
}

void GPlayer::Replication(int cnum, CReplicator* rep)
{
  GEntity::Replication(cnum, rep); // cnum!=brain_owner || brain_owner==250); 
  
  frags.Write(rep, cnum);
  points.Write(rep, cnum);
  brain_type.Write(rep, cnum);
  brain_owner.Write(rep, cnum);

  pistolka_ammo.Write(rep, cnum);
  becheromet_ammo.Write(rep, cnum);
  railgun_ammo.Write(rep, cnum);
  bombs_ammo.Write(rep, cnum);
  grenades_ammo.Write(rep, cnum);

  kbindex.Write(rep, cnum);
  player_name.Write(rep, cnum);
  skin_name.Write(rep, cnum);

  special.Write(rep, cnum);
  weapon.Write(rep, cnum);

  witems.Write(rep, cnum);
  shields.Write(rep, cnum);
  warps.Write(rep, cnum);
  power.Write(rep, cnum);

  ping.Write(rep, cnum);

  fire_anim_end.Write(rep, cnum);

  speed_end.Write(rep, cnum);
  slow_end.Write(rep, cnum);
  shield_end.Write(rep, cnum);
  glue_end.Write(rep, cnum);
  infra_end.Write(rep, cnum);
  reverse_end.Write(rep, cnum);
  noweapon_end.Write(rep, cnum);
  invis_end.Write(rep, cnum);
  berserk_end.Write(rep, cnum);

  warpin_end.Write(rep, cnum);
  bornin_end.Write(rep, cnum);
  warpout_end.Write(rep, cnum);
}

char GPlayer::GetReplicated(Uint8 id, net_msg* msg, TICK_TYPE time)
{
  if (GEntity::GetReplicated(id, msg, time)) return 1;
  
  if (frags.Read(id, msg ,time)) return 1;
  if (points.Read(id, msg ,time)) return 1;
  if (brain_type.Read(id, msg ,time))
  {
    predictor.Reset();  
    if ((game->state==GS_CLIENT) && (brain_type==bt_bot)) bot_init(this, game);
    return 1;
  }
  if (brain_owner.Read(id, msg ,time))
  {
    if ((game->state==GS_CLIENT) && (brain_owner==client_info.client_num))
      role = ROLE_AutonomousProxy;
    else
      role = ROLE_SimulatedProxy;
    
    if ((game->state==GS_CLIENT) && (brain_owner==client_info.client_num))
    {
      //client_info.game.vars.chasing = slot;
    }
    
    predictor.Reset();  
    return 1;
  } 

  if (pistolka_ammo.Read(id, msg ,time)) return 1;
  if (becheromet_ammo.Read(id, msg ,time)) return 1;
  if (railgun_ammo.Read(id, msg ,time)) return 1;
  if (bombs_ammo.Read(id, msg ,time)) return 1;
  if (grenades_ammo.Read(id, msg ,time)) return 1;
  
  if (kbindex.Read(id, msg ,time)) return 1;
  if (player_name.Read(id, msg ,time)) return 1;
  if (skin_name.Read(id, msg ,time)) 
  {
    SetSkin(skin_name.GetValRef()->chars);
    return 1;
  }

  if (special.Read(id, msg ,time)) return 1;
  if (weapon.Read(id, msg ,time)) return 1;

  if (witems.Read(id, msg ,time)) return 1;
  if (shields.Read(id, msg ,time)) return 1;
  if (warps.Read(id, msg ,time)) return 1;
  if (power.Read(id, msg ,time)) return 1;

  if (ping.Read(id, msg, time)) return 1;

  if (fire_anim_end.Read(id, msg, time)) return 1;

  if (speed_end.Read(id, msg, time)) return 1;
  if (slow_end.Read(id, msg, time)) return 1;
  if (shield_end.Read(id, msg, time)) return 1;
  if (glue_end.Read(id, msg, time)) return 1;
  if (infra_end.Read(id, msg, time)) return 1;
  if (reverse_end.Read(id, msg, time)) return 1;
  if (noweapon_end.Read(id, msg, time)) return 1;
  if (invis_end.Read(id, msg, time)) return 1;
  if (berserk_end.Read(id, msg, time)) return 1;

  if (warpin_end.Read(id, msg, time)) return 1;
  if (bornin_end.Read(id, msg, time)) return 1;
  if (warpout_end.Read(id, msg, time)) return 1;


  return 0;
}

void GPlayer::MakeDirty(int i, bool dirty)
{
  GEntity::MakeDirty(i, dirty);
  
  brain_type.MakeDirty(i, dirty);
  brain_owner.MakeDirty(i, dirty);
  kbindex.MakeDirty(i, dirty);

  frags.MakeDirty(i, dirty);
  points.MakeDirty(i, dirty);
  
  bombs_ammo.MakeDirty(i, dirty);
  railgun_ammo.MakeDirty(i, dirty);
  pistolka_ammo.MakeDirty(i, dirty);
  grenades_ammo.MakeDirty(i, dirty);
  becheromet_ammo.MakeDirty(i, dirty);

  player_name.MakeDirty(i, dirty);
  skin_name.MakeDirty(i, dirty);

  special.MakeDirty(i, dirty);
  weapon.MakeDirty(i, dirty);

  witems.MakeDirty(i, dirty);
  shields.MakeDirty(i, dirty);
  warps.MakeDirty(i, dirty);
  power.MakeDirty(i, dirty);

  ping.MakeDirty(i, dirty);

  fire_anim_end.MakeDirty(i, dirty);

  speed_end.MakeDirty(i, dirty);
  slow_end.MakeDirty(i, dirty);
  shield_end.MakeDirty(i, dirty);
  glue_end.MakeDirty(i, dirty);
  infra_end.MakeDirty(i, dirty);
  reverse_end.MakeDirty(i, dirty);
  noweapon_end.MakeDirty(i, dirty);
  invis_end.MakeDirty(i, dirty);
  berserk_end.MakeDirty(i, dirty);

  warpin_end.MakeDirty(i, dirty);
  bornin_end.MakeDirty(i, dirty);
  warpout_end.MakeDirty(i, dirty);
}

static GObj* ColObj;           // do object collision ?
static bool do_map_collision;  // do map collision ?
static int dx, dy;
static Uint8 witemsx; 
static GPlayer* me;
static TICK_TYPE xtime;

static bool GPlayerObjColA(GObj* x)
{
  ColObj = x;
  return false;
}

static bool GPlayerMapColA(BLKSTR* blk, int mx, int my)
{
  if (blk)
  {
    if (blk->tl1) // wall
    {
      do_map_collision = false;
      return true;
    }
  }
  return false;
}

static bool GPlayerObjColB(GObj* x)
{
  int pd;
  GPlayer* p;
  GExtra* e;
  GShot* s;
  GBShot* b;
  GFray* f;
  GRail* r;
  GGrenade* g;

  if (me->game->state==GS_CLIENT) return false;  // client wont do any efect
  // TODO: later implement client "graphics" only version

  if (!ColObj || x!=ColObj)
  {
    switch (x->GetType()) {
    case ot_extra:
      e = (GExtra*)x;
      if (e->state&OSTATE_VISIBLE)
        e->Take(me, xtime);
      return false;
    case ot_bomb:
      if (witemsx&SI_PUNCH)
      {
        if (dx>0) ((GBomb*)x)->movementx = bomb_mv_speed;
        else if (dx<0) ((GBomb*)x)->movementx = -bomb_mv_speed;
        if (dy>0) ((GBomb*)x)->movementy = bomb_mv_speed;
        else if (dy<0) ((GBomb*)x)->movementy = -bomb_mv_speed;
      }
      if (xtime<me->berserk_end)
        ((GBomb*)x)->Explode();
      return true;
    case ot_player:
      p = (GPlayer*)x;
      if (xtime<me->berserk_end)
      {
        if (xtime<p->berserk_end)
        {
          if (p->Kill() && me->Kill())
          {
            SV_BroadcastPrintf("%s and %s berserked each other", me->player_name.GetValRef()->chars, p->player_name.GetValRef()->chars);
          }
        }
        else
        {
          if (p->Kill()) 
          {
            me->AddFrag();
            SV_BroadcastPrintf("%s berserked %s", me->player_name.GetValRef()->chars, p->player_name.GetValRef()->chars);
          }
        }
      }
      return false;
    case ot_shot:
      s = (GShot*)x;
      if (s->bounced || me->slot!=s->owner)
      {
        pd = me->anim%2?-1:1;
        if (me->curtime<me->shield_end && pd!=SIGN(s->dx))
        {
          s->Bounce();
          s->CloneSound(SM_RSHOT, 0, 0);
        }
        else
        {
          if (me->Kill()) 
          {
            if (me->slot!=s->owner)
            {
              ((GPlayer*)me->game->objs[s->owner])->AddFrag();
              SV_BroadcastPrintf("%s pistoled %s", ((GPlayer*)me->game->objs[s->owner])->player_name.GetValRef()->chars, me->player_name.GetValRef()->chars);
            }
            else
            {
              me->DelFrag();
              SV_BroadcastPrintf("%s pistoled himself", me->player_name.GetValRef()->chars);
            }
          }
          s->state = 0;
        }
      }
      return false;
    case ot_bshot:
      b = (GBShot*)x;
      if (b->owner!=me->slot)
      {
        pd = me->anim%2?-1:1;
        if (me->curtime<me->shield_end && pd!=SIGN(b->dx))
        {
          // TODO: sound naraz bechera do stitu
        }
        else
          if (me->Kill()) 
          {
            ((GPlayer*)me->game->objs[b->owner])->AddFrag();
            SV_BroadcastPrintf("%s becherized %s", ((GPlayer*)me->game->objs[b->owner])->player_name.GetValRef()->chars, me->player_name.GetValRef()->chars);
          }
        b->state = 0;
      }
      return false;
    case ot_fray:
      f = (GFray*)x;
      // perform action on player me
      if (me->Kill())
      {
        if (me->slot!=f->owner)
        {
          ((GPlayer*)me->game->objs[f->owner])->AddFrag();
          SV_BroadcastPrintf("%s blown up %s", ((GPlayer*)me->game->objs[f->owner])->player_name.GetValRef()->chars, me->player_name.GetValRef()->chars);
        }
        else
        {
          me->DelFrag();
          SV_BroadcastPrintf("%s blown up himself", me->player_name.GetValRef()->chars);
        }
      }
      return false;
    case ot_grenade:
      g = (GGrenade*)x;
      // perform action on player me
      if (g->anim==3)
      {
        if (me->Kill()) 
        {
          if (me->slot!=g->owner)
          {
            ((GPlayer*)me->game->objs[g->owner])->AddFrag();
            SV_BroadcastPrintf("%s grenaded %s", ((GPlayer*)me->game->objs[g->owner])->player_name.GetValRef()->chars, me->player_name.GetValRef()->chars);
          }
          else
          {
            me->DelFrag();
            SV_BroadcastPrintf("%s grenaded himself", me->player_name.GetValRef()->chars);
          }
        }  
        return false;
      }
      else
      {
        if (g->owner!=me->slot)
          g->Explode();
        return true;
      }
    case ot_rail:
      r = (GRail*)x;
      if (me->slot!=r->owner && (xtime-r->createtime<4))
      {
        // perform action on player me
        if (me->Kill()) 
        {
          ((GPlayer*)me->game->objs[r->owner])->AddFrag();
          SV_BroadcastPrintf("%s railed %s", ((GPlayer*)me->game->objs[r->owner])->player_name.GetValRef()->chars, me->player_name.GetValRef()->chars);
        }
      }
      return false;
    }
  }
  return false;
}

static bool GPlayerMapColB(BLKSTR* blk, int mx, int my)
{
  if (blk)
  {
    if (blk->tl1) // wall
    {
      return do_map_collision;
    }
  }
  return false;
}

void GPlayer::MoveAutonomous(MoveVector* mv, TICK_TYPE time)
{
#define SHIFT 1
  if (game->state==GS_CLIENT) 
    if (time<warpin_end || time<warpout_end) return;
  
  int STEPS = 8;
  if (time<slow_end) STEPS-=4;
  if (time<speed_end) STEPS+=6;
  if (time<glue_end) STEPS=0;

  last_speed = STEPS;

  rect_t r1;
  GetBoundingBox(&r1);
  
  dx = 0;
  dy = 0;
  if (mv->UP) 
  {
    dy = -SHIFT;
  }
  else {
    if (mv->DOWN) 
    {
      dy = SHIFT;
    }
  }
  if (direction==0)
  {
    if (mv->RIGHT) 
    { 
      dx = SHIFT;
      if (!mv->LEFT && anim!=0) SetAnim(0); 
      if (mv->LEFT && anim!=1) SetAnim(1);

    } 
    else
    {
      if (mv->LEFT) 
      { 
        dx = -SHIFT;
        if (anim!=1) SetAnim(1); 
        direction = 1;
      }
    }
  }
  else
  {
    if (mv->LEFT) 
    { 
      dx = -SHIFT;
      if (!mv->RIGHT && anim!=1) SetAnim(1); 
      if (mv->RIGHT && anim!=0) SetAnim(0);
    }
    else
    {
      if (mv->RIGHT) 
      { 
        dx = SHIFT;
        if (anim!=0) SetAnim(0); 
        direction = 0;
      } 
    }
  }

  int i;


  // if we are in collision with some object in the map 
  // get out of here - do not test collisions
  xtime = time;
  do_map_collision = true;
  me = this;
  witemsx = witems;
  ColObj = NULL;
  GlobalCollision(&r1, GPlayerObjColA, GPlayerMapColA);

  for (i=0; i<STEPS;)
  {
    rect_t sr = r1;
    MoveBoundingBox(&r1, dx, dy);

    // collision with other objects
      if (GlobalCollision(&r1, GPlayerObjColB, GPlayerMapColB))
      {
        rect_t r2 = r1;
        int optimized = 0;
        if (dx)
        {
          if (TryToMoveDx(&r2))
          {
            
            if (abs(sr.top - r2.top) > STEPS - i) 
            {
              if ((sr.top - r2.top) > 0) 
              {
                r2 = sr;
                r2.top = sr.top - (STEPS - i); 
              }
              else 
              {
                r2 = sr;
                r2.top = sr.top + (STEPS - i);
              }
            }
            
            optimized = 1;
            goto OPTIMIZE_DONE;
          }
        }
        if (dy)
        {
          if (TryToMoveDy(&r2))
          {
            if (abs(sr.left - r2.left) > (STEPS - i)) 
            {
              if ((sr.left - r2.left) > 0) 
              {
                r2 = sr;
                r2.left = sr.left - (STEPS - i); 
              }
              else 
              {
                r2 = sr;
                r2.left = sr.left + (STEPS - i);
              }
            }
            optimized = 2;
          }
        }
        OPTIMIZE_DONE:
        if (!optimized) 
        {
          r1 = sr;
          break;
        }
        r1 = r2;
        
        
        i+=MAX(abs(sr.left - r1.left), abs(sr.top - r1.top));
        continue;
      }
    i++;
  }

  /*
  for (i=0; i<STEPS;)
  {
    rect_t sr = r1;
    MoveBoundingBox(&r1, dx, dy);

    // collision with other objects
      if (GlobalCollision(&r1, GPlayerObjColB, GPlayerMapColB))
      {
        if (dx)
        {
          r1 = sr;
          MoveBoundingBox(&r1, dx, 0);
          if (GlobalCollision(&r1, GPlayerObjColB, GPlayerMapColB))
          {
            if (dy)
            {
              r1 = sr;
              MoveBoundingBox(&r1, 0, dy);
              if (GlobalCollision(&r1, GPlayerObjColB, GPlayerMapColB))
              {
                // cannot move
                r1 = sr;
                break;
              }
            }
          }
        }
      }
    i++;
  }
  */

  xpos = r1.left + xsize/2;
  ypos = r1.top + ysize/2;
  NormalizePos();
  
  if (mv->FIRE && time>=noweapon_end && time>warpin_end && time>warpout_end)
  {
    switch (weapon) {
    case 0:
      if (time-last_pistolka_time>pistolka_reload_time && pistolka_ammo) 
      {
        if (game->state!=GS_CLIENT) FirePistolka();
        last_pistolka_time = curtime;
        fire_anim_end = time + pistolka_fire_end_anim;
        if (time<invis_end) invis_end = 0;
      }
      break;
    case 1:
      if (time-last_becheromet_time>becheromet_reload_time && becheromet_ammo) 
      {
        if (game->state!=GS_CLIENT) FireBecheromet();
        last_becheromet_time = curtime;
        fire_anim_end = time + becheromet_fire_end_anim;
        if (time<invis_end) invis_end = 0;
      }
      break;
    case 2:
      if (time-last_railgun_time>railgun_reload_time && !rail_scheduler && railgun_ammo) 
      {
        rail_scheduler = time + player_rail_delay;
        last_railgun_time = curtime;
        fire_anim_end = time + railgun_fire_end_anim;
        if (time<invis_end) invis_end = 0;
      }
      break;
    case 3:
      if (time-last_bomb_time>bomb_reload_time && bombs_ammo) 
      {
        if (game->state!=GS_CLIENT) FireBomb();
        last_bomb_time = curtime;
        fire_anim_end = time + bombs_fire_end_anim;
      }
      break;
    case 4:
      if (time-last_grenade_time>grenade_reload_time && grenades_ammo) 
      {
        if (game->state!=GS_CLIENT) FireGrenade();
        last_grenade_time = curtime;
        fire_anim_end = time + grenades_fire_end_anim;
        if (time<invis_end) invis_end = 0;
      }
      break;
    }
  }
  
  if (game->state!=GS_CLIENT) 
  {
    if (mv->WEAPON[0])
    {
      if (witems&WI_PISTOLKA)
        weapon = 0;
      else
        ; // TODO: play sound
    }
    if (mv->WEAPON[1])
    {
      if (witems&WI_BECHEROMET)
        weapon = 1;
      else
        ; // TODO: play sound
    }
    if (mv->WEAPON[2])
    {
      if (witems&WI_RAILGUN)
        weapon = 2;
      else
        ; // TODO: play sound
    }
    if (mv->WEAPON[3])
    {
      if (witems&WI_BOMBS)
        weapon = 3;
      else
        ; // TODO: play sound
    }
    if (mv->WEAPON[4])
    {
      if (witems&WI_GRENADES)
        weapon = 4;
      else
        ; // TODO: play sound
    }
    if (mv->SHIELD)
    {
      if (shields && time>=shield_end)
      {
        shield_end = time + shieldtime;
        shields--;
      }
    }
    if (mv->WARP)
    {
      if (warps && time-last_warp_time>warp_reload_time && time>warpin_end && time>warpout_end)
      {
        warpout_end = time + warpout_time;
        warps--;
      }
    }
  }
}

bool GPlayer::TryToMoveDx(rect_t * r)
{
  // try to optimize movement
  rect_t orect = *r;
  int ory = orect.top%MBSH;
  if (ory < MBSW/2)
  {
    MoveBoundingBox(&orect, 0, -ory);
    if (GlobalCollision(&orect, GPlayerObjColB, GPlayerMapColB))
    {
      MoveBoundingBox(&orect, 0, MBSH);
      if (GlobalCollision(&orect, GPlayerObjColB, GPlayerMapColB))
      {
        MoveBoundingBox(&orect, 0, -2*MBSH);
        if (GlobalCollision(&orect, GPlayerObjColB, GPlayerMapColB))
        {
//          MoveBoundingBox(&or, 0, 3*MBSH);
//          if (GlobalCollision(&or, GPlayerObjColB, GPlayerMapColB))
//          {
            return false;
//          }
        }
      }
    }
  }
  else
  {
    MoveBoundingBox(&orect, 0, MBSH-ory);
    if (GlobalCollision(&orect, GPlayerObjColB, GPlayerMapColB))
    {
      MoveBoundingBox(&orect, 0, -MBSH);
      if (GlobalCollision(&orect, GPlayerObjColB, GPlayerMapColB))
      {
        MoveBoundingBox(&orect, 0, 2*MBSH);
        if (GlobalCollision(&orect, GPlayerObjColB, GPlayerMapColB))
        {
//          MoveBoundingBox(&or, 0, -3*MBSH);
//          if (GlobalCollision(&or, GPlayerObjColB, GPlayerMapColB))
//          {
            return false;
//          }
        }
      }
    }
  }
  *r = orect;
  return true;
}


bool GPlayer::TryToMoveDy(rect_t * r)
{
  // try to optimize movement
  rect_t orect = *r;
  int orx = orect.left%MBSW;
  if (orx < MBSW/2)
  {
    MoveBoundingBox(&orect, -orx, 0);
    if (GlobalCollision(&orect, GPlayerObjColB, GPlayerMapColB))
    {
      MoveBoundingBox(&orect, MBSW, 0);
      if (GlobalCollision(&orect, GPlayerObjColB, GPlayerMapColB))
      {
        MoveBoundingBox(&orect, -2*MBSW, 0);
        if (GlobalCollision(&orect, GPlayerObjColB, GPlayerMapColB))
        {
//          MoveBoundingBox(&or, 3*MBSW, 0);
//          if (GlobalCollision(&or, GPlayerObjColB, GPlayerMapColB))
//          {
            return false;
//          }
        }
      }
    }
  }
  else
  {
    MoveBoundingBox(&orect, MBSW-orx, 0);
    if (GlobalCollision(&orect, GPlayerObjColB, GPlayerMapColB))
    {
      MoveBoundingBox(&orect, -MBSW, 0);
      if (GlobalCollision(&orect, GPlayerObjColB, GPlayerMapColB))
      {
        MoveBoundingBox(&orect, 2*MBSW, 0);
        if (GlobalCollision(&orect, GPlayerObjColB, GPlayerMapColB))
        {
//          MoveBoundingBox(&or, -3*MBSW, 0);
//          if (GlobalCollision(&or, GPlayerObjColB, GPlayerMapColB))
//          {
            return false;
//          }
        }
      }
    }
  }
  *r = orect;
  return true;
}

void GPlayer::ServerThink(Uint32 time)
{
  GEntity::ServerThink(time);
  if (brain_type!=bt_soulfree) ping = client[brain_owner].ping;
  if (anim>=2 && skin->anims[anim]->frames-1==frame) 
  { 
    Born();
    return; 
  }
  if (anim>=2) return;  // player is going to die
  
  // do test collissions, hurting players, random actions, and so on
  rect_t r1;
  GetBoundingBox(&r1);
  xtime = time;
  do_map_collision = true;
  me = this;
  witemsx = witems;
  ColObj = NULL;
  GlobalCollision(&r1, GPlayerObjColB, GPlayerMapColB);  // killed by map ?

  rect_t mr;
  GetMapRect(&mr);
  for (int my=mr.top; my<=mr.bottom; my++)
  {
    for (int mx=mr.left; mx<=mr.right; mx++)
    {
      BLKSTR * b = game->map.GetBlock(mx, my);
      if (b)
      {
        if (b->tr2) // killing tile
        {
          Kill();
          //SV_ClientVoice(brain_owner, SM_UPSSS, 100, 11);
          DelFrag();
          return;
        }
      }
    }
  }

  if (rail_scheduler==time)
  {
    if (weapon==2) FireRailgun();
    rail_scheduler = 0;
  }

  if (time==warpout_end)
  {
    Warp();
    warpin_end = time + warpin_time;
  }
}


void GPlayer::AdjustPosition(Uint16 nx, Uint16 ny, TICK_TYPE time)
{
  // called from client side
  if (dbg_prediction.value) ConOut("adjust position - stime=%d spos=[%d,%d] (ctime=%d)", time, nx, ny, curtime);
  SetPos(nx, ny);
  Uint8 sh = predictor.head;
  predictor.head = predictor.Find(time);
  while (predictor.head != sh) 
  {
    MoveAutonomous(&predictor.p[predictor.head], predictor.p[predictor.head].tick);
    if (dbg_prediction.value) ConOut("           step - ctime=%d cpos=[%d,%d]", predictor.p[predictor.head].tick, *xpos.GetValRef(), *ypos.GetValRef(), time, curtime);
    predictor.FwHead();
  }
}

bool GPlayer::Warp()
{
  int wpx=200, wpy=100;
  // ask script, where should this pacman born
  ScriptMan->scripts[map_script_num].VRunScript("ssGeneratePos", (int)PLAYER_WARPPOS, &wpx, &wpy);
  SetPos(wpx, wpy); 
  last_warp_time = curtime;
  CloneSound(SM_WARP, 0, 0);
  return true;
}

void GPlayer::Born()
{
  int bpx=200, bpy=100;
  // ask script, where should this pacman born
  ScriptMan->scripts[map_script_num].VRunScript("ssGeneratePos", (int)PLAYER_BORNPOS, &bpx, &bpy);
  SetPos(bpx, bpy); 
  
//  game->IncrementLOID();
//  oid = game->last_oid;
  SetAnim(0);
  SetFrame(0);
  xsize.Set(32);
  ysize.Set(32);
  direction=0;

  weapon = player_sa_weapon;

  last_pistolka_time = 0;
  last_becheromet_time = 0;
  last_railgun_time = 0;
  last_bomb_time = 0;
  last_grenade_time = 0;
  last_warp_time = 0;

  pistolka_ammo = player_sa_pistolka;
  becheromet_ammo = player_sa_becheromet;
  railgun_ammo = player_sa_railgun;
  bombs_ammo = player_sa_bombs;
  grenades_ammo = player_sa_grenades;
  witems = player_sa_witems;
  warps = player_sa_warps;
  shields = player_sa_shields;
  power = player_sa_power;
  special = 0;

  rail_scheduler = 0;
  
  fire_anim_end = 0;
  speed_end = 0;  
  slow_end = 0;  
  shield_end = 0;  
  glue_end = 0;  
  infra_end = 0;  
  reverse_end = 0;  
  noweapon_end = 0;  
  invis_end = 0;
  bornin_end = curtime + bornin_time;
  warpout_end = 0;
  berserk_end = 0;

  killindex = 0;
  killlimit_time = 0;
  banzaiindex = 0;
  banzailimit_time = 0;

  CloneSound(SM_BORN, 0, 0);
}

void GPlayer::FirePistolka()
{
  game->IncrementLOID();
  GAME_MAXOBJS_TYPE shot_slot = game->FindUnusedSlot();
  game->RebornObject(game->last_oid, shot_slot, ot_shot);
  
  GShot * s = (GShot*)game->objs[shot_slot];
  
  if (anim%2==0)
  {
    s->dx.Set(shot_step);
    s->SetPos(xpos-shot_step+player_pistolka_shiftx, ypos+player_pistolka_shifty);
    CloneSound(SM_PISTOLKAF1, sound_shiftx, 0);
  }
  else
  {
    s->dx.Set(-shot_step);
    s->SetPos(xpos+shot_step-player_pistolka_shiftx, ypos+player_pistolka_shifty);
    CloneSound(SM_PISTOLKAF1, -sound_shiftx, 0);
  }
  s->owner.Set(slot);
  s->createtime.Set(curtime);

  CloneSound(SM_PISTOLKAF1, 0, 0);
  pistolka_ammo--;
  Banzai();
}

void GPlayer::FireBecheromet()
{
  game->IncrementLOID();
  GAME_MAXOBJS_TYPE bshot_slot = game->FindUnusedSlot();
  game->RebornObject(game->last_oid, bshot_slot, ot_bshot);
  
  GBShot * s = (GBShot*)game->objs[bshot_slot];
  
  if (anim%2==0)
  {
    s->dx.Set(bshot_step);
    s->SetPos(xpos-bshot_step+player_becheromet_shiftx, ypos+player_becheromet_shifty);
    CloneSound(SM_BECHERF1, sound_shiftx, 0);
  }
  else
  {
    s->dx.Set(-bshot_step);
    s->SetPos(xpos+bshot_step-player_becheromet_shiftx, ypos+player_becheromet_shifty);
    CloneSound(SM_BECHERF1, -sound_shiftx, 0);
  }
  s->owner.Set(slot);
  s->createtime.Set(curtime);

  becheromet_ammo--;
  Banzai();
}

void GPlayer::FireBomb()
{
  Sint16 x = xpos - xpos%16;
  Sint16 y = ypos - ypos%16;
  NormalizePos(&x, &y);
  
  rect_t r = { x - 16, y - 16, x + 16, y + 16};
  if (GlobalCollision(&r))
  {
    // TODO: sound
    return;
  }

  game->IncrementLOID();
  GAME_MAXOBJS_TYPE bomb_slot = game->FindUnusedSlot();
  game->RebornObject(game->last_oid, bomb_slot, ot_bomb);
  
  GBomb * b = (GBomb*)game->objs[bomb_slot];
  
  b->owner.Set(slot);
  b->SetPos(x,y);
  b->createtime.Set(curtime);
  b->power = power;
  b->explode_time = curtime+bomb_timeout;
  b->movementx = anim%2?-bomb_mv_speed:bomb_mv_speed;

  CloneSound(SM_BOMBPUT, 0, 0);
  bombs_ammo--;
  Banzai();
}

void GPlayer::FireRailgun()
{
  game->IncrementLOID();
  GAME_MAXOBJS_TYPE rail_slot = game->FindUnusedSlot();
  game->RebornObject(game->last_oid, rail_slot, ot_rail);
  
  GRail * r = (GRail*)game->objs[rail_slot];
  
  r->owner.Set(slot);
  r->createtime.Set(curtime);
  if (anim%2==0) 
  {
    r->SetPos(xpos+player_rail_shiftx, ypos+player_rail_shifty);
    r->ExpandRail(1);
    CloneSound(SM_RAIL_F1, sound_shiftx, 0);
  }
  else 
  {
    r->SetPos(xpos-player_rail_shiftx, ypos+player_rail_shifty);
    r->ExpandRail(3);
    CloneSound(SM_RAIL_F1, -sound_shiftx, 0);
  }

  railgun_ammo--;
  Banzai();
}

void GPlayer::FireGrenade()
{
  game->IncrementLOID();
  GAME_MAXOBJS_TYPE grenade_slot = game->FindUnusedSlot();
  game->RebornObject(game->last_oid, grenade_slot, ot_grenade);
  
  GGrenade * r = (GGrenade*)game->objs[grenade_slot];
  
  r->owner.Set(slot);
  r->createtime.Set(curtime);
  if (anim%2==0) 
  {
    r->SetPos(xpos+player_grenade_shiftx, ypos+player_grenade_shifty);
    r->movementx = player_grenade_movement;
    CloneSound(SM_GRANATF1, sound_shiftx, 0);
  }
  else 
  {
    r->SetPos(xpos-player_grenade_shiftx, ypos+player_grenade_shifty);
    r->movementx = -player_grenade_movement;
    CloneSound(SM_GRANATF1, -sound_shiftx, 0);
  }

  grenades_ammo--;
  Banzai();
}

bool GPlayer::Kill()
{
  if (anim<2 && curtime>=warpin_end && curtime>=warpout_end && curtime>=bornin_end)
  {
    if (direction==0)
      SetAnim(2);
    else
      SetAnim(3);

    if (curtime<invis_end) invis_end = 0;

    CloneSound(SM_DEATH1+rand()%5, 0, 0);
    return true;
  }
  else
    return false;
}

void GPlayer::Animate()
{
  if (anim>=2 && frame==skin->anims[anim]->frames-1) return;
  if (framedelay) 
  {
    framedelay--;
    framedelay.MakeDirty(-1, false);
  }
  else
  {
    frame++;
    if (frame>=skin->anims[anim]->frames) frame = 0;
    frame.MakeDirty(-1, false);
    
    framedelay = skin->anims[anim]->delays[frame];
    framedelay.MakeDirty(-1, false);
  }
}

bool GPlayer::SetAnim(Uint8 na)
{
  if (na>=skin->num_anims) return false;
  
  anim = na;
  return SetFrame(0);
}

bool GPlayer::SetFrame(Uint8 nf)
{
  if (nf>=skin->anims[anim]->frames) return false;
  
  frame = nf;
  framedelay = skin->anims[anim]->delays[frame];
  return true;
}


void GPlayer::DelFrag()  
{ 
  if (frags>-100) frags--; 
  DelPoints(points_mfrag);
  
  SV_ClientVoice(brain_owner, SM_LAMER+rand()%6, 100, 5);
}

void GPlayer::Banzai()  
{ 
  if (curtime<banzailimit_time)
  {
    banzaiindex++;
    switch (banzaiindex) {
    case 8: 
      CloneSound(SM_BANZAI, 0, 0);
      banzaiindex = 0;
      break;
    }
  }
  else
  {
    banzaiindex = 0;
  }
  banzailimit_time = curtime + banzaiing_time;
}

void GPlayer::AddFrag() { 
  if (frags<100) frags++; 
  AddPoints(points_frag);
  SV_ClientVoice(brain_owner, SM_ACCURACY1+rand()%6, 100, 10);
  if (curtime<killlimit_time)
  {
    killindex++;
    switch (killindex) {
    case 3: 
      SV_BroadcastPrintf("%s leads", player_name.GetValRef()->chars);
      SV_BroadcastVoice(SM_LEADER1 ,100, 20);
      break;
    case 6: 
      SV_BroadcastPrintf("%s is dominating", player_name.GetValRef()->chars);
      SV_BroadcastVoice(SM_DOMINATION1 ,100, 30);
      break;
    case 9: 
      SV_BroadcastPrintf("%s is a killing machine", player_name.GetValRef()->chars);
      SV_BroadcastVoice(SM_KMACHINE1 ,100, 40);
      break;
    }
  }
  else
  {
    killindex = 0;
  }
  killlimit_time = curtime + killing_time;
}
