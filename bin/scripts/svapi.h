/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   Scripting Vars API
//## author:  Antonin Hildebrand
//## 
//## started: 25.6.2000
//## revised: ???
//## 
//##          exports vapi.h to scripts
//## 
//###########################################################################

#ifndef _SCRIPT_VAPI_H_
#define _SCRIPT_VAPI_H_

// (IMPORTS/EXPORTS)
// MUST BE COMPATIBILE WITH PACSCRIPT

//###########################################################################
//## script map functions
//###########################################################################

//###########################################################################
//## SERVER SIDE
//###########################################################################

import int svGetVars(SCVAR& vars);
import int svSetVars(SCVAR& vars);

//###########################################################################
//## CLIENT SIDE
//###########################################################################

import int cvGetVars(SCVAR& vars);
import int cvSetVars(SCVAR& vars);

import int cvPlayMusic(char* fname);
import int cvPauseMusic();
import int cvVolumeMusic(char* _0to64asstring);

import bool smPlaySample3D(Uint8 id, Sint16 x, Sint16 y, Uint8 volume);
import bool smPlaySample(Uint8 id, Uint8 volume = SM_STD_VOLUME);
import bool smPlayVoice(Uint8 id, Uint8 volume, Uint8 priority=1);
import void smSetPosition(Uint16 x, Uint16 y, int direction=-1);

#endif