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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL_endian.h"
#include "console/SDL_Console.h"
#include "sprite.h"
#include "client.h"

CSpriteManager SpriteMan(0);
CSpriteManager SkinMan(1);

SDL_Surface* LoadImage(SDL_RWops *src)
{
  SDL_Surface *s;
  SDL_Surface *temp;
  temp = IMG_LoadPNG_RW(src); 
  if (!temp) return NULL;

  if(SDL_WasInit(SDL_INIT_VIDEO)!=0)     
  {
    s = SDL_DisplayFormat(temp);      // we are in game
    SDL_FreeSurface(temp);
    SDL_SetColorKey(s, SDL_SRCCOLORKEY|SDL_RLEACCEL, SDL_MapRGB(s->format, 255, 0, 255));
  }
  else
    s = temp;                         // we are dedicated windows traybar server

  

/*  if (s->format->BitsPerPixel==16)
  {
    Uint16* px = (Uint16*)s->pixels;
    SDL_SetColorKey(s, SDL_SRCCOLORKEY|SDL_RLEACCEL, *px);
  }
  else
  {
    if (s->format->BitsPerPixel==32)
    {
      Uint32* px = (Uint32*)s->pixels;
      SDL_SetColorKey(s, SDL_SRCCOLORKEY|SDL_RLEACCEL, *px);
    }
  }
*/
  return s; 
}

//###########################################################################
//## CAnimation
//###########################################################################

CAnimation::CAnimation()
{
  gfx = NULL;
  delays = NULL;
}

CAnimation::~CAnimation()
{
  Free();
}

int CAnimation::Load(SDL_RWops *src)
{
  Free();

  w = SDL_ReadLE16(src);
  h = SDL_ReadLE16(src);
  frames = SDL_ReadLE16(src);
  delays = (Uint16*)malloc(sizeof(Uint16)*frames);
  if (!delays) return 2;
  for (int i=0; i<frames; i++) {
    // file format stores delays in 1/100th of second
    // we will convert them to game ticks
    delays[i] = THINKINGS_PER_TICK*((10*SDL_ReadLE16(src))/FRAMETICKS);  
  }

  // load image data
  gfx = LoadImage(src);
  return 0;
}

void CAnimation::Free()
{
  if (gfx) { 
    SDL_FreeSurface(gfx);
    gfx = NULL;
  }
  if (delays) {
    free(delays);
    delays = NULL;
  }
}

//###########################################################################
//## CSpriteInfo
//###########################################################################

CSpriteInfo::CSpriteInfo()
{
  name[0] = 0;
  describtion[0] = 0;
  anims[0] = NULL;
  refs=0;
  num_anims=0;
  next = NULL;
  refs = 0;
  loaded = false;
}

CSpriteInfo::~CSpriteInfo()
{
  Free();
}

int CSpriteInfo::Load(SDL_RWops *src)
{
  SDL_RWread(src, describtion, 100, 1);
  timetag = SDL_ReadLE32(src);
  SDL_RWread(src, &num_anims, 1, 1);
  
  int res = 0;
  for (int i=0; i<num_anims; i++) 
  {
    anims[i] = new CAnimation;
    res|=anims[i]->Load(src);
  }

  loaded = true;
  return res;
}

void CSpriteInfo::Free()
{
//  if (refs) fprintf(stderr, "Warning: PacSpriteInfo %s freed beeing referenced !", name);
  for (int i=0; i<num_anims; i++) 
  {
    delete anims[i];
    anims[i] = NULL;
  }

  refs = 0;
  loaded = false;
}

//###########################################################################
//## CSpriteManager
//###########################################################################

CSpriteManager::CSpriteManager()
{
  sprites = NULL;
}

CSpriteManager::~CSpriteManager()
{
  Destroy();
}

void CSpriteManager::Destroy()
{
  CSpriteInfo *a, *n;
  a = sprites;
  while (a) {
    n = a->next;
    delete a;
    a = n;
  }
  sprites = NULL;
}

