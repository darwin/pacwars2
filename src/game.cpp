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

#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "const.h"
#include "game.h"
#include "console/SDL_DrawText.h"
#include "server.h"
#include "client.h"
#include "scripting.h"
#include "messenger.h"
#include "pw2.h"

#include "pac.h"

cvar_t g_results_x = { "g_results_x", "100", true };
cvar_t g_results_y = { "g_results_y", "100", true };
cvar_t g_results_time = { "g_results_time", "60", true };

SDL_Surface *gamebar_bkg;
SDL_Surface *statusbar_bkg;
SDL_Surface *gamebar_playerbar;
SDL_Surface *statusbar;
SDL_Surface *becherbar;

//###########################################################################
//## Globals
//###########################################################################

CGame::CGame()
{
	tick = 0;
	for (int i = 0; i < GAME_MAX_OBJS; i++) {
		objs[i] = new GObj(0, this, i);
//    objs[i]->state = 0;
	}
	state = GS_UNINITED;
	last_oid = 1;
	MainSlot = -1;
	BecherSlot = -1;

	replicator.Reset();
}

CGame::~CGame()
{
	map.Free();
	MapFreeMem();
	for (int i = 0; i < GAME_MAX_OBJS; i++)
		if (objs[i]) {
			delete objs[i];
		}
	//
	l1 = 0;
	l2 = 0;
	l3 = 0;
	l4 = 0;
	l5 = 0;

	w1 = 0;
	w2 = 0;
	w3 = 0;
	w4 = 0;
	w5 = 0;
	w6 = 0;
	h6 = 0;
	w7 = 0;
	h7 = 0;
}

void CGame::Clear()
{
	int i;

	MainSlot = -1;
	BecherSlot = -1;
	tick = 0;

	// call done script sequence

	// TODO: add this - here was problem
	if (state == GS_SERVER)
		ScriptMan->RunScript(map_script_num, "ssDone");
	else if (state == GS_CLIENT)
		ScriptMan->RunScript(map_script_num, "csDone");

	map.Free();
	for (i = 0; i < GAME_MAX_OBJS; i++)
		if (objs[i]) {
			delete objs[i];
			objs[i] = new GObj(0, this, i);
			objs[i]->MakeDirty(-1, false);	// constructors are same on both client and server side, from now replicate changes
			objs[i]->state = 0;
		}

	vars.camx = 0;
	vars.camy = 0;
	vars.zonex = 0;
	vars.zoney = 0;
	vars.chasing = -1;

	state = GS_UNINITED;
	last_oid = 1;

	replicator.Reset();

	//
	l1 = 0;
	l2 = 0;
	l3 = 0;
	l4 = 0;
	l5 = 0;

	w1 = 0;
	w2 = 0;
	w3 = 0;
	w4 = 0;
	w5 = 0;
	w6 = 0;
	h6 = 0;
	w7 = 0;
	h7 = 0;
}

void CGame::Reset()
{
	tick = 0;
	last_oid = 1;
	replicator.Reset();
}

int CGame::RebornObject(Uint16 id, GAME_MAXOBJS_TYPE slot, Uint8 ot)
{
	delete objs[slot];
/*
  if (state!=GS_CLIENT) 
    ConOut("reborn ID=%d, SLOT=%d OT=%s SERVER", id, slot, OBJECT_NAMES[ot]);
  else
    ConOut("reborn ID=%d, SLOT=%d OT=%s CLIENT", id, slot, OBJECT_NAMES[ot]);
*/
	switch (ot) {
	case ot_player:
		objs[slot] = new GPlayer(id, this, slot);
		break;

	case ot_shot:
		objs[slot] = new GShot(id, this, slot);
		break;

	case ot_bomb:
		objs[slot] = new GBomb(id, this, slot);
		break;

	case ot_extra:
		objs[slot] = new GExtra(id, this, slot);
		break;

	case ot_rail:
		objs[slot] = new GRail(id, this, slot);
		break;

	case ot_fray:
		objs[slot] = new GFray(id, this, slot);
		break;

	case ot_sound:
		objs[slot] = new GSound(id, this, slot);
		break;

	case ot_bshot:
		objs[slot] = new GBShot(id, this, slot);
		break;

	case ot_grenade:
		objs[slot] = new GGrenade(id, this, slot);
		break;

	case ot_obj:
		objs[slot] = new GObj(id, this, slot);
		break;

	case ot_entity:
		ConErr( "RebornObject: attempt to create abstract object");
		return 3;

	default:
		ConErr(
				"RebornObject: attempt to reborn to unknown object");
	}

	objs[slot]->MakeDirty(-1, false);	// constructors are same on both client and server side, from now replicate changes
	objs[slot]->state = OSTATE_ACTIVE | OSTATE_VISIBLE;

	if (id > last_oid)
		last_oid = id;
	return 0;
}

