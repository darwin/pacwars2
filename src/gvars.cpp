/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   main game structure
//## author:  Antonin Hildebrand
//##         
//## started: 3.4.2000
//## revised: ???
//## 
//## 
//###########################################################################

#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "const.h"
#include "game.h"

//###########################################################################
//## Globals
//###########################################################################

CVars::CVars()
{
	camx = 0;
	camy = 0;
	zonex = 0;
	zoney = 0;
	chasing = -1;
}

CVars::~CVars()
{

}
