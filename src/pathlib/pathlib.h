/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## pathlib.h [PATHLIB]
//## 
//## desc.:   pathlib main header
//## author:  Antonin Hildebrand [hildi@atlas.cz]
//## 
//## started: 12/05/1999
//## revised: 02/09/2000
//## 
//###########################################################################
#ifndef _PATHLIB_H_
#define _PATHLIB_H_

#include "types.h"
#include "heap.h"

// PE error codes
#define PE_NOERR                0
#define PE_NOMEM                1
#define PE_NOWALKER             2
#define PE_NOREQ                3
#define PE_INVALIDDEST          4
#define PE_BADCOORD             5
#define PE_IOFAILED             6
#define PE_CORRUPTED            7

// Walker state flags
#define PEW_FREE                0x0
#define PEW_USED                0x1
#define PEW_SELECTED            0x2
#define PEW_STANDING            0x4
#define PEW_BLOCKED             0x8
#define PEW_DEADLOCK            0x10

// Pathstep type
#define PST_CONTINUE            0x0
#define PST_END                 0x1
#define PST_FOOTSTEP            0x2             // internal

// Request types
#define PRT_STD                 0
#define PRT_NTC                 1
#define PRT_WW                  2
#define PRT_BRS                 3

// Default constants
#define PC_MAXTC                10000           // maximal terrain cost
#define PC_STANDING_BLOCKER_COST 100            // additional cost of blocked node by walker
#define PC_WALKING_BLOCKER_COST 60              // additional cost of temoprary blocked node
#define PC_STEP_COST            2               // additional cost of node with step
#define PC_BASE_COST            24              // cost of clear node (for heuristics)
#define PC_BASE_COST1           20              // cost of horizontal/vertical clear move
#define PC_BASE_COST2           28              // cost of diagonal clear move
#define PC_NEAR_DESTINATION     PC_BASE_COST*6  // near destination is <= 10 maptiles
#define PC_BADFINALRATIO        3               // how much times (than heuristics) is final way expensive 
#define PC_BADSHIFT             -20             // bad shift for final way cutting
#define PC_FSBEHIND             6               // how many steps remains behind walker
#define PC_WAITING_TIMEOUT      8               // waiting timeout to replann way
#define PC_DEADLOCK_TIMEOUT     40              // waiting timeout to say we are in deadlock

// LIMITS
#define PE_MAX_INT              2147483647

#define MAXPATH 200