// vraci slot nepouzivaneho objektu
GAME_MAXOBJS_TYPE CGame::FindUnusedSlot()
{
	if (state == GS_CLIENT)
		ConErr( "CLIENT MADE NEW OBJECT !!!\n");
	for (GAME_MAXOBJS_TYPE i = 0; i < GAME_MAX_OBJS; i++) {
		if (!(objs[i]->state & OSTATE_ACTIVE))
			return i;
	}
	ConErr( "FindUnusedSlot: no unused slot available");
	return 0;
}

void CGame::ServerThink(TICK_TYPE time)
{
	tick = time;

	for (GAME_MAXOBJS_TYPE i = 0; i < GAME_MAX_OBJS; i++) {
		if (objs[i]->state & OSTATE_ACTIVE)
			objs[i]->ServerThink(time);
	}
}

void CGame::ClientThink(TICK_TYPE time)
{
	tick = time;

	for (GAME_MAXOBJS_TYPE i = 0; i < GAME_MAX_OBJS; i++) {
		if (objs[i]->state & OSTATE_ACTIVE)
			objs[i]->ClientThink(time);
	}
}

void CGame::GameThink(TICK_TYPE time)
{
	int i;
	Uint32 limit;
	int c;
	static int last = -1;

	switch (game_type) {
	case GT_DEATHMATCH_TIME:
		limit = ((int) s_timelimit.value) * 1000;
		c = limit - (SDL_GetTicks() - starttick);
		if (c < 11000)			// 11s
		{
			if (limit < (SDL_GetTicks() - starttick))	// it's time to end the game
			{
				SV_ChangeGame();
			} else {
				if (c > 1000) {
					if (c / 1000 != last) {
						last = c / 1000;
						if (last == 1)
							SV_BroadcastPrintf("Server: last second remains");
						else {
							if (last == 10 || last <= 3)
								SV_BroadcastPrintf("Server: %d seconds remain", last);
							if (last == 10)
								SV_BroadcastVoice(SM_10_0, 100, 100);
						}
					}
				}
			}
		}

		break;
	case GT_DEATHMATCH_FRAG:
		for (i = 0; i < GAME_MAX_OBJS; i++)
			if (objs[i]->GetType() == ot_player	&& ((GPlayer *) objs[i])->frags >= s_fraglimit.value) 
      {	// player i has fraglimit
				SV_ChangeGame();
				break;
			}
		break;
	}
}

