/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   Object API
//## author:  Antonin Hildebrand
//## 
//## started: 25.6.2000
//## revised: ???
//## 
//##          safe object api for import to scripts
//## 
//###########################################################################

#include "oapi.h"
#include "seer.h"
#include "server.h"
#include "client.h"
#include "game.h"

#include "pac.h"

//###########################################################################
//## scripting externals
//###########################################################################

void AddOapiExternals()
{
	scAdd_External_Symbol("soBornPlayer", (void*)soBornPlayer);
	scAdd_External_Symbol("soGetPlayer", (void*)soGetPlayer);
	scAdd_External_Symbol("soSetPlayer", (void*)soSetPlayer);
	scAdd_External_Symbol("soBornShot", (void*)soBornShot);
	scAdd_External_Symbol("soGetShot", (void*)soGetShot);
	scAdd_External_Symbol("soSetShot", (void*)soSetShot);
	scAdd_External_Symbol("soBornExtra", (void*)soBornExtra);
	scAdd_External_Symbol("soGetExtra", (void*)soGetExtra);
	scAdd_External_Symbol("soSetExtra", (void*)soSetExtra);
	scAdd_External_Symbol("soBornBomb", (void*)soBornBomb);
	scAdd_External_Symbol("soGetBomb", (void*)soGetBomb);
	scAdd_External_Symbol("soSetBomb", (void*)soSetBomb);

	scAdd_External_Symbol("coBornPlayer", (void*)coBornPlayer);
	scAdd_External_Symbol("coGetPlayer", (void*)coGetPlayer);
	scAdd_External_Symbol("coSetPlayer", (void*)coSetPlayer);
	scAdd_External_Symbol("coBornShot", (void*)coBornShot);
	scAdd_External_Symbol("coGetShot", (void*)coGetShot);
	scAdd_External_Symbol("coSetShot", (void*)coSetShot);
	scAdd_External_Symbol("coBornExtra", (void*)coBornExtra);
	scAdd_External_Symbol("coGetExtra", (void*)coGetExtra);
	scAdd_External_Symbol("coSetExtra", (void*)coSetExtra);
	scAdd_External_Symbol("coBornBomb", (void*)coBornBomb);
	scAdd_External_Symbol("coGetBomb", (void*)coGetBomb);
	scAdd_External_Symbol("coSetBomb", (void*)coSetBomb);
}

//###########################################################################
//## script sync functions
//###########################################################################

void ExportObj(GObj * go, SCOBJ * so)
{
	so->state = go->state;
	so->createtime = go->createtime;
	so->curtime = go->curtime;
	so->oid = go->oid;
	so->slot = go->slot;
}

void ImportObj(GObj * go, SCOBJ * so)
{
	go->state = so->state;
	go->createtime = so->createtime;
	go->curtime = so->curtime;
	go->oid = so->oid;
	go->slot = so->slot;
}

void ExportEntity(GEntity * ge, SCENTITY * se)
{
	ExportObj((GObj *) ge, &se->obj);

	se->anim = ge->anim;
	se->frame = ge->frame;
	se->framedelay = ge->framedelay;
//  se->radius = ge->radius;
//  se->xaccel = ge->xaccel;
	se->xpos = ge->xpos;
	se->xsize = ge->xsize;
//  se->xspeed = ge->xspeed;
//  se->yaccel = ge->yaccel;
	se->ypos = ge->ypos;
	se->ysize = ge->ysize;
//  se->yspeed = ge->yspeed;
}

void ImportEntity(GEntity * ge, SCENTITY * se)
{
	ImportObj((GObj *) ge, &se->obj);

	ge->anim = se->anim;
	ge->frame = se->frame;
	ge->framedelay = se->framedelay;
//  ge->radius = se->radius;
//  ge->xaccel = se->xaccel;
	ge->xpos = se->xpos;
	ge->xsize = se->xsize;
//  ge->xspeed = se->xspeed;
//  ge->yaccel = se->yaccel;
	ge->ypos = se->ypos;
	ge->ysize = se->ysize;
//  ge->yspeed = se->yspeed;
}

