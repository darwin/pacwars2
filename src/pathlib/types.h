/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## types.h [PATHLIB]
//## 
//## desc.:   pathlib types
//## author:  Antonin Hildebrand [hildi@atlas.cz]
//## 
//## started: 12/05/1999
//## revised: 05/06/2000
//## 
//###########################################################################
#ifndef _TYPES_H_
#define _TYPES_H_

// std. includes
#include <stdlib.h>
#include <stdio.h>

#define PE_MONITOR          // monitoring mode

#ifdef __cplusplus
	extern "C" {
#endif

typedef int  peTime;
typedef char peError;

typedef struct _Position
{
  int       X;
  int       Y;
} Position;

typedef struct _PathStep
{
  peTime    Time;           // time when walker should enter tile
  short int Type;           // step type (PST_)
  Position  Pos;            // absolute step coordinates in map
} PathStep;

typedef struct _Walker
{
  // atributes
  int       State;          // walker's state flags
  int       Speed;          // walker's speed
  int       Priority;       // walker's priority

  // position
  Position  Pos;            // actual position
  Position  Dest;           // destination position

  // planned path - valid when state flag PEW_STANDING isn't set
  PathStep* Path;           // planned path steps
  int       Step;           // actual step
  int       Length;         // steps in path

  // linked list
  struct _Walker*  Next;    // next walker in linked list
  struct _Walker*  Prev;    // prev walker in linked list

  // user game data
  void*     GD;             // back reference to the game walker's information
} Walker;


typedef struct _WalkerIO
{
  // atributes
  int       State;          // walker's state flags
  int       Speed;          // step type
  int       Priority;       // absolute step coordinates in map

  // position
  Position  Pos;
  Position  Dest;

  // planned path - valid when state flag PEW_STANDING isn't set
  int       Step;           // actual step
  int       Length;         // steps in path

  // linked list indexes
  int       Next;
  int       Prev;

  // user game data are not saved !!!
  // it is responsibility of caller
} WalkerIO;


typedef struct _MapNode
{
  Position  Pos;            // position
  int       TC;             // terrain cost
  int       Owner;          // tile owner number
  int       FootSteps;      // how many footsteps are crossing this tile

  // temporary data for findpath
  
  // tags
  int       Steps;           
  peTime    Time;            
  int       g;              // the cost of the best path from the start
  int       h;              // an estimate of the cost to the goal
  int       f;              // f+g = total tile cost
  Position  CameFrom;        

  // heap
  int       SearchID;       
  int       HeapPos;        // 0 = is not in heap

} MapNode;

typedef struct _Request
{
  int       Owner;          // index of request sender
  Position  Dest;           // dest position
  char      Type;           // request type (PRT_)
} Request;

#ifdef __cplusplus
	}
#endif

#endif
