/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   mouse handling routines
//## author:  Antonin Hildebrand
//##         
//## started: 11.9.2000
//## revised: ???
//## 
//###########################################################################

#ifndef _PACMOUSE_H_
#define _PACMOUSE_H_

#include "SDL.h"
#include "SDL_image.h"

extern DECLSPEC short int mouse_hot_x, mouse_hot_y;
extern DECLSPEC int mouse_x, mouse_y;
extern DECLSPEC SDL_Surface* mouse_surface;
extern DECLSPEC bool mouse_on;

void InitMouse(short int hotx, short int hoty);
bool LoadMouse(SDL_RWops *src);
void UpdateMouse();
void ShowMouse();
void HideMouse();
void DrawMouse(SDL_Surface* screen);

void DoneMouse();




#endif