void ExportPlayer(GPlayer * gp, SCPLAYER * sp)
{
	ExportEntity((GEntity *) gp, &sp->entity);

	sp->adjusttime = gp->adjusttime;
//  sp->becher_ammo = gp->becher_ammo;
	sp->bombs_ammo = gp->bombs_ammo;
	sp->brain_owner = gp->brain_owner;
	sp->brain_type = gp->brain_type;
	sp->direction = gp->direction;
	sp->frags = gp->frags;
	sp->kbindex = gp->kbindex;
	sp->last_bomb_time = gp->last_bomb_time;
//  sp->last_fire_time = gp->last_fire_time;
//  sp->mines_ammo = gp->mines_ammo;
	sp->player_name = PString(gp->player_name);
}

void ImportPlayer(GPlayer * gp, SCPLAYER * sp)
{
	ImportEntity((GEntity *) gp, &sp->entity);

	gp->adjusttime = sp->adjusttime;
//  gp->becher_ammo = sp->becher_ammo;
	gp->bombs_ammo = sp->bombs_ammo;
	gp->brain_owner = sp->brain_owner;
	gp->brain_type = sp->brain_type;
	gp->direction = sp->direction;
	gp->frags = sp->frags;
	gp->kbindex = sp->kbindex;
	gp->last_bomb_time = sp->last_bomb_time;
//  gp->last_fire_time = sp->last_fire_time;
//  gp->mines_ammo = sp->mines_ammo;
	gp->player_name = sp->player_name;
}

void ExportExtra(GExtra * ge, SCEXTRA * se)
{
	ExportEntity((GEntity *) ge, &se->entity);

	se->data = ge->data;
	se->life = ge->life;
	se->modifier = ge->modifier;
	se->type = ge->type;
}

void ImportExtra(GExtra * ge, SCEXTRA * se)
{
	ImportEntity((GEntity *) ge, &se->entity);

	ge->data = se->data;
	ge->life = se->life;
	ge->modifier = se->modifier;
	ge->type = se->type;
}

void ExportShot(GShot * gs, SCSHOT * ss)
{
	ExportEntity((GEntity *) gs, &ss->entity);

	ss->bounced = gs->bounced;
	ss->dx = gs->dx;
	ss->owner = gs->owner;
}

void ImportShot(GShot * gs, SCSHOT * ss)
{
	ImportEntity((GEntity *) gs, &ss->entity);

  gs->bounced = ss->bounced?true:false;
	gs->dx = ss->dx;
	gs->owner = ss->owner;
}

void ExportBomb(GBomb * gb, SCBOMB * sb)
{
	ExportEntity((GEntity *) gb, &sb->entity);

	sb->owner = gb->owner;
}

void ImportBomb(GBomb * gb, SCBOMB * sb)
{
	ImportEntity((GEntity *) gb, &sb->entity);

	gb->owner = sb->owner;
}



//###########################################################################
//## SERVER SIDE
//###########################################################################

int soBornPlayer(SCPLAYER & player)
{
	// TODO: validity checks
	server_info.game.IncrementLOID();
	GAME_MAXOBJS_TYPE slot = server_info.game.FindUnusedSlot();
	server_info.game.RebornObject(server_info.game.last_oid, slot,
								  ot_player);

	soGetPlayer(slot, player);

	return slot;
}

int soGetPlayer(int slot, SCPLAYER & player)
{
	// TODO: validity checks
	if (server_info.game.objs[slot]->state & OSTATE_ACTIVE) {
		if (server_info.game.objs[slot]->GetType() == ot_player) {
			GPlayer *p = (GPlayer *) server_info.game.objs[slot];
			ExportPlayer(p, &player);

			return SC_NOERR;
		} else
			return SC_OBJECTTYPEMISMATCH;
	} else
		return SC_OBJECTNOTACTIVE;
}

int soSetPlayer(int slot, SCPLAYER & player)
{
	// TODO: validity checks
	if (server_info.game.objs[slot]->GetType() == ot_player) {
		GPlayer *p = (GPlayer *) server_info.game.objs[slot];
		ImportPlayer(p, &player);

		return SC_NOERR;
	} else
		return SC_OBJECTTYPEMISMATCH;
}

int soBornShot(SCSHOT & shot)
{
	// TODO: validity checks
	server_info.game.IncrementLOID();
	GAME_MAXOBJS_TYPE slot = server_info.game.FindUnusedSlot();
	server_info.game.RebornObject(server_info.game.last_oid, slot,
								  ot_shot);

	soGetShot(slot, shot);

	return slot;
}