void CGame::UpdateGamebarSlots()
{
	int slot = 0;
	int i = 0;
	BecherSlot = -1;
  
  // test main slot
  if (MainSlot!=-1)
  {
    if ((!(objs[MainSlot]->state & OSTATE_ACTIVE)) || objs[MainSlot]->GetType()!=ot_player)
      MainSlot = -1;
  }

	for (i = 0; i < GAME_MAX_OBJS; i++) {
		if ((objs[i]->GetType() == ot_player)	&& (objs[i]->state & OSTATE_ACTIVE)) {
			/*GPlayer *p = (GPlayer *) objs[i];
			if (MainSlot==-1 && p->brain_owner == client_info.client_num)
				MainSlot = i;*/
			if (slot < MAX_GAMEBAR_PLAYERS) {
				GBSlots[slot] = i;
				slot++;
			}
		}
		if ((objs[i]->state & OSTATE_ACTIVE) && (objs[i]->GetType() == ot_extra)) {
			GExtra *e = (GExtra *) objs[i];
			if (e->type >= EX_SUP_BECHER) {
				BecherSlot = i;
			}
		}
	}
	if (slot != MAX_GAMEBAR_PLAYERS)
		GBSlots[slot] = GAME_MAX_OBJS;	// end marker


	// sort playerbars according to #points
	bool sorted = false;
	while (!sorted) {
		sorted = true;
		if (GBSlots[0] != GAME_MAX_OBJS)
			for (i = 0; i < MAX_GAMEBAR_PLAYERS - 1; i++) {
				if (GBSlots[i + 1] == GAME_MAX_OBJS)
					break;
				GPlayer *p1 = (GPlayer *) objs[GBSlots[i]];
				GPlayer *p2 = (GPlayer *) objs[GBSlots[i + 1]];
				if (p1->points < p2->points)	// swap
				{
					int save;
					save = GBSlots[i + 1];
					GBSlots[i + 1] = GBSlots[i];
					GBSlots[i] = save;
					sorted = false;
				}
			}
	}

	static int last_leader = GAME_MAX_OBJS;
	if (last_leader != GAME_MAX_OBJS && GBSlots[0] != GAME_MAX_OBJS) {
		if (last_leader != GBSlots[0]) {
			GPlayer *p1 = (GPlayer *) objs[last_leader];
			GPlayer *p2 = (GPlayer *) objs[GBSlots[0]];

			if (objs[last_leader]->GetType() == ot_player
				&& p1->brain_type == bt_client
				&& p1->brain_owner ==
				client_info.client_num) smPlayVoice(SM_LOSTLEAD, 100, 80);
			if (objs[GBSlots[0]]->GetType() == ot_player
				&& p2->brain_type == bt_client
				&& p2->brain_owner ==
				client_info.client_num) smPlayVoice(SM_TAKENLEAD, 100, 80);
		}
	}
	last_leader = GBSlots[0];
}


void CGame::RenderPlayerBar(GPlayer * p, SDL_Surface * screen, int font,
							int px, int py)
{
	SDL_Rect r;
	SDL_Rect s;

	r.x = px;
	r.y = py;
	r.w = gamebar_playerbar->w;
	r.h = gamebar_playerbar->h;
	SDL_BlitSurface(gamebar_playerbar, NULL, screen, &r);

	r.x = px + 4;
	r.y = py + 4;
	r.w = 32;
	r.h = 32;
	s.x = 32 * p->frame;
	s.y = 0;
	s.w = 32;
	s.h = 32;
	SDL_BlitSurface(p->skin->anims[(Uint8) p->anim]->gfx, &s, screen, &r);

	char buf[40];
	p->player_name.GetValRef()->chars[p->player_name.GetValRef()->len -
									  1] = 0;
	DrawText(p->player_name.GetValRef()->chars, screen, font, px + 42,
			 py + 3);
	sprintf(buf, "%2d", (int) *p->frags.GetValRef());
	DrawText(buf, screen, font, px + 136, py + 22);
	sprintf(buf, "%04d", (int) *p->points.GetValRef());
	DrawText(buf, screen, font, px + 70, py + 22);
	sprintf(buf, "%3d", (int) *p->ping.GetValRef());
	DrawText(buf, screen, font, px + 129, py + 3);
}

