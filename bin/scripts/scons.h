/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   Scripting API - constants
//## author:  Antonin Hildebrand
//## 
//## started: 12.3.2000
//## revised: ???
//## 
//##          used to include in scripts & main program
//## 
//###########################################################################

#ifndef _SCRIPT_CONS_H_
#define _SCRIPT_CONS_H_

// ONLY CONSTANTS
// MUST BE COMPATIBILE WITH PACSCRIPT

//#define ABS(val) (val)>0?val:-(val)

#define SC_NOERR                         0
#define SC_INDEX_OUT_OF_RANGE            1
#define SC_LAYER_UNDEFINED               2
#define SC_MAPTILE_OUT_OF_RANGE          3
#define SC_TILE_IS_ANIMATED              4
#define SC_TILE_IS_STILL                 5

#define SC_OBJECTTYPEMISMATCH            20
#define SC_OBJECTNOTACTIVE               21

#endif