/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   Scripting API
//## author:  Antonin Hildebrand
//## 
//## started: 12.3.2000
//## revised: 25.6.2000
//## 
//##          this file is included by each game script
//##          defines interface between main program and scripts
//## 
//###########################################################################

// (IMPORTS/EXPORTS)
// MUST BE COMPATIBILE WITH PACSCRIPT

#define SCRIPT_INTERFACE

#ifndef _SDL_types_h
typedef unsigned char	Uint8;
typedef signed char	Sint8;
typedef unsigned short	Uint16;
typedef signed short	Sint16;
typedef unsigned int	Uint32;
typedef signed int	Sint32;
#endif

#include "scripts/gameplay.h"
#include "scripts/scons.h"
#include "scripts/sdata.h"
#include "scripts/sgeneral.h"
#include "scripts/smapi.h"
#include "scripts/soapi.h"
#include "scripts/svapi.h"
