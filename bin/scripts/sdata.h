/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   Scripting API - data structures
//## author:  Antonin Hildebrand
//## 
//## started: 12.3.2000
//## revised: ???
//## 
//##          used to include in scripts & main program
//## 
//###########################################################################

#ifndef _SCRIPT_DATA_H_
#define _SCRIPT_DATA_H_

// ONLY DATA TYPES
// MUST BE COMPATIBILE WITH PACSCRIPT

// NO POINTERS AT ALL - only indexes

// block & anim structures remaked for scripts
// "!!" means changes in data type from dual structure defined in "map.h"
struct SCBLK{				                // Structure for data blocks 
  int bgoff, fgoff;			            // offsets from start of graphic blocks 
  unsigned int user1, user2;	      //!! user long data 
  unsigned int user3, user4;	      //!! user  data 
  unsigned char user5, user6, user7;// user byte data 
  unsigned char tl1;				        // able to go over (tile collision)
  unsigned char tr1;                // able to throw over
  unsigned char bl1;                
  unsigned char br1;                
  unsigned char tl2;				        // tile with color-key
  unsigned char tr2;                // kill
  unsigned char bl2;                // tile with alpha
  unsigned char br2;                // bounce
};

struct SCANI{		          // Animation control structure
  signed char antype;	    // Type of anim, AN_?
  signed char andelay;	  // Frames to go before next frame
  signed char ancount;	  // Counter, decs each frame, till 0, then resets to andelay
  signed char anuser;	    // User info
  int curframe;	          // Current frame from 0 to numframes-1
  int numframes;	        // READ ONLY!
};

struct SCVAR{		          // game vars control structure
  int camx;
  int camy;
  int zonex;
  int zoney;
  int chasing;
};

#ifndef _CVAR_H_
class cvar_t;

typedef void (cvar_callback)(cvar_t* var, int* changed);

class cvar_t
{
public:
	char	*name;
	char	*string;
	int   archive;		// set to true to cause it to be saved to vars.rc
	int   server;		  // notifies players when changed
  cvar_callback* callback;
	float	value;
	cvar_t *next;
/*
  inline operator int() 
  { 
    return (int)value; 
  };
*/
};
#endif

//###########################################################################
//# objects
//###########################################################################


#ifdef SCRIPT_INTERFACE
#define MAX_PSTRING_CHARS 30 // must be enough for all uses of PString
class PString {
public:
  int len;
  char chars[MAX_PSTRING_CHARS];

  PString() { len = 0; }
//  operator char*() { return chars; }
//  void Clear() { for (int i=0; i<len; i++) chars[i]=0; }
};
#endif

struct SCOBJ{
  Uint16 oid;
  GAME_MAXOBJS_TYPE slot;
  
  Uint8 state;
  
  TICK_TYPE createtime;     // the time the object was created
  TICK_TYPE curtime;
};

struct SCENTITY{
  SCOBJ obj;

  Uint8 visible;

  // position
  Sint16 xpos;
  Sint16 ypos;
  
  // size
  Uint16 xsize;
  Uint16 ysize;
  
  // speed
  Uint16 xspeed;
  Uint16 yspeed;
  
  // acceleration
  Uint16 yaccel;
  Uint16 xaccel;
  
  // frame
  Uint16 framedelay;
  Uint8  frame;
  Uint8  anim;
  
  // polomer
  Uint8 radius;  // kdyz je radius=0, pouziju is_passable
  
  // sprite graphics
  PString spritename;
};

struct SCPLAYER{
  SCENTITY entity;
  
  Uint8 direction;  // 0=top, 1=right, 2=bottom, 3=left
  
  Uint16 last_fire_time;
  Uint16 last_bomb_time;
  
  Uint16 adjusttime;
  
  PString player_name;
  
  Uint8 brain_type;    // soulfree, client, bot,
  Uint8 brain_owner;   // 0,.., maxclients-1, maxclients==owner_is_server
  Uint8 kbindex;       // index to keybindigns array
  
  Sint8 frags;
  Uint8 bombs_ammo;
  Uint8 mines_ammo;
  Uint8 becher_ammo;
};

struct SCSHOT{
  SCENTITY entity;
  
  Uint8 bounced;
  Sint8 dx;
  
  GAME_MAXOBJS_TYPE owner;
};

struct SCEXTRA{
  SCENTITY entity;

  Uint8 type;
  Uint8 life;
  Uint8 modifier;       // 0 = instant, 1 = respawn, 2 = random
  Uint8 data;       
};

struct SCBOMB{
  SCENTITY entity;
  
  GAME_MAXOBJS_TYPE owner;
};

#endif
