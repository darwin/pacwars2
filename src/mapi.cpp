/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   Map API
//## author:  Antonin Hildebrand
//## 
//## started: 12.3.2000
//## revised: ???
//## 
//##          safe map api for import to scripts
//## 
//###########################################################################

#include "mapi.h"
#include "seer/seer.h"
#include "server.h"
#include "client.h"

//###########################################################################
//## scripting externals
//###########################################################################

void AddMapiExternals()
{
	scAdd_External_Symbol("smGetBlockStr", smGetBlockStr);
	scAdd_External_Symbol("smSetBlockStr", smSetBlockStr);
	scAdd_External_Symbol("smGetAnimStr", smGetAnimStr);
	scAdd_External_Symbol("smSetAnimStr", smSetAnimStr);
	scAdd_External_Symbol("smSetMapLayer", smSetMapLayer);
	scAdd_External_Symbol("smGetMapVal", smGetMapVal);
	scAdd_External_Symbol("smGetMapBlock", smGetMapBlock);
	scAdd_External_Symbol("smGetMapAnim", smGetMapAnim);
	scAdd_External_Symbol("smSetMapVal", smSetMapVal);
	scAdd_External_Symbol("smGetMapBlock", smGetMapBlock);
	scAdd_External_Symbol("smGetMapWidth", smGetMapWidth);
	scAdd_External_Symbol("smGetMapHeiht", smGetMapHeight);

	scAdd_External_Symbol("cmGetBlockStr", cmGetBlockStr);
	scAdd_External_Symbol("cmSetBlockStr", cmSetBlockStr);
	scAdd_External_Symbol("cmGetAnimStr", cmGetAnimStr);
	scAdd_External_Symbol("cmSetAnimStr", cmSetAnimStr);
	scAdd_External_Symbol("cmSetMapLayer", cmSetMapLayer);
	scAdd_External_Symbol("cmGetMapVal", cmGetMapVal);
	scAdd_External_Symbol("cmGetMapBlock", cmGetMapBlock);
	scAdd_External_Symbol("cmGetMapAnim", cmGetMapAnim);
	scAdd_External_Symbol("cmSetMapVal", cmSetMapVal);
	scAdd_External_Symbol("cmGetMapBlock", cmGetMapBlock);
	scAdd_External_Symbol("cmGetMapWidth", cmGetMapWidth);
	scAdd_External_Symbol("cmGetMapHeiht", cmGetMapHeight);

	// map global vars
	scAdd_External_Symbol("MapLoaded", &MapLoaded);
	scAdd_External_Symbol("MapName", &MapName);

	scAdd_External_Symbol("MSCRW", &MSCRW);
	scAdd_External_Symbol("MSCRH", &MSCRH);

	scAdd_External_Symbol("MMOX", &MMOX);
	scAdd_External_Symbol("MMOY", &MMOY);

	scAdd_External_Symbol("maperror", &maperror);
	scAdd_External_Symbol("mapwidth", &mapwidth);
	scAdd_External_Symbol("mapheight", &mapheight);
	scAdd_External_Symbol("mappxwidth", &mappxwidth);
	scAdd_External_Symbol("mappxheight", &mappxheight);
	scAdd_External_Symbol("mapblockwidth", &mapblockwidth);
	scAdd_External_Symbol("mapblockheight", &mapblockheight);
	scAdd_External_Symbol("mapdepth", &mapdepth);
	scAdd_External_Symbol("mapblockstrsize", &mapblockstrsize);
	scAdd_External_Symbol("mapnumblockstr", &mapnumblockstr);
	scAdd_External_Symbol("mapnumanimstr", &mapnumanimstr);
	scAdd_External_Symbol("mapnumblockgfx", &mapnumblockgfx);
}

//###########################################################################
//## script map functions
//###########################################################################

//###########################################################################
//## SERVER SIDE
//###########################################################################

