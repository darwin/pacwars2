/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   sprite manager
//## author:  Antonin Hildebrand
//##         
//## started: 9.4.2000
//## revised: ???
//## 
//## 
//###########################################################################

#ifndef _PACPACMAP_H_
#define _PACPACMAP_H_

#include "SDL.h"
#include "SDL_image.h"
#include "const.h"

//###########################################################################
//## Data structures
//###########################################################################

class DECLSPEC CMapInfo {
public:
  char	          name[MAX_MAP_NAME];

  CMapInfo();
  ~CMapInfo();
  
  CMapInfo *next;
};

class DECLSPEC CMapManager {
public:
  CMapInfo *Maps;
  char fdir[_MAX_PATH];
  char fext[MAX_EXT];

  CMapManager();
  ~CMapManager();

  void Destroy();           // frees linked list
  int  Scan(char* dir, char* ext);
  void List(char* t);

  CMapInfo* Find(char* Map_name);
};

//###########################################################################
//## Globals
//###########################################################################

extern DECLSPEC CMapManager MapMan;

//###########################################################################
//## Exported functions
//###########################################################################


#endif
