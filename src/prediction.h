/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   move vector & prediction
//## author:  Antonin Hildebrand
//##         
//## started: 14.6.2000
//## revised: ???
//## 
//## 
//###########################################################################

#ifndef _PACPREDICTION_H_
#define _PACPREDICTION_H_

#include "const.h"
#include "net.h"

//###########################################################################
//## Data structures
//###########################################################################

// movevector
// is temporary key state 

// in future it will be pac movement vector
// computed from physical forces & player input

typedef struct _MoveVector {
    TICK_TYPE tick;
  
    Uint8 UP;
    Uint8 DOWN;
    Uint8 LEFT;
    Uint8 RIGHT;
    Uint8 SHIELD;
    Uint8 WARP;
    Uint8 FIRE;
    Uint8 WEAPON[MAX_WEAPONS];
} MoveVector;


class DECLSPEC CPredictor {
public:
  MoveVector p[PREDICTION_BUF_LENGTH] ;  // ring
  Uint8 head;
  
  CPredictor();
  ~CPredictor();

  void FwHead() { head++; if (head>=PREDICTION_BUF_LENGTH) head=0; }
  void BwHead() { if (head==0) head=PREDICTION_BUF_LENGTH-1; else head--; }
  void Add(MoveVector *mv);
  Uint8 Find(TICK_TYPE ftick); // najde nejblizsi vetsi
  void Reset();

};

#endif