int soGetShot(int slot, SCSHOT & shot)
{
	// TODO: validity checks
	if (server_info.game.objs[slot]->state & OSTATE_ACTIVE) {
		if (server_info.game.objs[slot]->GetType() == ot_shot) {
			GShot *p = (GShot *) server_info.game.objs[slot];
			ExportShot(p, &shot);

			return SC_NOERR;
		} else
			return SC_OBJECTTYPEMISMATCH;
	} else
		return SC_OBJECTNOTACTIVE;
}

int soSetShot(int slot, SCSHOT & shot)
{
	// TODO: validity checks
	if (server_info.game.objs[slot]->GetType() == ot_shot) {
		GShot *p = (GShot *) server_info.game.objs[slot];
		ImportShot(p, &shot);

		return SC_NOERR;
	} else
		return SC_OBJECTTYPEMISMATCH;
}


int soBornExtra(SCEXTRA & extra)
{
	// TODO: validity checks
	server_info.game.IncrementLOID();
	GAME_MAXOBJS_TYPE slot = server_info.game.FindUnusedSlot();
	server_info.game.RebornObject(server_info.game.last_oid, slot,
								  ot_extra);

	soGetExtra(slot, extra);

	return slot;
}

int soGetExtra(int slot, SCEXTRA & extra)
{
	// TODO: validity checks
	if (server_info.game.objs[slot]->state & OSTATE_ACTIVE) {
		if (server_info.game.objs[slot]->GetType() == ot_extra) {
			GExtra *p = (GExtra *) server_info.game.objs[slot];
			ExportExtra(p, &extra);

			return SC_NOERR;
		} else
			return SC_OBJECTTYPEMISMATCH;
	} else
		return SC_OBJECTNOTACTIVE;
}

int soSetExtra(int slot, SCEXTRA & extra)
{
	// TODO: validity checks
	if (server_info.game.objs[slot]->GetType() == ot_extra) {
		GExtra *p = (GExtra *) server_info.game.objs[slot];
		ImportExtra(p, &extra);

		return SC_NOERR;
	} else
		return SC_OBJECTTYPEMISMATCH;
}


int soBornBomb(SCBOMB & bomb)
{
	// TODO: validity checks
	server_info.game.IncrementLOID();
	GAME_MAXOBJS_TYPE slot = server_info.game.FindUnusedSlot();
	server_info.game.RebornObject(server_info.game.last_oid, slot,
								  ot_bomb);

	soGetBomb(slot, bomb);

	return slot;
}

int soGetBomb(int slot, SCBOMB & bomb)
{
	// TODO: validity checks
	if (server_info.game.objs[slot]->state & OSTATE_ACTIVE) {
		if (server_info.game.objs[slot]->GetType() == ot_bomb) {
			GBomb *p = (GBomb *) server_info.game.objs[slot];
			ExportBomb(p, &bomb);

			return SC_NOERR;
		} else
			return SC_OBJECTTYPEMISMATCH;
	} else
		return SC_OBJECTNOTACTIVE;
}

int soSetBomb(int slot, SCBOMB & bomb)
{
	// TODO: validity checks
	if (server_info.game.objs[slot]->GetType() == ot_bomb) {
		GBomb *p = (GBomb *) server_info.game.objs[slot];
		ImportBomb(p, &bomb);

		return SC_NOERR;
	} else
		return SC_OBJECTTYPEMISMATCH;
}


//###########################################################################
//## CLIENT SIDE
//###########################################################################

int coBornPlayer(SCPLAYER & player)
{
	// TODO: validity checks
	client_info.game.IncrementLOID();
	GAME_MAXOBJS_TYPE slot = client_info.game.FindUnusedSlot();
	client_info.game.RebornObject(client_info.game.last_oid, slot,
								  ot_player);

	coGetPlayer(slot, player);

	return slot;
}

int coGetPlayer(int slot, SCPLAYER & player)
{
	// TODO: validity checks
	if (client_info.game.objs[slot]->state & OSTATE_ACTIVE) {
		if (client_info.game.objs[slot]->GetType() == ot_player) {
			GPlayer *p = (GPlayer *) client_info.game.objs[slot];
			ExportPlayer(p, &player);

			return SC_NOERR;
		} else
			return SC_OBJECTTYPEMISMATCH;
	} else
		return SC_OBJECTNOTACTIVE;
}

