/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   rapid bomb
//## author:  Antonin Hildebrand [hildi@atlas.cz]
//##
//## started: 30.8.2000
//## revised: ???
//## 
//###########################################################################

#ifndef PAC_BOMB_H
#define PAC_BOMB_H

#include "const.h"
#include "replication.h"
#include "prediction.h"
#include "net.h"
#include "protocol.h"
#include "pac_obj.h"
#include "pac_entity.h"

class GBomb : public GEntity {
public:
    GBomb(Uint16 id, CGame* igame, GAME_MAXOBJS_TYPE islot);
    virtual ~GBomb();
    
    virtual Uint8 GetType();
    virtual void Replication(int cnum, CReplicator* rep);
    virtual char GetReplicated(Uint8 id, net_msg* msg, TICK_TYPE time);
    virtual void MakeDirty(int i, bool dirty);

    virtual void ServerThink(Uint32 time);
    virtual void ClientThink(Uint32 time);
    void Explode();

        
    //###########################################################################
    // non-synchronized data
    //###########################################################################
    
    short int movementx; 
    short int movementy; 
    bool moved;

    Uint8 power;
    TICK_TYPE explode_time;

    //###########################################################################
    // synchronized data
    //###########################################################################
    
    RValue<GAME_MAXOBJS_TYPE> owner;
};

#endif
















































































