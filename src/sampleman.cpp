/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   Sample manager
//## author:  Antonin Hildebrand
//## 
//## started: 17.11.2000
//## revised: ???
//## 
//###########################################################################
#include "const.h"

#ifdef DISABLE_BASS
#undef PW_BASS
#endif


#ifdef PW_BASS
#include "gmath.h"
#include "map.h"

#include "bass.h"
#else
#include "SDL_mixer.h"
#endif

#include "sampleman.h"
#include "SDL_Console.h"

#define SM_MAX_SAMPLES 100

short int frontz = 1;

bool loaded3dsounds = true;
#ifdef PW_BASS
BASS_3DVECTOR v = { 0, 0, 1};
#endif

#define MAX_AUDIBLEDESTINATION (40*16)   // only non 3d mode

void CHsound_lr(cvar_t* var, int* changed)
{
#ifdef PW_BASS
  switch ((short int)var->value) {
  case 0: frontz = 1; break;
  case 1: frontz = -1; break;
  }
  smSetPosition((int)v.x, (int)v.y, 0);
#endif
}

void CHsound_settings(cvar_t* var, int* changed)
{
#ifdef PW_BASS
  BASS_Set3DFactors(snd_dist.value, snd_roll.value, snd_dopp.value);
  BASS_Apply3D(); // apply the change 
#endif
}

void CHsound_3dnotify(cvar_t* var, int* changed)
{
  if (var->value==1.0f) 
    ConOut("3D sound is ON, action will take effect after restarting program.");
  else
    ConOut("3D sound is OFF, action will take effect after restarting program.");
}

// cvars
cvar_t	snd_swap = {"snd_swap", "0", true, false, CHsound_lr};

cvar_t	snd_dist = {"snd_dist", "0.05", true, false, CHsound_settings};
cvar_t	snd_roll = {"snd_roll", "0.008", true, false, CHsound_settings};
cvar_t	snd_dopp = {"snd_dopp", "1.0", true, false, CHsound_settings};
cvar_t	snd_3don = {"snd_3don", "1.0", true, false, CHsound_3dnotify};


#ifdef PW_BASS

#define LOAD_SAMPLE(id, fname, flags) \
	FileNameConversion(snd_dir.string, fname, "wav", tmptxt); \
	if (!(sm_samples[id] = BASS_SampleLoad(FALSE,tmptxt,0,0,3,flags))) \
		ConErr("SampleManager: Coudn't load sample %s", tmptxt);

#else

#define LOAD_SAMPLE(id, fname, flags) \
	FileNameConversion(snd_dir.string, fname, "wav", tmptxt); \
	if (!(sm_samples[id] = Mix_LoadWAV(tmptxt))) \
		ConErr("SampleManager: Coudn't load sample %s", tmptxt);

#endif

#ifdef PW_BASS
HSAMPLE sm_samples[SM_MAX_SAMPLES];
#else
Mix_Chunk* sm_samples[SM_MAX_SAMPLES];
#endif

