/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   Map system
//## author:  Robin Burrows
//##          Antonin Hildebrand
//##         
//## started: 7.3.2000
//## revised: 12.3.2000
//##
//##          Based on Mappy by Robin Burrows
//##          Ported under SDL by rewriting 
//##          MAPPYDX.CPP & MAPPYDX.H by Robin Burrows     
//## 
//## remarks: 8pbb display support removed
//## 
//###########################################################################

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL.h"
#include "SDL_image.h"
#include "const.h"
#include "SDL_Console.h"
#include "map.h"
#include "scripting.h"

//#define new DEBUG_NEW

//###########################################################################
//## Globals
//## All global variables used are here 
//###########################################################################
int MSCRW = 480;
int MSCRH = 480;

// These define how far from the left and top of the surface the drawn area is 
int MMOX = 0;
int MMOY = 0;

char MapLoaded = 0;
char MapName[_MAX_PATH] = "none";
char ScriptName[_MAX_PATH] = "none";

int maperror;					// Set to a MER_ error if something wrong happens 
int mapwidth;
int mapheight;
int mappxwidth;
int mappxheight;
int mapblockwidth;
int mapblockheight;
int mapdepth;
int mapblockstrsize;
int mapnumblockstr;
int mapnumanimstr;
int mapnumblockgfx;

int map_script_num;				// index to script manager

FILE *mapfilept;

GENHEAD mapgenheader;
unsigned char *mapcmappt = NULL;
SDL_Surface *mapTiles[1024];	// Tiles SDL Draw Surfaces
char *mapblockgfxpt = NULL;		// loaded graphics
Uint32 transpcol;

CMapState MapLoadedState;

// End of globals 

//###########################################################################
//## Releasing map
//###########################################################################

void MapFreeMem(void)
{
	int i;

	MapLoadedState.Free();

	if (mapcmappt != NULL) {
		free(mapcmappt);
		mapcmappt = NULL;
	}
	if (mapblockgfxpt != NULL) {
		free(mapblockgfxpt);
		mapblockgfxpt = NULL;
	}

	for (i = 0; mapTiles[i]; i++) {
		SDL_FreeSurface(mapTiles[i]);
		mapTiles[i] = NULL;
	}

	if (map_script_num != -1) {
		ScriptMan->DeleteScript(map_script_num);	// unload old script
		map_script_num = -1;
	}
	strcpy(MapName, "none");
	//strcpy(ScriptName, "none");

	MapLoaded = 0;
}

//###########################################################################
//## Helpers
//###########################################################################

unsigned long int Mapbyteswapl(unsigned long int i)
{
	unsigned long int j;
	j = i & 0xFF;
	j <<= 8;
	i >>= 8;
	j |= i & 0xFF;
	j <<= 8;
	i >>= 8;
	j |= i & 0xFF;
	j <<= 8;
	i >>= 8;
	j |= i & 0xFF;
	return j;
}

Uint32 MapTranspToPixel(SDL_PixelFormat * pf)
{
	Uint32 transpresult;
	unsigned int i, j, rshift, gshift, bshift, rbits, gbits, bbits;

	transpresult = 0;
	if (pf->BitsPerPixel > 8) {
		j = (int) pf->Rmask;
		rshift = 0;
		i = 1;
		while (!(i & j)) {
			rshift++;
			i <<= 1;
		}
		rbits = 0;
		while (i & j) {
			rbits++;
			i <<= 1;
		}
		j = (int) pf->Gmask;
		gshift = 0;
		i = 1;
		while (!(i & j)) {
			gshift++;
			i <<= 1;
		}
		gbits = 0;
		while (i & j) {
			gbits++;
			i <<= 1;
		}
		j = (int) pf->Bmask;
		bshift = 0;
		i = 1;
		while (!(i & j)) {
			bshift++;
			i <<= 1;
		}
		bbits = 0;
		while (i & j) {
			bbits++;
			i <<= 1;
		}

		transpresult =
			(((MTRANSPR << rshift) >> (8 - rbits)) & pf->Rmask) |
			(((MTRANSPG << gshift) >> (8 - gbits)) & pf->Gmask) |
			(((MTRANSPB << bshift) >> (8 - bbits)) & pf->Bmask);
	}
	return transpresult;
}

//###########################################################################
//## Map initialization
//###########################################################################

int MapCopyBlocksToSurfaces(void)
{
	int i, j;
	unsigned int k, rshift, gshift, bshift, rbits, gbits, bbits, rval,
		gval, bval;
	unsigned char *tempblockgfxpt, *tempsurfacept;
	SDL_Surface *mss;
	SDL_PixelFormat *mpf;

	tempblockgfxpt = (unsigned char *) mapblockgfxpt;
	mpf = mapTiles[0]->format;
	if (mpf->BitsPerPixel > 8) {
		j = (int) mpf->Rmask;
		rshift = 0;
		i = 1;
		while (!(i & j)) {
			rshift++;
			i <<= 1;
		}
		rbits = 0;
		while (i & j) {
			rbits++;
			i <<= 1;
		}
		j = (int) mpf->Gmask;
		gshift = 0;
		i = 1;
		while (!(i & j)) {
			gshift++;
			i <<= 1;
		}
		gbits = 0;
		while (i & j) {
			gbits++;
			i <<= 1;
		}
		j = (int) mpf->Bmask;
		bshift = 0;
		i = 1;
		while (!(i & j)) {
			bshift++;
			i <<= 1;
		}
		bbits = 0;
		while (i & j) {
			bbits++;
			i <<= 1;
		}
	}

	for (i = 0; i < mapnumblockgfx; i++) {
		mss = mapTiles[i];
		if ((SDL_MUSTLOCK(mss)) && (SDL_LockSurface(mss) != 0))
			return -1;

		for (j = 0; j < mapblockheight; j++) {
			tempsurfacept = (unsigned char *) mss->pixels;
			tempsurfacept += j * mss->pitch;
			for (k = mapblockwidth; k > 0; k--) {
				switch (mapdepth) {
				case 8:
					rval = mapcmappt[tempblockgfxpt[0] * 3];
					rval >>= (8 - rbits);
					gval = mapcmappt[tempblockgfxpt[0] * 3 + 1];
					gval >>= (8 - gbits);
					bval = mapcmappt[tempblockgfxpt[0] * 3 + 2];
					bval >>= (8 - bbits);
					tempblockgfxpt++;
					break;
				case 15:
					rval =
						((((unsigned long int) tempblockgfxpt[0]) << 8) |
						 ((unsigned long int) tempblockgfxpt[1])) & 0x7E00;
					rval >>= (7 + (8 - rbits));
					gval =
						((((unsigned long int) tempblockgfxpt[0]) << 8) |
						 ((unsigned long int) tempblockgfxpt[1])) & 0x03E0;
					gval >>= (2 + (8 - gbits));
					bval =
						((((unsigned long int) tempblockgfxpt[0]) << 8) |
						 ((unsigned long int) tempblockgfxpt[1])) & 0x001F;
					bval <<= 3;
					bval >>= (8 - bbits);
					tempblockgfxpt += 2;
					break;
				case 16:		// this is prefered format for game map
					rval =
						((((unsigned long int) tempblockgfxpt[0]) << 8) |
						 ((unsigned long int) tempblockgfxpt[1])) & 0xF800;
					rval >>= (8 + (8 - rbits));
					gval =
						((((unsigned long int) tempblockgfxpt[0]) << 8) |
						 ((unsigned long int) tempblockgfxpt[1])) & 0x07E0;
					gval >>= (3 + (8 - gbits));
					bval =
						((((unsigned long int) tempblockgfxpt[0]) << 8) |
						 ((unsigned long int) tempblockgfxpt[1])) & 0x001F;
					bval <<= 3;
					bval >>= (8 - bbits);
					tempblockgfxpt += 2;
					break;
				case 24:
					rval = (unsigned long int) tempblockgfxpt[0];
					rval >>= (8 - rbits);
					gval = (unsigned long int) tempblockgfxpt[1];
					gval >>= (8 - gbits);
					bval = (unsigned long int) tempblockgfxpt[2];
					bval >>= (8 - bbits);
					tempblockgfxpt += 3;
					break;
				case 32:
					rval = (unsigned long int) tempblockgfxpt[1];
					rval >>= (8 - rbits);
					gval = (unsigned long int) tempblockgfxpt[2];
					gval >>= (8 - gbits);
					bval = (unsigned long int) tempblockgfxpt[3];
					bval >>= (8 - bbits);
					tempblockgfxpt += 4;
					break;
				default:
					return -1;
				}
				rval <<= rshift;
				rval &= mpf->Rmask;
				gval <<= gshift;
				gval &= mpf->Gmask;
				bval <<= bshift;
				bval &= mpf->Bmask;
				switch (mpf->BitsPerPixel) {
					// Apparently 16bits are reported even in 555 (15bit) mode
				case 16:
					*((unsigned short int *) tempsurfacept) =
						(unsigned short int) (rval | gval | bval);
					tempsurfacept += 2;
					break;
				case 24:
					tempsurfacept[2] =
						(unsigned char) (((rval | gval | bval) >> 16) &
										 0xFF);
					tempsurfacept[1] =
						(unsigned char) (((rval | gval | bval) >> 8) &
										 0xFF);
					tempsurfacept[0] =
						(unsigned char) ((rval | gval | bval) & 0xFF);
					tempsurfacept += 3;
					break;
				case 32:

					tempsurfacept[3] =
						(unsigned char) (((rval | gval | bval) >> 24) &
										 0xFF);
					tempsurfacept[2] =
						(unsigned char) (((rval | gval | bval) >> 16) &
										 0xFF);
					tempsurfacept[1] =
						(unsigned char) (((rval | gval | bval) >> 8) &
										 0xFF);
					tempsurfacept[0] =
						(unsigned char) ((rval | gval | bval) & 0xFF);
					tempsurfacept += 4;
					break;
				}
			}
		}
		SDL_UnlockSurface(mss);
	}
	return 0;
}

