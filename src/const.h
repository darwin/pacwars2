/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   Constants & general functions
//## author:  Antonin Hildebrand
//## 
//## started: 2.3.2000
//## revised: 2.3.2001
//## 
//###########################################################################

#ifndef _CONST2934_H_
#define _CONST2934_H_

#include "osdep.h"				// include OS dependent defines & rutines
#include "SDL.h"
#include "cvar.h"

// export declaration

#ifndef DECLSPEC
# ifdef __BEOS__
#  if defined(__GNUC__)
#   define DECLSPEC	__declspec(dllexport)
#  else
#   define DECLSPEC	__declspec(export)
#  endif
# else
# ifdef WIN32
#  define DECLSPEC	__declspec(dllexport)
# else
#  define DECLSPEC
# endif
# endif
#endif

//###########################################################################
//## Module / componets defines
//###########################################################################

// Defined components
#define PW_MUSIC
#define PW_SOUND
#define PW_BASS					// sound will be played via BASS sound system

// Dependent defines
#ifdef PW_MUSIC
#define PW_AUDIO
#endif

#ifdef PW_SOUND
#define PW_AUDIO
#endif

#define PW_NETCLIENT
#define PW_NETSERVER

#define SCRIPTING_STD_LIBS
//#define SCRIPT_DEBUG

//#define PARANOID
//#define BOUNDING_BOXES

#include "../bin/scripts/gameplay.h"

#include "stdconfig.h"

const Uint8 PREDICTION_BUF_LENGTH = 50;

const GAME_MAXOBJS_TYPE GAME_MAX_OBJS = 250;	// pozor pri praci na velikost <=250
const GAME_MAXOBJS_TYPE MAX_PACS_PER_STATION = 16;


//###########################################################################
//## Constants
//###########################################################################

const Uint8 VERSION_MAJOR = 01;
const Uint8 VERSION_MINOR = 10;

#define PW_CONFIG_FILE "config.cfg"

#define PW_WEBADDRESS "http://pw2.tsx.org"
// dirs
#define STD_SCRIPT_DIR "scripts/"
#define STD_MAP_DIR "maps/"
#define STD_GFX_DIR "gfx/game/"
#define STD_SKIN_DIR "gfx/skins/"
#define STD_SPRITE_DIR "gfx/sprites/"
#define STD_SND_DIR "snd/"
#define STD_GUI_DIR "gfx/gui/"

extern DECLSPEC cvar_t sprite_dir;
extern DECLSPEC cvar_t script_dir;
extern DECLSPEC cvar_t gui_dir;
extern DECLSPEC cvar_t map_dir;
extern DECLSPEC cvar_t gfx_dir;
extern DECLSPEC cvar_t skin_dir;
extern DECLSPEC cvar_t snd_dir;

// extensions
#define STD_TMP_EXT "tmp"
#define STD_SCRIPT_EXT "sc"
#define STD_SMAP_EXT "msc"
#define STD_MAP_EXT "fmp"
#define STD_SKIN_EXT "pac"
#define STD_SPRITE_EXT "pac"

extern DECLSPEC cvar_t tmp_ext;
extern DECLSPEC cvar_t map_ext;
extern DECLSPEC cvar_t script_ext;
extern DECLSPEC cvar_t smap_ext;
extern DECLSPEC cvar_t skin_ext;
extern DECLSPEC cvar_t sprite_ext;

#define STD_SPRITE_IMP "implicit"
#define STD_SKIN_IMP   "implicit"

extern DECLSPEC cvar_t skin_imp;
extern DECLSPEC cvar_t sprite_imp;

// std controls
#define STD_KEY_UP      SDLK_UP
#define STD_KEY_DOWN    SDLK_DOWN
#define STD_KEY_RIGHT   SDLK_RIGHT
#define STD_KEY_LEFT    SDLK_LEFT
#define STD_KEY_FIRE    SDLK_RCTRL
#define STD_KEY_SHIELD  SDLK_s
#define STD_KEY_WARP    SDLK_w
#define STD_KEY_WEAPON1 SDLK_1
#define STD_KEY_WEAPON2 SDLK_2
#define STD_KEY_WEAPON3 SDLK_3
#define STD_KEY_WEAPON4 SDLK_4
#define STD_KEY_WEAPON5 SDLK_5

// console
#define MAX_WORD_INPUTED 100
#define MAX_LINE_CHARS 110		// for line breaking in ConPar()

// scripting manager
#define MAX_SCRIPTS 10

// misc
#define FPS_MEASURE 400			// in ticks (500 = each 0.5s update)
#define PING_MEASURE 2000		// in ticks (each 2s update)

#define INFO_BASEY (20*13)
#define INFO_BASEX (6)

#define CMD_FONT    1
#define CHAT_FONT   2
#define VAR_FONT    3
#define SERVER_FONT 4
#define MISC_FONT   5
#define CLIENT_FONT 6

// temp
#define CSX 16
#define CSY 16

// animations
#define MAX_SPRITE_NAME 12
#define MAX_MAP_NAME 40
#define MAX_SSCRIPT_NAME 40
#define MAX_DESC_NAME 100
#define MAX_AUTHOR_NAME 100
#define MAX_SCRIPT_NAME 40
#define MAX_DESC_LEN    100
#define MAX_ANIMATIONS  100
#define MAX_WEAPONS     5

#define MENUALPHA 200


#ifndef _MAX_PATH
#define _MAX_PATH 260
#endif

#define FPS 30
#define FRAMETICKS (1000/FPS)
#define THINKINGS_PER_TICK 1
#define MOVE_SUB_STEPS 4

void FileNameConversion(char *dir, char *shortn, char *ext,
						char *expandedn);
void TempName(char *fname, char *tmpname);

#define MAX_EXT 10

extern DECLSPEC Uint8 video_bpp;
;

//###########################################################################
//## MapView
//###########################################################################

// These define the width and height of the drawn area... 
extern DECLSPEC int MSCRW;
extern DECLSPEC int MSCRH;

// These define how far from the left and top of the surface the drawn area is 
extern DECLSPEC int MMOX;
extern DECLSPEC int MMOY;

//###########################################################################
//## GamePlay2
//###########################################################################

#define OSTATE_ACTIVE   0x1
#define OSTATE_VISIBLE  0x2


//types of objects that lives in the world:
typedef enum object_type {
//player:
	ot_obj = 0,
	ot_entity,
	ot_player,
	ot_shot,
	ot_bomb,
	ot_extra,
	ot_rail,
	ot_fray,
	ot_sound,
	ot_bshot,
	ot_grenade,

	ot_error
};

const char OBJECT_NAMES[ot_error + 1][40] = {
	"base_object",
	"base_entity",
	"pac player",
	"pac shot",
	"rapid bomb",
	"extra",
	"rail",
	"fire ray",
	"sound",
	"bshot",
	"grenade",

	"ERROR"
};

// Brain types
typedef enum brain_type {
	bt_soulfree = 0,
	bt_client,
	bt_bot,
};

enum ENetRole {
	ROLE_None = 0,				// Means the actor is not relevant in network play.
	ROLE_DumbProxy,				// A dumb proxy.
	ROLE_SimulatedProxy,		// A simulated proxy.
	ROLE_AutonomousProxy,		// An autonomous proxy.
	ROLE_Authority,				// The one authoritative version of the actor.
};

#endif
