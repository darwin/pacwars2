/*
    IMGLIB:  An example image loading library for use with SDL
    Copyright (C) 1999  Sam Lantinga

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    5635-34 Springhouse Dr.
    Pleasanton, CA 94588 (USA)
    slouken@devolution.com
*/

/* A simple library to load images of various formats as SDL surfaces */

#ifndef _IMG_h
#define _IMG_h

#include "SDL.h"
#include "begin_code.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* Load an image from an SDL datasource */
extern DECLSPEC SDL_Surface *IMG_Load_RW(SDL_RWops *src, int freesrc);

/* Compatibility convenience function -- loads an image from a file */
#define IMG_Load(file)	IMG_Load_RW(SDL_RWFromFile(file, "rb"), 1)

/* Invert the alpha of a surface for use with OpenGL
   If you want to use a surface loaded with this library as an OpenGL texture,
   set invart_alpha to 1.  If you want to use it with SDL alpha blit routines,
   set it to 0.
   This function returns the old alpha inversion value.

   Currently this is is only used by the PNG loader.
 */
extern DECLSPEC int IMG_InvertAlpha(int on);

/* Functions to detect a file type, given a seekable source */
extern DECLSPEC int IMG_isBMP(SDL_RWops *src);
extern DECLSPEC int IMG_isPPM(SDL_RWops *src);
extern DECLSPEC int IMG_isPCX(SDL_RWops *src);
extern DECLSPEC int IMG_isGIF(SDL_RWops *src);
extern DECLSPEC int IMG_isJPG(SDL_RWops *src);
extern DECLSPEC int IMG_isTIF(SDL_RWops *src);
extern DECLSPEC int IMG_isPNG(SDL_RWops *src);

/* Individual loading functions */
extern DECLSPEC SDL_Surface *IMG_LoadBMP_RW(SDL_RWops *src);
extern DECLSPEC SDL_Surface *IMG_LoadPPM_RW(SDL_RWops *src);
extern DECLSPEC SDL_Surface *IMG_LoadPCX_RW(SDL_RWops *src);
extern DECLSPEC SDL_Surface *IMG_LoadGIF_RW(SDL_RWops *src);
extern DECLSPEC SDL_Surface *IMG_LoadJPG_RW(SDL_RWops *src);
extern DECLSPEC SDL_Surface *IMG_LoadTIF_RW(SDL_RWops *src);
extern DECLSPEC SDL_Surface *IMG_LoadPNG_RW(SDL_RWops *src);

/* We'll use SDL for reporting errors */
#define IMG_SetError	SDL_SetError
#define IMG_GetError	SDL_GetError

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
};
#endif
#include "close_code.h"

#endif /* _IMG_h */