int MapRestore(void)
{
/*int i;

	i = 0;
	while (maplpDDSTiles[i] != NULL)
	{
		if (maplpDDSTiles[i]->Restore() != DD_OK) return -1;
		i++;
	}*/
	return MapCopyBlocksToSurfaces();
}

int MapRelocate(CMapState * ms)
{
	int i, j;
	BLKSTR *myblkstrpt;
	ANISTR *myanpt;
	long int *myanblkpt;

	for (i = 0; i < mapnumblockgfx; i++) {

		mapTiles[i] = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCCOLORKEY,
										   mapblockwidth,
										   mapblockheight,
										   video_bpp, 0, 0, 0, 0);
		if (!mapTiles[i])
			return -1;

		//if (i==0) transpcol = MapTranspToPixel(mapTiles[i]->format);

		//SDL_SetColorKey(mapTiles[i], SDL_SRCCOLORKEY|SDL_RLEACCEL, 0);
	}
	mapTiles[i] = NULL;			// mark first unused tile slot
	if (MapCopyBlocksToSurfaces()) {
		MapFreeMem();
		return -1;
	}
	// this part is here because we load map as a one block of data with relative pointers
	i = mapnumblockstr;
	j = mapblockwidth * mapblockheight * ((mapdepth + 1) / 8);
	myblkstrpt = (BLKSTR *) ms->mapblockstrpt;
	while (i) {
		// make indexes
		myblkstrpt->bgoff = (((long int) myblkstrpt->bgoff) / j);
		if (myblkstrpt->fgoff != 0)
			myblkstrpt->fgoff = (((long int) myblkstrpt->fgoff) / j);
		if (myblkstrpt->fgoff2 != 0)
			myblkstrpt->fgoff2 = (((long int) myblkstrpt->fgoff2) / j);
		if (myblkstrpt->fgoff3 != 0)
			myblkstrpt->fgoff3 = (((long int) myblkstrpt->fgoff3) / j);
		myblkstrpt++;
		i--;
	}

	if (ms->mapanimstrpt != NULL) {
		myanpt = (ANISTR *) ms->mapanimstrendpt;
		myanpt--;
		while (myanpt->antype != AN_END) {
			// make absolute pointers
			myanpt->anstartoff += (long int) ms->mapanimstrendpt;
			myanpt->anendoff += (long int) ms->mapanimstrendpt;
			myanpt->ancuroff += (long int) ms->mapanimstrendpt;
			myanpt--;
		}
		myanblkpt = (long int *) ms->mapanimstrpt;
		while (myanblkpt != (long int *) myanpt) {
			*myanblkpt += (long int) ms->mapblockstrpt;
			myanblkpt++;
		}
	}
	ms->state = MS_INITED;
	return 0;
}

//###########################################################################
//## Map loader (FILE)
//###########################################################################

int MapDecodeMPHD(CMapState * ms)
{
	MPHD *hdrmempt;

	hdrmempt = (MPHD *) malloc(Mapbyteswapl(mapgenheader.headsize));
	if (hdrmempt == NULL)
		return 0;
	fread(hdrmempt, Mapbyteswapl(mapgenheader.headsize), 1, mapfilept);
	mapwidth = hdrmempt->mapwidth;
	mapheight = hdrmempt->mapheight;
	mappxwidth = hdrmempt->mapwidth * MBSW;
	mappxheight = hdrmempt->mapheight * MBSH;
	mapblockwidth = hdrmempt->blockwidth;
	mapblockheight = hdrmempt->blockheight;
	mapdepth = hdrmempt->blockdepth;
	mapblockstrsize = hdrmempt->blockstrsize;
	mapnumblockstr = hdrmempt->numblockstr;
	mapnumblockgfx = hdrmempt->numblockgfx;
	free(hdrmempt);
	return 0;
}

int MapDecodeBKDT(CMapState * ms)
{
	ms->sblockstr = Mapbyteswapl(mapgenheader.headsize);
	ms->mapblockstrpt = (char *) malloc(ms->sblockstr);
	if (ms->mapblockstrpt == NULL) {
		maperror = MER_OUTOFMEM;
		return -1;
	}
	fread(ms->mapblockstrpt, ms->sblockstr, 1, mapfilept);

	return 0;
}

int MapDecodeANDT(CMapState * ms)
{
	ms->sanimstr = Mapbyteswapl(mapgenheader.headsize);
	ms->mapanimstrpt = (char *) malloc(ms->sanimstr);
	if (ms->mapanimstrpt == NULL) {
		maperror = MER_OUTOFMEM;
		return -1;
	}
	ms->mapanimstrendpt = (char *) (ms->mapanimstrpt + ms->sanimstr);
	fread(ms->mapanimstrpt, ms->sanimstr, 1, mapfilept);

	return 0;
}

int MapDecodeCMAP(CMapState * ms)
{
	if (mapcmappt != NULL) {
		free(mapcmappt);
		mapcmappt = NULL;
	}
	mapcmappt =
		(unsigned char *) malloc(Mapbyteswapl(mapgenheader.headsize));
	if (mapcmappt == NULL) {
		maperror = MER_OUTOFMEM;
		return -1;
	}
	fread(mapcmappt, Mapbyteswapl(mapgenheader.headsize), 1, mapfilept);
//  MapCMAPtoPE (mapcmappt, mappept);
	return 0;
}

int MapDecodeBGFX(CMapState * ms)
{
	if (mapblockgfxpt != NULL) {
		free(mapblockgfxpt);
		mapblockgfxpt = NULL;
	}
	mapblockgfxpt = (char *) malloc(Mapbyteswapl(mapgenheader.headsize));
	if (mapblockgfxpt == NULL) {
		maperror = MER_OUTOFMEM;
		return -1;
	}
	fread(mapblockgfxpt, Mapbyteswapl(mapgenheader.headsize), 1,
		  mapfilept);

	return 0;
}

int MapDecodeBODY(CMapState * ms)
{
	ms->smapmap[0] = Mapbyteswapl(mapgenheader.headsize);
	ms->mapmappt[0] = (short int *) malloc(ms->smapmap[0]);
	if (ms->mapmappt[0] == NULL) {
		maperror = MER_OUTOFMEM;
		return -1;
	}
	fread(ms->mapmappt[0], ms->smapmap[0], 1, mapfilept);
	ms->mappt = ms->mapmappt[0];
	return 0;
}

