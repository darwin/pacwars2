#include "const.h"
#include "gamedefs.h"

GObj::GObj(Uint16 id, CGame* igame, GAME_MAXOBJS_TYPE islot):
// implicit object initialization
state       (GOBJ_ID+0 , 0            , 0), //OSTATE_ACTIVE|OSTATE_VISIBLE,  0),   
createtime  (GOBJ_ID+1 , 0            , 0)
{
	game = igame;
	slot = islot;
  oid = id;

	curtime = 0;
  was_relevant = false;

  role = ROLE_DumbProxy;
}

GObj::~GObj()
{
}

void GObj::Replication(int cnum, CReplicator* rep)
{
  state.Write(rep, cnum);
  createtime.Write(rep, cnum);
}

char GObj::GetReplicated(Uint8 id, net_msg* msg, TICK_TYPE time)
{
  if (state.Read(id, msg ,time)) return 1;
  if (createtime.Read(id, msg ,time)) return 1;
  
  return 0;
}

void GObj::MakeDirty(int i, bool dirty)
{
  state.MakeDirty(i, dirty);
  createtime.MakeDirty(i, dirty);
}


















