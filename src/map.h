/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   Map system
//## author:  Antonin Hildebrand
//##          Robin Burrows
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

#ifndef _PACMAP_H_
#define _PACMAP_H_

//###########################################################################
//## Constants & defines
//###########################################################################

#define MBSW 16			  // Width of block, in pixels 
#define MBSH 16			  // Height of block, in pixels 

#define MTRANSPR 0xFF	// 8bit RED transparency value for high/true colour 
#define MTRANSPG 0x00	// 8bit GREEN transparency value for high/true colour 
#define MTRANSPB 0xFF	// 8bit BLUE transparency value for high/true colour 

#define MER_NONE 0		// All the horrible things that can go wrong 
#define MER_OUTOFMEM 1
#define MER_MAPLOADERROR 2
#define MER_NOOPEN 3

#define AN_END  -1		// Animation types, AN_END = end of anims 
#define AN_NONE  0		// No anim defined 
#define AN_LOOPF 1 		// Loops from start to end, then jumps to start etc 
#define AN_LOOPR 2 		// As above, but from end to start 
#define AN_ONCE  3		// Only plays once 
#define AN_ONCEH 4 		// Only plays once, but holds end frame 
#define AN_PPFF  5		// Ping Pong start-end-start-end-start etc 
#define AN_PPRR  6		// Ping Pong end-start-end-start-end etc 
#define AN_PPRF  7		// Used internally by playback 
#define AN_PPFR  8		// Used internally by playback 
#define AN_ONCES 9		// Used internally by playback 

#define MS_UNINITED 0
#define MS_INITED   1

//###########################################################################
//## Data structures
//###########################################################################

typedef struct {				            // Structure for data blocks 
  int bgoff, fgoff;			            // offsets from start of graphic blocks 
  int fgoff2, fgoff3; 		          // more overlay blocks 
  unsigned long int user1, user2;	  // user long data 
  unsigned short int user3, user4;	// user short data 
  unsigned char user5, user6, user7;// user byte data 
  unsigned char tl1 : 1;				    // wall
  unsigned char tr1 : 1;            // bounce
  unsigned char bl1 : 1;
  unsigned char br1 : 1;
  unsigned char tl2 : 1;		        // transparency bit		      
  unsigned char tr2 : 1;            // killing tile
  unsigned char bl2 : 1;
  unsigned char br2 : 1;
} BLKSTR;

typedef struct {		      // Animation control structure 
  signed char antype;	    // Type of anim, AN_? 
  signed char andelay;	  // Frames to go before next frame 
  signed char ancount;	  // Counter, decs each frame, till 0, then resets to andelay 
  signed char anuser;	    // User info 
  long int ancuroff;	    // Points to current offset in list 
  long int anstartoff;	  // Points to start of blkstr offsets list, AFTER ref. blkstr offset 
  long int anendoff;	    // Points to end of blkstr offsets list 
} ANISTR;

typedef struct {			    // Generic structure for chunk headers 
  char id1, id2, id3, id4;// 4 byte header id. 
  long int headsize;		  // size of header chunk. 
} GENHEAD;

typedef struct {		      // Map header structure 
  char mapverhigh;		    // map version number to left of . (ie X.0). 
  char mapverlow;		      // map version number to right of . (ie 0.X). 
  char lsb;			          // if 1, data stored LSB first, otherwise MSB first. 
  char reserved;
  short int mapwidth;	    // width in blocks. 
  short int mapheight;	  // height in blocks. 
  short int reserved1;
  short int reserved2;
  short int blockwidth;	  // width of a block (tile) in pixels. 
  short int blockheight;	// height of a block (tile) in pixels. 
  short int blockdepth;	  // depth of a block (tile) in planes (ie. 256 colours is 8) 
  short int blockstrsize;	// size of a block data structure 
  short int numblockstr;	// Number of block structures in BKDT 
  short int numblockgfx;	// Number of 'blocks' in graphics (BGFX) 
} MPHD;

class DECLSPEC CMapState {
public:
  int state;

  // allocated
  int smapmap[8];
  short int *mapmappt[8];
  int sblockstr;
  char *mapblockstrpt;
  int sanimstr;
  char *mapanimstrpt;

  // pointers only
  short int *mappt;
  char *mapanimstrendpt;
  short int **maparraypt;
  short int **mapmaparraypt[8];

  CMapState();
  ~CMapState();

  int  Free();
  int  Copy(CMapState* o);
  void CountAnimStrs(void);
  void InitAnims (void);
  void UpdateAnims (void);
  int  Restore (void);
  int  GenerateYLookup (void);
  int  ChangeLayer (int);
  BLKSTR* GetBlock (int, int);
  void SetBlock (int, int, int);

  int  DrawBG(SDL_Surface* Dest, int mapxo, int mapyo);
  int  DrawFG(SDL_Surface* Dest, int mapxo, int mapyo);
};

//###########################################################################
//## Globals
//###########################################################################

extern DECLSPEC char MapLoaded;
extern DECLSPEC char MapName[_MAX_PATH];
extern DECLSPEC char ScriptName[_MAX_PATH];
extern DECLSPEC CMapState MapLoadedState; 

extern DECLSPEC int MSCRW;
extern DECLSPEC int MSCRH;

extern DECLSPEC int MMOX;
extern DECLSPEC int MMOY;

extern DECLSPEC int maperror;		// Set to a MER_ error if something wrong happens 
extern DECLSPEC int mapwidth;
extern DECLSPEC int mapheight;
extern DECLSPEC int mappxwidth;
extern DECLSPEC int mappxheight;
extern DECLSPEC int mapblockwidth;
extern DECLSPEC int mapblockheight;
extern DECLSPEC int mapdepth;
extern DECLSPEC int mapblockstrsize;
extern DECLSPEC int mapnumblockstr;
extern DECLSPEC int mapnumanimstr;
extern DECLSPEC int mapnumblockgfx;
extern DECLSPEC unsigned char *mapcmappt;
extern DECLSPEC char *mapblockgfxpt;
extern DECLSPEC GENHEAD mapgenheader;
extern DECLSPEC SDL_Surface *mapTiles[1024];
// script
extern DECLSPEC int map_script_num;  // index to script manager



//###########################################################################
//## Exported functions
//###########################################################################

void MapFreeMem (void);
int  MapLoad (char* name, char* scrip);
int MapRestore(void);

//int  MapDecode (unsigned char *);

#endif