int MapDecodeLYR1(CMapState * ms)
{
	ms->smapmap[1] = Mapbyteswapl(mapgenheader.headsize);
	ms->mapmappt[1] = (short int *) malloc(ms->smapmap[1]);
	if (ms->mapmappt[1] == NULL) {
		maperror = MER_OUTOFMEM;
		return -1;
	}
	fread(ms->mapmappt[1], ms->smapmap[1], 1, mapfilept);
	return 0;
}

int MapDecodeLYR2(CMapState * ms)
{
	ms->smapmap[2] = Mapbyteswapl(mapgenheader.headsize);
	ms->mapmappt[2] = (short int *) malloc(ms->smapmap[2]);
	if (ms->mapmappt[2] == NULL) {
		maperror = MER_OUTOFMEM;
		return -1;
	}
	fread(ms->mapmappt[2], ms->smapmap[2], 1, mapfilept);
	return 0;
}

int MapDecodeLYR3(CMapState * ms)
{
	ms->smapmap[3] = Mapbyteswapl(mapgenheader.headsize);
	ms->mapmappt[3] = (short int *) malloc(ms->smapmap[3]);
	if (ms->mapmappt[3] == NULL) {
		maperror = MER_OUTOFMEM;
		return -1;
	}
	fread(ms->mapmappt[3], ms->smapmap[3], 1, mapfilept);
	return 0;
}

int MapDecodeLYR4(CMapState * ms)
{
	ms->smapmap[4] = Mapbyteswapl(mapgenheader.headsize);
	ms->mapmappt[4] = (short int *) malloc(ms->smapmap[4]);
	if (ms->mapmappt[4] == NULL) {
		maperror = MER_OUTOFMEM;
		return -1;
	}
	fread(ms->mapmappt[4], ms->smapmap[4], 1, mapfilept);
	return 0;
}

int MapDecodeLYR5(CMapState * ms)
{
	ms->smapmap[5] = Mapbyteswapl(mapgenheader.headsize);
	ms->mapmappt[5] = (short int *) malloc(ms->smapmap[5]);
	if (ms->mapmappt[5] == NULL) {
		maperror = MER_OUTOFMEM;
		return -1;
	}
	fread(ms->mapmappt[5], ms->smapmap[5], 1, mapfilept);
	return 0;
}

int MapDecodeLYR6(CMapState * ms)
{
	ms->smapmap[6] = Mapbyteswapl(mapgenheader.headsize);
	ms->mapmappt[6] = (short int *) malloc(ms->smapmap[6]);
	if (ms->mapmappt[6] == NULL) {
		maperror = MER_OUTOFMEM;
		return -1;
	}
	fread(ms->mapmappt[6], ms->smapmap[6], 1, mapfilept);
	return 0;
}

int MapDecodeLYR7(CMapState * ms)
{
	ms->smapmap[7] = Mapbyteswapl(mapgenheader.headsize);
	ms->mapmappt[7] = (short int *) malloc(ms->smapmap[7]);
	if (ms->mapmappt[7] == NULL) {
		maperror = MER_OUTOFMEM;
		return -1;
	}
	fread(ms->mapmappt[7], ms->smapmap[7], 1, mapfilept);
	return 0;
}

int MapDecodeNULL(CMapState * ms)
{
	char *mynllpt;
	mynllpt = (char *) malloc(Mapbyteswapl(mapgenheader.headsize));
	if (mynllpt == NULL) {
		maperror = MER_OUTOFMEM;
		return -1;
	}
	fread(mynllpt, Mapbyteswapl(mapgenheader.headsize), 1, mapfilept);
	free(mynllpt);
	return 0;
}

int MapLoad(char *name, char *script)
{
	MapLoaded = 0;
	int i;
	unsigned long int mapfilesize;
	// check if map wasn't loaded before
	if (strcmp(MapName, name) == 0) {
		MapLoaded = 1;
		return 0;
	}

	char mapname[_MAX_PATH];
	FileNameConversion(map_dir.string, name, map_ext.string, mapname);
	MapLoadedState.Free();
	CMapState *ms = &MapLoadedState;
	MapFreeMem();
	maperror = 0;

	{
		// try to load, compile and instance map script
		if (map_script_num != -1)
			ScriptMan->DeleteScript(map_script_num);	// unload old script
		map_script_num = ScriptMan->NewScript(script, smap_ext.string);
		if (map_script_num == -1)
			return -10;
		if (ScriptMan->InstanceScript(map_script_num) != 0)
			return -10;
		ScriptMan->scripts[map_script_num].verbose = false;
		strcpy(ScriptName, script);
	}

	mapfilept = fopen(mapname, "rb");
	if (mapfilept == NULL) {
		maperror = MER_NOOPEN;
		return -1;
	}
	fseek(mapfilept, 0, SEEK_END);
	mapfilesize = ftell(mapfilept);
	fseek(mapfilept, 0, SEEK_SET);
	if (fread(&mapgenheader, 1, sizeof(GENHEAD), mapfilept) !=
		sizeof(GENHEAD)) {maperror = MER_MAPLOADERROR;
		fclose(mapfilept);
		return -1;
	}

	if (mapgenheader.id1 != 'F')
		maperror = MER_MAPLOADERROR;
	if (mapgenheader.id2 != 'O')
		maperror = MER_MAPLOADERROR;
	if (mapgenheader.id3 != 'R')
		maperror = MER_MAPLOADERROR;
	if (mapgenheader.id4 != 'M')
		maperror = MER_MAPLOADERROR;

	if (mapfilesize == -1)
		maperror = MER_MAPLOADERROR;
	if (mapfilesize != ((Mapbyteswapl(mapgenheader.headsize)) + 8))
		maperror = MER_MAPLOADERROR;

	if (maperror) {
		fclose(mapfilept);
		return -1;
	}

	if (fread(&mapgenheader, 1, sizeof(GENHEAD) - 4, mapfilept) !=
		sizeof(GENHEAD) - 4) {
		maperror = MER_MAPLOADERROR;
		fclose(mapfilept);
		return -1;
	}
	maperror = MER_MAPLOADERROR;
	if (mapgenheader.id1 == 'F') {
		if (mapgenheader.id2 == 'M') {
			if (mapgenheader.id3 == 'A') {
				if (mapgenheader.id4 == 'P')
					maperror = MER_NONE;
			}
		}
	}
	if (maperror) {
		fclose(mapfilept);
		return -1;
	}

	while (mapfilesize != ((unsigned long) ftell(mapfilept))) {
		if (fread(&mapgenheader, 1, sizeof(GENHEAD), mapfilept) !=
			sizeof(GENHEAD)) {maperror = MER_MAPLOADERROR;
			fclose(mapfilept);
			return -1;
		}

		i = 0;
		if (mapgenheader.id1 == 'M') {
			if (mapgenheader.id2 == 'P') {
				if (mapgenheader.id3 == 'H') {
					if (mapgenheader.id4 == 'D') {
						MapDecodeMPHD(ms);
						i = 1;
					}
				}
			}
		}
		if (mapgenheader.id1 == 'C') {
			if (mapgenheader.id2 == 'M') {
				if (mapgenheader.id3 == 'A') {
					if (mapgenheader.id4 == 'P') {
						MapDecodeCMAP(ms);
						i = 1;
					}
				}
			}
		}
		if (mapgenheader.id1 == 'B') {
			if (mapgenheader.id2 == 'K') {
				if (mapgenheader.id3 == 'D') {
					if (mapgenheader.id4 == 'T') {
						MapDecodeBKDT(ms);
						i = 1;
					}
				}
			}
		}
		if (mapgenheader.id1 == 'A') {
			if (mapgenheader.id2 == 'N') {
				if (mapgenheader.id3 == 'D') {
					if (mapgenheader.id4 == 'T') {
						MapDecodeANDT(ms);
						i = 1;
					}
				}
			}
		}
		if (mapgenheader.id1 == 'B') {
			if (mapgenheader.id2 == 'G') {
				if (mapgenheader.id3 == 'F') {
					if (mapgenheader.id4 == 'X') {
						MapDecodeBGFX(ms);
						i = 1;
					}
				}
			}
		}
		if (mapgenheader.id1 == 'B') {
			if (mapgenheader.id2 == 'O') {
				if (mapgenheader.id3 == 'D') {
					if (mapgenheader.id4 == 'Y') {
						MapDecodeBODY(ms);
						i = 1;
					}
				}
			}
		}
		if (mapgenheader.id1 == 'L') {
			if (mapgenheader.id2 == 'Y') {
				if (mapgenheader.id3 == 'R') {
					if (mapgenheader.id4 == '1') {
						MapDecodeLYR1(ms);
						i = 1;
					}
				}
			}
		}
		if (mapgenheader.id1 == 'L') {
			if (mapgenheader.id2 == 'Y') {
				if (mapgenheader.id3 == 'R') {
					if (mapgenheader.id4 == '2') {
						MapDecodeLYR2(ms);
						i = 1;
					}
				}
			}
		}
		if (mapgenheader.id1 == 'L') {
			if (mapgenheader.id2 == 'Y') {
				if (mapgenheader.id3 == 'R') {
					if (mapgenheader.id4 == '3') {
						MapDecodeLYR3(ms);
						i = 1;
					}
				}
			}
		}
		if (mapgenheader.id1 == 'L') {
			if (mapgenheader.id2 == 'Y') {
				if (mapgenheader.id3 == 'R') {
					if (mapgenheader.id4 == '4') {
						MapDecodeLYR4(ms);
						i = 1;
					}
				}
			}
		}
		if (mapgenheader.id1 == 'L') {
			if (mapgenheader.id2 == 'Y') {
				if (mapgenheader.id3 == 'R') {
					if (mapgenheader.id4 == '5') {
						MapDecodeLYR5(ms);
						i = 1;
					}
				}
			}
		}
		if (mapgenheader.id1 == 'L') {
			if (mapgenheader.id2 == 'Y') {
				if (mapgenheader.id3 == 'R') {
					if (mapgenheader.id4 == '6') {
						MapDecodeLYR6(ms);
						i = 1;
					}
				}
			}
		}
		if (mapgenheader.id1 == 'L') {
			if (mapgenheader.id2 == 'Y') {
				if (mapgenheader.id3 == 'R') {
					if (mapgenheader.id4 == '7') {
						MapDecodeLYR7(ms);
						i = 1;
					}
				}
			}
		}
		if (!i)
			MapDecodeNULL(ms);
		if (maperror) {
			fclose(mapfilept);
			return -1;
		}
	}
	fclose(mapfilept);
	MapLoaded = 1;
	strcpy(MapName, name);
	ms->CountAnimStrs();
	return MapRelocate(ms);
}

