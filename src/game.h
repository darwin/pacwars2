/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   main game structure
//## author:  Antonin Hildebrand
//##         
//## started: 3.4.2000
//## revised: ???
//## 
//##
//###########################################################################

#ifndef _PACGAME_H_
#define _PACGAME_H_

#include "map.h"
#include "gvars.h"
#include "prediction.h"
#include "pac_obj.h"
#include "pac_player.h"

//###########################################################################
//## Constants & defines
//###########################################################################

#define GS_UNINITED 0
#define GS_SERVER   1
#define GS_CLIENT   3
#define GS_ENDED    5

const char GAME_STATES_STR[6][20] = {
	"uninited",
	"server",
	"",
	"client",
	"",
	"ended"
};

// game types
#define GT_DEATHMATCH_TIME  0
#define GT_DEATHMATCH_FRAG  1
#define GT_CTF_TIME         2
#define GT_CTF_POINTS       3
#define GT_TEAM_TIME        4
#define GT_TEAM_FRAG        5
#define GT_1ON1_TIME        6
#define GT_1ON1_FRAG        7

const char GAME_TYPES_STR[8][40] = {
	"Deathmatch (timelimit)",
	"Deathmatch (fraglimit)",
	"CTF (timelimit)",
	"CTF (pointlimit)",
	"Team play (timelimit)",
	"Team play (fraglimit)",
	"1on1 (timelimit)",
	"1on1 (fraglimit)"
};

#define MAX_GAMEBAR_PLAYERS  10

extern DECLSPEC cvar_t g_results_x;
extern DECLSPEC cvar_t g_results_y;
extern DECLSPEC cvar_t g_results_time;

extern SDL_Surface *gamebar_bkg;
extern SDL_Surface *gamebar_playerbar;
extern SDL_Surface *statusbar;
extern SDL_Surface *statusbar_bkg;
extern SDL_Surface *becherbar;


//###########################################################################
//## Data structures
//###########################################################################

class DECLSPEC CGame { public:
	Uint8 state;
	Uint32 starttick;
	TICK_TYPE tick;				// last thinking tick
	Uint16 last_oid;			// last created object id

	// game data
	CVars vars;
	CMapState map;
	GObj *objs[GAME_MAX_OBJS];

	// game types
	Uint8 game_type;

	// helpers

	CReplicator replicator;

	int GBSlots[MAX_GAMEBAR_PLAYERS];
	int MainSlot;
	int BecherSlot;

	// methods
	 CGame();
	~CGame();

	GAME_MAXOBJS_TYPE FindUnusedSlot();
	void IncrementLOID() {
		last_oid++;
		if (last_oid == 0)
			last_oid = 1;
	}
		// 0 is reservedobject id####################################################he float interpretation of the string.  Generally, all cvar_t declarations should be registered in the apropriate init function before any console commands are executed:
		int RebornObject(Uint16 id, GAME_MAXOBJS_TYPE slot, Uint8 ot);	// chybovy kod

	void ServerThink(TICK_TYPE time);
	void ClientThink(TICK_TYPE time);
	void GameThink(TICK_TYPE time);

  char ServerAdjustPositions(int cnum, CReplicator & rep, TICK_TYPE time);
	char ServerReplicate(int cnum, CReplicator & rep, TICK_TYPE time);
	char ClientReplicate(TICK_TYPE time);

	void SetGameType(Uint8 gametype) {
		game_type = gametype;
	} void UpdateGamebarSlots();
	void RenderPlayerBar(GPlayer * p, SDL_Surface * screen, int font,
						 int px, int py);
	void RenderStatusBar(SDL_Surface * screen, int font, int px, int py);
	void RenderBecherBar(SDL_Surface * screen, int px, int py);
	void RenderGameBar(SDL_Surface * screen, int font, int px, int py);
	void RenderResults(SDL_Surface * screen, int font);

	void Clear();
	void Reset();

  private:
	 bool l1;
	bool l2;
	bool l3;
	bool l4;
	bool l5;

	TICK_TYPE w1;
	TICK_TYPE w2;
	TICK_TYPE w3;
	TICK_TYPE w4;
	TICK_TYPE w5;
	TICK_TYPE w6;
	int h6;
	TICK_TYPE w7;
	int h7;

};

//###########################################################################
//## Globals
//###########################################################################

//###########################################################################
//## Exported functions
//###########################################################################

#endif