int smGetBlockStr(int strnum, SCBLK & block)
{
	if ((strnum < 0) || (strnum >= mapnumblockstr))
		return SC_INDEX_OUT_OF_RANGE;

	BLKSTR *sb =
		(BLKSTR *) (server_info.game.map.mapblockstrpt +
					strnum * sizeof(BLKSTR));

	// sand wall
	block.bgoff = sb->bgoff;
	block.fgoff = sb->fgoff;
	block.bl1 = sb->bl1;
	block.br1 = sb->br1;
	block.tl1 = sb->tl1;
	block.tr1 = sb->tr1;
	block.bl2 = sb->bl2;
	block.br2 = sb->br2;
	block.tl2 = sb->tl2;
	block.tr2 = sb->tr2;
	block.user1 = sb->user1;
	block.user2 = sb->user2;
	block.user3 = sb->user3;
	block.user4 = sb->user4;
	block.user5 = sb->user5;
	block.user6 = sb->user6;
	block.user7 = sb->user7;

	return SC_NOERR;
}

int smSetBlockStr(int strnum, SCBLK & block)
{
	if ((strnum < 0) || (strnum >= mapnumblockstr))
		return SC_INDEX_OUT_OF_RANGE;

	BLKSTR *sb =
		(BLKSTR *) (server_info.game.map.mapblockstrpt +
					strnum * sizeof(BLKSTR));

	// sand wall
	if ((block.bgoff >= 0) && (block.bgoff < mapnumblockgfx))
		sb->bgoff = block.bgoff;
	if ((block.fgoff >= 0) && (block.fgoff < mapnumblockgfx))
		sb->fgoff = block.fgoff;
	sb->bl1 = !!block.bl1;
	sb->br1 = !!block.br1;
	sb->tl1 = !!block.tl1;
	sb->tr1 = !!block.tr1;
	sb->bl2 = !!block.bl2;
	sb->br2 = !!block.br2;
	sb->tl2 = !!block.tl2;
	sb->tr2 = !!block.tr2;
	sb->user1 = block.user1;
	sb->user2 = block.user2;
	sb->user3 = block.user3;
	sb->user4 = block.user4;
	sb->user5 = block.user5;
	sb->user6 = block.user6;
	sb->user7 = block.user7;

	return SC_NOERR;
}

int smGetAnimStr(int strnum, SCANI & anim)
{
	ABS(strnum);
	if ((strnum < 0) || (strnum >= mapnumanimstr))
		return SC_INDEX_OUT_OF_RANGE;

	ANISTR *sa =
		(ANISTR *) (server_info.game.map.mapanimstrendpt -
					strnum * sizeof(ANISTR));

	// sand wall
	anim.antype = sa->antype;	// Type of anim, AN_? 
	anim.andelay = sa->andelay;	// Frames to go before next frame 
	anim.ancount = sa->ancount;	// Counter, decs each frame, till 0, then resets to andelay 
	anim.anuser = sa->anuser;	// User info 

	anim.curframe = (sa->ancuroff - sa->anstartoff) >> 2;
	anim.numframes = ((sa->anendoff - sa->anstartoff) >> 2) + 1;	// read-only 

	return SC_NOERR;
}

int smSetAnimStr(int strnum, SCANI & anim)
{
	ABS(strnum);
	if ((strnum < 0) || (strnum >= mapnumanimstr))
		return SC_INDEX_OUT_OF_RANGE;

	ANISTR *sa =
		(ANISTR *) (server_info.game.map.mapanimstrendpt -
					strnum * sizeof(ANISTR));

	// sand wall
	if ((anim.antype >= AN_NONE) && (anim.antype <= AN_ONCES))
		sa->antype = anim.antype;	// Type of anim, AN_? 
	if (anim.andelay >= 0)
		sa->andelay = anim.andelay;	// Frames to go before next frame 
	if (anim.ancount >= 0)
		sa->ancount = anim.ancount;	// Counter, decs each frame, till 0, then resets to andelay 
	sa->anuser = anim.anuser;	// User info 

	long int cf = sa->anstartoff + (anim.curframe << 2);
	if ((cf <= sa->anendoff) && (cf >= sa->anstartoff))
		sa->ancuroff = cf;

	return SC_NOERR;
}