#ifdef __cplusplus
	extern "C" {
#endif

//###########################################################################
//# PE structure
//###########################################################################
typedef struct _PathEngineData PED;
typedef int peHeuristics(PED*, MapNode*,MapNode*);
typedef int peMixFunction(PED*, int, int, int);

// Path engine object
typedef struct _PathEngineData
{
  // engine states
  peTime    Time;           // engine time
  int       State;          // engine state

  // map
  int       MapSX, MapSY;   // map dimensions
  MapNode*  Map;            // map tiles array

  // walkers
  int       MaxWalkers;     // max walkers engine can handle
  Walker*   Walkers;        // array 
  Walker*   FirstWalker;    // linked list of walkers
  Walker*   LastWalker;     // end of linked list

  // paths
  int       MaxSteps;       // not used

  // requests
  int       MPCPT;          // maximum paths counted per tick
  Request*  RequestsTop;    // requests ring top boundary
  Request*  RequestsBottom; // requests ring bottom boundary
  Request*  FirstRequest;   // requests ring start
  Request*  LastRequest;    // requests ring end

  // heuristics
  peHeuristics* Heuristics; // Heuristics function

  // mix function
  float MixCoef;            // ranges from 0.0 to 1.0 // 1.0=dijkstra, 0.5=A*, 0.0=best-first
  peMixFunction* MixFunction; // mixing function, MixCustom, MixDijkstra, MixAStar, MixBestFirst
  
  // A-star heap
  int       HeapSize;       // heap size
  Heap*     Open;           // heap of open nodes
  int       LastSID;        // SID = Search ID

  // engine settings
  int MAXTC;                // maximal terrain cost
  int StandingBlockerCost;  // additional cost of blocked node by walker
  int WalkingBlockerCost;   // additional cost of temoprary blocked node
  int FootStepCost;         // additional cost of node with step
  int BaseCost;             // cost of clear node (for heuristics)
  int HorizontalCost;       // cost of horizontal/vertical clear move
  int DiagonalCost;         // cost of diagonal clear move
  int NearDestination;      // near destination is <= 10 maptiles
  int BadFinalRatio;        // how much times (than heuristics) is final way expensive 
  int BadFinalShift;        // bad shift for final way cutting
  int FSBehind;             // how many footsteps remain behind walker
  peTime WaitingTimeout;    // waiting timeout to replann way
  peTime DeadlockTimeout;   // waiting timeout to say we are in deadlock

} PED;

//###########################################################################
//# API
//###########################################################################

PED* peInit(int msx, int msy, int iMaxRequests, int iHeapSize, int iMPCPT, int iMaxWalkers);
  char peInitRequests(PED* PE, int iMaxRequests, int iMPCPT);
  char peInitMap(PED* PE, int msx, int msy);
  char peInitWalkers(PED* PE, int iMaxWalkers);
  char peInitPaths(PED* PE, int iMaxSteps);
char peDone(PED* PE);
  char peDoneRequests(PED* PE);
  char peDoneMap(PED* PE);
  char peDoneWalkers(PED* PE);
  char peDonePaths(PED* PE);
PED* peLoad(FILE* f);
  char peLoadRequests(PED* PE, FILE* f);
  char peLoadMap(PED* PE, FILE* f);
  char peLoadWalkers(PED* PE, FILE* f);
  char peLoadPaths(PED* PE, FILE* f);
char peSave(PED* PE, FILE* f);
  char peSaveRequests(PED* PE, FILE* f);
  char peSaveMap(PED* PE, FILE* f);
  char peSaveWalkers(PED* PE, FILE* f);
  char peSavePaths(PED* PE, FILE* f);
char peSetHeuristics(PED* PE, peHeuristics* H);
char peSetMixFunction(PED* PE, peMixFunction* M);
char peSetMixCoef(PED* PE, float C);
char peSetSettings(PED* PE, int MAXTC, int StandingBlockerCost, int WalkingBlockerCost, int FootStepCost, 
int BaseCost, int HorizontalCost, int DiagonalCost, int NearDestination,
int BadFinalRatio, int BadFinalShift, int FSBehind, peTime WaitingTimeout, peTime DeadlockTimeout);

Walker* peAddWalker(PED* PE);
char peRemoveWalker(PED* PE, Walker* w);
char peResizeWalkers(PED* PE, int NewMaxWalkers);
char peSetPosWalker(PED* PE, Walker* w, MapNode* pos);
char pePlanWalker(PED* PE, Walker* w, MapNode* dest, char type);
char peStopWalker(PED* PE, Walker* w);
char peDeadlockWalker(PED* PE, Walker* w);
char pePlanWay(PED* PE, Walker* w, MapNode* dest, char type);
char peSelectAllWalkers(PED* PE);
char peUnselectAllWalkers(PED* PE);

char peAddPath(PED* PE, PathStep** path, int length);
char peRemovePath(PED* PE, PathStep* Path);

char peAddRequest(PED* PE, Walker *w, MapNode* dest, char type);
char peRemoveRequest(PED * PE, Request * r);
char peDiscardRequests(PED * PE, Walker * w);
char peResetRequests(PED* PE);
char peManageRequests(PED* PE);

char peTimeTick(PED* PE);
char peTimeForward(PED* PE, int ticks);

int  peFindPath(PED* PE, MapNode* Start, MapNode* End, peTime StartTime, int Speed, PathStep** path);
int  peFindPathNTC(PED* PE, MapNode* Start, MapNode* End, peTime StartTime, int Speed, PathStep** path);
int  peFindPathWW(PED* PE, MapNode* Start, MapNode* End, peTime StartTime, int Speed, PathStep** path);
int  peBresPath(PED* PE, MapNode* Start, MapNode* End, peTime StartTime, int Speed, PathStep** path);

MapNode* peMapXY(PED* PE, int X, int Y);

int HeuristicsEuclidean(PED* PE, MapNode* A, MapNode* B);
int HeuristicsMax(PED* PE, MapNode* A, MapNode* B);
int HeuristicsManhattan(PED* PE, MapNode* A, MapNode* B);

int MixCustom(PED* PE, int g, int h, int z);
int MixDijkstra(PED* PE, int g, int h, int z);
int MixBestFirst(PED* PE, int g, int h, int z);
int MixAStar(PED* PE, int g, int h, int z);

// global vars
extern peError peLastError;
extern const signed char Delta2Dir[3][3];
extern const signed char Dir2Delta[8][2];

#ifdef PE_MONITOR
  extern long peENodes;     // examined nodes
#endif

#ifdef __cplusplus
	}
#endif

#endif