void CGame::RenderGameBar(SDL_Surface * screen, int font, int px, int py)
{
	for (int i = 0; i < MAX_GAMEBAR_PLAYERS; i++) {
		if (GBSlots[i] == GAME_MAX_OBJS)
			break;
		if (objs[GBSlots[i]]->GetType() == ot_player
			&& (objs[GBSlots[i]]->state & OSTATE_ACTIVE)) {
			RenderPlayerBar((GPlayer *) objs[GBSlots[i]], screen, font, px,	py + 40 * i);
      if (strcmp(dbg_playerpos.string, ((GPlayer*)objs[GBSlots[i]])->player_name.GetValRef()->chars)==0)
      {
        char genstr[500];
        sprintf(genstr, "%s: [%d,%d]", dbg_playerpos.string, *((GPlayer *)objs[GBSlots[i]])->xpos.GetValRef(), *((GPlayer *)objs[GBSlots[i]])->ypos.GetValRef());
        DrawText(genstr, screen, 1, screen->w - 150, 369);	// consolefont should be 0
      }
		}
	}
}

void CGame::RenderStatusBar(SDL_Surface * screen, int font, int px, int py)
{
	SDL_Rect r;
	SDL_Rect s;

#define SB_BLINK_TIME 60		// 2s with FPS=30
	r.x = px;
	r.y = py;
	r.w = statusbar_bkg->w;
	r.h = statusbar_bkg->h;
  SDL_BlitSurface(statusbar_bkg, NULL, screen, &r);
	if (MainSlot != -1) {
		GPlayer *p = (GPlayer *) objs[MainSlot];
		if (p->state & OSTATE_ACTIVE) {
			r.w = 36;
			r.h = 25;
			s.w = 36;
			s.h = 25;
			r.y = py + 3;

			if (p->witems & WI_PISTOLKA) {
				if (!l1) {
					w1 = tick;
					l1 = true;
				}
				s.x = 36;
				if (tick - w1 < SB_BLINK_TIME && ((tick - w1) & 4))
					s.x = 0;

				r.x = px + 2;
				s.y = 0 * 25;
				SDL_BlitSurface(statusbar, &s, screen, &r);
			} else
				l1 = false;
			if (p->witems & WI_BECHEROMET) {
				if (!l2) {
					w2 = tick;
					l2 = true;
				}
				s.x = 36;
				if (tick - w2 < SB_BLINK_TIME && ((tick - w2) & 4))
					s.x = 0;
				r.x = px + 2 + 1 * 64;
				s.y = 1 * 25;
				SDL_BlitSurface(statusbar, &s, screen, &r);
			} else
				l2 = false;

			if (p->witems & WI_RAILGUN) {
				if (!l3) {
					w3 = tick;
					l3 = true;
				}
				s.x = 36;
				if (tick - w3 < SB_BLINK_TIME && ((tick - w3) & 4))
					s.x = 0;
				r.x = px + 2 + 2 * 64;
				s.y = 2 * 25;
				SDL_BlitSurface(statusbar, &s, screen, &r);
			} else
				l3 = false;

			if (p->witems & WI_BOMBS) {
				if (!l4) {
					w4 = tick;
					l4 = true;
				}
				s.x = 36;
				if (tick - w4 < SB_BLINK_TIME && ((tick - w4) & 4))
					s.x = 0;
				r.x = px + 2 + 3 * 64;
				s.y = 3 * 25;
				SDL_BlitSurface(statusbar, &s, screen, &r);
			} else
				l4 = false;

			if (p->witems & WI_GRENADES) {
				if (!l5) {
					w5 = tick;
					l5 = true;
				}
				s.x = 36;
				if (tick - w5 < SB_BLINK_TIME && ((tick - w5) & 4))
					s.x = 0;
				r.x = px + 2 + 4 * 64;
				s.y = 4 * 25;
				SDL_BlitSurface(statusbar, &s, screen, &r);
			} else
				l5 = false;

			if (p->shields) {
				if (h6 != p->shields) {
					w6 = tick;
				}
				s.x = 36;
				if (tick - w6 < SB_BLINK_TIME && ((tick - w6) & 4))
					s.x = 0;
				r.x = px + 2 + 5 * 64;
				s.y = 5 * 25;
				SDL_BlitSurface(statusbar, &s, screen, &r);
			}
			h6 = p->shields;

			if (p->warps) {
				if (h7 != p->warps) {
					w7 = tick;
				}
				s.x = 36;
				if (tick - w7 < SB_BLINK_TIME && ((tick - w7) & 4))
					s.x = 0;
				r.x = px + 2 + 6 * 64;
				s.y = 6 * 25;
				SDL_BlitSurface(statusbar, &s, screen, &r);
			}
			h7 = p->warps;

#define SBFSX 9
#define SBFSY 9

			char buf[40];
			sprintf(buf, "%03d", (int) *p->pistolka_ammo.GetValRef());
			DrawText(buf, screen, font, px + 1 * 32 + SBFSX, py + SBFSY);
			sprintf(buf, "%03d", (int) *p->becheromet_ammo.GetValRef());
			DrawText(buf, screen, font, px + 3 * 32 + SBFSX, py + SBFSY);
			sprintf(buf, "%03d", (int) *p->railgun_ammo.GetValRef());
			DrawText(buf, screen, font, px + 5 * 32 + SBFSX, py + SBFSY);
			sprintf(buf, "%03d", (int) *p->bombs_ammo.GetValRef());
			DrawText(buf, screen, font, px + 7 * 32 + SBFSX, py + SBFSY);
			sprintf(buf, "%03d", (int) *p->grenades_ammo.GetValRef());
			DrawText(buf, screen, font, px + 9 * 32 + SBFSX, py + SBFSY);
			sprintf(buf, "%03d", (int) *p->shields.GetValRef());
			DrawText(buf, screen, font, px + 11 * 32 + SBFSX, py + SBFSY);
			sprintf(buf, "%03d", (int) *p->warps.GetValRef());
			DrawText(buf, screen, font, px + 13 * 32 + SBFSX, py + SBFSY);

			sprintf(buf, "%02d", (int) *p->frags.GetValRef());
			DrawText(buf, screen, font, px + 552, py + SBFSY);
			sprintf(buf, "%04d", (int) *p->points.GetValRef());
			DrawText(buf, screen, font, px + 605, py + SBFSY);
		}
	}
}

