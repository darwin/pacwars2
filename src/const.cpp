/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   Constants & general functions
//## author:  Antonin Hildebrand
//## 
//## started: 2.3.2000
//## revised: 12.3.2000
//## 
//###########################################################################

#include <stdio.h>
#include <string.h>
#include "const.h"

void FileNameConversion(char *dir, char *shortn, char *ext,
						char *expandedn)
{
	if (*ext == 0)
		sprintf(expandedn, "%s%s", dir, shortn);
	else
		sprintf(expandedn, "%s%s.%s", dir, shortn, ext);
}

void TempName(char *fname, char *tmpname)
{
	sprintf(tmpname, "%s.%s", fname, tmp_ext.string);
}


//###########################################################################
//## CVars
//###########################################################################

cvar_t sprite_dir = { "sprite_dir", STD_SPRITE_DIR, true };
cvar_t gui_dir = { "gui_dir", STD_GUI_DIR, true };
cvar_t script_dir = { "script_dir", STD_SCRIPT_DIR, true };
cvar_t map_dir = { "map_dir", STD_MAP_DIR, true };
cvar_t gfx_dir = { "gfx_dir", STD_GFX_DIR, true };
cvar_t skin_dir = { "skin_dir", STD_SKIN_DIR, true };
cvar_t snd_dir = { "snd_dir", STD_SND_DIR, true };

cvar_t tmp_ext = { "tmp_ext", STD_TMP_EXT, true };
cvar_t map_ext = { "map_ext", STD_MAP_EXT, true };
cvar_t script_ext = { "script_ext", STD_SCRIPT_EXT, true };
cvar_t smap_ext = { "smap_ext", STD_SMAP_EXT, true };
cvar_t skin_ext = { "skin_ext", STD_SKIN_EXT, true };
cvar_t sprite_ext = { "sprite_ext", STD_SPRITE_EXT, true };

cvar_t skin_imp = { "skin_imp", STD_SKIN_IMP, true };
cvar_t sprite_imp = { "sprite_imp", STD_SPRITE_IMP, true };
