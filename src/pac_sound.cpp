#include <string.h>
#include "SDL.h"
#include "gamedefs.h"
#include "pac_sound.h"

GSound::GSound(Uint16 id, CGame* igame, GAME_MAXOBJS_TYPE islot):GObj(id, igame, islot),
// implicit entity initialization

xpos        (GSOUND_ID+0 , 0         , 0),   
ypos        (GSOUND_ID+1 , 0         , 0),   
loop        (GSOUND_ID+2 , 0         , 0),   
volume      (GSOUND_ID+3 , SM_STD_VOLUME         , 0),   
sampleid    (GSOUND_ID+4 , 0         , 0)
{
  next_restart = 0;
}

GSound::~GSound()
{
}

Uint8 GSound::GetType()
{
  return ot_sound;
}


void GSound::ServerThink(Uint32 time)
{
  if (time-createtime>10) state = 0;
  // do test collissions, hurting players, random actions, and so on
}

void GSound::ClientThink(Uint32 time)
{
  curtime = time;

  if (!next_restart || next_restart == curtime)
  {
    smPlaySample3D(sampleid, xpos, ypos, volume);
    if (loop==0)
    {
      state = 0;
    }
    else
    {
      next_restart = curtime + loop;
    }
  }
}

void GSound::Draw(SDL_Surface * screen, SDL_Rect * screen_rect)
{
  // sounds are not drawn
}

void GSound::Replication(int cnum, CReplicator* rep)
{
  GObj::Replication(cnum, rep);

  xpos.Write(rep, cnum);
  ypos.Write(rep, cnum);
  sampleid.Write(rep, cnum);
  loop.Write(rep, cnum);
  volume.Write(rep, cnum);
}

char GSound::GetReplicated(Uint8 id, net_msg* msg, TICK_TYPE time)
{
  
  if (GObj::GetReplicated(id, msg, time)) return 1;
  
  if (xpos.Read(id, msg ,time)) return 1;
  if (ypos.Read(id, msg ,time)) return 1;
  if (sampleid.Read(id, msg ,time)) return 1;
  if (loop.Read(id, msg ,time)) return 1;
  if (volume.Read(id, msg ,time)) return 1;
  return 0;
}

void GSound::MakeDirty(int i, bool dirty)
{
  GObj::MakeDirty(i, dirty);
  
	xpos.MakeDirty(i, dirty);
	ypos.MakeDirty(i, dirty);
  sampleid.MakeDirty(i, dirty);
	loop.MakeDirty(i, dirty);
	volume.MakeDirty(i, dirty);
}

void GSound::NormalizePos()
{
  if (xpos<0) xpos+=mappxwidth;
  else
    if (xpos>=mappxwidth) xpos-=mappxwidth;

  if (ypos<0) ypos+=mappxheight;
  else
    if (ypos>=mappxheight) ypos-=mappxheight;
}










