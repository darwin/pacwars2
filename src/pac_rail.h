/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   pac rail
//## author:  Antonin Hildebrand [hildi@atlas.cz]
//##
//## started: 6.11.2000
//## revised: ???
//## 
//###########################################################################

#ifndef Rail
#define Rail

#include "const.h"
#include "replication.h"
#include "prediction.h"
#include "net.h"
#include "protocol.h"
#include "pac_obj.h"
#include "pac_entity.h"

class GRail : public GEntity {
public:
    GRail(Uint16 id, CGame* igame, GAME_MAXOBJS_TYPE islot);
    virtual ~GRail();
    
    virtual Uint8 GetType();
    virtual void Replication(int cnum, CReplicator* rep);
    virtual char GetReplicated(Uint8 id, net_msg* msg, TICK_TYPE time);
    virtual void MakeDirty(int i, bool dirty);

    virtual void Draw(SDL_Surface * s, SDL_Rect * r);

    virtual void ServerThink(Uint32 time);
    virtual void ClientThink(Uint32 time);

    virtual void ExpandRail(Uint8 direction);
 
    //###########################################################################
    // non-synchronized data
    //###########################################################################
    
    TICK_TYPE timestamp;

    //###########################################################################
    // synchronized data
    //###########################################################################
    
    RValue<GAME_MAXOBJS_TYPE> owner;
};

#endif
















































































