/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   map script manager
//## author:  Antonin Hildebrand
//##         
//## started: 24.11.2000
//## revised: ???
//## 
//## 
//###########################################################################

#ifndef _PACPACSMAP_H_
#define _PACPACSMAP_H_

#include "SDL.h"
#include "SDL_image.h"
#include "const.h"

//###########################################################################
//## Data structures
//###########################################################################

class DECLSPEC CSMapInfo {
public:
  char	          name[MAX_SCRIPT_NAME];
  char	          map[MAX_MAP_NAME];
  char	          sname[MAX_SSCRIPT_NAME];
  char	          desc[3][MAX_DESC_NAME];
  char	          author[MAX_AUTHOR_NAME];


  CSMapInfo();
  ~CSMapInfo();
  
  CSMapInfo *next;
};

class DECLSPEC CSMapManager {
public:
  CSMapInfo *Scripts;
  char fdir[_MAX_PATH];
  char fext[MAX_EXT];

  CSMapManager();
  ~CSMapManager();

  void Destroy();           // frees linked list
  int  Scan(char* dir, char* ext);
  void List(char* t);

  CSMapInfo* Find(char* script_name);
};

//###########################################################################
//## Globals
//###########################################################################

extern DECLSPEC CSMapManager SMapMan;

//###########################################################################
//## Exported functions
//###########################################################################


#endif