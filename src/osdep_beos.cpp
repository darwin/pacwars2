/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   OS dependent code
//## author:  Antonin Hildebrand
//##          Alexander Pipelka (UNIX implementation)
//## 
//## started: 9.4.2000
//## revised: 2.3.2001
//## 
//###########################################################################

#ifndef __BEOS__
#error "Hey,... I'm made for BeOS :)"
#endif

#include <string.h>
#include "osdep.h"
#include <be/kernel/fs_info.h>
#include <be/kernel/fs_query.h>
#include <be/kernel/fs_index.h>

//###########################################################################
//## BeOS implementation
//###########################################################################

#include <dirent.h>

static char file_pattern[256];

static int find_dummy(const struct dirent *unused)
{
	return 1;
}

fhandle_t pacFindFirst_new(char *dir, char *ext, fileinfo_t * fileinfo)
{
	char dev_query[100];
	
	fhandle_t h;

	strcpy(fileinfo->dir, dir);
	if (fileinfo->dir[strlen(fileinfo->dir) - 1] == '/') {
		fileinfo->dir[strlen(fileinfo->dir) - 1] = 0;
	}

	if (ext) {
		strcpy(fileinfo->ext, ext);
		sprintf(file_pattern, "*.%s", fileinfo->ext);
	} else {
		strcpy(file_pattern, "*");
	}

	sprintf(dev_query, "name=%s", file_pattern);
	dev_t device = dev_for_path((const char*)fileinfo->dir);
	DIR* q = fs_open_query(device, dev_query, 0);

	
	h.handle = (unsigned long)q;

	if (q == NULL) {
		h.handle = -1;
	} else {
		pacFindNext(h, fileinfo);
	}

	return h;
}

int pacFindNext(fhandle_t handle, fileinfo_t * fileinfo)
{
	struct dirent *eps = fs_read_query((DIR*)handle.handle);

	if (eps == NULL) {
		return -1;
	}

	strcpy(fileinfo->name, eps->d_name);
	
	return 0;
}

int pacFindClose(fhandle_t handle)
{
	fs_close_query((DIR*)handle.handle);
	return 0;
}