//###########################################################################
//## Map loader (Memory)
//###########################################################################
/*
int MapMemDecodeMPHD (MPHD * mapmempt)
{
	mapwidth=mapmempt->mapwidth;
	mapheight=mapmempt->mapheight;
	mapblockwidth=mapmempt->blockwidth;
	mapblockheight=mapmempt->blockheight;
	mapdepth=mapmempt->blockdepth;
	mapblockstrsize=mapmempt->blockstrsize;
	mapnumblockstr=mapmempt->numblockstr;
	mapnumblockgfx=mapmempt->numblockgfx;
	return 0;
}

int MapMemDecodeBKDT (unsigned char * mapmempt)
{
  int i, j;
  unsigned char * temppt;
	i = Mapbyteswapl(*(long int *)mapmempt);
	mapmempt += 4;
	mapblockstrpt = (char *) malloc (i);
	if (mapblockstrpt==NULL) { maperror = MER_OUTOFMEM; return -1; }
	temppt = (unsigned char *) mapblockstrpt;
	for (j=0;j<i;j++) { *temppt = *mapmempt; temppt++; mapmempt++; }

	return 0;
}

int MapMemDecodeANDT (unsigned char * mapmempt)
{
  int i, j;
  unsigned char * temppt;
	i = Mapbyteswapl(*(long int *)mapmempt);
	mapmempt += 4;
	mapanimstrpt = (char *) malloc (i);
	if (mapanimstrpt==NULL) { maperror = MER_OUTOFMEM; return -1; }
	mapanimstrendpt = (char *) (mapanimstrpt+i);
	temppt = (unsigned char *) mapanimstrpt;
	for (j=0;j<i;j++) { *temppt = *mapmempt; temppt++; mapmempt++; }

	return 0;
}

int MapMemDecodeCMAP (unsigned char * cmppt)
{
  int i;
  unsigned char * tempcmappt;
	mapcmappt = (unsigned char *) malloc (Mapbyteswapl(*(long int *)cmppt));
	cmppt += 4;
	if (mapcmappt==NULL) { maperror = MER_OUTOFMEM; return -1; }
	tempcmappt = mapcmappt;
	for (i=0;i<(256*3);i++) { *tempcmappt = *cmppt; tempcmappt++; cmppt++; }
//	MapCMAPtoPE (mapcmappt, mappept);
	return 0;
}


int MapMemDecodeBGFX (unsigned char * mapmempt)
{
  int i, j;
  unsigned char * temppt;
	i = Mapbyteswapl(*(long int *)mapmempt);
	mapmempt += 4;
	mapblockgfxpt = (char *) malloc (i);
	if (mapblockgfxpt==NULL) { maperror = MER_OUTOFMEM; return -1; }
	temppt = (unsigned char *) mapblockgfxpt;
	for (j=0;j<i;j++) { *temppt = *mapmempt; temppt++; mapmempt++; }
	return 0;
}

int MapMemDecodeBODY (unsigned char * mapmempt)
{
  int i, j;
  unsigned char * temppt;
	i = Mapbyteswapl(*(long int *)mapmempt);
	mapmempt += 4;
	mappt = (short int *) malloc (i);
	if (mappt==NULL) { maperror = MER_OUTOFMEM; return -1; }
	mapmappt[0] = mappt;
	temppt = (unsigned char *) mappt;
	for (j=0;j<i;j++) { *temppt = *mapmempt; temppt++; mapmempt++; }
	return 0;
}
int MapMemDecodeLYR (unsigned char * mapmempt, int maplayernum)
{
  int i, j;
  unsigned char * temppt;
	i = Mapbyteswapl(*(long int *)mapmempt);
	mapmempt += 4;
	mapmappt[maplayernum] = (short int *) malloc (i);
	if (mapmappt[maplayernum]==NULL) { maperror = MER_OUTOFMEM; return -1; }
	temppt = (unsigned char *) mappt;
	for (j=0;j<i;j++) { *temppt = *mapmempt; temppt++; mapmempt++; }
	return 0;
}

int MapDecode (unsigned char * mapmempt)
{
  int i;
  long int maplength;

	MapFreeMem ();
	maperror = 0;

	if (*mapmempt!='F') maperror = MER_MAPLOADERROR;
	if (*(mapmempt+1)!='O') maperror = MER_MAPLOADERROR;
	if (*(mapmempt+2)!='R') maperror = MER_MAPLOADERROR;
	if (*(mapmempt+3)!='M') maperror = MER_MAPLOADERROR;
	mapmempt += 4;
	maplength = (Mapbyteswapl(*(long int *)mapmempt)+8);

//	Just keeping this bit here in case I need it.
//	if (maplength!=((Mapbyteswapl(*(long int *)mapmempt))+8)) maperror = MER_MAPLOADERROR;
	if (maperror) return -1;
	mapmempt += 4;

	if (*mapmempt!='F') maperror = MER_MAPLOADERROR;
	if (*(mapmempt+1)!='M') maperror = MER_MAPLOADERROR;
	if (*(mapmempt+2)!='A') maperror = MER_MAPLOADERROR;
	if (*(mapmempt+3)!='P') maperror = MER_MAPLOADERROR;
	mapmempt+=4; i = 12;

	if (maperror) return -1;

	while (i!=maplength)
	{
		if ((*mapmempt=='M') && (*(mapmempt+1)=='P') && (*(mapmempt+2)=='H') &&
		(*(mapmempt+3)=='D')) MapMemDecodeMPHD ((MPHD *) (mapmempt+8));
		if ((*mapmempt=='C') && (*(mapmempt+1)=='M') && (*(mapmempt+2)=='A') &&
		(*(mapmempt+3)=='P')) MapMemDecodeCMAP (mapmempt+4);
		if ((*mapmempt=='B') && (*(mapmempt+1)=='K') && (*(mapmempt+2)=='D') &&
		(*(mapmempt+3)=='T')) MapMemDecodeBKDT (mapmempt+4);
		if ((*mapmempt=='A') && (*(mapmempt+1)=='N') && (*(mapmempt+2)=='D') &&
		(*(mapmempt+3)=='T')) MapMemDecodeANDT (mapmempt+4);
		if ((*mapmempt=='B') && (*(mapmempt+1)=='G') && (*(mapmempt+2)=='F') &&
		(*(mapmempt+3)=='X')) MapMemDecodeBGFX (mapmempt+4);
		if ((*mapmempt=='B') && (*(mapmempt+1)=='O') && (*(mapmempt+2)=='D') &&
		(*(mapmempt+3)=='Y')) MapMemDecodeBODY (mapmempt+4);
		if ((*mapmempt=='L') && (*(mapmempt+1)=='Y') && (*(mapmempt+2)=='R') &&
		(*(mapmempt+3)=='1')) MapMemDecodeLYR (mapmempt+4, 1);
		if ((*mapmempt=='L') && (*(mapmempt+1)=='Y') && (*(mapmempt+2)=='R') &&
		(*(mapmempt+3)=='2')) MapMemDecodeLYR (mapmempt+4, 2);
		if ((*mapmempt=='L') && (*(mapmempt+1)=='Y') && (*(mapmempt+2)=='R') &&
		(*(mapmempt+3)=='3')) MapMemDecodeLYR (mapmempt+4, 3);
		if ((*mapmempt=='L') && (*(mapmempt+1)=='Y') && (*(mapmempt+2)=='R') &&
		(*(mapmempt+3)=='4')) MapMemDecodeLYR (mapmempt+4, 4);
		if ((*mapmempt=='L') && (*(mapmempt+1)=='Y') && (*(mapmempt+2)=='R') &&
		(*(mapmempt+3)=='5')) MapMemDecodeLYR (mapmempt+4, 5);
		if ((*mapmempt=='L') && (*(mapmempt+1)=='Y') && (*(mapmempt+2)=='R') &&
		(*(mapmempt+3)=='6')) MapMemDecodeLYR (mapmempt+4, 6);
		if ((*mapmempt=='L') && (*(mapmempt+1)=='Y') && (*(mapmempt+2)=='R') &&
		(*(mapmempt+3)=='7')) MapMemDecodeLYR (mapmempt+4, 7);
		if (maperror) return -1;
		mapmempt += 4; i += Mapbyteswapl(*(long int *)mapmempt)+8;
		mapmempt += Mapbyteswapl(*(long int *)mapmempt); mapmempt += 4;
	}

  MapLoaded = 1;
  strcpy(MapName, "memory");
  MapCountAnimStrs();
  return MapRelocate();
}
*/