// Adds new "pac" filenames to manager (do not load them now)
int CSpriteManager::Scan(char* dir, char* ext)
{
  fhandle_t h;
  fileinfo_t fi;
  char fspec[_MAX_PATH];
  char *spritename;
  CSpriteInfo* si;
  int len;
  int count=0;

  strcpy(fdir, dir);
  strcpy(fext, ext);
  FileNameConversion(fdir, "*", fext, fspec);
#ifdef _WIN32
  h = pacFindFirst(fspec, &fi);
#else
	h = pacFindFirst_new(fdir, fext, &fi);
#endif

	if (h.handle==-1) return 0;
  
  // filename -> name : cut-off ".pac", cut-off "gfx_dir.string/" & shorten to MAX_SPRITE_NAME
  len = strlen(fi.name)-(1+strlen(skin_ext.string));
  spritename = fi.name; // + strlen(gfx_dir.string+1);
  if (len>(MAX_SPRITE_NAME-1)) len = MAX_SPRITE_NAME-1;
  spritename[len]=0;  

  if (!Find(fi.name)) // add only new names
  {
    // create new sprite info & link it to the list
    si = new CSpriteInfo;
    si->next = sprites;
    sprites = si;
  
    // write name to the new CSpriteInfo
    strcpy(si->name, fi.name);
    
    count++;
  }

  // find next names
  while (pacFindNext(h, &fi)==0)
  {
    // filename: cut-off ".pac" & shorten to MAX_SPRITE_NAME
    len = strlen(fi.name)-(1+strlen(skin_ext.string));
    spritename = fi.name; // + strlen(gfx_dir.string+1);
    if (len>(MAX_SPRITE_NAME-1)) len = MAX_SPRITE_NAME-1;
    spritename[len]=0;  

    if (!Find(fi.name)) // add only new names
    {
      // create new sprite info & link it to the list
      si = new CSpriteInfo;
      si->next = sprites;
      sprites = si;
  
      // write name to the new CSpriteInfo
      strcpy(si->name, fi.name);

      count++;
    }
  }
  pacFindClose(h);
  return count;
}

CSpriteInfo* CSpriteManager::Find(char* sprite_name)
{
  CSpriteInfo* a;
  a = sprites;
  while (a && stricmp(sprite_name, a->name)!=0) {
    a=a->next;
  }
  return a;
}

int CSpriteManager::Load(CSpriteInfo* si)
{
  if (si) 
  {
    char fname[_MAX_PATH];
    FileNameConversion(fdir, si->name, fext, fname);
    SDL_RWops *src = SDL_RWFromFile(fname, "rb");
    if ((!src) || (si->Load(src))) 
    {
      FileNameConversion(fdir, sprite_imp.string, fext, fname);
      SDL_RWops *src2 = SDL_RWFromFile(fname, "rb");
      if ((!src2) || (si->Load(src2))) 
      {
        fprintf(stderr, "Entity: fatal error - implicit sprite %s not found\n", sprite_imp.string);
        exit(255);
      }
      SDL_RWclose(src2);
      return 0;
    }
    SDL_RWclose(src);
    return 0;
  }
  else
    return 1;
}

int CSpriteManager::Free(CSpriteInfo* si)
{
  if (si) {
    si->Free();
    return 0;
  }
  else 
    return 1;
}

void CSpriteManager::FreeAll()
{
  CSpriteInfo* a;
  a = sprites;
  while (a) {
    a->Free();
    a = a->next;
  }
}

void CSpriteManager::List()
{
  ConOut("PacSprites:");
  CSpriteInfo* a;
  a = sprites;
  while (a) {
    if (!a->anims[0])
      ConOut("%12s [unloaded]", a->name); 
    else
      ConOut("%12s [%d anims, %d refs]", a->name, a->num_anims, a->refs);
    a = a->next;
  }
}

CSpriteInfo * CSpriteManager::Register(char * name)
{
  CSpriteInfo * si = Find(name);
  if (!si)
  {
    si = new CSpriteInfo;
    si->next = sprites;
    sprites = si;
    strcpy(si->name, name);
    Load(si);

    char s[100];
    if (type==0)
    {
      sprintf(s, "%s.%s", name, sprite_ext.string);
      client_info.AddDlRequest(DLID_SPRITE, s);
    }
    else
    {
      sprintf(s, "%s.%s", name, skin_ext.string);
      client_info.AddDlRequest(DLID_SKIN, s);
    }
  } 
  if (!si->loaded) 
  {
    if (Load(si)) return NULL;
  }
  si->refs++;
  return si;
}

void CSpriteManager::Unregister(CSpriteInfo* si)
{
//  if (si->refs) si->refs--;

  // TODO: remove from memory ?
}

void CSpriteManager::GarbageCollection()
{
  // TODO: purge memory from unreferenced sprites

}

void CSpriteManager::Restore()
{
  CSpriteInfo* a;
  a = sprites;
  while (a) {
    if (a->loaded)
    {
      Free(a);
      Load(a);
    }
    a = a->next;
  }
}