#ifndef PI
#define PI (double)3.14159265
#endif

void CGame::RenderBecherBar(SDL_Surface * screen, int px, int py)
{
	SDL_Rect r;
	SDL_Rect s;
/*
  r.x = px;
  r.y = py;
  r.w = becherbart->w;
  r.h = becherbart->h;
  SDL_BlitSurface(becherbart, NULL, screen, &r);
*/
	if (BecherSlot != -1) {
		GExtra *e = (GExtra *) objs[BecherSlot];
		if ((e->state & OSTATE_ACTIVE) && (e->state & OSTATE_VISIBLE)) {
			if (MainSlot != -1) {
				GPlayer *p = (GPlayer *) objs[MainSlot];
				if (p->state & OSTATE_ACTIVE) {
					Sint16 dx = e->xpos - p->xpos;
					Sint16 dy = p->ypos - e->ypos;
					if (ABS(dx) > mappxwidth / 2)
						dx = (-SIGN(dx)) * (mappxwidth - ABS(dx));
					if (ABS(dy) > mappxheight / 2)
						dy = (-SIGN(dy)) * (mappxheight - ABS(dy));

					double len = (double) sqrt(SQR(dx) + SQR(dy));
					double ac = acos(dx / len);
					double as = asin(dy / len);
					double angle = as >= 0 ? ac : 2 * PI - ac;
					angle = angle + PI / 8;
//          while (angle>0) angle+=2*PI;
					int tile = (int) (angle / (PI / 4));
					while (tile < 0)
						tile += 8;
					while (tile >= 8)
						tile -= 8;

					s.x = 0;
					s.y = 23 * tile;
					s.w = 43;
					s.h = 23;
					r.x = px;
					r.y = py;
					r.w = 43;
					r.h = 23;
					SDL_BlitSurface(becherbar, &s, screen, &r);
				}
			}
		}
	}
}

