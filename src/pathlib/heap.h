/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## heap.h [PATHLIB]
//## 
//## desc.:   MapNodes heap implementation
//## author:  Antonin Hildebrand [hildi@atlas.cz]
//## 
//## started: 12/05/1999
//## revised: 05/06/2000
//## 
//###########################################################################
#ifndef _HEAP_H_
#define _HEAP_H_

#include "types.h"

#ifdef __cplusplus
	extern "C" {
#endif

#define HP_NOERR            0
#define HP_NOMEM            1
#define HP_NOSPACE          2
#define HP_NODENOTFOUND     3

typedef struct _Heap
{
  int       MaxSize;
  int       ActSize;
  MapNode** HeapBuffer;
} Heap;

extern int hpLastError;

Heap* heapInit(int MaxSize);
char heapDone(Heap* h);

char heapClear(Heap* h);
char heapAdd(Heap* h, MapNode* node);
char heapRemove(Heap* h, MapNode* node);
char heapSiftUp(Heap* h, int index);
char heapSiftDown(Heap* h, int index);

#ifdef __cplusplus
	}
#endif
#endif