void smLoadSamples()
{
  char tmptxt[_MAX_PATH];

#ifdef PW_BASS
  unsigned int varBASS_SAMPLE_3D;
  if (snd_3don.value) 
  {
    varBASS_SAMPLE_3D = BASS_SAMPLE_OVER_DIST|BASS_SAMPLE_MUTEMAX|BASS_SAMPLE_3D; 
    loaded3dsounds = true;
  }
  else 
  {
    varBASS_SAMPLE_3D = BASS_SAMPLE_OVER_POS;
    loaded3dsounds = false;
  }
#endif

  LOAD_SAMPLE(SM_RAIL_F1,   "railf1"  ,varBASS_SAMPLE_3D);
//  LOAD_SAMPLE(SM_SHOT_F1,   "shotf1"  ,varBASS_SAMPLE_3D);
//  LOAD_SAMPLE(SM_SHOT_R1,   "shotr1"  ,varBASS_SAMPLE_3D);
  LOAD_SAMPLE(SM_10_0   ,   "10_0"    ,BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_BOMBPUT,   "bombput" ,varBASS_SAMPLE_3D);
//  LOAD_SAMPLE(SM_BORN,      "born"    ,varBASS_SAMPLE_3D);
  LOAD_SAMPLE(SM_MESSENGER, "cink"   ,BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_MENU1,     "menu1"   ,BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_MENU2,     "menu2"   ,BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_MENU3,     "menu3"   ,BASS_SAMPLE_OVER_POS);

  LOAD_SAMPLE(SM_XPLO1  ,   "xplo1"   ,varBASS_SAMPLE_3D);
  LOAD_SAMPLE(SM_XPLO2  ,   "xplo2"   ,varBASS_SAMPLE_3D);

  LOAD_SAMPLE(SM_GRANATF1  , "granatf1",varBASS_SAMPLE_3D);

  LOAD_SAMPLE(SM_NARAZ    , "naraz",varBASS_SAMPLE_3D);
  LOAD_SAMPLE(SM_BECHERF1  , "becherf1",varBASS_SAMPLE_3D);
  LOAD_SAMPLE(SM_WARP    , "warp",varBASS_SAMPLE_3D);

  LOAD_SAMPLE(SM_GPREMIE    , "getpremie",varBASS_SAMPLE_3D);
  LOAD_SAMPLE(SM_GAMMO    , "getammo",varBASS_SAMPLE_3D);

  LOAD_SAMPLE(SM_PISTOLKAF1  , "pistolkaf1",varBASS_SAMPLE_3D);
  LOAD_SAMPLE(SM_RSHOT    , "rshot",varBASS_SAMPLE_3D);


  LOAD_SAMPLE(SM_BECHERC1  , "bc1", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_BECHERA1  , "bd1", BASS_SAMPLE_OVER_POS);

  LOAD_SAMPLE(SM_BERSERK  , "berserk1", BASS_SAMPLE_OVER_POS);

  LOAD_SAMPLE(SM_LEADER1    , "leader1", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_DOMINATION1  , "dom1", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_KMACHINE1  , "km1", BASS_SAMPLE_OVER_POS);


  LOAD_SAMPLE(SM_ACCURACY1  , "acu1", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_EXCELENT1  , "excelent1", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_GOTCHA     , "gotcha", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_IMPRESIVE  , "imp1", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_PERFECT1   , "perfect1", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_PERFECT2   , "perfect2", BASS_SAMPLE_OVER_POS);

  LOAD_SAMPLE(SM_FIGHT1     , "fight1", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_FIGHT2     , "fight2", BASS_SAMPLE_OVER_POS);

  LOAD_SAMPLE(SM_LOSTLEAD   , "llead1", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_TAKENLEAD  , "tlead1", BASS_SAMPLE_OVER_POS);

  LOAD_SAMPLE(SM_OHNO   , "ohno", varBASS_SAMPLE_3D);

  LOAD_SAMPLE(SM_PREPARE  , "prepare1", BASS_SAMPLE_OVER_POS);

  LOAD_SAMPLE(SM_DEATH1    , "d1", varBASS_SAMPLE_3D);
  LOAD_SAMPLE(SM_DEATH2    , "d2", varBASS_SAMPLE_3D);
  LOAD_SAMPLE(SM_DEATH3    , "d3", varBASS_SAMPLE_3D);
  LOAD_SAMPLE(SM_DEATH4    , "d4", varBASS_SAMPLE_3D);
  LOAD_SAMPLE(SM_DEATH5    , "d5", varBASS_SAMPLE_3D);

  LOAD_SAMPLE(SM_BANZAI  , "banzai", varBASS_SAMPLE_3D);
  LOAD_SAMPLE(SM_BYEBYE  , "byebye", BASS_SAMPLE_OVER_POS);

  LOAD_SAMPLE(SM_END1    , "end1", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_END2    , "end2", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_END3    , "end3", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_END4    , "end4", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_END5    , "end5", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_END6    , "end6", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_END7    , "end7", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_END8    , "end8", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_END9    , "end9", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_END10   , "end10", BASS_SAMPLE_OVER_POS);


  LOAD_SAMPLE(SM_WIN1    , "win1", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_WIN2    , "win2", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_WIN3    , "win3", BASS_SAMPLE_OVER_POS);

  LOAD_SAMPLE(SM_LAMER    , "lamer", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_LOOSER1  , "looser1", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_LOOSER2  , "looser2", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_SMOKE    , "smoke", BASS_SAMPLE_OVER_POS);
  LOAD_SAMPLE(SM_STUPID   , "stupid", BASS_SAMPLE_OVER_POS);

  LOAD_SAMPLE(SM_OUTCH    , "outch", BASS_SAMPLE_OVER_POS);

  LOAD_SAMPLE(SM_UPSSS  , "upsss", BASS_SAMPLE_OVER_POS);

#ifdef PW_BASS
  BASS_Set3DFactors(snd_dist.value, snd_roll.value, snd_dopp.value);
  BASS_Apply3D(); // apply the change 
#endif

  int i;
  CHsound_lr(&snd_swap, &i);
}

