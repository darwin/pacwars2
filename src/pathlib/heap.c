/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## heap.c [PATHLIB]
//## 
//## desc.:   MapNodes heap implementation
//## author:  Antonin Hildebrand [hildi@atlas.cz]
//## 
//## started: 12/05/1999
//## revised: 05/06/2000
//## 
//###########################################################################

#include <malloc.h>
#include "heap.h"

int hpLastError;

Heap* heapInit(int MaxSize)
{
  Heap* h;
  h = (Heap*)malloc(sizeof(Heap));
  if (!h) { hpLastError = HP_NOMEM; return NULL; }

  h->MaxSize = MaxSize;
  h->ActSize = 0;
  h->HeapBuffer = (MapNode**)malloc((MaxSize+1)*sizeof(MapNode*));
  if (!h->HeapBuffer) 
  { 
    free(h); 
    hpLastError = HP_NOMEM; 
    return NULL; 
  }
  
  h->HeapBuffer[0] = (MapNode*)malloc(sizeof(MapNode));
  if (!h->HeapBuffer[0]) 
  { 
    free(h->HeapBuffer);
    free(h); 
    hpLastError = HP_NOMEM; 
    return NULL; 
  }
  h->HeapBuffer[0]->f = -1;  // zarazka

  return h;
}

char heapDone(Heap* h)
{
  free(h->HeapBuffer[0]);
  free(h->HeapBuffer);
  free(h);
  return 1;
}

char heapClear(Heap* h)
{
  h->ActSize = 0;
  return 1;
}

char heapAdd(Heap* h, MapNode* node)
{
  if (h->ActSize == h->MaxSize) return HP_NOSPACE;
  h->ActSize++;
  h->HeapBuffer[h->ActSize] = node;
  node->HeapPos = h->ActSize;
  return heapSiftUp(h, h->ActSize);
}

char heapRemove(Heap* h, MapNode* node)
{
  char res;
  if (node->HeapPos == 0) return HP_NODENOTFOUND;
  h->HeapBuffer[node->HeapPos] = h->HeapBuffer[h->ActSize];
  h->HeapBuffer[node->HeapPos]->HeapPos = node->HeapPos;
  h->ActSize--;
  res = heapSiftDown(h, node->HeapPos);
  node->HeapPos = 0;
  return res;
}

char heapSiftUp(Heap* h, int index)
{
  MapNode* save;
  while ((index>0) && (h->HeapBuffer[index]->f < h->HeapBuffer[index/2]->f))
  {
    save = h->HeapBuffer[index];
    h->HeapBuffer[index] = h->HeapBuffer[index/2];
    h->HeapBuffer[index/2] = save;
    h->HeapBuffer[index]->HeapPos = index;
    h->HeapBuffer[index/2]->HeapPos = index/2;
    index/=2;
  }
  return 1;
}

char heapSiftDown(Heap* h, int index)
{
  MapNode* save;
  int j;

  while (index <= h->ActSize) 
  {  
    j=2*index;
    if (j > h->ActSize) break;
    if ((j < h->ActSize) && (h->HeapBuffer[j]->f > h->HeapBuffer[j+1]->f)) j=j+1;
    if (h->HeapBuffer[index]->f > h->HeapBuffer[j]->f)
    {
      save = h->HeapBuffer[index];
      h->HeapBuffer[index] = h->HeapBuffer[j];
      h->HeapBuffer[j] = save;
      h->HeapBuffer[index]->HeapPos = index;
      h->HeapBuffer[j]->HeapPos = j;
    }
    else break;
    index=j;
  }
  return 1;
}