//###########################################################################
//###########################################################################
//## CMapState
//###########################################################################
//###########################################################################

CMapState::CMapState()
{
	int i;

	for (i = 0; i < 8; i++)
		mapmappt[i] = NULL;
	mappt = NULL;
	for (i = 0; i < 8; i++)
		mapmaparraypt[i] = NULL;
	maparraypt = NULL;
	mapblockstrpt = NULL;
	mapanimstrpt = NULL;
	mapanimstrendpt = NULL;
	map_script_num = -1;

	state = MS_UNINITED;
}

CMapState::~CMapState()
{
	int i;

	for (i = 0; i < 8; i++) {
		if (mapmappt[i] != NULL) {
			free(mapmappt[i]);
			mapmappt[i] = NULL;
		}
	}
	mappt = NULL;
	for (i = 0; i < 8; i++) {
		if (mapmaparraypt[i] != NULL) {
			free(mapmaparraypt[i]);
			mapmaparraypt[i] = NULL;
		}
	}
	maparraypt = NULL;
	if (mapblockstrpt != NULL) {
		free(mapblockstrpt);
		mapblockstrpt = NULL;
	}
	if (mapanimstrpt != NULL) {
		free(mapanimstrpt);
		mapanimstrpt = NULL;
	}
}

int CMapState::Free()
{
	int i;

	for (i = 0; i < 8; i++) {
		if (mapmappt[i] != NULL) {
			free(mapmappt[i]);
			mapmappt[i] = NULL;
		}
	}
	mappt = NULL;
	for (i = 0; i < 8; i++) {
		if (mapmaparraypt[i] != NULL) {
			free(mapmaparraypt[i]);
			mapmaparraypt[i] = NULL;
		}
	}
	maparraypt = NULL;
	if (mapblockstrpt != NULL) {
		free(mapblockstrpt);
		mapblockstrpt = NULL;
	}
	if (mapanimstrpt != NULL) {
		free(mapanimstrpt);
		mapanimstrpt = NULL;
	}
	state = MS_UNINITED;
	return 0;
}

int CMapState::Copy(CMapState * o)
{
	if (o->state == MS_INITED) {
		Free();
		sblockstr = o->sblockstr;
		mapblockstrpt = (char *) malloc(sblockstr);
		if (mapblockstrpt == NULL)
			return MER_OUTOFMEM;
		memcpy(mapblockstrpt, o->mapblockstrpt, sblockstr);

		sanimstr = o->sanimstr;
		mapanimstrpt = (char *) malloc(sanimstr);
		if (mapanimstrpt == NULL)
			return MER_OUTOFMEM;
		memcpy(mapanimstrpt, o->mapanimstrpt, sanimstr);
		mapanimstrendpt = (char *) (mapanimstrpt + sanimstr);

		for (int i = 0; i < 8; i++) {
			smapmap[i] = o->smapmap[i];
			if (smapmap[i]) {
				mapmappt[i] = (short int *) malloc(smapmap[i]);
				if (mapmappt[i] == NULL)
					return MER_OUTOFMEM;
				memcpy(mapmappt[i], o->mapmappt[i], smapmap[i]);
			}
		}

		mappt = mapmappt[0];
		state = MS_INITED;
		return 0;
	} else
		return 1;
}

int CMapState::GenerateYLookup(void)
{
	int i, j;

	for (i = 0; i < 8; i++) {
		if (mapmaparraypt[i] != NULL) {
			free(mapmaparraypt[i]);
			mapmaparraypt[i] = NULL;
		}
		if (mapmappt[i] != NULL) {
			mapmaparraypt[i] =
				(short int **) malloc(mapheight * sizeof(short int *));
			if (mapmaparraypt[i] == NULL)
				return -1;
			for (j = 0; j < mapheight; j++)
				mapmaparraypt[i][j] = (mapmappt[i] + (j * mapwidth));
			if (mapmappt[i] == mappt)
				maparraypt = mapmaparraypt[i];
		}
	}
	return 0;
}

void CMapState::CountAnimStrs(void)
{
	ANISTR *myanpt;
	if (mapanimstrpt == NULL)
		return;

	mapnumanimstr = 0;
	myanpt = (ANISTR *) mapanimstrendpt;
	myanpt--;
	while (myanpt->antype != -1) {
		mapnumanimstr++;
		myanpt--;
	}
}

//###########################################################################
//## Map blocks and layers
//###########################################################################

BLKSTR *CMapState::GetBlock(int x, int y)
{
	short int *mymappt;
	ANISTR *myanpt;

	if (x < 0)
		x += mapwidth;
	else if (x >= mapwidth)
		x -= mapwidth;
	if (y < 0)
		y += mapheight;
	else if (y >= mapheight)
		y -= mapheight;

	if (maparraypt != NULL) {
		mymappt = maparraypt[y] + x;
	} else {
		mymappt = mappt;
		mymappt += x;
		mymappt += y * mapwidth;
	}
	if (*mymappt >= 0)
		return (BLKSTR *) (((char *) mapblockstrpt) + *mymappt);
	else {
		myanpt = (ANISTR *) (mapanimstrendpt + *mymappt);
		return (BLKSTR *) * ((long int *) (myanpt->ancuroff));
	}
}

