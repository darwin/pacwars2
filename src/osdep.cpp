/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   OS dependent code
//## author:  Antonin Hildebrand
//## 
//## started: 9.4.2000
//## revised: ???
//## 
//###########################################################################

#include <string.h>
#include "osdep.h"

//###########################################################################
//## WIN implementation
//###########################################################################

#ifdef _WIN32

fhandle_t pacFindFirst(char *filespec, fileinfo_t *fileinfo)
{
  struct _finddata_t winfi;
  fhandle_t h;
  h.handle = _findfirst(filespec, &winfi);
  strcpy(fileinfo->name, winfi.name);
  return h; 
}

int pacFindNext(fhandle_t handle, fileinfo_t *fileinfo)
{
  struct _finddata_t winfi;
  long res = _findnext(handle.handle, &winfi);
  strcpy(fileinfo->name, winfi.name);
  return res; 
}

int pacFindClose(fhandle_t handle)
{
  return _findclose(handle.handle);
}

#else

//###########################################################################
//## UNIX (GNU) implementation
//###########################################################################

#warning "implement pacFindFirst"

#endif

//###########################################################################
//## UNIX implementation
//###########################################################################
