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

#include <dirent.h>

static int count;
static int file_index;

static int find_dummy (const struct dirent *unused) {
  return 1;
}

fhandle_t pacFindFirst(char *filespec, fileinfo_t *fileinfo) {
	struct dirent **eps;
	fhandle_t h;

	count = scandir (filespec, &eps, find_dummy, alphasort);
	h.handle = (long)eps;
	file_index = 0;

	if(count <= 0) {
		h.handle = 0;
	}

	strcpy(fileinfo->name, eps[file_index++]->d_name);

	return h;
}

int pacFindNext(fhandle_t handle, fileinfo_t *fileinfo) {
	struct dirent **eps = (struct dirent **)handle.handle;

	if(file_index < count) {
		strcpy(fileinfo->name, eps[file_index++]->d_name);
		return 1;
	}

	return -1;
}

int pacFindClose(fhandle_t handle) {
	free( (struct dirent **) (handle.handle));
}

#endif

//###########################################################################
//## UNIX implementation
//###########################################################################
