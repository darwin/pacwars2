/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   Map API
//## author:  Antonin Hildebrand
//##
//## started: 12.3.2000
//## revised: ???
//## 
//##          safe map api for import to scripts
//##
//###########################################################################

#ifndef _MAPI_H_
#define _MAPI_H_

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include "SDL.h"
#include "const.h"
#include "map.h"
#include "net.h"

#include "../bin/scripts/scons.h"
#include "../bin/scripts/sdata.h"

//###########################################################################
//## scripting externals - do not export to smapi.h
//###########################################################################

void AddMapiExternals();

//###########################################################################
//## script map functions
//###########################################################################

//###########################################################################
//## SERVER SIDE
//###########################################################################

int smGetBlockStr(int strnum, SCBLK& block);
int smSetBlockStr(int strnum, SCBLK& block);

int smGetAnimStr(int strnum, SCANI& anim);
int smSetAnimStr(int strnum, SCANI& anim);

int smSetMapLayer(int laynum);
int smGetMapVal(int x, int y, int& val);
int smGetMapBlock(int x, int y, SCBLK& block);
int smGetMapAnim(int x, int y, SCANI& anim);
int smTrsMapBlockVal(int x, int y, int& val); // translate [x,y] -> block_val
int smTrsMapBlock(int x, int y, SCBLK& block);// translate [x,y] -> block
int smSetMapVal(int x, int y, int val);

int smGetMapWidth(int& w);
int smGetMapHeight(int& h);

//###########################################################################
//## CLIENT SIDE
//###########################################################################

int cmGetBlockStr(int strnum, SCBLK& block);
int cmSetBlockStr(int strnum, SCBLK& block);

int cmGetAnimStr(int strnum, SCANI& anim);
int cmSetAnimStr(int strnum, SCANI& anim);

int cmSetMapLayer(int laynum);
int cmGetMapVal(int x, int y, int& val);
int cmGetMapBlock(int x, int y, SCBLK& block);
int cmGetMapAnim(int x, int y, SCANI& anim);
int cmTrsMapBlockVal(int x, int y, int& val); // translate [x,y] -> block_val
int cmTrsMapBlock(int x, int y, SCBLK& block);// translate [x,y] -> block
int cmSetMapVal(int x, int y, int val);

int cmGetMapWidth(int& w);
int cmGetMapHeight(int& h);


#endif
