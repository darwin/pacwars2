/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   Scripting Map API
//## author:  Antonin Hildebrand
//## 
//## started: 12.3.2000
//## revised: ???
//## 
//##          exports mapi.h to scripts
//## 
//###########################################################################

#ifndef _SCRIPT_MAPI_H_
#define _SCRIPT_MAPI_H_

// (IMPORTS/EXPORTS)
// MUST BE COMPATIBILE WITH PACSCRIPT

//###########################################################################
//## script map functions
//###########################################################################

#ifndef _MAX_PATH
  #define _MAX_PATH 260
#endif

//###########################################################################
//## SERVER SIDE
//###########################################################################

import int smGetBlockStr(int strnum, SCBLK& block);
import int smSetBlockStr(int strnum, SCBLK& block);

import int smGetAnimStr(int strnum, SCANI& anim);
import int smSetAnimStr(int strnum, SCANI& anim);

import int smSetMapLayer(int laynum);
import int smGetMapVal(int x, int y, int& val);
import int smGetMapBlock(int x, int y, SCBLK& block);
import int smGetMapAnim(int x, int y, SCANI& anim);
import int smTrsMapBlockVal(int x, int y, int& val); // translate [x,y] -> block_val
import int smTrsMapBlock(int x, int y, SCBLK& block);// translate [x,y] -> block
import int smSetMapVal(int x, int y, int val);

import int smGetMapWidth(int& w);
import int smGetMapHeight(int& h);

//###########################################################################
//## CLIENT SIDE
//###########################################################################

import int cmGetBlockStr(int strnum, SCBLK& block);
import int cmSetBlockStr(int strnum, SCBLK& block);

import int cmGetAnimStr(int strnum, SCANI& anim);
import int cmSetAnimStr(int strnum, SCANI& anim);

import int cmSetMapLayer(int laynum);
import int cmGetMapVal(int x, int y, int& val);
import int cmGetMapBlock(int x, int y, SCBLK& block);
import int cmGetMapAnim(int x, int y, SCANI& anim);
import int cmTrsMapBlockVal(int x, int y, int& val); // translate [x,y] -> block_val
import int cmTrsMapBlock(int x, int y, SCBLK& block);// translate [x,y] -> block
import int cmSetMapVal(int x, int y, int val);

import int cmGetMapWidth(int& w);
import int cmGetMapHeight(int& h);

//###########################################################################
//## BOTH SIDE - shared data
//###########################################################################

import char MapLoaded;
import char MapName[_MAX_PATH];

import int MSCRW;
import int MSCRH;

import int MMOX;
import int MMOY;

import int maperror;		// Set to a MER_ error if something wrong happens 
import int mapwidth;
import int mapheight;
import int mappxwidth;
import int mappxheight;
import int mapblockwidth;
import int mapblockheight;
import int mapdepth;
import int mapblockstrsize;
import int mapnumblockstr;
import int mapnumanimstr;
import int mapnumblockgfx;

#endif
