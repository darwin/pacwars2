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
#include "smapman.h"

CSMapManager SMapMan;


//###########################################################################
//## CSMapInfo
//###########################################################################

CSMapInfo::CSMapInfo()
{
  name[0] = 0;
  map[0] = 0;
}

CSMapInfo::~CSMapInfo()
{
}

//###########################################################################
//## CSMapManager
//###########################################################################

CSMapManager::CSMapManager()
{
  Scripts = NULL;
}

CSMapManager::~CSMapManager()
{
  Destroy();
}

void CSMapManager::Destroy()
{
  CSMapInfo *a, *n;
  a = Scripts;
  while (a) {
    n = a->next;
    delete a;
    a = n;
  }
  Scripts = NULL;
}

void ReadMapName(char* fname, char* store)
{
  FILE* f;
  f=fopen(fname,"rt");
  if (f)
  {
    fscanf(f,"//%s", store);
    fclose(f);
  }
}

// Adds new "fmp" filenames to manager (do not load them now)
int CSMapManager::Scan(char* dir, char* ext)
{
  fhandle_t h;
  fileinfo_t fi;
  char fspec[_MAX_PATH];
  char *Scriptname;
  CSMapInfo* si;
  int len;
  int count=0;

  strcpy(fdir, dir);
  strcpy(fext, ext);
  FileNameConversion(fdir, "*", fext, fspec);
  h = pacFindFirst(fspec, &fi);
  if (h.handle==-1) return 0;
  
  // filename -> name : cut-off ".pac", cut-off "gfx_dir.string/" & shorten to MAX_SCRIPT_NAME
  len = strlen(fi.name)-(1+strlen(smap_ext.string));
  Scriptname = fi.name; 
  if (len>(MAX_SCRIPT_NAME-1)) len = MAX_SCRIPT_NAME-1;
  Scriptname[len]=0;  

  if (!Find(fi.name)) // add only new names
  {
    // create new Map info & link it to the list
    si = new CSMapInfo;
    si->next = Scripts;
    Scripts = si;
  
    // write name to the new CSMapInfo
    strcpy(si->name, fi.name);

    FileNameConversion(fdir, fi.name, fext, fspec);
    ReadMapName(fspec, si->map);
    
    count++;
  }

  // find next names
  while (pacFindNext(h, &fi)==0)
  {
    // filename: cut-off ".pac" & shorten to MAX_SCRIPT_NAME
    len = strlen(fi.name)-(1+strlen(smap_ext.string));
    Scriptname = fi.name; // + strlen(gfx_dir.string+1);
    if (len>(MAX_SCRIPT_NAME-1)) len = MAX_SCRIPT_NAME-1;
    Scriptname[len]=0;  

    if (!Find(fi.name)) // add only new names
    {
      // create new Map info & link it to the list
      si = new CSMapInfo;
      si->next = Scripts;
      Scripts = si;
  
      // write name to the new CSMapInfo
      strcpy(si->name, fi.name);

      FileNameConversion(fdir, fi.name, fext, fspec);
      ReadMapName(fspec, si->map);

      count++;
    }
  }
  
  pacFindClose(h);
  return count;
}

CSMapInfo* CSMapManager::Find(char* Map_name)
{
  CSMapInfo* a;
  a = Scripts;
#ifdef _WIN32
  while (a && stricmp(Map_name, a->name)!=0) {
#else
  while (a && strcasecmp(Map_name, a->name)!=0) {
#endif
    a=a->next;
  }
  return a;
}

void CSMapManager::List(char * t)
{
  ConOut(t);
  CSMapInfo* a;
  a = Scripts;
  while (a) {
    ConOut("%12s", a->name);
    a = a->next;
  }
}

