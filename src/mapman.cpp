/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   Map manager
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
#include "mapman.h"

CMapManager MapMan;


//###########################################################################
//## CMapInfo
//###########################################################################

CMapInfo::CMapInfo()
{
  name[0] = 0;
}

CMapInfo::~CMapInfo()
{
}

//###########################################################################
//## CMapManager
//###########################################################################

CMapManager::CMapManager()
{
  Maps = NULL;
}

CMapManager::~CMapManager()
{
  Destroy();
}

void CMapManager::Destroy()
{
  CMapInfo *a, *n;
  a = Maps;
  while (a) {
    n = a->next;
    delete a;
    a = n;
  }
  Maps = NULL;
}

// Adds new "fmp" filenames to manager (do not load them now)
int CMapManager::Scan(char* dir, char* ext)
{
  fhandle_t h;
  fileinfo_t fi;
  char fspec[_MAX_PATH];
  char *Mapname;
  CMapInfo* si;
  int len;
  int count=0;

  strcpy(fdir, dir);
  strcpy(fext, ext);
  FileNameConversion(fdir, "*", fext, fspec);
  h = pacFindFirst(fspec, &fi);
  if (h.handle==-1) return 0;
  
  // filename -> name : cut-off ".pac", cut-off "gfx_dir.string/" & shorten to MAX_MAP_NAME
  len = strlen(fi.name)-(1+strlen(map_ext.string));
  Mapname = fi.name; 
  if (len>(MAX_MAP_NAME-1)) len = MAX_MAP_NAME-1;
  Mapname[len]=0;  

  if (!Find(fi.name)) // add only new names
  {
    // create new Map info & link it to the list
    si = new CMapInfo;
    si->next = Maps;
    Maps = si;
  
    // write name to the new CMapInfo
    strcpy(si->name, fi.name);
    
    count++;
  }

  // find next names
  while (pacFindNext(h, &fi)==0)
  {
    // filename: cut-off ".pac" & shorten to MAX_MAP_NAME
    len = strlen(fi.name)-(1+strlen(map_ext.string));
    Mapname = fi.name; // + strlen(gfx_dir.string+1);
    if (len>(MAX_MAP_NAME-1)) len = MAX_MAP_NAME-1;
    Mapname[len]=0;  

    if (!Find(fi.name)) // add only new names
    {
      // create new Map info & link it to the list
      si = new CMapInfo;
      si->next = Maps;
      Maps = si;
  
      // write name to the new CMapInfo
      strcpy(si->name, fi.name);

      count++;
    }
  }
  pacFindClose(h);
  return count;
}

CMapInfo* CMapManager::Find(char* Map_name)
{
  CMapInfo* a;
  a = Maps;
#ifdef _WIN32
  while (a && stricmp(Map_name, a->name)!=0) {
#else
  while (a && strcasecmp(Map_name, a->name)!=0) {
#endif

    a=a->next;
  }
  return a;
}

void CMapManager::List(char * t)
{
  ConOut(t);
  CMapInfo* a;
  a = Maps;
  while (a) {
    ConOut("%12s", a->name);
    a = a->next;
  }
}

