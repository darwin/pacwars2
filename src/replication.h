/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   replication object
//## author:  Antonin Hildebrand
//##         
//## started: 7.6.2000
//## revised: ???
//## 
//## 
//###########################################################################

#ifndef _PACREPLIC_H_
#define _PACREPLIC_H_

#include "const.h"
#include "net.h"

//###########################################################################
//## Data structures
//###########################################################################

// replikator ma za ukol:
// 1. roztridit vkladana data podle layeru

// 2. AssembleMsg - sestroji paket pro poslani

class DECLSPEC CReplicator {
public:
    Uint16 pos[REP_MAX_LAYER];
    Uint8  buf[REP_MAX_LAYER][REP_BUFFER_SIZE+100];
    
    Uint16 markers[REP_MAX_LAYER][REP_MAX_MARKERS];
    Uint16 mpos[REP_MAX_LAYER];
    
    Uint16 wpos[REP_MAX_LAYER];
   
    CReplicator(int ilayer=0);
    ~CReplicator();

    void Reset();
    void SetLayer(char ilayer=0);
    void Mark();

    void W8(Uint8 val);
    void W16(Uint16 val);
    void W32(Uint32 val);
    void WStr(char* s);
    void WMem(void* src, int len);
    
    Uint8  R8(void);
    Uint16 R16(void);
    Uint32 R32(void);
    char*  RStr(void);
    void   RMem(void* dest, int len);

    CReplicator& operator<<(CReplicator&);
    CReplicator& operator<<(Uint8);
    CReplicator& operator<<(Uint16);
    CReplicator& operator<<(Uint32);
    CReplicator& operator<<(Sint8);
    CReplicator& operator<<(Sint16);
    CReplicator& operator<<(Sint32);
    CReplicator& operator<<(char *);
    CReplicator& operator<<(PString);
    
    CReplicator& operator>>(CReplicator&);
    CReplicator& operator>>(Uint8&);
    CReplicator& operator>>(Uint16&);
    CReplicator& operator>>(Uint32&);
    CReplicator& operator>>(Sint8&);
    CReplicator& operator>>(Sint16&);
    CReplicator& operator>>(Sint32&);
    CReplicator& operator>>(char*&);
    CReplicator& operator>>(PString&);

    void nprintf(size_t count, const char *fmt, ... );

    char AssembleMsg(net_msg * msg, int max_len=0);

    int layer;
    bool dirty;
};

template <class VT> 
class RValue {
private:
  VT        value;
  VT        ivalue;
  TICK_TYPE timestamp;
  Uint8     id;
  bool      dirty[PWP_TOTALMAX_CLIENTS];

public:

  RValue(Uint8 iid, VT implicit_value, TICK_TYPE start_time) 
  { 
    id = iid;
    value = implicit_value;
    ivalue = implicit_value;
    timestamp = start_time;
    MakeDirty(-1, false);
  };

  ~RValue() 
  { 
  };



  VT* GetValRef() { return &value; }

  operator VT() 
  { 
    return value; 
  };

  RValue<VT>& operator=(VT rs) 
  { 
    Set(rs);
    return *this;
  };

  RValue<VT>& operator+=(VT rs) 
  { 
    Set(value+rs);
    return *this;
  };

  RValue<VT>& operator-=(VT rs) 
  { 
    Set(value-rs);
    return *this;
  };

  RValue<VT>& operator*=(VT rs) 
  { 
    Set(value*rs);
    return *this;
  };

  RValue<VT>& operator/=(VT rs) 
  { 
    Set(value/rs);
    return *this;
  };

  RValue<VT>& operator++(int) 
  { 
    VT nv = value+1;
    Set(nv);
    return *this;
  };

  RValue<VT>& operator--(int) 
  { 
    VT nv = value-1;
    Set(nv);
    return *this;
  };
  
  inline bool Set(VT new_value, TICK_TYPE time=0)
  {
    if (value != new_value)
    {
      value = new_value;
      if (time) timestamp = time;
      MakeDirty(-1, true);
      return true;
    }
    else
      return false;
  }

  inline bool SetImplicit(VT new_value, TICK_TYPE time=0)
  {
    ivalue = new_value;
    return true;
  }

  inline bool Read(Uint8 nid, net_msg * msg, TICK_TYPE time)
  {
    if (nid==id) // is there waiting my value type in message ?
    {
      if (timestamp<=time) // we got fresh replication code
      {
        (*msg)>>value ;  // update data
        timestamp = time;
        return true;
      }
      else  // we got old replication code
      {
        VT temp;         
        (*msg)>>temp;     // drop data
        return false;
      }
    }
    return false;
  }

  inline bool ReadAndDiscard(Uint8 nid, net_msg * msg, TICK_TYPE time)
  {
    if (nid==id) // is there waiting my value type in message ?
    {
      VT temp;         
      (*msg)>>temp;     // drop data
      return true;
    }
    return false;
  }
  
  inline void MakeDirty(int i, bool flag)
  {
    if (i!=-1) 
    {  
      if (i<PWP_TOTALMAX_CLIENTS)
        dirty[i] = flag;                                            
      else
        if (value==ivalue) 
          dirty[i-PWP_TOTALMAX_CLIENTS] = false; 
        else 
          dirty[i-PWP_TOTALMAX_CLIENTS] = true;               // this is special hack- make dirty only nonimplicit values
    }
	  else for (int i=0; i<PWP_TOTALMAX_CLIENTS; i++) dirty[i] = flag;
     
  }

  inline bool IsDirty(int i)
  {
    if (i!=-1) return dirty[i];
	else
    for (int i=0; i<PWP_TOTALMAX_CLIENTS; i++) if (dirty[i]) return true;
    return false;
  }

  inline bool Write(CReplicator * rep, int cnum)
  {
    if (dirty[cnum])
      if (rep->pos[rep->layer]<REP_BUFFER_SIZE)
      {
        (*rep)<<id<<value;
        dirty[cnum]=false;
        return true;
      }
      else ConOut("Rep. full");
    return false;
  }

};

#endif
