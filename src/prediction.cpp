/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   move vector & prediction
//## author:  Antonin Hildebrand
//##         
//## started: 14.6.2000
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
#include "SDL_Console.h"
#include "SDL_net.h"
#include "net.h"
#include "server.h"
#include "client.h"


  
CPredictor::CPredictor()
{
  Reset();
}

CPredictor::~CPredictor()
{
}

void CPredictor::Reset()
{
  for (Uint8 i=0; i<PREDICTION_BUF_LENGTH; i++)
    p[i].tick = 0;
  head=0;
}

void CPredictor::Add(MoveVector *mv)
{
  p[head] = *mv;
  FwHead();
}

Uint8 CPredictor::Find(TICK_TYPE ftick)
{
  Uint8 sh = head ;
  
  BwHead();
  while (head != sh) 
  {
    if (p[head].tick <= ftick) break;
    BwHead();
  }
  FwHead();

  Uint8 th = head;
  head = sh;
  return th;
}