// OBSOLETE - is not used
void CGame::RenderResults(SDL_Surface * screen, int font)
{
	char line[1024];

	int px = (int) g_results_x.value;
	int py = (int) g_results_y.value;
	int i;
	GPlayer *p;

	sprintf(line, "RESULTS             ");
	DrawText(line, screen, font, px, py);
	py += 13;
	sprintf(line, "-----------+--------");
	DrawText(line, screen, font, px, py);
	py += 13;
	sprintf(line, "  PLAYER     FRAGS  ");
	DrawText(line, screen, font, px, py);
	py += 13;
	sprintf(line, "-----------+--------");
	DrawText(line, screen, font, px, py);
	py += 13;

	for (i = 0; i < MAX_GAMEBAR_PLAYERS; i++) {
		if (GBSlots[i] == GAME_MAX_OBJS)
			break;
		if (objs[GBSlots[i]]->GetType() == ot_player) {
			p = (GPlayer *) objs[GBSlots[i]];
			sprintf(line, "%-10s +  %3d   ",
					p->player_name.GetValRef()->chars,
					(int) *p->frags.GetValRef());
			DrawText(line, screen, font, px, py);
			py += 13;
		}
	}
	sprintf(line, "-----------+--------");
	DrawText(line, screen, font, px, py);
}

char CGame::ServerAdjustPositions(int cnum, CReplicator & rep, TICK_TYPE time)
{
	for (GAME_MAXOBJS_TYPE i = 0; i < GAME_MAX_OBJS; i++) 
  {
    if (objs[i]->GetType()==ot_player && ((GPlayer*)objs[i])->brain_owner==cnum) 
    {
      GPlayer* player = (GPlayer*)objs[i];
      if (player->xpos.IsDirty(cnum) || player->ypos.IsDirty(cnum))
      {
        rep.SetLayer(1);
        // adjust position
        rep<<REP_ADJUSTPOSITION<<i<<player->xpos<<player->ypos<<time<<player->oid;
        rep.Mark();
        player->xpos.MakeDirty(cnum, false);
        player->ypos.MakeDirty(cnum, false);
      }
    }
	}

	return 1;
}

char CGame::ServerReplicate(int cnum, CReplicator & rep, TICK_TYPE time)
{
	// replicate data from server to client

	// TODO: choose relevant set of actors

	for (GAME_MAXOBJS_TYPE i = 0; i < GAME_MAX_OBJS; i++) {
		Uint8 ot = objs[i]->GetType();
		bool relevant = true;
/*
    if (vars.chasing!=-1)
    {
      if ((objs[i]->state&OSTATE_ACTIVE) && (objs[vars.chasing]->state&OSTATE_ACTIVE) 
        && objs[vars.chasing]->GetType()==ot_player && vars.chasing!=i)
      {
        GPlayer* p = (GPlayer*)objs[vars.chasing];
        if (ot==ot_player || ot==ot_bomb || ot==ot_rail || ot==ot_shot || ot==ot_grenade || 
          ot==ot_fray || ot==ot_entity || ot==ot_extra || ot==ot_bshot)
        {
          if (p->Destination((GEntity*)objs[i])>RELEVANT_DESTINATION*16) 
            relevant = false;
        }
      }
    } 
*/
    if (relevant || objs[i]->was_relevant) 
    {
      bool sd = rep.dirty;
      
      Uint8 layer = LAY_REPLICATION;
      Uint16 base;
      rep.SetLayer(layer);
      
      base = rep.pos[layer];
      rep << REP_REPLICATION << i << objs[i]->oid << ot;	// write REP_CODE header
      rep.dirty = false;
      
      objs[i]->Replication(cnum, &rep);
      // did the object any replication ?
      if (!rep.dirty) {
        rep.pos[layer] = base;	// clear REP_CODE
      } else {
        rep.Mark();		// mark data block
      }
      
      rep.dirty = sd || rep.dirty;
    }

		objs[i]->was_relevant = relevant;
	}

	return 1;
}

char CGame::ClientReplicate(TICK_TYPE time)
{
	// momentalne klient replikuje pouze svuj pohybovy vektor (stav)
	// to se deje v ramci CL_Move

	return 1;
}
