/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   game entity object
//## author:  Antonin Hildebrand [hildi@atlas.cz]
//##
//## started: 8.6.2000
//## revised: ???
//## 
//###########################################################################

#ifndef PAC_ENTITY_H
#define PAC_ENTITY_H

#include "const.h"
#include "replication.h"
#include "net.h"
#include "pac_obj.h"
#include "sprite.h"
#include "sampleman.h"
#include "gmath.h"
#include "map.h"

typedef bool ObjColCB(GObj* obj);
typedef bool MapColCB(BLKSTR* blk, int mx, int my);

class GEntity : public GObj {
public:
    GEntity(Uint16 id, CGame* igame, GAME_MAXOBJS_TYPE islot);
    virtual ~GEntity();
    
    virtual Uint8 GetType();
    virtual bool IsPassable(Uint16 px, Uint16 py);

    void BlitSurface(SDL_Surface * ssurface, SDL_Rect sr, SDL_Surface *dsurface, SDL_Rect dr, SDL_Rect *r, Uint8 alpha=255);

    virtual void ServerThink(Uint32 time);
    virtual void Draw(SDL_Surface * s, SDL_Rect * r);
    virtual void Draw(SDL_Surface * screen, SDL_Rect * screen_rect, Uint16 x, Uint16 y, Uint16 sx, Uint16 sy, Uint8 frame, Uint8 anim, CSpriteInfo* dsprite = NULL, Uint8 alpha=255);
    virtual void Replication(int cnum, CReplicator* rep, bool replicate_position = true);
    virtual char GetReplicated(Uint8 id, net_msg* msg, TICK_TYPE time, bool replicate_position = true);
    virtual void MakeDirty(int i, bool dirty);

    virtual void Animate();
    virtual bool SetFrame(Uint8 nf);
    virtual bool SetAnim(Uint8 na);
    virtual bool SetSprite(char* ns, CSpriteInfo ** sprite);

    virtual void ClientThink(Uint32 time);

    virtual void SetPos(Uint16 x, Uint16 y) {xpos = x; ypos = y; NormalizePos(); }
    virtual bool DetectCollision(GAME_MAXOBJS_TYPE islot, rect_t * bbox);
    virtual bool GlobalCollision(rect_t * bbox, ObjColCB* ocb=NULL, MapColCB* mcb=NULL);

    inline virtual void GetMapRect(rect_t * r);
    inline virtual void GetBoundingBox(rect_t * r);
    inline virtual void MapRectFromBoundingBox(rect_t * r);
    inline virtual void MoveBoundingBox(rect_t * r, int dx, int dy);
    int DecomposeBoundingBox(rect_t * r, rect_t *q);

    inline virtual void NormalizePos();
    inline virtual void NormalizePos(Sint16* x, Sint16* y);

    void RandomPlace(Uint16 sx, Uint16 sy, rect_t * r);

    virtual void CloneSound(Uint8 id, int dx, int dy, Uint8 volume=SM_STD_VOLUME, TICK_TYPE loop=0);
    int Destination(GEntity* e);


    //###########################################################################
    // non-syncronized data
    //###########################################################################
    CSpriteInfo * sprite;

    //###########################################################################
    // syncronized data
    //###########################################################################
	  // position
	  RValue<Sint16> xpos;
	  RValue<Sint16> ypos;

    // size
    RValue<Uint16> xsize;
	  RValue<Uint16> ysize;

    // frame
    RValue<Uint16> framedelay;
    RValue<Uint8> frame;
    RValue<Uint8> anim;
};

#endif
















































































