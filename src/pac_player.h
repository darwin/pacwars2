/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   game player object
//## author:  Antonin Hildebrand [hildi@atlas.cz]
//##
//## started: 9.6.2000
//## revised: ???
//## 
//###########################################################################

#ifndef PAC_PLAYER_H
#define PAC_PLAYER_H

#include "const.h"
#include "replication.h"
#include "prediction.h"
#include "net.h"
#include "protocol.h"
#include "pac_obj.h"
#include "pac_entity.h"

typedef struct keybindings_s
{
    bool used;
    Uint16 key_up;
    Uint16 key_down;
    Uint16 key_left;
    Uint16 key_right;
    Uint16 key_fire;
    Uint16 key_shield;
    Uint16 key_warp;
    Uint16 key_weapon[MAX_WEAPONS];
} keybindings_t;

#define WI_PISTOLKA   1
#define WI_BECHEROMET 2
#define WI_RAILGUN    4
#define WI_BOMBS      8
#define WI_GRENADES   16
#define SI_PUNCH      32


extern DECLSPEC int blocked_inputs;

class GPlayer : public GEntity {
public:
    GPlayer(Uint16 id, CGame* igame, GAME_MAXOBJS_TYPE islot);
    virtual ~GPlayer();
    
    virtual Uint8 GetType();
    virtual void ServerThink(Uint32 time);
    virtual void Replication(int cnum, CReplicator* rep);
    virtual char GetReplicated(Uint8 id, net_msg* msg, TICK_TYPE time);
    virtual void MakeDirty(int i, bool dirty);
    void Draw(SDL_Surface * screen, SDL_Rect * screen_rect);

    virtual void ClientThink(Uint32 time);

    virtual void MoveAutonomous(MoveVector* mv, TICK_TYPE time);
    MoveVector DecodeMV(net_msg* msg, TICK_TYPE time);

    virtual void Animate();
    virtual void ServerMove(MoveVector* mv);
    
    virtual void AdjustPosition(Uint16 nx, Uint16 ny, TICK_TYPE time);

    virtual void Born();
    virtual void AddPoints(int n) { points+=n; if (points>9999) points = 9999; }
    virtual void DelPoints(int n) { points-=n; if (points<-999) points = -999; }
    virtual void AddFrag();
    virtual void DelFrag(); 
    virtual bool Kill();
    virtual bool Warp();

    virtual void FirePistolka();
    virtual void FireBomb();
    virtual void FireGrenade();
    virtual void FireRailgun();
    virtual void FireBecheromet();

    virtual bool SetFrame(Uint8 nf);
    virtual bool SetAnim(Uint8 na);

    void GPlayer::Banzai();


    bool TryToMoveDx(rect_t * r);
    bool TryToMoveDy(rect_t * r);

    // accessing skin manager
    virtual bool SetSkin(char* ns);

    //###########################################################################
    // non-synchronized data
    //###########################################################################
    
    CPredictor predictor;

    Uint8 direction;  // 0=top, 1=right, 2=bottom, 3=left
    
    Uint16 last_pistolka_time;
    Uint16 last_becheromet_time;
    Uint16 last_bomb_time;
    Uint16 last_railgun_time;
    Uint16 last_grenade_time;
    Uint16 last_warp_time;

    Uint16 adjusttime;
    
    TICK_TYPE rail_scheduler;  // time when rail should fire, else 0

    CSpriteInfo* skin;
    
    CSpriteInfo* pistolka;
    CSpriteInfo* xbomb;
    CSpriteInfo* railgun;
    CSpriteInfo* grenade;
    CSpriteInfo* becheromet;

    CSpriteInfo* addons;

    int killindex;
    Uint16 killlimit_time;

    int banzaiindex;
    Uint16 banzailimit_time;

    TICK_TYPE last_x_tick;
    TICK_TYPE last_y_tick;

    //###########################################################################
    // synchronized data
    //###########################################################################
    RValue<PString> player_name;

    RValue<Uint8> brain_type;    // soulfree, client, bot,
    RValue<Uint8> brain_owner;   // 0,.., maxclients-1, maxclients==owner_is_server, 250 = undefined
    RValue<Uint8> kbindex;       // index to keybindigns array

    RValue<Sint8> frags;
    RValue<Sint16> points;
    
    RValue<Uint8> bombs_ammo;
    RValue<Uint8> pistolka_ammo;
    RValue<Uint8> becheromet_ammo;
    RValue<Uint8> railgun_ammo;
    RValue<Uint8> grenades_ammo;

    RValue<Uint8> special;
    RValue<Uint8> weapon;

    RValue<Uint8> witems;  // picked weapons
    RValue<Uint8> shields;  // picked specials
    RValue<Uint8> warps;  // picked specials
    
    RValue<Uint8> power;

    RValue<Uint16> ping;

    RValue<PString> skin_name;

    RValue<TICK_TYPE> fire_anim_end;  // time when rail should fire, else 0
    RValue<TICK_TYPE> speed_end;  
    RValue<TICK_TYPE> slow_end;  
    RValue<TICK_TYPE> shield_end;  
    RValue<TICK_TYPE> glue_end;  
    RValue<TICK_TYPE> infra_end;  
    RValue<TICK_TYPE> reverse_end;  
    RValue<TICK_TYPE> noweapon_end;  
    RValue<TICK_TYPE> invis_end;  
    RValue<TICK_TYPE> berserk_end;  

    RValue<TICK_TYPE> warpin_end;  
    RValue<TICK_TYPE> bornin_end;  
    RValue<TICK_TYPE> warpout_end;  
};

#endif
















































































