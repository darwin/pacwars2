/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   game sound object
//## author:  Antonin Hildebrand [hildi@atlas.cz]
//##
//## started: 17.11.2000
//## revised: ???
//## 
//###########################################################################

#ifndef PAC_SOUND_H
#define PAC_SOUND_H

#include "const.h"
#include "gmath.h"
#include "replication.h"
#include "net.h"
#include "pac_obj.h"
#include "sprite.h"
#include "sampleman.h"

class GSound : public GObj {
public:
    GSound(Uint16 id, CGame* igame, GAME_MAXOBJS_TYPE islot);
    virtual ~GSound();
    
    virtual Uint8 GetType();

    virtual void ServerThink(Uint32 time);
    virtual void Draw(SDL_Surface * s, SDL_Rect * r);
    virtual void Replication(int cnum, CReplicator* rep);
    virtual char GetReplicated(Uint8 id, net_msg* msg, TICK_TYPE time);
    virtual void MakeDirty(int i, bool dirty);

    virtual void ClientThink(Uint32 time);

    virtual void SetPos(Uint16 x, Uint16 y) {xpos = x; ypos = y; }
    inline virtual void NormalizePos();


    //###########################################################################
    // non-syncronized data
    //###########################################################################

    TICK_TYPE next_restart; // time when sample should be played again

    //###########################################################################
    // syncronized data
    //###########################################################################
	  // position
	  RValue<Sint16> xpos;
	  RValue<Sint16> ypos;

    RValue<Uint8>  sampleid;
    RValue<TICK_TYPE> loop;
    RValue<Uint8>  volume;
};

#endif
















































































