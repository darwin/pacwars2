/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   base game object
//## author:  Antonin Hildebrand [hildi@atlas.cz]
//##          based on Flemming Frandsen's code of NiL game [dion@swamp.dk]           
//##
//## started: 7.6.2000
//## revised: ???
//## 
//###########################################################################

#ifndef PAC_OBJ_H
#define PAC_OBJ_H

#include "const.h"
#include "replication.h"
#include "net.h"

class CGame;

// All objects must inherit from this class
// An object in the system

// this is implicit dead object inited in all slots when Game structure is created
class GObj {
public:
    GObj(Uint16 id, CGame* igame, GAME_MAXOBJS_TYPE islot);
    virtual ~GObj();
    
    //###########################################################################
    // Start of MANDATORY functions
    // These fuctions MUST be implemented by ALL clild objects
    //###########################################################################
    
    // Must return the type of the concrete class
    virtual Uint8 GetType() { return ot_obj; };
    
    // Does the thinking needed for the object on the serverside.
    // Thinking on the server means checking for collisions,
    // becoming an explosion and hurting players in range of explosions.
    // Calls client_think() for updating of animation the object and applying physics
    virtual void ServerThink(Uint32 time) {};
    
    virtual bool SetImplicitSprite() { return true;};


   // Draws the object into the viewport
    // This routine should handle clipping on its own.
    virtual void Draw(SDL_Surface * s, SDL_Rect * r) {};
    
    virtual void Replication(int cnum, CReplicator* rep);
    virtual char GetReplicated(Uint8 id, net_msg* msg, TICK_TYPE time);
    virtual void MakeDirty(int i, bool dirty);

    //###########################################################################
    // End of MANDATORY functions
    //###########################################################################
    
    // called to read or write the state of the object
//    virtual void serialize(Serializer *serializer);
    
    // Does the thinking needed for the object on the client side (that is completely non-random)
    // Thinking means updating the physics and other deterministic things, such as animations.
    // MUST NOT alter other objects, this must be handled by server_think()
    virtual void ClientThink(Uint32 time) {};
    
   // return true if this object can be hurt, if it can it should also implement hurt()
    // Do you really want to hurt me?
//    virtual bool hurtable() {return false;}
    
    // called when something hurts the object,
    // the default is to die if it is hurt at all, this is not very practical for most objects:)
//    virtual void hurt(float hitpoints, Vector speed, int hurter) {if (hitpoints>0) done=true;}
    
    // Used for collisiondetection, alter it if you need to have another shape than a point.
    virtual bool IsPassable(Uint16 px, Uint16 py) {return true;}
    
    // Be done, the object should make peace with it's maker and set the done flag...
//    virtual void bedone() {done = true;}
    
    // Returns true once the object wants to die
    // In the server this means that the object must be delete'ed
    // and that the clients should be told to do the same.
//    virtual bool isdone() {return done;}
    
    //Misc access functions:
    
    // The objectid uniquely indentifies all objects in the system,
    // pointers should only be used inside a function as they cannot
    // cross from the server to the client as the id's can
    // and pointers have a way of leading to madness once the target object
    // dies (something that happens quite often in this program)
    int GetSlot() {return slot;};
    
    //An easy way to get the name of an object
    const char *GetName() {return OBJECT_NAMES[GetType()];}

    // Net Role
    ENetRole role;
    
    // the game we are in
    CGame * game;

    // id of this object, assigned by the server at creation
    GAME_MAXOBJS_TYPE slot;	
   
    //the time this data was last updated
    TICK_TYPE curtime;
    
    Uint16 oid;

    bool was_relevant;  // was relevant last tick

    //###########################################################################
    // syncronized data
    //###########################################################################

    RValue<Uint8> state;

    // the time the object was created
    RValue<TICK_TYPE> createtime;
};

#endif
















































































