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

#ifndef _PACPacSprite_H_
#define _PACPacSprite_H_

#include "SDL.h"
#include "SDL_image.h"
#include "const.h"

//###########################################################################
//## Constants & defines
//###########################################################################
#define PACANIM_NORMAL  0
#define PACANIM_DEAD    1
#define PACANIM_SPECIAL 2

//###########################################################################
//## Data structures
//###########################################################################

class DECLSPEC CAnimation {
public:
  Uint16        w, h;
  Uint16        frames;
  Uint16        *delays;
  SDL_Surface*  gfx;

  CAnimation::CAnimation();
  CAnimation::~CAnimation();

  int Load(SDL_RWops *src);
  void Free();
};

class DECLSPEC CSpriteInfo {
public:
  char	          name[MAX_SPRITE_NAME];
  char            describtion[MAX_DESC_LEN];
  Uint8           num_anims;
  CAnimation      *anims[MAX_ANIMATIONS];
  int             refs;
  Uint32          timetag;
  bool            loaded;

  CSpriteInfo();
  ~CSpriteInfo();
  
  int  Load(SDL_RWops *src);
  void Free();

  CSpriteInfo *next;
};

class DECLSPEC CSpriteManager {
public:
  CSpriteManager(short int itype) { type = itype; }
  CSpriteInfo *sprites;
  char fdir[_MAX_PATH];
  char fext[MAX_EXT];
  short int type;  // skins = 1 /sprites = 0

  CSpriteManager();
  ~CSpriteManager();

  void Destroy();           // frees linked list
  int  Scan(char* dir, char* ext);
  void List();

  CSpriteInfo* Register(char* name);
  void Unregister(CSpriteInfo* si);
  
  void FreeAll();
  
  void GarbageCollection();

//protected:
  CSpriteInfo* Find(char* sprite_name);
  int  Free(CSpriteInfo* si);
  int  Load(CSpriteInfo* si);

  void Restore();
};

//###########################################################################
//## Globals
//###########################################################################

extern DECLSPEC CSpriteManager SpriteMan;
extern DECLSPEC CSpriteManager SkinMan;

//###########################################################################
//## Exported functions
//###########################################################################


#endif