/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   Sample manager
//## author:  Antonin Hildebrand
//## 
//## started: 17.11.2000
//## revised: ???
//## 
//###########################################################################

#ifndef _SAMPLEMAN_H_
#define _SAMPLEMAN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL.h"
#include "const.h"
#include "cvar.h"

// sample ids
#define SM_RAIL_F1  0
#define SM_SHOT_F1  1
#define SM_SHOT_R1  2
#define SM_10_0     4
#define SM_BOMBPUT  5
#define SM_BORN     7
#define SM_MESSENGER 8
#define SM_MENU1    9
#define SM_MENU2    10
#define SM_MENU3    11

#define SM_XPLO1    21
#define SM_XPLO2    22

#define SM_BECHERC1  26
#define SM_BECHERA1  27

#define SM_BERSERK  28

#define SM_LEADER1    31
#define SM_DOMINATION1  32
#define SM_KMACHINE1  33

#define SM_UPSSS  36
#define SM_BYEBYE  37
#define SM_OHNO   38
#define SM_BANZAI  39

#define SM_ACCURACY1  40
#define SM_EXCELENT1  41
#define SM_GOTCHA     42
#define SM_IMPRESIVE  43
#define SM_PERFECT1   44
#define SM_PERFECT2   45

#define SM_FIGHT1     47
#define SM_FIGHT2     48

#define SM_LOSTLEAD   50
#define SM_TAKENLEAD  51


#define SM_LAMER    52
#define SM_LOOSER1  53
#define SM_LOOSER2  54
#define SM_SMOKE    55
#define SM_STUPID   56
#define SM_OUTCH    57


#define SM_PREPARE  58

#define SM_RSHOT  59

#define SM_DEATH1    60
#define SM_DEATH2    61
#define SM_DEATH3    62
#define SM_DEATH4    63
#define SM_DEATH5    64

#define SM_GRANATF1  70

#define SM_NARAZ    71
#define SM_BECHERF1  72

#define SM_WARP    73

#define SM_WIN1    75
#define SM_WIN2    76
#define SM_WIN3    77

#define SM_GPREMIE    85
#define SM_GAMMO    86

#define SM_PISTOLKAF1  89


#define SM_END1    90
#define SM_END2    91
#define SM_END3    92
#define SM_END4    93
#define SM_END5    94
#define SM_END6    95
#define SM_END7    96
#define SM_END8    97
#define SM_END9    98
#define SM_END10   99


extern cvar_t	snd_swap;

extern cvar_t	snd_dist;
extern cvar_t	snd_roll;
extern cvar_t	snd_dopp;
extern cvar_t	snd_3don;

void smLoadSamples();
bool smPlaySample3D(Uint8 id, Sint16 x, Sint16 y, Uint8 volume);
bool smPlaySample(Uint8 id, Uint8 volume = SM_STD_VOLUME);
bool smPlayVoice(Uint8 id, Uint8 volume, Uint8 priority=1);
void smSetPosition(Uint16 x, Uint16 y, int direction=-1);
void smFreeSamples();

#endif
