/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   replication class
//## author:  Antonin Hildebrand
//##         
//## started: 7.6.2000
//## revised: ???
//## 
//## 
//###########################################################################

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL.h"
#include "SDL_image.h"
#include "zlib.h"
#include "const.h"
#include "console/SDL_Console.h"
#include "SDL_net.h"
#include "net.h"
#include "server.h" 
#include "client.h"

//###########################################################################
//## Globals
//###########################################################################

//###########################################################################
//## General func
//###########################################################################


//###########################################################################
//## CReplicator functions
//###########################################################################

CReplicator::CReplicator(int ilayer)
{
    Reset();
    layer = ilayer;
}

CReplicator::~CReplicator()
{
}

void CReplicator::Reset()
{
    for (int i=0; i<REP_MAX_LAYER; i++) 
    {  
      pos[i] = 0;
      mpos[i] = 0;
      wpos[i] = 0;
    }
    dirty = false;
}

void CReplicator::SetLayer(char ilayer)
{
    layer = ilayer;
}

void CReplicator::W8(Uint8 val)
{
    if (pos[layer]>=REP_BUFFER_SIZE) return;
    buf[layer][pos[layer]] = val;
    pos[layer]++;
#ifdef PARANOID
    if (pos[layer]>REP_BUFFER_SIZE) fprintf(stderr, "REP: Write8 overflow");
#endif
    dirty = true;
}

void CReplicator::W16(Uint16 val)
{
    if (pos[layer]>=REP_BUFFER_SIZE) return;
    SDLNet_Write16(val, &buf[layer][pos[layer]]);
    pos[layer]+=2;
#ifdef PARANOID
    if (pos[layer]>REP_BUFFER_SIZE) fprintf(stderr, "REP: Write16 overflow");
#endif
    dirty = true;
}

void CReplicator::W32(Uint32 val)
{
    if (pos[layer]>=REP_BUFFER_SIZE) return;
    SDLNet_Write32(val, &buf[layer][pos[layer]]);
    pos[layer]+=4;
#ifdef PARANOID
    if (pos[layer]>REP_BUFFER_SIZE) fprintf(stderr, "REP: Write32 overflow");
#endif
    dirty = true;
}

void CReplicator::WStr(char* s)
{
    if (pos[layer]>=REP_BUFFER_SIZE) return;
    memcpy(&buf[layer][pos[layer]], s, strlen(s)+1);
    pos[layer]+=strlen(s)+1;
#ifdef PARANOID
    if (pos[layer]>REP_BUFFER_SIZE) fprintf(stderr, "REP: WriteString overflow");
#endif
    dirty = true;
}

void CReplicator::WMem(void* src, int len)
{
    if (pos[layer]>=REP_BUFFER_SIZE) return;
    memcpy(&buf[layer][pos[layer]], src, len);
    pos[layer]+=len;
#ifdef PARANOID
    if (pos[layer]>REP_BUFFER_SIZE) fprintf(stderr, "REP: WriteMem overflow");
#endif
    dirty = true;
}

CReplicator& CReplicator::operator<<(CReplicator& right)
{
    if (pos[layer]>=REP_BUFFER_SIZE) return *this;
    memcpy(&buf[layer][pos[layer]],&right.buf[right.layer][0], right.pos[right.layer]);
    pos[layer]+=right.pos[right.layer];
#ifdef PARANOID
    if (pos[layer]>REP_BUFFER_SIZE) fprintf(stderr, "REP: msg copy overflow");
#endif
    dirty = true;
    return *this;
}

CReplicator& CReplicator::operator<<(Uint8 val)
{
    W8(val);
    dirty = true;
    return *this;
}

CReplicator& CReplicator::operator<<(Uint16 val)
{
    W16(val);
    dirty = true;
    return *this;
}

CReplicator& CReplicator::operator<<(Uint32 val)
{
    W32(val);
    dirty = true;
    return *this;
}

CReplicator& CReplicator::operator<<(Sint8 val)
{
    W8((Uint8)val);
    dirty = true;
    return *this;
}

CReplicator& CReplicator::operator<<(Sint16 val)
{
    W16((Uint16)val);
    dirty = true;
    return *this;
}

CReplicator& CReplicator::operator<<(Sint32 val)
{
    W32((Uint32)val);
    dirty = true;
    return *this;
}


CReplicator& CReplicator::operator<<(char * str)
{
    WStr(str);
    dirty = true;
    return *this;
}

CReplicator& CReplicator::operator<<(PString ps)
{
    W8(ps.len);
    WMem(ps.chars, ps.len);
    dirty = true;
    return *this;
}


