/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   pac fire ray
//## author:  Antonin Hildebrand [hildi@atlas.cz]
//##
//## started: 16.11.2000
//## revised: ???
//## 
//###########################################################################

#ifndef __FRAY__
#define __FRAY__

#include "const.h"
#include "replication.h"
#include "prediction.h"
#include "net.h"
#include "protocol.h"
#include "pac_obj.h"
#include "pac_entity.h"

class GFray : public GEntity {
public:
    GFray(Uint16 id, CGame* igame, GAME_MAXOBJS_TYPE islot);
    virtual ~GFray();
    
    virtual Uint8 GetType();
    virtual void Replication(int cnum, CReplicator* rep);
    virtual char GetReplicated(Uint8 id, net_msg* msg, TICK_TYPE time);
    virtual void MakeDirty(int i, bool dirty);

    virtual void Draw(SDL_Surface * s, SDL_Rect * r);

    virtual void ServerThink(Uint32 time);
    virtual void ClientThink(Uint32 time);

    virtual void ExpandFrayX(int length);
    virtual void ExpandFrayY(int length);
 
    //###########################################################################
    // non-synchronized data
    //###########################################################################
    
    TICK_TYPE timestamp;

    //###########################################################################
    // synchronized data
    //###########################################################################
    
    RValue<Uint8> roundings;  // 0=top, 1=right, 2=bottom, 3=left
    RValue<GAME_MAXOBJS_TYPE> owner;

    RValue<Sint16>  xcenter;
    RValue<Sint16>  ycenter;
};

#endif
















































































