/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   game Vars API
//## author:  Antonin Hildebrand
//## 
//## started: 25.6.2000
//## revised: ???
//## 
//##          safe vars api for import to scripts
//## 
//###########################################################################

#include "vapi.h"
#include "seer.h"
#include "server.h"
#include "client.h"
#include "pw2.h"
#include "sampleman.h"

//###########################################################################
//## scripting externals
//###########################################################################

void AddVapiExternals()
{
  scAdd_External_Symbol("svGetVars",      (void*)svGetVars);
  scAdd_External_Symbol("svSetVars",      (void*)svSetVars);

  scAdd_External_Symbol("cvGetVars",      (void*)cvGetVars);
  scAdd_External_Symbol("cvSetVars",      (void*)cvSetVars);

  scAdd_External_Symbol("cvPlayMusic",      (void*)cvPlayMusic);
  scAdd_External_Symbol("cvPauseMusic",     (void*)cvPauseMusic);
  scAdd_External_Symbol("cvVolumeMusic",    (void*)cvVolumeMusic);

  scAdd_External_Symbol("smPlaySample3D",   (void*)smPlaySample3D);
  scAdd_External_Symbol("smPlaySample",     (void*)smPlaySample);
  scAdd_External_Symbol("smPlayVoice",      (void*)smPlayVoice);
  scAdd_External_Symbol("smSetPosition",    (void*)smSetPosition);
}
 
//###########################################################################
//## script actors functions
//###########################################################################

//###########################################################################
//## SERVER SIDE
//###########################################################################

int svGetVars(SCVAR& vars)
{
  vars.camx = server_info.game.vars.camx;
  vars.camy = server_info.game.vars.camy;
  vars.zonex = server_info.game.vars.zonex;
  vars.zoney = server_info.game.vars.zoney;
  vars.chasing = server_info.game.vars.chasing;
  return SC_NOERR;
}

int svSetVars(SCVAR& vars)
{
  // TODO: consistency checks
  server_info.game.vars.camx = vars.camx;
  server_info.game.vars.camy = vars.camy;
  server_info.game.vars.zonex = vars.zonex;
  server_info.game.vars.zoney = vars.zoney;
  server_info.game.vars.chasing = vars.chasing;
  return SC_NOERR;
}

//###########################################################################
//## CLIENT SIDE
//###########################################################################

int cvGetVars(SCVAR& vars)
{
  vars.camx = client_info.game.vars.camx;
  vars.camy = client_info.game.vars.camy;
  vars.zonex = client_info.game.vars.zonex;
  vars.zoney = client_info.game.vars.zoney;
  vars.chasing = client_info.game.vars.chasing;
  return SC_NOERR;
}

int cvSetVars(SCVAR& vars)
{
  // TODO: consistency checks
  client_info.game.vars.camx = vars.camx;
  client_info.game.vars.camy = vars.camy;
  client_info.game.vars.zonex = vars.zonex;
  client_info.game.vars.zoney = vars.zoney;
  client_info.game.vars.chasing = vars.chasing;
  if (client_info.game.vars.chasing==-1) 
  {
    smSetPosition(vars.camx + MSCRW/2, vars.camy + MSCRH/2);
  }
  else
  {
    SCPLAYER p;
    if (client_info.game.objs[client_info.game.vars.chasing]->state&OSTATE_ACTIVE)
    {
      if (client_info.game.objs[client_info.game.vars.chasing]->GetType()==ot_player)    
      {
        GPlayer * p = (GPlayer*)client_info.game.objs[client_info.game.vars.chasing];
        smSetPosition(p->xpos, p->ypos, p->direction?-1:1);
      }
    }
  }
  return SC_NOERR;
}

int cvPlayMusic(char* fname)
{
  Play_Music(fname);
  return SC_NOERR;
}

int cvPauseMusic()
{
  Pause_Music("");
  return SC_NOERR;
}

int cvVolumeMusic(char* _0to64asstring)
{
  Volume_Music(_0to64asstring);
  return SC_NOERR;
}