int coSetPlayer(int slot, SCPLAYER & player)
{
	// TODO: validity checks
	if (client_info.game.objs[slot]->GetType() == ot_player) {
		GPlayer *p = (GPlayer *) client_info.game.objs[slot];
		ImportPlayer(p, &player);

		return SC_NOERR;
	} else
		return SC_OBJECTTYPEMISMATCH;
}

int coBornShot(SCSHOT & shot)
{
	// TODO: validity checks
	client_info.game.IncrementLOID();
	GAME_MAXOBJS_TYPE slot = client_info.game.FindUnusedSlot();
	client_info.game.RebornObject(client_info.game.last_oid, slot,
								  ot_shot);

	coGetShot(slot, shot);

	return slot;
}

int coGetShot(int slot, SCSHOT & shot)
{
	// TODO: validity checks
	if (client_info.game.objs[slot]->state & OSTATE_ACTIVE) {
		if (client_info.game.objs[slot]->GetType() == ot_shot) {
			GShot *p = (GShot *) client_info.game.objs[slot];
			ExportShot(p, &shot);

			return SC_NOERR;
		} else
			return SC_OBJECTTYPEMISMATCH;
	} else
		return SC_OBJECTNOTACTIVE;
}

int coSetShot(int slot, SCSHOT & shot)
{
	// TODO: validity checks
	if (client_info.game.objs[slot]->GetType() == ot_shot) {
		GShot *p = (GShot *) client_info.game.objs[slot];
		ImportShot(p, &shot);

		return SC_NOERR;
	} else
		return SC_OBJECTTYPEMISMATCH;
}


int coGetExtra(int slot, SCEXTRA & extra)
{
	// TODO: validity checks
	if (client_info.game.objs[slot]->state & OSTATE_ACTIVE) {
		if (client_info.game.objs[slot]->GetType() == ot_extra) {
			GExtra *p = (GExtra *) client_info.game.objs[slot];
			ExportExtra(p, &extra);

			return SC_NOERR;
		} else
			return SC_OBJECTTYPEMISMATCH;
	} else
		return SC_OBJECTNOTACTIVE;
}

int coBornExtra(SCEXTRA & extra)
{
	// TODO: validity checks
	client_info.game.IncrementLOID();
	GAME_MAXOBJS_TYPE slot = client_info.game.FindUnusedSlot();
	client_info.game.RebornObject(client_info.game.last_oid, slot,
								  ot_extra);

	coGetExtra(slot, extra);

	return slot;
}

int coSetExtra(int slot, SCEXTRA & extra)
{
	// TODO: validity checks
	if (client_info.game.objs[slot]->GetType() == ot_extra) {
		GExtra *p = (GExtra *) client_info.game.objs[slot];
		ImportExtra(p, &extra);

		return SC_NOERR;
	} else
		return SC_OBJECTTYPEMISMATCH;
}


int coBornBomb(SCBOMB & bomb)
{
	// TODO: validity checks
	client_info.game.IncrementLOID();
	GAME_MAXOBJS_TYPE slot = client_info.game.FindUnusedSlot();
	client_info.game.RebornObject(client_info.game.last_oid, slot,
								  ot_bomb);

	coGetBomb(slot, bomb);

	return slot;
}

int coGetBomb(int slot, SCBOMB & bomb)
{
	// TODO: validity checks
	if (client_info.game.objs[slot]->state & OSTATE_ACTIVE) {
		if (client_info.game.objs[slot]->GetType() == ot_bomb) {
			GBomb *p = (GBomb *) client_info.game.objs[slot];
			ExportBomb(p, &bomb);

			return SC_NOERR;
		} else
			return SC_OBJECTTYPEMISMATCH;
	} else
		return SC_OBJECTNOTACTIVE;
}

int coSetBomb(int slot, SCBOMB & bomb)
{
	// TODO: validity checks
	if (client_info.game.objs[slot]->GetType() == ot_bomb) {
		GBomb *p = (GBomb *) client_info.game.objs[slot];
		ImportBomb(p, &bomb);

		return SC_NOERR;
	} else
		return SC_OBJECTTYPEMISMATCH;
}