Uint8  CReplicator::R8(void)
{
  Uint8 r = buf[layer][pos[layer]];
  pos[layer]++;
#ifdef PARANOID
  if (pos[layer]>REP_BUFFER_SIZE) fprintf(stderr, "REP: Read8 overflow");
#endif
  return r;
}

Uint16 CReplicator::R16(void)
{
  Uint16 r = SDLNet_Read16(&buf[layer][pos[layer]]);
  pos[layer]+=2;
#ifdef PARANOID
  if (pos[layer]>REP_BUFFER_SIZE) fprintf(stderr, "REP: Read16 overflow");
#endif
  return r;
}

Uint32 CReplicator::R32(void)
{
  Uint32 r = SDLNet_Read32(&buf[layer][pos[layer]]);
  pos[layer]+=4;
#ifdef PARANOID
  if (pos[layer]>REP_BUFFER_SIZE) fprintf(stderr, "REP: Read32 overflow");
#endif
  return r;
}

char*  CReplicator::RStr(void)
{
  char* r = (char*)(&buf[layer][pos[layer]]);
  pos[layer]+=strlen(r)+1;
#ifdef PARANOID
  if (pos[layer]>REP_BUFFER_SIZE) fprintf(stderr, "REP: ReadString overflow");
#endif
  return r;
}

void   CReplicator::RMem(void* dest, int len)
{
  memcpy(dest, &buf[layer][pos[layer]], len);
  pos[layer]+=len;
#ifdef PARANOID
  if (pos[layer]>REP_BUFFER_SIZE) fprintf(stderr, "REP: ReadMem overflow");
#endif
}

CReplicator& CReplicator::operator>>(CReplicator& right)
{
    memcpy(&right.buf[right.layer][right.pos[right.layer]], &buf[layer][0], pos[layer]);
    right.pos[right.layer]+=pos[layer];
#ifdef PARANOID
    if (right.pos[right.layer]>REP_BUFFER_SIZE) fprintf(stderr, "REP: msg copy overflow");
#endif
    return *this;
}

CReplicator& CReplicator::operator>>(Uint8& val)
{
    val = R8();
    return *this;
}

CReplicator& CReplicator::operator>>(Uint16& val)
{
    val = R16();
    return *this;
}

CReplicator& CReplicator::operator>>(Uint32& val)
{
    val = R32();
    return *this;
}

CReplicator& CReplicator::operator>>(Sint8& val)
{
    val = (Sint8)R8();
    return *this;
}

CReplicator& CReplicator::operator>>(Sint16& val)
{
    val = (Sint16)R16();
    return *this;
}

CReplicator& CReplicator::operator>>(Sint32& val)
{
    val = (Sint32)R32();
    return *this;
}

CReplicator& CReplicator::operator>>(char*& str)
{
    str = RStr();
    return *this;
}

CReplicator& CReplicator::operator>>(PString& ps)
{
    ps.len = R8();
    RMem(ps.chars, ps.len);
    ps.chars[ps.len]=0; 
    return *this;
}

void CReplicator::nprintf(size_t count, const char *fmt, ... )
{
    va_list		argptr;
    va_start (argptr,fmt);
    int written=vsnprintf ((char*)&buf[layer][pos[layer]], count, fmt, argptr);
    va_end (argptr);
    pos[layer]+=(written+1);
    dirty = true;
}

char CReplicator::AssembleMsg(net_msg * msg, int max_len)
{
    for (int i=0; i<REP_MAX_LAYER; i++) 
    {
      
      Uint16 lastmark;
      if (wpos[i])
        lastmark = markers[i][wpos[i]-1];
      else
        lastmark = 0;

      while (wpos[i]<mpos[i])
      {
        Uint16 size = markers[i][wpos[i]]-lastmark;
        if (size<max_len-1) 
        {
          msg->WMem(&buf[i][lastmark], size);
          lastmark = markers[i][wpos[i]];
          max_len-=size;
          wpos[i]++;
        }
        else 
        {
          msg->W8(REP_END);
          fprintf(stderr, "multipacket replication\n");
          return 1;  // there are more data to send in replicator
        }
      }
    }
    msg->W8(REP_END);
    return 0; // all data were flushed
}

void CReplicator::Mark() 
{ 
   if (mpos[layer]<REP_MAX_MARKERS) 
   { 
     markers[layer][mpos[layer]] = pos[layer]; 
     mpos[layer]++; 
   } 
   else 
     ConErr("Out of markers"); 
}