void CMapState::SetBlock(int x, int y, int strvalue)
{
	short int *mymappt;

	if (x < 0)
		x += mapwidth;
	else if (x >= mapwidth)
		x -= mapwidth;
	if (y < 0)
		y += mapheight;
	else if (y >= mapheight)
		y -= mapheight;

	if (maparraypt != NULL) {
		mymappt = maparraypt[y] + x;
	} else {
		mymappt = mappt;
		mymappt += x;
		mymappt += y * mapwidth;
	}
	if (strvalue >= 0)
		*mymappt = strvalue * sizeof(BLKSTR);

	else
		*mymappt = strvalue * sizeof(ANISTR);
}

int CMapState::ChangeLayer(int newlyr)
{
	if (newlyr < 0 || newlyr > 7 || mapmappt[newlyr] == NULL)
		return -1;
	mappt = mapmappt[newlyr];
	maparraypt = mapmaparraypt[newlyr];
	return newlyr;
}

//###########################################################################
//## Map animations
//###########################################################################

void CMapState::InitAnims(void)
{
	ANISTR *myanpt;
	if (mapanimstrpt == NULL)
		return;
	myanpt = (ANISTR *) mapanimstrendpt;
	myanpt--;
	while (myanpt->antype != -1) {
		if (myanpt->antype == AN_PPFR)
			myanpt->antype = AN_PPFF;
		if (myanpt->antype == AN_PPRF)
			myanpt->antype = AN_PPRR;
		if (myanpt->antype == AN_ONCES)
			myanpt->antype = AN_ONCE;
		if ((myanpt->antype == AN_LOOPR) || (myanpt->antype == AN_PPRR)) {
			myanpt->ancuroff = myanpt->anstartoff;
			if ((myanpt->anstartoff) != (myanpt->anendoff))
				myanpt->ancuroff = (myanpt->anendoff) - 4;
		} else {
			myanpt->ancuroff = myanpt->anstartoff;
		}
		myanpt->ancount = myanpt->andelay;
		myanpt--;
	}
}

void CMapState::UpdateAnims(void)
{
	ANISTR *myanpt;

	if (mapanimstrpt == NULL)
		return;
	myanpt = (ANISTR *) mapanimstrendpt;
	myanpt--;
	while (myanpt->antype != -1) {
		if (myanpt->antype != AN_NONE) {
			myanpt->ancount--;
			if (myanpt->ancount < 0) {
				myanpt->ancount = myanpt->andelay;
				if (myanpt->antype == AN_LOOPF) {
					if (myanpt->anstartoff != myanpt->anendoff) {
						myanpt->ancuroff += 4;
						if (myanpt->ancuroff == myanpt->anendoff)
							myanpt->ancuroff = myanpt->anstartoff;
					}
				}
				if (myanpt->antype == AN_LOOPR) {
					if (myanpt->anstartoff != myanpt->anendoff) {
						myanpt->ancuroff -= 4;
						if (myanpt->ancuroff == ((myanpt->anstartoff) - 4))
							myanpt->ancuroff = (myanpt->anendoff) - 4;
					}
				}
				if (myanpt->antype == AN_ONCE) {
					if (myanpt->anstartoff != myanpt->anendoff) {
						myanpt->ancuroff += 4;
						if (myanpt->ancuroff == myanpt->anendoff) {
							myanpt->antype = AN_ONCES;
							myanpt->ancuroff = myanpt->anstartoff;
						}
					}
				}
				if (myanpt->antype == AN_ONCEH) {
					if (myanpt->anstartoff != myanpt->anendoff) {
						if (myanpt->ancuroff != ((myanpt->anendoff) - 4))
							myanpt->ancuroff += 4;
					}
				}
				if (myanpt->antype == AN_PPFF) {
					if (myanpt->anstartoff != myanpt->anendoff) {
						myanpt->ancuroff += 4;
						if (myanpt->ancuroff == myanpt->anendoff) {
							myanpt->ancuroff -= 8;
							myanpt->antype = AN_PPFR;
							if (myanpt->ancuroff < myanpt->anstartoff)
								myanpt->ancuroff += 4;
						}
					}
				} else {
					if (myanpt->antype == AN_PPFR) {
						if (myanpt->anstartoff != myanpt->anendoff) {
							myanpt->ancuroff -= 4;
							if (myanpt->ancuroff ==
								((myanpt->anstartoff) - 4)) {
								myanpt->ancuroff += 8;
								myanpt->antype = AN_PPFF;
								if (myanpt->ancuroff > myanpt->anendoff)
									myanpt->ancuroff -= 4;
							}
						}
					}
				}
				if (myanpt->antype == AN_PPRR) {
					if (myanpt->anstartoff != myanpt->anendoff) {
						myanpt->ancuroff -= 4;
						if (myanpt->ancuroff == ((myanpt->anstartoff) - 4)) {
							myanpt->ancuroff += 8;
							myanpt->antype = AN_PPRF;
							if (myanpt->ancuroff > myanpt->anendoff)
								myanpt->ancuroff -= 4;
						}
					}
				} else {
					if (myanpt->antype == AN_PPRF) {
						if (myanpt->anstartoff != myanpt->anendoff) {
							myanpt->ancuroff += 4;
							if (myanpt->ancuroff == myanpt->anendoff) {
								myanpt->ancuroff -= 8;
								myanpt->antype = AN_PPRR;
								if (myanpt->ancuroff < myanpt->anstartoff)
									myanpt->ancuroff += 4;
							}
						}
					}
				}
			}
		}
		myanpt--;
	}
}

//###########################################################################
//## Map vizualization
//###########################################################################

