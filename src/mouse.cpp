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

#include "mouse.h"

short int mouse_hot_x, mouse_hot_y;
int mouse_x, mouse_y;
SDL_Surface *mouse_surface;
bool mouse_on;


void InitMouse(short int hotx, short int hoty)
{
	mouse_hot_x = hotx;
	mouse_hot_y = hoty;
	mouse_on = true;
}

void ShowMouse()
{
	mouse_on = true;
}

void HideMouse()
{
	mouse_on = false;
}

bool LoadMouse(SDL_RWops * src)
{
	SDL_Surface *s;
	SDL_Surface *temp;
	temp = IMG_Load_RW(src, 0);
	if (!temp)
		return false;

	s = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	if (s->format->BitsPerPixel == 16) {
		Uint16 *px = ((Uint16 *) s->pixels) + s->w - 1;
		SDL_SetColorKey(s, SDL_SRCCOLORKEY | SDL_RLEACCEL, *px);
	} else {
		if (s->format->BitsPerPixel == 32) {
			Uint32 *px = ((Uint32 *) s->pixels) + s->w - 1;
			SDL_SetColorKey(s, SDL_SRCCOLORKEY | SDL_RLEACCEL, *px);
		}
	}

	DoneMouse();
	mouse_surface = s;
	return true;
}

void UpdateMouse()
{
	SDL_GetMouseState(&mouse_x, &mouse_y);
}

void DrawMouse(SDL_Surface * dest)
{
	if (mouse_on) {
		SDL_Rect r;
		r.x = mouse_x - mouse_hot_x;
		r.y = mouse_y - mouse_hot_y;
		r.w = mouse_surface->w;
		r.h = mouse_surface->h;
		SDL_BlitSurface(mouse_surface, NULL, dest, &r);
	}
}

void DoneMouse()
{
	if (mouse_surface)
		SDL_FreeSurface(mouse_surface);
	mouse_surface = NULL;
}
