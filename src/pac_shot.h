/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   old pac projectile
//## author:  Antonin Hildebrand [hildi@atlas.cz]
//##
//## started: 23.6.2000
//## revised: ???
//## 
//###########################################################################

#ifndef PAC_SHOT_H
#define PAC_SHOT_H

#include "const.h"
#include "replication.h"
#include "prediction.h"
#include "net.h"
#include "protocol.h"
#include "pac_obj.h"
#include "pac_entity.h"

class GShot : public GEntity {
public:
    GShot(Uint16 id, CGame* igame, GAME_MAXOBJS_TYPE islot);
    virtual ~GShot();
    
    virtual Uint8 GetType();
    virtual void Replication(int cnum, CReplicator* rep);
    virtual char GetReplicated(Uint8 id, net_msg* msg, TICK_TYPE time);
    virtual void MakeDirty(int i, bool dirty);

    virtual void ServerThink(Uint32 time);
    virtual void ClientThink(Uint32 time);

    virtual void Animate();
            void Bounce();
        
    //###########################################################################
    // non-synchronized data
    //###########################################################################
    
    bool bounced;

    //###########################################################################
    // synchronized data
    //###########################################################################
    
    RValue<Sint8> dx;

    RValue<GAME_MAXOBJS_TYPE> owner;
};

#endif
















































































