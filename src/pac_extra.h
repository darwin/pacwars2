/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   pac extras
//## author:  Antonin Hildebrand [hildi@atlas.cz]
//##
//## started: 30.10.2000
//## revised: ???
//## 
//###########################################################################

#ifndef PAC_EXTRA_H
#define PAC_EXTRA_H

#include "const.h"
#include "replication.h"
#include "prediction.h"
#include "net.h"
#include "protocol.h"
#include "pac_obj.h"
#include "pac_entity.h"

class GExtra : public GEntity {
public:
    GExtra(Uint16 id, CGame* igame, GAME_MAXOBJS_TYPE islot);
    virtual ~GExtra();
    
    virtual Uint8 GetType();
    virtual void Replication(int cnum, CReplicator* rep);
    virtual char GetReplicated(Uint8 id, net_msg* msg, TICK_TYPE time);
    virtual void MakeDirty(int i, bool dirty);

    virtual void ServerThink(Uint32 time);
    virtual void ClientThink(Uint32 time);
 
    void ServerCHS(Uint32 time);
    void Draw(SDL_Surface * screen, SDL_Rect * screen_rect);
    bool SetAnim(Uint8 na);
    bool SetFrame(Uint8 nf);
    void Animate();
    int TranslateAnim(Uint8 anim);

    void Take(GPlayer* p, TICK_TYPE time);


    //###########################################################################
    // non-synchronized data
    //###########################################################################
    
    TICK_TYPE timestamp;
    TICK_TYPE lifetime;

    CSpriteInfo * spriteZ; 
    CSpriteInfo * spriteB; 
    CSpriteInfo * spriteN; 
    CSpriteInfo * spriteM; 
    CSpriteInfo * spriteP; 
    CSpriteInfo * spriteA; 

    //###########################################################################
    // synchronized data
    //###########################################################################
    
    RValue<Uint8> type;
    RValue<Uint8> life;
    RValue<Uint8> modifier;       // 0 = instant, 1 = respawn, 2 = random
    RValue<Uint8> data;
};

#endif
















































