int CMapState::DrawBG(SDL_Surface * Dest, int mapxo, int mapyo)
{
	int i, j, mapvclip, maphclip;
	short int *mymappt;
	short int *mymap2pt;
	BLKSTR *blkdatapt;
	ANISTR *myanpt;
	SDL_Rect srrec, dsrec;

/*	for (i=0;i<mapnumblockgfx;i++)
	{
    SDL_SetColorKey(mapTiles[i], 0, 0);
	}*/
	// Draw blocks 
	maphclip = mapxo % MBSW;	// Number of pixels to clip from left 
	mapvclip = mapyo % MBSH;	/// Number of pixels to clip from top 

	// Draw top left clipped block
	srrec.x = maphclip;
	srrec.y = mapvclip;
	srrec.w = MBSW - maphclip;
	srrec.h = MBSH - mapvclip;

	dsrec.x = MMOX;
	dsrec.y = MMOY;
	dsrec.w = MBSW - maphclip;
	dsrec.h = MBSH - mapvclip;

	mymappt = (short int *) mappt;
	mymappt += (mapxo / MBSW) + ((mapyo / MBSH) * mapwidth);
	if (*mymappt >= 0)
		blkdatapt = (BLKSTR *) (((char *) mapblockstrpt) + *mymappt);
	else {
		myanpt = (ANISTR *) (mapanimstrendpt + *mymappt);
		blkdatapt = (BLKSTR *) * ((long int *) (myanpt->ancuroff));
	}
	if (SDL_BlitSurface(mapTiles[blkdatapt->bgoff], &srrec, Dest, &dsrec)
		== -2) {
		return 1;
	}
	// Draw left clipped blocks
	srrec.y = 0;

	srrec.h = MBSH;
	i = MMOY + MBSH - mapvclip;
	while (i < (MSCRH + MMOY - MBSH)) {
		mymappt += mapwidth;
		if (*mymappt >= 0)
			blkdatapt = (BLKSTR *) (((char *) mapblockstrpt) + *mymappt);
		else {
			myanpt = (ANISTR *) (mapanimstrendpt + *mymappt);
			blkdatapt = (BLKSTR *) * ((long int *) (myanpt->ancuroff));
		}
		dsrec.y = i;
		dsrec.h = MBSH;
		if (SDL_BlitSurface
			(mapTiles[blkdatapt->bgoff], &srrec, Dest, &dsrec) == -2) {
			return 1;
		}
		i += MBSH;
	}

	// Draw bottom left clipped block
	mymappt += mapwidth;
	if (*mymappt >= 0)
		blkdatapt = (BLKSTR *) (((char *) mapblockstrpt) + *mymappt);
	else {
		myanpt = (ANISTR *) (mapanimstrendpt + *mymappt);
		blkdatapt = (BLKSTR *) * ((long int *) (myanpt->ancuroff));
	}
	srrec.h = MMOY + MSCRH - i;
	dsrec.y = i;
	dsrec.h = MMOY + MSCRH - i;
	if (SDL_BlitSurface(mapTiles[blkdatapt->bgoff], &srrec, Dest, &dsrec)
		== -2) {
		return 1;
	}
	// Draw bottom clipped blocks
	srrec.x = 0;
	srrec.w = MBSW;
	j = MMOX + MBSW - maphclip;
	while (j < (MSCRW + MMOX - MBSW)) {
		mymappt++;
		if (*mymappt >= 0)
			blkdatapt = (BLKSTR *) (((char *) mapblockstrpt) + *mymappt);
		else {
			myanpt = (ANISTR *) (mapanimstrendpt + *mymappt);
			blkdatapt = (BLKSTR *) * ((long int *) (myanpt->ancuroff));
		}
		dsrec.x = j;
		dsrec.w = MBSW;
		if (SDL_BlitSurface
			(mapTiles[blkdatapt->bgoff], &srrec, Dest, &dsrec) == -2) {
			return 1;
		}
		j += MBSW;
	}

	// Draw bottom right clipped block
	srrec.w = MMOX + MSCRW - j;
	mymappt++;
	if (*mymappt >= 0)
		blkdatapt = (BLKSTR *) (((char *) mapblockstrpt) + *mymappt);
	else {
		myanpt = (ANISTR *) (mapanimstrendpt + *mymappt);
		blkdatapt = (BLKSTR *) * ((long int *) (myanpt->ancuroff));
	}
	dsrec.x = j;
	dsrec.w = MMOX + MSCRW - j;
	if (SDL_BlitSurface(mapTiles[blkdatapt->bgoff], &srrec, Dest, &dsrec)
		== -2) {
		return 1;
	}
	// Draw right clipped blocks
	srrec.h = MBSH;
	while (i > (MMOY + MBSH)) {
		mymappt -= mapwidth;
		i -= MBSH;
		if (*mymappt >= 0)
			blkdatapt = (BLKSTR *) (((char *) mapblockstrpt) + *mymappt);
		else {
			myanpt = (ANISTR *) (mapanimstrendpt + *mymappt);
			blkdatapt = (BLKSTR *) * ((long int *) (myanpt->ancuroff));
		}
		dsrec.y = i;
		dsrec.h = MBSH;
		if (SDL_BlitSurface
			(mapTiles[blkdatapt->bgoff], &srrec, Dest, &dsrec) == -2) {
			return 1;
		}
	}

	// Draw top right clipped block
	dsrec.y = MMOY;
	dsrec.h = i - MMOY;
	mymappt -= mapwidth;
	i = MMOY;
	srrec.y = mapvclip;
	srrec.h = MBSH - mapvclip;
	if (*mymappt >= 0)
		blkdatapt = (BLKSTR *) (((char *) mapblockstrpt) + *mymappt);
	else {
		myanpt = (ANISTR *) (mapanimstrendpt + *mymappt);
		blkdatapt = (BLKSTR *) * ((long int *) (myanpt->ancuroff));
	}
	if (SDL_BlitSurface(mapTiles[blkdatapt->bgoff], &srrec, Dest, &dsrec)
		== -2) {
		return 1;
	}
	// Draw top clipped blocks
	srrec.w = MBSW;
	while (j > (MMOX + MBSW)) {
		mymappt--;
		j -= MBSW;
		if (*mymappt >= 0)
			blkdatapt = (BLKSTR *) (((char *) mapblockstrpt) + *mymappt);
		else {
			myanpt = (ANISTR *) (mapanimstrendpt + *mymappt);
			blkdatapt = (BLKSTR *) * ((long int *) (myanpt->ancuroff));
		}
		dsrec.x = j;
		dsrec.w = MBSW;
		if (SDL_BlitSurface
			(mapTiles[blkdatapt->bgoff], &srrec, Dest, &dsrec) == -2) {
			return 1;
		}
	}


	// Draw unclipped (full) blocks
	srrec.y = 0;
	srrec.h = MBSH;
	dsrec.h = MBSH;
	dsrec.w = MBSW;
	i = MMOY + MBSH - mapvclip;
	while (i < (MSCRH + MMOY - MBSH)) {
		mymappt += mapwidth;
		mymap2pt = mymappt;
		j = MMOX + MBSW - maphclip;
		while (j < (MSCRW + MMOX - MBSW)) {
			if (*mymappt >= 0)
				blkdatapt =
					(BLKSTR *) (((char *) mapblockstrpt) + *mymappt);
			else {
				myanpt = (ANISTR *) (mapanimstrendpt + *mymappt);
				blkdatapt = (BLKSTR *) * ((long int *) (myanpt->ancuroff));
			}

			dsrec.x = j;
			dsrec.y = i;
			if (SDL_BlitSurface
				(mapTiles[blkdatapt->bgoff], &srrec, Dest, &dsrec) == -2) {
				return 1;
			}
			mymappt++;
			j += MBSW;
		}
		mymappt = mymap2pt;
		i += MBSH;
	}
	return 0;
}

