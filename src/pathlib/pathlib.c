/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## pathlib.c [PATHLIB]
//## 
//## desc.:   pathlib implementation
//## author:  Antonin Hildebrand [hildi@atlas.cz]
//## 
//## started: 12/05/1999
//## revised: 02/09/2000
//## 
//###########################################################################

#include <malloc.h>
#include <math.h>
#include <string.h>

#include "pathlib.h"

#ifdef PE_MONITOR  // pro monitorovani enginu
long peENodes = 0;
#endif

const char Header[20]="PathLib file\n\0";

peError peLastError;

const signed char DirType[8]={0,1,0,1,0,1,0,1};
const signed char Dir2Delta[8][2]={{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};
const signed char Delta2Dir[3][3]={{7,0,1},{6,-1,2},{5,4,3}};

/////////////////////////////////////////////////////////////////////////////
// Standard heuristics

int HeuristicsEuclidean(PED* PE, MapNode* A, MapNode* B)
{
  const int dx = (A->Pos.X - B->Pos.X);
  const int dy = (A->Pos.Y - B->Pos.Y);
  
  return (int)(PE->BaseCost*sqrt((double)(dx*dx + dy*dy))); // Pythagoras
}

#ifndef WIN32
#define max(a,b) ((a>b) ? a : b)
#endif

int HeuristicsMax(PED* PE, MapNode* A, MapNode* B)
{
  const int dx = abs(A->Pos.X - B->Pos.X);
  const int dy = abs(A->Pos.Y - B->Pos.Y);
  
  return PE->BaseCost*max(dx,dy); // Max
}

int HeuristicsManhattan(PED* PE, MapNode* A, MapNode* B)
{
  const int dx = abs(A->Pos.X - B->Pos.X);
  const int dy = abs(A->Pos.Y - B->Pos.Y);
  
  return PE->BaseCost*(dx+dy); // Manhattan
}

/////////////////////////////////////////////////////////////////////////////
// Mixing functions

int MixCustom(PED* PE, int g, int h, int z)
{
  // this is general mixing function, 
  // uses MixCoef for ratio between weight of heuristic and distance from start
  // all other mixing function could be simulated by this function
  return 2*((int)(PE->MixCoef*g + (1.0-PE->MixCoef)*h)) + z;
}

int MixDijkstra(PED* PE, int g, int h, int z)
{
  // same as MixCustom, with MixCoef=1.0, but faster
  return g + z; 
}

int MixBestFirst(PED* PE, int g, int h, int z)
{
  // same as MixCustom, with MixCoef=0.0, but faster
  return h + z; 
}

int MixAStar(PED* PE, int g, int h, int z)
{
  // same results as MixCustom, with MixCoef=0.5, but faster
  return h + g + z; 
}

/////////////////////////////////////////////////////////////////////////////
// Initialization

PED* peInit(int msx, int msy, int iMaxRequests, int iHeapSize, int iMPCPT, int iMaxWalkers )
{
  PED* PE;
  PE = (PED*)malloc(sizeof(PED));
  if (!PE) return NULL;
  
  PE->Time = 0;
  if (!peInitMap(PE, msx, msy))                   goto error;
  if (!peInitRequests(PE, iMaxRequests+1, iMPCPT+1))  goto error;
  if (!peInitPaths(PE, 5000))                     goto error;
  if (!peInitWalkers(PE, iMaxWalkers))            goto error;
  if (!(PE->Open = heapInit(iHeapSize)))          goto error;
  
  PE->HeapSize = iHeapSize; 
  PE->LastSID = 0;
  peSetHeuristics(PE, &HeuristicsMax);
  peSetMixFunction(PE, &MixAStar);
  peSetMixCoef(PE, 0.5);  // just for sure, Coef1 is not out of boundaries
  peSetSettings(PE, PC_MAXTC, PC_STANDING_BLOCKER_COST, PC_WALKING_BLOCKER_COST, PC_STEP_COST,
    PC_BASE_COST, PC_BASE_COST1, PC_BASE_COST2, PC_NEAR_DESTINATION, PC_BADFINALRATIO,
    PC_BADSHIFT, PC_FSBEHIND, PC_WAITING_TIMEOUT, PC_DEADLOCK_TIMEOUT);
  PE->State = 0;
  return PE;
  
error:
  if (PE) peDone(PE);
  return NULL;
}

char peInitRequests(PED* PE, int iMaxRequests, int iMPCPT)
{
  PE->MPCPT = iMPCPT;
  
  PE->RequestsBottom = (Request*)malloc(iMaxRequests*sizeof(Request));
  if (!PE->RequestsBottom) return 0;
  
  PE->RequestsTop = PE->RequestsBottom + iMaxRequests - 1;
  peResetRequests(PE);
  return 1;
}

char peInitMap(PED* PE, int msx, int msy)
{
  int i,j;
  
  PE->MapSX = msx;
  PE->MapSY = msy;
  
  PE->Map = (MapNode*)malloc(msx*msy*sizeof(MapNode));
  if (!PE->Map) { peLastError = PE_NOMEM; return 0; }
  
  
  for (i=0; i<msx; i++)
  {
    for (j=0; j<msy; j++)
    {
      MapNode* N = peMapXY(PE, i, j);
      
      N->Owner = -1;
      N->Pos.X = i;
      N->Pos.Y = j;
      N->TC = 0;
      N->SearchID = 0;
      N->f = 0;
      N->FootSteps = 0;
    }
  }    
  return 1;
}

char peInitPaths(PED* PE, int iMaxSteps)
{
  PE->MaxSteps = iMaxSteps;
  return 1;
}

char peInitWalkers(PED* PE, int iMaxWalkers)
{
  int i;
  Walker *w;
  
  PE->MaxWalkers = iMaxWalkers;
  PE->Walkers = (Walker*)malloc(iMaxWalkers*sizeof(Walker));
  if (!PE->Walkers) { peLastError = PE_NOMEM; return 0; };
  
  for (i=0; i<iMaxWalkers; i++)
  {
    w = &PE->Walkers[i];
    w->State = PEW_FREE;
    w->Path = NULL;
    w->Prev = NULL;
    w->Next = NULL;
  }
  
  PE->LastWalker = 0;
  PE->FirstWalker = 0;
  return 1;
}

/////////////////////////////////////////////////////////////////////////////
// Deintialization

char peDone(PED* PE)
{
  char res = 1;
  
  res &= peDoneRequests(PE);
  res &= peDoneWalkers(PE);
  res &= peDonePaths(PE);
  res &= peDoneMap(PE);
  res &= heapDone(PE->Open);
  
  free(PE);
  return res;
}

char peDoneRequests(PED* PE)
{
  if (PE->RequestsBottom)
  {
    free(PE->RequestsBottom);
    PE->RequestsBottom=0;
  }
  return 1;
}

char peDoneMap(PED* PE)
{
  if (PE->Map)
  {
    free(PE->Map);
    PE->Map=0;
  }
  return 1;
}

char peDonePaths(PED* PE)
{
  return 1;
}

char peDoneWalkers(PED* PE)
{
  int i; 
  Walker *w;
  
  for (i=0; i<PE->MaxWalkers; i++)
  {
    w = &PE->Walkers[i];
    if (w->Path)
      free(w->Path);
  }
  
  if (PE->Walkers)
  {
    free(PE->Walkers);
    PE->MaxWalkers = 0;
  }
  return 1;
}

/////////////////////////////////////////////////////////////////////////////
// Load

PED* peLoad(FILE* f)
{
  PED* PE;
  
  char NewHeader[20];
  int msx;
  int msy;
  int iMaxRequests;
  int iHeapSize;
  int iMPCPT;
  int iMaxWalkers;
  
  // load  header
  fread(NewHeader, sizeof(NewHeader), 1, f);
  if (strcmp(Header,NewHeader)!=0) 
  {
    peLastError = PE_CORRUPTED;
    return 0;
  }
  
  // load init data
  fread(&msx, sizeof(msx), 1, f);
  fread(&msy, sizeof(msy), 1, f);
  fread(&iMaxRequests, sizeof(int), 1, f);
  fread(&iHeapSize, sizeof(iHeapSize), 1, f);
  fread(&iMPCPT, sizeof(iMPCPT), 1, f);
  fread(&iMaxWalkers, sizeof(iMaxWalkers), 1, f);
  
  PE = peInit(msx, msy, iMaxRequests, iHeapSize, iMPCPT, iMaxWalkers);
  if (!PE) return 0;
  
  // load important data
  fread(&PE->Time, sizeof(PE->Time), 1, f);
  fread(&PE->State, sizeof(PE->State), 1, f);
  fread(&PE->MaxSteps, sizeof(PE->MaxSteps), 1, f);
  fread(&PE->LastSID, sizeof(PE->LastSID), 1, f);
  
  // load settings
  fread(&PE->MAXTC, sizeof(PE->MAXTC), 1, f);
  fread(&PE->StandingBlockerCost, sizeof(PE->StandingBlockerCost), 1, f); 
  fread(&PE->WalkingBlockerCost, sizeof(PE->WalkingBlockerCost), 1, f); 
  fread(&PE->FootStepCost, sizeof(PE->FootStepCost), 1, f); 
  fread(&PE->BaseCost, sizeof(PE->BaseCost), 1, f); 
  fread(&PE->HorizontalCost, sizeof(PE->HorizontalCost), 1, f); 
  fread(&PE->DiagonalCost, sizeof(PE->DiagonalCost), 1, f); 
  fread(&PE->NearDestination, sizeof(PE->NearDestination), 1, f);
  fread(&PE->BadFinalRatio, sizeof(PE->BadFinalRatio), 1, f); 
  fread(&PE->BadFinalShift, sizeof(PE->BadFinalShift), 1, f); 
  fread(&PE->FSBehind, sizeof(PE->FSBehind), 1, f); 
  fread(&PE->WaitingTimeout, sizeof(PE->WaitingTimeout), 1, f);
  fread(&PE->DeadlockTimeout, sizeof(PE->DeadlockTimeout), 1, f);
  
  peLoadWalkers(PE, f);
  peLoadRequests(PE, f);
  peLoadMap(PE, f);
  
  peLoadPaths(PE, f);
  
  if (ferror(f))
  {
    peLastError = PE_IOFAILED;
    peDone(PE);
    return 0;
  }
  else
    return PE;
}

char peLoadRequests(PED* PE, FILE* f)
{
  int i;
  
  fread(&i, sizeof(int), 1, f);
  PE->FirstRequest = PE->RequestsBottom + i;
  fread(&i, sizeof(int), 1, f);
  PE->LastRequest = PE->RequestsBottom + i;
  
  i = (PE->RequestsTop - PE->RequestsBottom);
  fread(PE->RequestsBottom, sizeof(Request), i, f);
  return 1;
}

char peLoadMap(PED* PE, FILE* f)
{
  fread(PE->Map, sizeof(MapNode), PE->MapSX * PE->MapSY, f);
  return 1;
}

char peLoadWalkers(PED* PE, FILE* f)
{
  int i;
  WalkerIO wio;
  Walker   *w;
  
  fread(&i, sizeof(int), 1, f);
  if (i == -1)
    PE->FirstWalker = 0;
  else
    PE->FirstWalker = &PE->Walkers[i];
  
  fread(&i, sizeof(int), 1, f);
  if (i == -1)
    PE->LastWalker = 0;
  else
    PE->LastWalker = &PE->Walkers[i];
  
  for (i=0; i<PE->MaxWalkers; i++)
  {
    w=&PE->Walkers[i];
    
    fread(&wio, sizeof(wio), 1, f);
    
    w->Dest = wio.Dest;
    w->Length = wio.Length;
    w->Pos = wio.Pos;
    w->Priority = wio.Priority;
    w->Speed = wio.Speed;
    w->State = wio.State;
    w->Step = wio.Step;
    if (wio.Next!=-1)
      w->Next = &PE->Walkers[wio.Next];
    else
      w->Next = NULL;
    
    if (wio.Prev!=-1)
      w->Prev = &PE->Walkers[wio.Prev];
    else
      w->Prev = NULL;
    
    if (w->Length>0) // load path
    {
      w->Path = (PathStep*)malloc(w->Length*sizeof(PathStep));
      if (!w->Path)
      {
        peLastError = PE_NOMEM;
        return 0;
      }
      fread(w->Path, sizeof(PathStep), w->Length, f);
    }
    else
      w->Path = 0; 
  }
  return 1;
}

char peLoadPaths(PED* PE, FILE* f)
{
  return 1;
}

/////////////////////////////////////////////////////////////////////////////
// Save

char peSave(PED* PE, FILE* f)
{
  int i;
  
  // save  header
  fwrite(Header, sizeof(Header), 1, f);
  
  // save init data
  fwrite(&PE->MapSX, sizeof(PE->MapSX), 1, f);
  fwrite(&PE->MapSY, sizeof(PE->MapSY), 1, f);
  i = (PE->RequestsTop - PE->RequestsBottom);
  fwrite(&i, sizeof(int), 1, f);
  fwrite(&PE->HeapSize, sizeof(PE->HeapSize), 1, f);
  fwrite(&PE->MPCPT, sizeof(PE->MPCPT), 1, f);
  fwrite(&PE->MaxWalkers, sizeof(PE->MaxWalkers), 1, f);
  
  // save important data
  fwrite(&PE->Time, sizeof(PE->Time), 1, f);
  fwrite(&PE->State, sizeof(PE->State), 1, f);
  fwrite(&PE->MaxSteps, sizeof(PE->MaxSteps), 1, f);
  fwrite(&PE->LastSID, sizeof(PE->LastSID), 1, f);
  
  // save settings
  fwrite(&PE->MAXTC, sizeof(PE->MAXTC), 1, f);
  fwrite(&PE->StandingBlockerCost, sizeof(PE->StandingBlockerCost), 1, f);
  fwrite(&PE->WalkingBlockerCost, sizeof(PE->WalkingBlockerCost), 1, f);
  fwrite(&PE->FootStepCost, sizeof(PE->FootStepCost), 1, f);
  fwrite(&PE->BaseCost, sizeof(PE->BaseCost), 1, f);
  fwrite(&PE->HorizontalCost, sizeof(PE->HorizontalCost), 1, f);
  fwrite(&PE->DiagonalCost, sizeof(PE->DiagonalCost), 1, f);
  fwrite(&PE->NearDestination, sizeof(PE->NearDestination), 1, f);
  fwrite(&PE->BadFinalRatio, sizeof(PE->BadFinalRatio), 1, f);
  fwrite(&PE->BadFinalShift, sizeof(PE->BadFinalShift), 1, f);
  fwrite(&PE->FSBehind, sizeof(PE->FSBehind), 1, f); 
  fwrite(&PE->WaitingTimeout, sizeof(PE->WaitingTimeout), 1, f);
  fwrite(&PE->DeadlockTimeout, sizeof(PE->DeadlockTimeout), 1, f);
  
  peSaveWalkers(PE, f);
  peSaveRequests(PE, f);
  peSaveMap(PE, f);
  
  peSavePaths(PE, f);
  
  if (ferror(f))
  {
    peLastError = PE_IOFAILED;
    return 0;
  }
  else
    return 1;
}

char peSaveRequests(PED* PE, FILE* f)
{
  int i;
  
  i = PE->FirstRequest - PE->RequestsBottom;
  fwrite(&i, sizeof(int), 1, f);
  i = PE->LastRequest - PE->RequestsBottom;
  fwrite(&i, sizeof(int), 1, f);
  
  i = (PE->RequestsTop - PE->RequestsBottom);
  fwrite(PE->RequestsBottom, sizeof(Request), i, f);
  return 1;
}

char peSaveMap(PED* PE, FILE* f)
{
  fwrite(PE->Map, sizeof(MapNode), PE->MapSX * PE->MapSY, f);
  return 1;
}

char peSaveWalkers(PED* PE, FILE* f)
{
  int i;
  WalkerIO wio;
  Walker   *w;
  
  if (PE->FirstWalker)
    i = PE->FirstWalker - PE->Walkers;
  else
    i = -1;
  fwrite(&i, sizeof(int), 1, f);
  
  if (PE->LastWalker)
    i = PE->LastWalker - PE->Walkers;
  else
    i = -1;
  fwrite(&i, sizeof(int), 1, f);
  
  for (i=0; i<PE->MaxWalkers; i++)
  {
    w=&PE->Walkers[i];
    
    if (!(w->State&PEW_USED))
    {   // normalize unused data pointers
      w->Next = 0;
      w->Prev = 0;
      w->Path = 0;
    }
    
    wio.Dest = w->Dest;
    if (w->Path)
      wio.Length = w->Length;
    else
      wio.Length = 0;
    
    wio.Pos = w->Pos;
    wio.Priority = w->Priority;
    wio.Speed =w->Speed;
    wio.State = w->State;
    wio.Step = w->Step;
    if (w->Next)
      wio.Next = w->Next - PE->Walkers;
    else
      wio.Next = -1;
    
    if (w->Prev)
      wio.Prev = w->Prev - PE->Walkers;
    else
      wio.Prev = -1;
    
    fwrite(&wio, sizeof(wio), 1, f);
    
    if (w->Path) // save path
      fwrite(w->Path, sizeof(PathStep), w->Length, f);
  }
  
  return 1;
}

char peSavePaths(PED* PE, FILE* f)
{
  return 1;
}

/////////////////////////////////////////////////////////////////////////////
// Map

MapNode* peMapXY(PED* PE, int X, int Y)
{
  while (X<0) X+=PE->MapSX;
  while (X>=PE->MapSX) X-=PE->MapSX;
  while (Y<0) Y+=PE->MapSY;
  while (Y>=PE->MapSY) Y-=PE->MapSY;
  return &(PE->Map[Y*PE->MapSX + X]);
}

/////////////////////////////////////////////////////////////////////////////
// Walkers

char peResizeWalkers(PED* PE, int NewMaxWalkers)
{
  int i;
  PE->Walkers = (Walker*)realloc(PE->Walkers, NewMaxWalkers*sizeof(Walker));
  if (!PE->Walkers) { peLastError = PE_NOMEM; return 0; };
  
  for (i=PE->MaxWalkers; i<NewMaxWalkers; i++)
    PE->Walkers[i].State = PEW_FREE;
  PE->MaxWalkers = NewMaxWalkers;
  return 1;
}

Walker* peAddWalker(PED* PE)
{
  int i;
  for (i=0; i<PE->MaxWalkers; i++)
  {
    if (PE->Walkers[i].State == PEW_FREE)
    {  
      PE->Walkers[i].State = PEW_USED|PEW_STANDING;
      
      // Link him to the list
      PE->Walkers[i].Next = 0;
      if (PE->LastWalker)
      {
        PE->Walkers[i].Prev = PE->LastWalker;
        PE->LastWalker->Next = &PE->Walkers[i];
      }
      else
      {
        PE->Walkers[i].Prev = 0;
        PE->FirstWalker = &PE->Walkers[i];
      }
      PE->LastWalker = &PE->Walkers[i];
      
      // set temporary position (upper left corner)
      PE->Walkers[i].Path = 0;
      PE->Walkers[i].Pos.X = 0;
      PE->Walkers[i].Pos.Y = 0;
      PE->Walkers[i].Length = 0;
      
      //peSetPosWalker(PE, &PE->Walkers[i], PE->Map);
      
      return &PE->Walkers[i];
    }
  }
  
  peLastError = PE_NOWALKER;
  return NULL;
}

char peRemoveWalker(PED* PE, Walker* w)
{
  peDiscardRequests(PE, w);
  peStopWalker(PE, w);
  peMapXY(PE, w->Pos.X, w->Pos.Y)->Owner = -1;
  w->State = PEW_FREE;
  // unlink him from the list
  if (w == PE->FirstWalker)
  {
    if (w == PE->LastWalker)  // only one walker remains => empty list 
    {
      PE->LastWalker = 0;
      PE->FirstWalker = 0;
    }
    else // removing first walker
    {
      PE->FirstWalker = w->Next;
      PE->FirstWalker->Prev = 0;
    }
  }
  else
  {
    if (w == PE->LastWalker)  // removing last walker
    {
      PE->LastWalker = w->Prev;
      PE->LastWalker->Next = 0;
    }
    else  // removing middle walker
    {
      w->Next->Prev = w->Prev;
      w->Prev->Next = w->Next;
    }
  }
  
  return 1;
}

char pePlanWalker(PED* PE, Walker* w, MapNode* dest, char type)
{
  // pokud je cilove pole zakazane, planovani skonci neuspechem
  if (dest->TC==PE->MAXTC) 
  {
    peLastError = PE_INVALIDDEST;
    return 0;
  }
  
  // stopni walkera
  peStopWalker(PE, w);
  
  // pozadavek na naplanovani cesty
  return peAddRequest(PE, w, dest, type);
}

char peSetPosWalker(PED* PE, Walker* w, MapNode* pos)
{
  MapNode *destnode;
  
  peStopWalker(PE, w);
  
  destnode = peMapXY(PE, pos->Pos.X, pos->Pos.Y);
  if (destnode && (destnode->Owner!=-1)) return 0; // on the dest node is already another walker
  
  // clear old position (tile) owner
  (peMapXY(PE, w->Pos.X, w->Pos.Y))->Owner = -1;
  
  w->Pos = pos->Pos;
  
  // set new position (tile) owner
  destnode->Owner = w - PE->Walkers;
  
  return 1;
}

char peStopWalker(PED* PE, Walker* w)
{
  if ((!(w->State&PEW_STANDING)) || (w->State&PEW_DEADLOCK))
  {
    if (!peRemovePath(PE, w->Path)) return 0;
    w->Path = NULL;
    w->State|=PEW_STANDING;
    w->State&=~PEW_BLOCKED;
    w->State&=~PEW_DEADLOCK;
  }
  return 1;
}

char peDeadlockWalker(PED* PE, Walker* w)
{
  if (!(w->State&PEW_STANDING))
  {
    Position SavedFirstStep = w->Path[w->Step+1].Pos;
    
    if (!peRemovePath(PE, w->Path)) return 0;
    w->Path = NULL;
    
    if (!peAddPath(PE, &w->Path, 2)) return 0; // just for saving time & first step
    w->Path[0].Time = PE->Time;     
    w->Path[0].Type = PST_CONTINUE;
    w->Path[0].Pos = w->Pos;
    w->Path[1].Type = PST_END;
    w->Path[1].Pos = SavedFirstStep;
    w->Step=0;
    
    w->State|=PEW_STANDING;
    w->State&=~PEW_BLOCKED;
    w->State|=PEW_DEADLOCK;
  }
  
  return 1;
}

char peSelectAllWalkers(PED* PE)
{
  Walker *w = PE->FirstWalker;
  
  while (w)
  {
    w->State|=PEW_SELECTED; // select walker
    w = w->Next;
  }
  return 1;
}

char peUnselectAllWalkers(PED* PE)
{
  Walker *w = PE->FirstWalker;
  while (w)
  {
    w->State&=~PEW_SELECTED; // unselect walker
    w = w->Next;
  }
  return 1;
}

/////////////////////////////////////////////////////////////////////////////
// Ways

char pePlanWay(PED* PE, Walker* w, MapNode* dest, char type)
{
  MapNode *Start;
  
  // stopni walkera
  peStopWalker(PE, w);
  
  Start=peMapXY(PE, w->Pos.X, w->Pos.Y);
  switch (type) {
  case PRT_STD:
    w->Length=peFindPath(PE, Start, dest, PE->Time, w->Speed, &w->Path);
    break;
  case PRT_NTC:
    w->Length=peFindPathNTC(PE, Start, dest, PE->Time, w->Speed, &w->Path);
    break;
  case PRT_WW:
    w->Length=peFindPathWW(PE, Start, dest, PE->Time, w->Speed, &w->Path);
    break;
  case PRT_BRS:
    w->Length=peBresPath(PE, Start, dest, PE->Time, w->Speed, &w->Path);
    break;
  }
  
  if (w->Length)
  {
    w->State&=~PEW_STANDING;
    w->Dest = dest->Pos;
    w->Step = 0;
    return 1;  
  }
  else // path not found 
    return 0;  
}

/////////////////////////////////////////////////////////////////////////////
// Paths

char peAddPath(PED* PE, PathStep** Path, int Length)
{
  (*Path) = (PathStep*)malloc(Length*sizeof(PathStep));
  if (!*Path)
  {
    peLastError = PE_NOMEM;
    return 0;
  }
  
  return 1;
}

char peClearFootSteps(PED* PE, PathStep* Path)
{
  PathStep *ps = Path-1;
  MapNode* node;
  
  do 
  {
    ps++;
    node = peMapXY(PE, ps->Pos.X, ps->Pos.Y);
    if (node && (ps->Type&PST_FOOTSTEP)) node->FootSteps-=PE->FootStepCost;
  }
  while (!(ps->Type&PST_END));
  
  return 1;
}

char peRemovePath(PED* PE, PathStep* Path)
{
  if (Path) 
  {
    peClearFootSteps(PE, Path);
    free(Path);
    Path=0;
  }
  return 1;
}

/////////////////////////////////////////////////////////////////////////////
// Requests

char peAddRequest(PED * PE, Walker * w, MapNode * dest, char type)
{
  Request* r;
  
  peDiscardRequests(PE , w); // keep only one request per Walker
  if (PE->LastRequest!=PE->RequestsTop) r = PE->LastRequest+1; else r = PE->RequestsBottom;
  
  if (r == PE->FirstRequest) // there is no room for new request 
  {    
    peLastError = PE_NOREQ;
    return 0;
  }
  
  PE->LastRequest->Dest = dest->Pos;
  PE->LastRequest->Owner = w - PE->Walkers;
  PE->LastRequest->Type = type;
  
  PE->LastRequest = r;
  return 1;
}

char peRemoveRequest(PED * PE, Request * r)
// removes request r from engine PE
{
  Request* n;
  if (r!=PE->RequestsTop) n = r+1; else n = PE->RequestsBottom;
  
  while (r != PE->LastRequest)
  {
    *r = *n;
    if (r!=PE->RequestsTop) r += 1; else r = PE->RequestsBottom;
    if (n!=PE->RequestsTop) n += 1; else n = PE->RequestsBottom;
  }
  
  if (PE->LastRequest!=PE->RequestsBottom) 
    PE->LastRequest = PE->LastRequest-1; 
  else 
    PE->LastRequest = PE->RequestsTop;
  
  return 1;
}

char peDiscardRequests(PED * PE, Walker * w)
// discards all requests from walker w
{
  Request* n = PE->FirstRequest;
  
  while (n!=PE->LastRequest)
  {
    if (n->Owner == (w - PE->Walkers)) 
    {            
      peRemoveRequest(PE, n);
      peStopWalker(PE, w);  // for case he had temporary path
      n = PE->FirstRequest; // restart searching
      continue;
    }
    
    if (n!=PE->RequestsTop) n += 1; else n = PE->RequestsBottom;
  }
  return 1;
}

char peManageRequests(PED* PE)
// zpracuje prvnich MPCPT requestu 
// pokud je request NTC -> provede NTC hledani
// zbylym naplanuje provizorni cestu, pokud ji nemaji
{
  int Max = PE->MPCPT;
  MapNode *Start, *End, *TempNode;
  Walker *w;
  Request *r;
  
  r = PE->FirstRequest;
  while ((Max--) && (r!=PE->LastRequest))
  {
    w = &PE->Walkers[r->Owner];
    Start=peMapXY(PE, w->Pos.X, w->Pos.Y);
    End=peMapXY(PE, r->Dest.X, r->Dest.Y);
    
    if (!(w->State&PEW_STANDING)) // walker has temporary path
      peStopWalker(PE, w);
    
    switch (r->Type) {
    case PRT_STD: 
      w->Length=peFindPath(PE, Start, End, PE->Time, w->Speed, &w->Path);
      break;
    case PRT_NTC: 
      w->Length=peFindPathNTC(PE, Start, End, PE->Time, w->Speed, &w->Path);
      break;
    case PRT_WW: 
      w->Length=peFindPathWW(PE, Start, End, PE->Time, w->Speed, &w->Path);
      break;
    case PRT_BRS: 
      w->Length=peBresPath(PE, Start, End, PE->Time, w->Speed, &w->Path);
      break;
    }
    
    
    if (w->Length)
    {
      // check if we have deadlock here
      TempNode = peMapXY(PE, w->Path[1].Pos.X, w->Path[1].Pos.Y);
      w->State&=~PEW_STANDING;
      w->Dest = r->Dest;
      w->Step = 0;
      if (TempNode->Owner!=-1)
        peDeadlockWalker(PE, w);  // deadlock
    }
    else // path not found => do bres path
    {
    /*   if (w->Length=peBresPath(PE, Start, End, PE->Time, w->Speed, &w->Path))
    {
    w->State&=~PEW_STANDING;
    w->Dest = r->Dest;
    w->Step = 0;
    }
      else // shit */
      w->State|=PEW_STANDING;
    }
    if (r==PE->RequestsTop) r=PE->RequestsBottom; else r++; 
  }
  
  PE->FirstRequest=r; // posun pointer, za ty, co jsme zpracovali
  
  // provizorni bres cesty
  while (r!=PE->LastRequest)
  {
    w = &PE->Walkers[r->Owner];
    if (w->State&PEW_STANDING) 
    {
      Start=peMapXY(PE, w->Pos.X, w->Pos.Y);
      End=peMapXY(PE, r->Dest.X, r->Dest.Y);
      
      w->Length=peBresPath(PE, Start, End, PE->Time, w->Speed, &w->Path);
      if (w->Length)
      {
        w->State&=~PEW_STANDING;
        w->Dest = r->Dest;
        w->Step = 0;
      }
      else // path not found
      {
        w->State|=PEW_STANDING;
      }
    }
    if (r==PE->RequestsTop) r=PE->RequestsBottom; else r++; 
  }
  
  return 1;
  
}

char peResetRequests(PED* PE)
{
  PE->FirstRequest=PE->RequestsBottom;
  PE->LastRequest=PE->RequestsBottom;
  return 1;
}

/////////////////////////////////////////////////////////////////////////////
// Settings

char peSetHeuristics(PED* PE, peHeuristics* H)
{
  PE->Heuristics = H;
  return 1;
}

char peSetMixFunction(PED* PE, peMixFunction* M)
{
  PE->MixFunction = M;
  return 1;
}

char peSetMixCoef(PED* PE, float C)
{
  PE->MixCoef = C;
  return 1;
}

char peSetSettings(PED* PE, int MAXTC, int StandingBlockerCost, int WalkingBlockerCost, int FootStepCost, 
                   int BaseCost, int HorizontalCost, int DiagonalCost, int NearDestination,
                   int BadFinalRatio, int BadFinalShift, int FSBehind, peTime WaitingTimeout, peTime DeadlockTimeout)
{
  PE->MAXTC = MAXTC;
  PE->StandingBlockerCost = StandingBlockerCost; 
  PE->WalkingBlockerCost = WalkingBlockerCost; 
  PE->FootStepCost = FootStepCost; 
  PE->BaseCost = BaseCost; 
  PE->HorizontalCost = HorizontalCost; 
  PE->DiagonalCost = DiagonalCost; 
  PE->NearDestination = NearDestination;
  PE->BadFinalRatio = BadFinalRatio; 
  PE->BadFinalShift = BadFinalShift; 
  PE->FSBehind = FSBehind; 
  PE->WaitingTimeout = WaitingTimeout;
  PE->DeadlockTimeout = DeadlockTimeout;
  return 1;
}

/////////////////////////////////////////////////////////////////////////////
// Time control

char peTimeForward(PED* PE, int ticks)
{
  char res = 1;
  while ((ticks--) && res) res&=peTimeTick(PE);
  return res;
}

char peTimeTick(PED* PE)
{
  Walker *w, *x;
  Walker *owner;
  PathStep *ps;
  //    PathStep *minps;
  MapNode *OldNode;
  MapNode *NewNode;
  MapNode *TempNode;
  MapNode *MNode1;
  MapNode *MNode2;
  int Destination;
  //    int td;
  int i;
  
  peManageRequests(PE);
  
  PE->Time++;
  w = PE->FirstWalker;
  while (w) // for each walker 
  {
    if (w->State&PEW_STANDING) // standing walkers
    {
      // check to resolve deadlocks        
      if (w->State&PEW_DEADLOCK)
      {
        if (PE->Time - w->Path[0].Time > PE->DeadlockTimeout)
        {
          // try to replan way one more time
          if (pePlanWay(PE, w, peMapXY(PE, w->Dest.X, w->Dest.Y), PRT_STD)) // re-find path
          {  
            // check if we have deadlock here
            TempNode = peMapXY(PE, w->Path[1].Pos.X, w->Path[1].Pos.Y);
            if (TempNode->Owner!=-1)
              peStopWalker(PE, w);  // final deadlock - could not be solved
            else
              w->State&=~PEW_DEADLOCK;  // deadlock has disappeared, keep walking
          }
        }
        else // is deadlock -> check first step node if is free
        {
          TempNode = peMapXY(PE, w->Path[1].Pos.X, w->Path[1].Pos.Y);
          if (TempNode->Owner==-1)  
          {
            // deadlock has disappeared, replan will should be sooner
            w->Path[0].Time -= PE->DeadlockTimeout << 4;
          }  
        }
      }
    }
    else //  walking walkers
    {
      if (w->Path[w->Step+1].Time <= PE->Time) // is the time for new step ?
      {
        // colision test
        OldNode = peMapXY(PE,w->Path[w->Step].Pos.X, w->Path[w->Step].Pos.Y);
        NewNode = peMapXY(PE,w->Path[w->Step+1].Pos.X, w->Path[w->Step+1].Pos.Y);
        
        if (NewNode->TC==PE->MAXTC) // collision with hard obstacle
        { // stena neuhne
          peStopWalker(PE, w);
          w = w->Next; // get next walker
          continue;
        }
        
        if (w->State & PEW_BLOCKED) // check if we are still blocked <- nonfrontal collision
        {
          if (NewNode->Owner!=-1)  // yep, we are still blocked
          {
            MNode1 = peMapXY(PE, w->Dest.X, w->Dest.Y);
            MNode2 = peMapXY(PE, w->Pos.X, w->Pos.Y);
            Destination = HeuristicsManhattan(PE, MNode2, MNode1);
            if (Destination<=PE->NearDestination) // are we too close to the goal ?
            {
              if (pePlanWay(PE, w, peMapXY(PE, w->Dest.X, w->Dest.Y), PRT_NTC)) // re-find path
              {  
                // check if we have deadlock here
                TempNode = peMapXY(PE, w->Path[1].Pos.X, w->Path[1].Pos.Y);
                if (TempNode->Owner!=-1)
                  peStopWalker(PE, w);  // deadlock, but we are near destination - so stopwalker
              }
              // ? get worse position in exchange long way to get to better position
              if (w->Length*PE->BaseCost > (PE->BadFinalRatio*Destination + PE->BadFinalShift))
              {
                peStopWalker(PE, w);
                w = w->Next; // get next walker
                continue;
              }
            }
            else // are we waiting too long ?
              if (PE->Time - w->Path[w->Step+1].Time > PE->WaitingTimeout)
              {
                if (pePlanWay(PE, w, peMapXY(PE, w->Dest.X, w->Dest.Y), PRT_STD)) // re-find path
                {  
                  // check if we have deadlock here
                  TempNode = peMapXY(PE, w->Path[1].Pos.X, w->Path[1].Pos.Y);
                  if (TempNode->Owner!=-1)
                    peDeadlockWalker(PE, w);  // deadlock
                }
              }
              w = w->Next; // get next walker
              continue;
          }
          else  // the way have been unblocked
            w->State &= ~PEW_BLOCKED; 
        }
        
        if (NewNode->Owner!=-1)  // we have new collision here
        {
          // advanced test of collision type
          // 1) me & standing walker (hard collision) => i'll refind path
          //   a) i'm near to my destination => find new friendly destinations
          //   b) i'm far away from my destination => use new path
          // 2) frontal crash => prior walker refinds path
          // 3) nonfrontal crash => i'll wait for clearing
          
          if (PE->Walkers[NewNode->Owner].State&PEW_STANDING)
          {
            MNode1 = peMapXY(PE, w->Dest.X, w->Dest.Y);
            MNode2 = peMapXY(PE, w->Pos.X, w->Pos.Y);
            
            Destination = HeuristicsManhattan(PE, MNode2, MNode1);
            if (Destination>PE->NearDestination) // we are far from destination, go without finding better destination
            {
              pePlanWalker(PE, w, MNode1, PRT_STD); // re-find path
              w = w->Next; // get next walker
              continue;
            }
            
            // we are near destination
            w->State|=PEW_BLOCKED;
          }
          else // collision with walking walker
          {
            owner = &PE->Walkers[NewNode->Owner];
            ps = &(owner->Path[owner->Step]);
            if (!(ps->Type&PST_END))
            {  
              ps++;
              if ((ps->Pos.X==w->Pos.X) && (ps->Pos.Y==w->Pos.Y))  // frontal collision ?
              {
                // who has higher priority -> x
                if (w->Priority < owner->Priority) x=owner; else x=w;
                if (pePlanWay(PE, x, peMapXY(PE, x->Dest.X, x->Dest.Y), PRT_STD)) // re-find path
                {  
                  // check if we have deadlock here
                  TempNode = peMapXY(PE, w->Path[1].Pos.X, w->Path[1].Pos.Y);
                  if (TempNode->Owner!=-1)
                    peDeadlockWalker(PE, w);  // deadlock
                }
              }
              else
              {
                w->State|=PEW_BLOCKED;
              }
            }
            else // i'm standing on his last tile
            {
              peStopWalker(PE, owner);
            }
          }
        }
        else // do step
        {
          
          // move
          w->Step++;
          OldNode->Owner = -1;
          w->Pos = w->Path[w->Step].Pos;
          NewNode->Owner = w - PE->Walkers;
          
          // clear footstep
          i = w->Step - PE->FSBehind;  // clear footstep before PE->FSBehind steps
          if ((i>=0) && (w->Path[i].Type&PST_FOOTSTEP))  // range check & footstep was dropped
          {
            TempNode = peMapXY(PE, w->Path[i].Pos.X, w->Path[i].Pos.Y);
            TempNode->FootSteps-=PE->FootStepCost;
            w->Path[i].Type&=~PST_FOOTSTEP; // clear footstep-set flag
          }
          
          if (w->Path[w->Step].Type&PST_END) {
            if ((w->Path[w->Step].Pos.X==w->Dest.X) &&
              (w->Path[w->Step].Pos.Y==w->Dest.Y))  // walker reached dest position
              peStopWalker(PE, w);
            else
            {
              TempNode = peMapXY(PE, w->Dest.X, w->Dest.Y);
              pePlanWalker(PE, w, TempNode, PRT_STD);
            }
          }
        }
       }
    }
    w = w->Next; // get next walker
  }
  
  return 1;
}


/////////////////////////////////////////////////////////////////////////////
// Path finders

int peFindPath(PED* PE, MapNode* Start, MapNode* End, peTime StartTime, int Speed, PathStep** path)
{
  MapNode* v = Start;
  MapNode* s;
  MapNode* best = Start;
  int i;
  int BaseCost;
  int c, z;
  
  if (Start == End) return 0;
  
  PE->LastSID++;
  heapClear(PE->Open);
  
  v->g = 0;
  v->f = 0;
  v->h = (*PE->Heuristics)(PE, v, End);
  v->Steps = 0;
  v->Time = 0;
  heapAdd(PE->Open, v);
  
  while ((v != End) && (PE->Open->ActSize!=0))
  {
#ifdef PE_MONITOR  // pro monitorovani enginu
    peENodes++;
#endif
    
    v = PE->Open->HeapBuffer[1];
    heapRemove(PE->Open, v);
    
    // best position is the closest to the dest
    // and the most distanced from the start
    if ((v->h<best->h) || (v->h=best->h) && (v->g>best->g)) best = v; 
    
    for (i=0; i<8; i++)
    {
      s = peMapXY(PE, v->Pos.X+Dir2Delta[i][0], v->Pos.Y+Dir2Delta[i][1]); // s = sousedni policko
      if ((!s) || (s->TC==PE->MAXTC)) continue;
      if (s->SearchID == PE->LastSID)
      {
        if (s->HeapPos>0)
        {
          if (DirType[i]==0)
            BaseCost = PE->HorizontalCost;
          else
            BaseCost = PE->DiagonalCost;
          
          c = v->g + BaseCost + v->TC;
          if (s->Owner!=-1) {
            if (PE->Walkers[s->Owner].State&PEW_STANDING)
              z = PE->StandingBlockerCost + v->FootSteps;
            else
              z = PE->WalkingBlockerCost + v->FootSteps;
          }
          else
            z = v->FootSteps;
          
          if (c + z < s->g)
          {
            s->h = (*PE->Heuristics)(PE, s, End);
            s->Steps = v->Steps + 1;
            s->Time = v->Time + Speed;
            s->f = (*PE->MixFunction)(PE, c, s->h, z);
            s->g = c + z;
            s->CameFrom = v->Pos;
            heapSiftUp(PE->Open, s->HeapPos);
          }
        }
        // else konecny
      } 
      else
      {
        s->SearchID = PE->LastSID;
        if (DirType[i]==0)
          BaseCost = PE->HorizontalCost;
        else
          BaseCost = PE->DiagonalCost;
        
        c = v->g + BaseCost + v->TC;
        if (s->Owner!=-1) {
          if (PE->Walkers[s->Owner].State&PEW_STANDING)
            z = PE->StandingBlockerCost + v->FootSteps;
          else
            z = PE->WalkingBlockerCost + v->FootSteps;
        }
        else
          z = v->FootSteps;
        
        s->h = (*PE->Heuristics)(PE, s, End);
        s->Steps = v->Steps + 1;
        s->Time = v->Time + Speed;
        s->f = (*PE->MixFunction)(PE, c, s->h, z);
        s->g = c + z;
        s->CameFrom = v->Pos;
        if (heapAdd(PE->Open, s)==HP_NOSPACE)
        {
          // we ran out off free heap space
          // get the best known position and continue
          // after walker reaches this position
          End = best;
          v = best;
          break;
        }
      }
    }
  }
  
  if (Start == End) return 0;
  if (v!=End) return 0;
  if (!peAddPath(PE, path, End->Steps+1)) return 0;
  
  i=v->Steps+1;
  while (i>0)
  {
    i--;
    (*path)[i].Time = StartTime + v->Time;
    (*path)[i].Type = PST_CONTINUE | PST_FOOTSTEP;
    (*path)[i].Pos = v->Pos;
    v->FootSteps+=PE->FootStepCost;
    
    v=peMapXY(PE, v->CameFrom.X, v->CameFrom.Y);
  }
  
  (*path)[End->Steps].Type = PST_END | PST_FOOTSTEP;
  
  return End->Steps+1;
}

int peFindPathNTC(PED* PE, MapNode* Start, MapNode* End, peTime StartTime, int Speed, PathStep** path)
{
  MapNode* v = Start;
  MapNode* s;
  MapNode* best = Start;
  int i;
  int BaseCost;
  int c,z;
  
  if (Start == End) return 0;
  
  PE->LastSID++;
  heapClear(PE->Open);
  
  v->g = 0;
  v->f = 0;
  v->h = (*PE->Heuristics)(PE, v, End);
  v->Steps = 0;
  v->Time = 0;
  heapAdd(PE->Open, v);
  
  while ((v != End) && (PE->Open->ActSize!=0))
  {
#ifdef PE_MONITOR  // pro monitorovani enginu
    peENodes++;
#endif
    
    v = PE->Open->HeapBuffer[1];
    heapRemove(PE->Open, v);
    
    // best position is the closest to the dest
    // and the most distanced from the start
    if ((v->h<best->h) || (v->h=best->h) && (v->g>best->g)) best = v; 
    
    for (i=0; i<8; i++)
    {
      s = peMapXY(PE, v->Pos.X+Dir2Delta[i][0], v->Pos.Y+Dir2Delta[i][1]); // s = sousedni policko
      if ((!s) || (s->TC==PE->MAXTC)) continue;
      if (s->SearchID == PE->LastSID)
      {
        if (s->HeapPos>0)
        {
          if (DirType[i]==0)
            BaseCost = PE->HorizontalCost;
          else
            BaseCost = PE->DiagonalCost;
          
          c = v->g + BaseCost;
          if (s->Owner!=-1)
            z = PE->WalkingBlockerCost + v->FootSteps;
          else
            z = v->FootSteps;
          
          if (c + z < s->g)
          {
            s->h = (*PE->Heuristics)(PE, s, End);
            s->Steps = v->Steps + 1;
            s->Time = v->Time + Speed;
            s->f = (*PE->MixFunction)(PE, c, s->h, z);
            s->g = c + z;
            s->CameFrom = v->Pos;
            heapSiftUp(PE->Open, s->HeapPos);
          }
        }
        // else konecny
      } 
      else
      {
        s->SearchID = PE->LastSID;
        if (DirType[i]==0)
          BaseCost = PE->HorizontalCost;
        else
          BaseCost = PE->DiagonalCost;
        
        c = v->g + BaseCost;
        if (s->Owner!=-1)
          z = PE->WalkingBlockerCost + v->FootSteps;
        else
          z = v->FootSteps;
        
        s->h = (*PE->Heuristics)(PE, s, End);
        s->Steps = v->Steps + 1;
        s->Time = v->Time + Speed;
        s->f = (*PE->MixFunction)(PE, c, s->h, z);
        s->g = c + z;
        s->CameFrom = v->Pos;
        if (heapAdd(PE->Open, s)==HP_NOSPACE)
        {
          // we ran out off free heap space
          // get the best known position and continue
          // after walker reaches this position
          End = best;
          v = best;
          break;
        }
      }
    }
  }
  
  if (Start == End) return 0;
  if (v!=End) return 0;
  if (!peAddPath(PE, path, End->Steps+1)) return 0;
  
  i=v->Steps+1;
  while (i>0)
  {
    i--;
    (*path)[i].Time = StartTime + v->Time;
    (*path)[i].Type = PST_CONTINUE | PST_FOOTSTEP;
    (*path)[i].Pos = v->Pos;
    v->FootSteps+=PE->FootStepCost;
    
    v=peMapXY(PE, v->CameFrom.X, v->CameFrom.Y);
  }
  
  (*path)[End->Steps].Type = PST_END | PST_FOOTSTEP;
  
  return End->Steps+1;
}

int peFindPathWW(PED* PE, MapNode* Start, MapNode* End, peTime StartTime, int Speed, PathStep** path)
{
  MapNode* v = Start;
  MapNode* s;
  MapNode* best = Start;
  int i;
  int BaseCost;
  int c,z;
  
  if (Start == End) return 0;
  
  PE->LastSID++;
  heapClear(PE->Open);
  
  v->g = 0;
  v->f = 0;
  v->h = (*PE->Heuristics)(PE, v, End);
  v->Steps = 0;
  v->Time = 0;
  heapAdd(PE->Open, v);
  
  while ((v != End) && (PE->Open->ActSize!=0))
  {
#ifdef PE_MONITOR  // pro monitorovani enginu
    peENodes++;
#endif
    
    v = PE->Open->HeapBuffer[1];
    heapRemove(PE->Open, v);
    
    // best position is the closest to the dest
    // and the most distanced from the start
    if ((v->h<best->h) || (v->h=best->h) && (v->g>best->g)) best = v; 
    
    for (i=0; i<8; i++)
    {
      s = peMapXY(PE, v->Pos.X+Dir2Delta[i][0], v->Pos.Y+Dir2Delta[i][1]); // s = sousedni policko
      if ((!s) || (s->TC==PE->MAXTC) || (s->Owner!=-1)) continue;
      if (s->SearchID == PE->LastSID)
      {
        if (s->HeapPos>0)
        {
          if (DirType[i]==0)
            BaseCost = PE->HorizontalCost;
          else
            BaseCost = PE->DiagonalCost;
          c = v->g + BaseCost;
          z = v->FootSteps;
          
          if (c + z < s->g)
          {
            s->h = (*PE->Heuristics)(PE, s, End);
            s->Steps = v->Steps + 1;
            s->Time = v->Time + Speed;
            s->f = (*PE->MixFunction)(PE, c, s->h, z);
            s->g = c + z;
            s->CameFrom = v->Pos;
            heapSiftUp(PE->Open, s->HeapPos);
          }
        }
        // else konecny
      } 
      else
      {
        s->SearchID = PE->LastSID;
        if (DirType[i]==0)
          BaseCost = PE->HorizontalCost;
        else
          BaseCost = PE->DiagonalCost;
        
        c = v->g + BaseCost;
        z = v->FootSteps;
        
        s->h = (*PE->Heuristics)(PE, s, End);
        s->Steps = v->Steps + 1;
        s->Time = v->Time + Speed;
        s->f = (*PE->MixFunction)(PE, c, s->h, z);
        s->g = c + z;
        s->CameFrom = v->Pos;
        if (heapAdd(PE->Open, s)==HP_NOSPACE)
        {
          // we ran out off free heap space
          // get the best known position and continue
          // after walker reaches this position
          End = best;
          v = best;
          break;
        }
      }
    }
  }
  if (Start == End) return 0;
  if (v!=End) return 0;
  if (!peAddPath(PE, path, End->Steps+1)) return 0;
  
  i=v->Steps+1;
  while (i>0)
  {
    i--;
    (*path)[i].Time = StartTime + v->Time;
    (*path)[i].Type = PST_CONTINUE | PST_FOOTSTEP;
    (*path)[i].Pos = v->Pos;
    v->FootSteps+=PE->FootStepCost;
    
    v=peMapXY(PE, v->CameFrom.X, v->CameFrom.Y);
  }
  
  (*path)[End->Steps].Type = PST_END | PST_FOOTSTEP;
  
  return End->Steps+1;
}


int peBresPath(PED* PE, MapNode* Start, MapNode* End, peTime StartTime, int Speed, PathStep** path)
// based on Kenny Hoff Bresenham line algorithm implementation
{
  int Ax = Start->Pos.X, Ay = Start->Pos.Y;
  int Bx = End->Pos.X, By = End->Pos.Y;
  PathStep *ps;
  MapNode *node;
  
  int dPr;
  int dPru;
  int P;
  
  //------------------------------------------------------------------------
  // INITIALIZE THE COMPONENTS OF THE ALGORITHM THAT ARE NOT AFFECTED BY THE
  // SLOPE OR DIRECTION OF THE LINE
  //------------------------------------------------------------------------
  int dX = abs(Bx-Ax);	// store the change in X and Y of the line endpoints
  int dY = abs(By-Ay);
  
  int CurrentX = Ax;		// store the starting point (just point A)
  int CurrentY = Ay;
  
  //------------------------------------------------------------------------
  // DETERMINE "DIRECTIONS" TO INCREMENT X AND Y (REGARDLESS OF DECISION)
  //------------------------------------------------------------------------
  int Xincr, Yincr;
  
  if (Start == End) return 0;
  
  node = peMapXY(PE, CurrentX, CurrentY);
  if (node->TC==PE->MAXTC)      // standing in wall
    return 0;
  
  if (Ax > Bx) { Xincr=-1; } else { Xincr=1; }	// which direction in X?
  if (Ay > By) { Yincr=-1; } else { Yincr=1; }	// which direction in Y?
  
  //------------------------------------------------------------------------
  // DETERMINE INDEPENDENT VARIABLE (ONE THAT ALWAYS INCREMENTS BY 1 (OR -1) )
  // AND INITIATE APPROPRIATE LINE DRAWING ROUTINE (BASED ON FIRST OCTANT
  // ALWAYS). THE X AND Y'S MAY BE FLIPPED IF Y IS THE INDEPENDENT VARIABLE.
  //------------------------------------------------------------------------
  if (dX >= dY)	// if X is the independent variable
  {           
    if (!peAddPath(PE, path, dX+1)) return 0;
    ps = *path;
    
    dPr 	= dY<<1;   						// amount to increment decision if right is chosen (always)
    dPru 	= dPr - (dX<<1);				// amount to increment decision if up is chosen
    P 		= dPr - dX;						// decision variable start value
    
    for (; dX>=0; dX--)							// process each point in the line one at a time (just use dX)
    {
      ps->Time = StartTime;           // plot the footstep
      ps->Type = PST_CONTINUE/*&~PST_FOOTSTEP*/;
      ps->Pos.X = CurrentX;
      ps->Pos.Y = CurrentY;
      node = peMapXY(PE, CurrentX, CurrentY);
      if (node->TC==PE->MAXTC)      // collision with wall
        break;
      //      node->FootSteps+=PE->FootStepCost;
      ps++;
      StartTime+=Speed;
      
#ifdef PE_MONITOR  // pro monitorovani enginu
      peENodes++;
#endif
      
      if (P > 0)                              // is the pixel going right AND up?
      { 
        CurrentX+=Xincr;					// increment independent variable
        CurrentY+=Yincr; 					// increment dependent variable
        P+=dPru;							// increment decision (for up)
      }
      else									// is the pixel just going right?
      {
        CurrentX+=Xincr;					// increment independent variable
        P+=dPr;								// increment decision (for right)
      }
    }
    ps--;
    ps->Type=PST_END/*&~PST_FOOTSTEP*/;
    return abs(Bx-Ax);
  }
  else			// if Y is the independent variable
  {
    if (!peAddPath(PE, path, dY+1)) return 0;
    ps = *path;
    
    dPr 	= dX<<1;   						// amount to increment decision if right is chosen (always)
    dPru 	= dPr - (dY<<1);    				// amount to increment decision if up is chosen
    P 		= dPr - dY;						// decision variable start value
    
    for (; dY>=0; dY--)							// process each point in the line one at a time (just use dY)
    {
      ps->Time = StartTime;           // plot the footstep
      ps->Type = PST_CONTINUE/*&~PST_FOOTSTEP*/;
      ps->Pos.X = CurrentX;
      ps->Pos.Y = CurrentY;
      node = peMapXY(PE, CurrentX, CurrentY);
      if (node->TC==PE->MAXTC)      // collision with wall
        break;
      //      node->FootSteps+=PE->FootStepCost;
      ps++;
      StartTime+=Speed;
      
#ifdef PE_MONITOR  // pro monitorovani enginu
      peENodes++;
#endif
      
      if (P > 0)                              // is the pixel going up AND right?
      { 
        CurrentX+=Xincr; 					// increment dependent variable
        CurrentY+=Yincr;					// increment independent variable
        P+=dPru;							// increment decision (for up)
      }
      else									// is the pixel just going up?
      {
        CurrentY+=Yincr;					// increment independent variable
        P+=dPr;								// increment decision (for right)
      }
    }		
    ps--;
    ps->Type=PST_END/*&~PST_FOOTSTEP*/;
    return abs(By-Ay);
  }		
}
