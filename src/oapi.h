/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   Object API
//## author:  Antonin Hildebrand
//## 
//## started: 25.6.2000
//## revised: ???
//## 
//##          safe object api for import to scripts
//## 
//###########################################################################

#ifndef _OAPI_H_
#define _OAPI_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL.h"
#include "const.h"
#include "net.h"

#include "../bin/scripts/scons.h"
#include "../bin/scripts/sdata.h"


//###########################################################################
//## scripting externals - do not export to saapi.h
//###########################################################################

void AddOapiExternals();

//###########################################################################
//## SERVER SIDE
//###########################################################################

int soBornPlayer(SCPLAYER& player);
int soGetPlayer(int slot, SCPLAYER& player);
int soSetPlayer(int slot, SCPLAYER& player);

int soBornShot(SCSHOT& shot);
int soGetShot(int slot, SCSHOT& shot);
int soSetShot(int slot, SCSHOT& shot);

int soBornExtra(SCEXTRA& extra);
int soGetExtra(int slot, SCEXTRA& extra);
int soSetExtra(int slot, SCEXTRA& extra);

int soBornBomb(SCBOMB& bomb);
int soGetBomb(int slot, SCBOMB& bomb);
int soSetBomb(int slot, SCBOMB& bomb);


//###########################################################################
//## CLIENT SIDE
//###########################################################################

int coBornPlayer(SCPLAYER& player);
int coGetPlayer(int slot, SCPLAYER& player);
int coSetPlayer(int slot, SCPLAYER& player);

int coBornShot(SCSHOT& shot);
int coGetShot(int slot, SCSHOT& shot);
int coSetShot(int slot, SCSHOT& shot);

int coBornExtra(SCEXTRA& extra);
int coGetExtra(int slot, SCEXTRA& extra);
int coSetExtra(int slot, SCEXTRA& extra);

int coBornBomb(SCBOMB& bomb);
int coGetBomb(int slot, SCBOMB& bomb);
int coSetBomb(int slot, SCBOMB& bomb);

#endif