int smSetMapLayer(int laynum)
{
	if (server_info.game.map.ChangeLayer(laynum) == -1)
		return SC_LAYER_UNDEFINED;
	else
		return SC_NOERR;
}

int smGetMapVal(int x, int y, int &val)
{
	if ((x < 0) || (x >= mapwidth) || (y < 0) || (y >= mapheight))
		return SC_MAPTILE_OUT_OF_RANGE;

	short int *mymappt;
	if (server_info.game.map.maparraypt) {
		mymappt = server_info.game.map.maparraypt[y] + x;
	} else {
		mymappt = server_info.game.map.mappt;
		mymappt += x;
		mymappt += y * mapwidth;
	}

	if (*mymappt >= 0)
		val = (*mymappt) / sizeof(BLKSTR);
	else
		val = (*mymappt) / sizeof(ANISTR);

	return SC_NOERR;
}

int smGetMapBlock(int x, int y, SCBLK & block)
{
	int err;
	int blknum;

	err = smGetMapVal(x, y, blknum);
	if (err)
		return err;
	if (blknum < 0)
		return SC_TILE_IS_ANIMATED;

	err = smGetBlockStr(blknum, block);
	return err;
}

int smGetMapAnim(int x, int y, SCANI & anim)
{
	int err;
	int aninum;

	err = smGetMapVal(x, y, aninum);
	if (err)
		return err;
	if (aninum >= 0)
		return SC_TILE_IS_STILL;

	err = smGetAnimStr(aninum, anim);
	return err;
}

int smTrsMapBlockVal(int x, int y, int &val)
{
	ANISTR *myanpt;
	if ((x < 0) || (x >= mapwidth) || (y < 0) || (y >= mapheight))
		return SC_MAPTILE_OUT_OF_RANGE;

	short int *mymappt;
	if (server_info.game.map.maparraypt) {
		mymappt = server_info.game.map.maparraypt[y] + x;
	} else {
		mymappt = server_info.game.map.mappt;
		mymappt += x;
		mymappt += y * mapwidth;
	}

	if (*mymappt >= 0)
		val = (*mymappt) / sizeof(BLKSTR);
	else {
		myanpt =
			(ANISTR *) (server_info.game.map.mapanimstrendpt + *mymappt);
		val =
			((BLKSTR *) (myanpt->ancuroff)) -
			(BLKSTR *) server_info.game.map.mapblockstrpt;
	}
	return SC_NOERR;
}

int smTrsMapBlock(int x, int y, SCBLK & block)
{
	int err;
	int blknum;

	err = smTrsMapBlockVal(x, y, blknum);
	if (err)
		return err;

	err = smGetBlockStr(blknum, block);
	return err;
}


int smSetMapVal(int x, int y, int val)
{
	if ((x < 0) || (x >= mapwidth) || (y < 0) || (y >= mapheight))
		return SC_MAPTILE_OUT_OF_RANGE;
	if (((val >= 0) && (val >= mapnumblockstr)) || ((val < 0)
													&& (val <
														-mapnumanimstr)))
			return SC_INDEX_OUT_OF_RANGE;

	short int *mymappt;
	if (server_info.game.map.maparraypt) {
		mymappt = server_info.game.map.maparraypt[y] + x;
	} else {
		mymappt = server_info.game.map.mappt;
		mymappt += x;
		mymappt += y * mapwidth;
	}

	if (val >= 0)
		*mymappt = val * sizeof(BLKSTR);
	else
		*mymappt = val * sizeof(ANISTR);

	return SC_NOERR;
}

int smGetMapWidth(int &w)
{
	w = mapwidth;
	return SC_NOERR;
}

int smGetMapHeight(int &h)
{
	h = mapheight;
	return SC_NOERR;
}

//###########################################################################
//## CLIENT SIDE
//###########################################################################

