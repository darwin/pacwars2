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

#ifndef _PACOS_H_
#define _PACOS_H_

#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <io.h>
#endif

//###########################################################################
//## Platform dependencies
//###########################################################################

#ifdef _WIN32
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#define beep _beep
#endif

#ifndef _MAX_FNAME
#define _MAX_FNAME 255
#endif

typedef struct {
  long handle;
} fhandle_t;

typedef struct {
  char name[_MAX_FNAME];
  char dir[_MAX_FNAME];
  char ext[_MAX_FNAME];
} fileinfo_t;


fhandle_t pacFindFirst(char *filespec, fileinfo_t *fileinfo);
fhandle_t pacFindFirst_new(char *dir, char* ext, fileinfo_t *fileinfo);
int pacFindNext(fhandle_t handle, fileinfo_t *fileinfo);
int pacFindClose(fhandle_t handle);

#endif

#ifndef _WIN32
#define stricmp strcasecmp
#endif

