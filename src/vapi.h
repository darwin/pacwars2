/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   game Vars API
//## author:  Antonin Hildebrand
//## 
//## started: 25.6.2000
//## revised: ???
//## 
//##          safe vars api for import to scripts
//## 
//###########################################################################

#ifndef _VAPI_H_
#define _VAPI_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL.h"
#include "const.h"
#include "net.h"
#include "sampleman.h"

#include "../bin/scripts/scons.h"
#include "../bin/scripts/sdata.h"


//###########################################################################
//## scripting externals - do not export to saapi.h
//###########################################################################

void AddVapiExternals();

//###########################################################################
//## script actor functions
//###########################################################################

//###########################################################################
//## SERVER SIDE
//###########################################################################

int svGetVars(SCVAR& vars);
int svSetVars(SCVAR& vars);

//###########################################################################
//## CLIENT SIDE
//###########################################################################

int cvGetVars(SCVAR& vars);
int cvSetVars(SCVAR& vars);

int cvPlayMusic(char* fname);
int cvPauseMusic();
int cvVolumeMusic(char* _0to64asstring);

#endif
