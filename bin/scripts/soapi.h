/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   Scripting Object API
//## author:  Antonin Hildebrand
//## 
//## started: 25.6.2000
//## revised: ???
//## 
//##          exports oapi.h to scripts
//## 
//###########################################################################

#ifndef _SCRIPT_OAPI_H_
#define _SCRIPT_OAPI_H_

// (IMPORTS/EXPORTS)
// MUST BE COMPATIBILE WITH PACSCRIPT

//###########################################################################
//## SERVER SIDE
//###########################################################################

import int soBornPlayer(SCPLAYER& player);
import int soGetPlayer(int slot, SCPLAYER& player);
import int soSetPlayer(int slot, SCPLAYER& player);

import int soBornShot(SCSHOT& shot);
import int soGetShot(int slot, SCSHOT& shot);
import int soSetShot(int slot, SCSHOT& shot);

import int soBornExtra(SCEXTRA& extra);
import int soGetExtra(int slot, SCEXTRA& extra);
import int soSetExtra(int slot, SCEXTRA& extra);

import int soBornBomb(SCBOMB& bomb);
import int soGetBomb(int slot, SCBOMB& bomb);
import int soSetBomb(int slot, SCBOMB& bomb);


//###########################################################################
//## CLIENT SIDE
//###########################################################################

import int coBornPlayer(SCPLAYER& player);
import int coGetPlayer(int slot, SCPLAYER& player);
import int coSetPlayer(int slot, SCPLAYER& player);

import int coBornShot(SCSHOT& shot);
import int coGetShot(int slot, SCSHOT& shot);
import int coSetShot(int slot, SCSHOT& shot);

import int coBornExtra(SCEXTRA& extra);
import int coGetExtra(int slot, SCEXTRA& extra);
import int coSetExtra(int slot, SCEXTRA& extra);

import int coBornBomb(SCBOMB& bomb);
import int coGetBomb(int slot, SCBOMB& bomb);
import int coSetBomb(int slot, SCBOMB& bomb);

#endif