int cmGetBlockStr(int strnum, SCBLK & block)
{
	if ((strnum < 0) || (strnum >= mapnumblockstr))
		return SC_INDEX_OUT_OF_RANGE;

	BLKSTR *sb =
		(BLKSTR *) (client_info.game.map.mapblockstrpt +
					strnum * sizeof(BLKSTR));

	// sand wall
	block.bgoff = sb->bgoff;
	block.fgoff = sb->fgoff;
	block.bl1 = sb->bl1;
	block.br1 = sb->br1;
	block.tl1 = sb->tl1;
	block.tr1 = sb->tr1;
	block.bl2 = sb->bl2;
	block.br2 = sb->br2;
	block.tl2 = sb->tl2;
	block.tr2 = sb->tr2;
	block.user1 = sb->user1;
	block.user2 = sb->user2;
	block.user3 = sb->user3;
	block.user4 = sb->user4;
	block.user5 = sb->user5;
	block.user6 = sb->user6;
	block.user7 = sb->user7;

	return SC_NOERR;
}

int cmSetBlockStr(int strnum, SCBLK & block)
{
	if ((strnum < 0) || (strnum >= mapnumblockstr))
		return SC_INDEX_OUT_OF_RANGE;

	BLKSTR *sb =
		(BLKSTR *) (client_info.game.map.mapblockstrpt +
					strnum * sizeof(BLKSTR));

	// sand wall
	if ((block.bgoff >= 0) && (block.bgoff < mapnumblockgfx))
		sb->bgoff = block.bgoff;
	if ((block.fgoff >= 0) && (block.fgoff < mapnumblockgfx))
		sb->fgoff = block.fgoff;
	sb->bl1 = !!block.bl1;
	sb->br1 = !!block.br1;
	sb->tl1 = !!block.tl1;
	sb->tr1 = !!block.tr1;
	sb->bl2 = !!block.bl2;
	sb->br2 = !!block.br2;
	sb->tl2 = !!block.tl2;
	sb->tr2 = !!block.tr2;
	sb->user1 = block.user1;
	sb->user2 = block.user2;
	sb->user3 = block.user3;
	sb->user4 = block.user4;
	sb->user5 = block.user5;
	sb->user6 = block.user6;
	sb->user7 = block.user7;

	return SC_NOERR;
}

int cmGetAnimStr(int strnum, SCANI & anim)
{
	ABS(strnum);
	if ((strnum < 0) || (strnum >= mapnumanimstr))
		return SC_INDEX_OUT_OF_RANGE;

	ANISTR *sa =
		(ANISTR *) (client_info.game.map.mapanimstrendpt -
					strnum * sizeof(ANISTR));

	// sand wall
	anim.antype = sa->antype;	// Type of anim, AN_? 
	anim.andelay = sa->andelay;	// Frames to go before next frame 
	anim.ancount = sa->ancount;	// Counter, decs each frame, till 0, then resets to andelay 
	anim.anuser = sa->anuser;	// User info 

	anim.curframe = (sa->ancuroff - sa->anstartoff) >> 2;
	anim.numframes = ((sa->anendoff - sa->anstartoff) >> 2) + 1;	// read-only 

	return SC_NOERR;
}

int cmSetAnimStr(int strnum, SCANI & anim)
{
	ABS(strnum);
	if ((strnum < 0) || (strnum >= mapnumanimstr))
		return SC_INDEX_OUT_OF_RANGE;

	ANISTR *sa =
		(ANISTR *) (client_info.game.map.mapanimstrendpt -
					strnum * sizeof(ANISTR));

	// sand wall
	if ((anim.antype >= AN_NONE) && (anim.antype <= AN_ONCES))
		sa->antype = anim.antype;	// Type of anim, AN_? 
	if (anim.andelay >= 0)
		sa->andelay = anim.andelay;	// Frames to go before next frame 
	if (anim.ancount >= 0)
		sa->ancount = anim.ancount;	// Counter, decs each frame, till 0, then resets to andelay 
	sa->anuser = anim.anuser;	// User info 

	long int cf = sa->anstartoff + (anim.curframe << 2);
	if ((cf <= sa->anendoff) && (cf >= sa->anstartoff))
		sa->ancuroff = cf;

	return SC_NOERR;
}