bool smPlaySample3D(Uint8 id, Sint16 x, Sint16 y, Uint8 volume)
{
#ifdef PW_BASS

  if (loaded3dsounds) 
  {
    if (sm_samples[id])
    {
      BASS_3DVECTOR v;
      v.x = x;
      v.y = y;
      v.z = 0;
      BASS_SamplePlay3DEx(sm_samples[id], &v, NULL, NULL, 0, -1, volume, -1);
    
      return true;
    }
  }
  else // not loaded samples with 3d flag
  {
    if (sm_samples[id])
    {
      Sint16 dx = ABS(int(x - v.x));
      Sint16 dy = ABS(int(y - v.y));
      
      if (dx<0) dx+=mappxwidth;
      else
        if (dx>=mappxwidth) dx-=mappxwidth;

      if (dy<0) dy+=mappxheight;
      else
        if (dy>=mappxheight) dy-=mappxheight;
      
      if (dx>mappxwidth/2) dx=mappxwidth-dx;
      if (dy>mappxheight/2) dy=mappxheight-dy;
      
      int m = MAX(dx, dy);
      if (m>=MAX_AUDIBLEDESTINATION) return true;
      m = MAX_AUDIBLEDESTINATION - m;
      
      BASS_SamplePlayEx(sm_samples[id], 0, -1, ((volume)*m)/MAX_AUDIBLEDESTINATION, -101, -1);
      //ConOut("m=%d , v=%d", m, ((volume)*m)/MAX_AUDIBLEDESTINATION);
      return true;
    }
  }
#else
	sm_samples[id]->volume = volume*2;
	Mix_PlayChannel(-1, sm_samples[id], 0);
#endif
  return false;
}

bool smPlaySample(Uint8 id, Uint8 volume)
{
	if (sm_samples[id]) {
#ifdef PW_BASS
		BASS_SamplePlayEx(sm_samples[id], 0, -1, volume, -101, -1);
#else
		sm_samples[id]->volume = volume*2;
		Mix_PlayChannel(-1, sm_samples[id], 0);
#endif
		return true;
	}
	return false;
}

bool smPlayVoice(Uint8 id, Uint8 volume, Uint8 priority)
{
#ifdef PW_BASS

  static HCHANNEL last_channel = -1;
  static Uint8 last_priority = 0;
  if (sm_samples[id])
  {
    if (!BASS_ChannelIsActive(last_channel))
    {  
      last_channel = BASS_SamplePlayEx(sm_samples[id], 0, -1, volume, -101, -1);
      last_priority = priority;
    }
    else
      if (priority>last_priority)
      {
        BASS_ChannelStop(last_channel);
        last_channel = BASS_SamplePlayEx(sm_samples[id], 0, -1, volume, -101, -1);
        last_priority = priority;
      }

    return true;
  }

#else

	if (sm_samples[id]) {
		sm_samples[id]->volume = volume*2;
		Mix_PlayChannel(-1, sm_samples[id], 0);
	}

#endif

  return false;
}

void smFreeSamples()
{
  for (int i=0; i<SM_MAX_SAMPLES; i++)
    if (sm_samples[i])
    {
#ifdef PW_BASS
      BASS_SampleFree(sm_samples[i]);
#else
		Mix_FreeChunk(sm_samples[i]);
#endif
      sm_samples[i] = 0;
    }
}

void smSetPosition(Uint16 x, Uint16 y, int direction)
{
#ifdef PW_BASS

  BASS_3DVECTOR front = { 0, 0, frontz};//{ direction, 0, 0};
  BASS_3DVECTOR top = { 0, 1, 0};
  
  if (v.x!=x || v.y!=y || v.z!=0)
  {
    v.x = x;
    v.y = y;
    v.z = 0;
  
    BASS_Set3DPosition(&v, NULL, &front, &top);
    BASS_Apply3D(); // apply the change 
  }
#endif
}