int CMapState::DrawFG(SDL_Surface * Dest, int mapxo, int mapyo)
// allways draws 1. foreground layer
{
	int i, j, mapvclip, maphclip;
	short int *mymappt;
	short int *mymap2pt;
	BLKSTR *blkdatapt;
	ANISTR *myanpt;
	SDL_Rect srrec, dsrec;

	transpcol = MapTranspToPixel(mapTiles[1]->format);	// MapTranspToPixel(mapTiles[i]->format)

/*	for (i=0;i<mapnumblockgfx;i++)
	{
    SDL_SetColorKey(mapTiles[i], SDL_SRCCOLORKEY|SDL_RLEACCEL, transpcol);
	}*/

	// Draw blocks 
	maphclip = mapxo % MBSW;	// Number of pixels to clip from left 
	mapvclip = mapyo % MBSH;	/// Number of pixels to clip from top 

	// Draw top left clipped block
	srrec.x = maphclip;
	srrec.y = mapvclip;
	srrec.w = MBSW - maphclip;
	srrec.h = MBSH - mapvclip;

	dsrec.x = MMOX;
	dsrec.y = MMOY;
	dsrec.w = MBSW - maphclip;
	dsrec.h = MBSH - mapvclip;

	mymappt = (short int *) mappt;
	mymappt += (mapxo / MBSW) + ((mapyo / MBSH) * mapwidth);
	if (*mymappt >= 0)
		blkdatapt = (BLKSTR *) (((char *) mapblockstrpt) + *mymappt);
	else {
		myanpt = (ANISTR *) (mapanimstrendpt + *mymappt);
		blkdatapt = (BLKSTR *) * ((long int *) (myanpt->ancuroff));
	}
	if (blkdatapt->fgoff) {
		if (blkdatapt->tl2)		// transparency bit
			SDL_SetColorKey(mapTiles[blkdatapt->fgoff], SDL_SRCCOLORKEY,
							transpcol);

		if (SDL_BlitSurface
			(mapTiles[blkdatapt->fgoff], &srrec, Dest, &dsrec) == -2) {
			return 1;
		}

		if (blkdatapt->tl2)		// transparency bit
			SDL_SetColorKey(mapTiles[blkdatapt->fgoff], 0, 0);
	}
	// Draw left clipped blocks
	srrec.y = 0;
	srrec.h = MBSH;
	i = MMOY + MBSH - mapvclip;
	while (i < (MSCRH + MMOY - MBSH)) {
		mymappt += mapwidth;
		if (*mymappt >= 0)
			blkdatapt = (BLKSTR *) (((char *) mapblockstrpt) + *mymappt);
		else {
			myanpt = (ANISTR *) (mapanimstrendpt + *mymappt);
			blkdatapt = (BLKSTR *) * ((long int *) (myanpt->ancuroff));
		}
		dsrec.y = i;
		dsrec.h = MBSH;
		if (blkdatapt->fgoff) {
			if (blkdatapt->tl2)	// transparency bit
				SDL_SetColorKey(mapTiles[blkdatapt->fgoff],
								SDL_SRCCOLORKEY, transpcol);

			if (SDL_BlitSurface
				(mapTiles[blkdatapt->fgoff], &srrec, Dest, &dsrec) == -2) {
				return 1;
			}

			if (blkdatapt->tl2)	// transparency bit
				SDL_SetColorKey(mapTiles[blkdatapt->fgoff], 0, 0);
		}
		i += MBSH;
	}

	// Draw bottom left clipped block
	mymappt += mapwidth;
	if (*mymappt >= 0)
		blkdatapt = (BLKSTR *) (((char *) mapblockstrpt) + *mymappt);
	else {
		myanpt = (ANISTR *) (mapanimstrendpt + *mymappt);
		blkdatapt = (BLKSTR *) * ((long int *) (myanpt->ancuroff));
	}
	srrec.h = MMOY + MSCRH - i;
	dsrec.y = i;
	dsrec.h = MMOY + MSCRH - i;
	if (blkdatapt->fgoff) {
		if (blkdatapt->tl2)		// transparency bit
			SDL_SetColorKey(mapTiles[blkdatapt->fgoff], SDL_SRCCOLORKEY,
							transpcol);

		if (SDL_BlitSurface
			(mapTiles[blkdatapt->fgoff], &srrec, Dest, &dsrec) == -2) {
			return 1;
		}

		if (blkdatapt->tl2)		// transparency bit
			SDL_SetColorKey(mapTiles[blkdatapt->fgoff], 0, 0);
	}
	// Draw bottom clipped blocks
	srrec.x = 0;
	srrec.w = MBSW;
	j = MMOX + MBSW - maphclip;
	while (j < (MSCRW + MMOX - MBSW)) {
		mymappt++;
		if (*mymappt >= 0)
			blkdatapt = (BLKSTR *) (((char *) mapblockstrpt) + *mymappt);
		else {
			myanpt = (ANISTR *) (mapanimstrendpt + *mymappt);
			blkdatapt = (BLKSTR *) * ((long int *) (myanpt->ancuroff));
		}
		dsrec.x = j;
		dsrec.w = MBSW;
		if (blkdatapt->fgoff) {
			if (blkdatapt->tl2)	// transparency bit
				SDL_SetColorKey(mapTiles[blkdatapt->fgoff],
								SDL_SRCCOLORKEY, transpcol);

			if (SDL_BlitSurface
				(mapTiles[blkdatapt->fgoff], &srrec, Dest, &dsrec) == -2) {
				return 1;
			}

			if (blkdatapt->tl2)	// transparency bit
				SDL_SetColorKey(mapTiles[blkdatapt->fgoff], 0, 0);
		}
		j += MBSW;
	}

	// Draw bottom right clipped block
	srrec.w = MMOX + MSCRW - j;
	mymappt++;
	if (*mymappt >= 0)
		blkdatapt = (BLKSTR *) (((char *) mapblockstrpt) + *mymappt);
	else {
		myanpt = (ANISTR *) (mapanimstrendpt + *mymappt);
		blkdatapt = (BLKSTR *) * ((long int *) (myanpt->ancuroff));
	}
	dsrec.x = j;
	dsrec.w = MMOX + MSCRW - j;
	if (blkdatapt->fgoff) {
		if (blkdatapt->tl2)		// transparency bit
			SDL_SetColorKey(mapTiles[blkdatapt->fgoff], SDL_SRCCOLORKEY,
							transpcol);

		if (SDL_BlitSurface
			(mapTiles[blkdatapt->fgoff], &srrec, Dest, &dsrec) == -2) {
			return 1;
		}

		if (blkdatapt->tl2)		// transparency bit
			SDL_SetColorKey(mapTiles[blkdatapt->fgoff], 0, 0);
	}
	// Draw right clipped blocks
	srrec.h = MBSH;
	while (i > (MMOY + MBSH)) {
		mymappt -= mapwidth;
		i -= MBSH;
		if (*mymappt >= 0)
			blkdatapt = (BLKSTR *) (((char *) mapblockstrpt) + *mymappt);
		else {
			myanpt = (ANISTR *) (mapanimstrendpt + *mymappt);
			blkdatapt = (BLKSTR *) * ((long int *) (myanpt->ancuroff));
		}
		dsrec.y = i;
		dsrec.h = MBSH;
		if (blkdatapt->fgoff) {
			if (blkdatapt->tl2)	// transparency bit
				SDL_SetColorKey(mapTiles[blkdatapt->fgoff],
								SDL_SRCCOLORKEY, transpcol);

			if (SDL_BlitSurface
				(mapTiles[blkdatapt->fgoff], &srrec, Dest, &dsrec) == -2) {
				return 1;
			}

			if (blkdatapt->tl2)	// transparency bit
				SDL_SetColorKey(mapTiles[blkdatapt->fgoff], 0, 0);
		}
	}

	// Draw top right clipped block
	dsrec.y = MMOY;
	dsrec.h = i - MMOY;
	mymappt -= mapwidth;
	i = MMOY;
	srrec.y = mapvclip;
	srrec.h = MBSH - mapvclip;
	if (*mymappt >= 0)
		blkdatapt = (BLKSTR *) (((char *) mapblockstrpt) + *mymappt);
	else {
		myanpt = (ANISTR *) (mapanimstrendpt + *mymappt);
		blkdatapt = (BLKSTR *) * ((long int *) (myanpt->ancuroff));
	}
	if (blkdatapt->fgoff) {
		if (blkdatapt->tl2)		// transparency bit
			SDL_SetColorKey(mapTiles[blkdatapt->fgoff], SDL_SRCCOLORKEY,
							transpcol);

		if (SDL_BlitSurface
			(mapTiles[blkdatapt->fgoff], &srrec, Dest, &dsrec) == -2) {
			return 1;
		}

		if (blkdatapt->tl2)		// transparency bit
			SDL_SetColorKey(mapTiles[blkdatapt->fgoff], 0, 0);
	}
	// Draw top clipped blocks
	srrec.w = MBSW;
	while (j > (MMOX + MBSW)) {
		mymappt--;
		j -= MBSW;
		if (*mymappt >= 0)
			blkdatapt = (BLKSTR *) (((char *) mapblockstrpt) + *mymappt);
		else {
			myanpt = (ANISTR *) (mapanimstrendpt + *mymappt);
			blkdatapt = (BLKSTR *) * ((long int *) (myanpt->ancuroff));
		}
		dsrec.x = j;
		dsrec.w = MBSW;
		if (blkdatapt->fgoff) {
			if (blkdatapt->tl2)	// transparency bit
				SDL_SetColorKey(mapTiles[blkdatapt->fgoff],
								SDL_SRCCOLORKEY, transpcol);

			if (SDL_BlitSurface
				(mapTiles[blkdatapt->fgoff], &srrec, Dest, &dsrec) == -2) {
				return 1;
			}

			if (blkdatapt->tl2)	// transparency bit
				SDL_SetColorKey(mapTiles[blkdatapt->fgoff], 0, 0);
		}
	}

	// Draw unclipped (full) blocks
	srrec.y = 0;
	srrec.h = MBSH;
	dsrec.h = MBSH;
	dsrec.w = MBSW;
	i = MMOY + MBSH - mapvclip;
	while (i < (MSCRH + MMOY - MBSH)) {
		mymappt += mapwidth;
		mymap2pt = mymappt;
		j = MMOX + MBSW - maphclip;
		while (j < (MSCRW + MMOX - MBSW)) {
			if (*mymappt >= 0)
				blkdatapt =
					(BLKSTR *) (((char *) mapblockstrpt) + *mymappt);
			else {
				myanpt = (ANISTR *) (mapanimstrendpt + *mymappt);
				blkdatapt = (BLKSTR *) * ((long int *) (myanpt->ancuroff));
			}

			dsrec.x = j;
			dsrec.y = i;
			if (blkdatapt->fgoff) {
				if (blkdatapt->tl2)	// transparency bit
					SDL_SetColorKey(mapTiles[blkdatapt->fgoff],
									SDL_SRCCOLORKEY, transpcol);

				if (SDL_BlitSurface
					(mapTiles[blkdatapt->fgoff], &srrec, Dest,
					 &dsrec) == -2) {
					return 1;
				}

				if (blkdatapt->tl2)	// transparency bit
					SDL_SetColorKey(mapTiles[blkdatapt->fgoff], 0, 0);
			}
			mymappt++;
			j += MBSW;
		}
		mymappt = mymap2pt;
		i += MBSH;
	}
	return 0;
}