int cmSetMapLayer(int laynum)
{
	if (client_info.game.map.ChangeLayer(laynum) == -1)
		return SC_LAYER_UNDEFINED;
	else
		return SC_NOERR;
}

int cmGetMapVal(int x, int y, int &val)
{
	if ((x < 0) || (x >= mapwidth) || (y < 0) || (y >= mapheight))
		return SC_MAPTILE_OUT_OF_RANGE;

	short int *mymappt;
	if (client_info.game.map.maparraypt) {
		mymappt = client_info.game.map.maparraypt[y] + x;
	} else {
		mymappt = client_info.game.map.mappt;
		mymappt += x;
		mymappt += y * mapwidth;
	}

	if (*mymappt >= 0)
		val = (*mymappt) / sizeof(BLKSTR);
	else
		val = (*mymappt) / sizeof(ANISTR);

	return SC_NOERR;
}

int cmGetMapBlock(int x, int y, SCBLK & block)
{
	int err;
	int blknum;

	err = cmGetMapVal(x, y, blknum);
	if (err)
		return err;
	if (blknum < 0)
		return SC_TILE_IS_ANIMATED;

	err = cmGetBlockStr(blknum, block);
	return err;
}

int cmGetMapAnim(int x, int y, SCANI & anim)
{
	int err;
	int aninum;

	err = cmGetMapVal(x, y, aninum);
	if (err)
		return err;
	if (aninum >= 0)
		return SC_TILE_IS_STILL;

	err = cmGetAnimStr(aninum, anim);
	return err;
}

int cmTrsMapBlockVal(int x, int y, int &val)
{
	ANISTR *myanpt;
	if ((x < 0) || (x >= mapwidth) || (y < 0) || (y >= mapheight))
		return SC_MAPTILE_OUT_OF_RANGE;

	short int *mymappt;
	if (client_info.game.map.maparraypt) {
		mymappt = client_info.game.map.maparraypt[y] + x;
	} else {
		mymappt = client_info.game.map.mappt;
		mymappt += x;
		mymappt += y * mapwidth;
	}

	if (*mymappt >= 0)
		val = (*mymappt) / sizeof(BLKSTR);
	else {
		myanpt =
			(ANISTR *) (client_info.game.map.mapanimstrendpt + *mymappt);
		val =
			((BLKSTR *) (myanpt->ancuroff)) -
			(BLKSTR *) client_info.game.map.mapblockstrpt;
	}
	return SC_NOERR;
}

int cmTrsMapBlock(int x, int y, SCBLK & block)
{
	int err;
	int blknum;

	err = cmTrsMapBlockVal(x, y, blknum);
	if (err)
		return err;

	err = cmGetBlockStr(blknum, block);
	return err;
}


int cmSetMapVal(int x, int y, int val)
{
	if ((x < 0) || (x >= mapwidth) || (y < 0) || (y >= mapheight))
		return SC_MAPTILE_OUT_OF_RANGE;
	if (((val >= 0) && (val >= mapnumblockstr)) || ((val < 0)
													&& (val <
														-mapnumanimstr)))
			return SC_INDEX_OUT_OF_RANGE;

	short int *mymappt;
	if (client_info.game.map.maparraypt) {
		mymappt = client_info.game.map.maparraypt[y] + x;
	} else {
		mymappt = client_info.game.map.mappt;
		mymappt += x;
		mymappt += y * mapwidth;
	}

	if (val >= 0)
		*mymappt = val * sizeof(BLKSTR);
	else
		*mymappt = val * sizeof(ANISTR);

	return SC_NOERR;
}

int cmGetMapWidth(int &w)
{
	w = mapwidth;
	return SC_NOERR;
}

int cmGetMapHeight(int &h)
{
	h = mapheight;
	return SC_NOERR;
}
