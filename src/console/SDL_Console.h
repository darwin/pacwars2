#ifndef SDL_Console_H
#define SDL_Console_H

#include "SDL.h"

#include "../const.h"
#include "SDL_ConsoleCommands.h"

#include "../cvar.h"

extern int  *ConsoleFonts;

#define CHARS_PER_LINE	128

int InitConsole(const char *FontName, SDL_Surface * DisplayScreen,
				int lines);
int DoneConsole();

void ConsoleEvents(SDL_Event * event);
void DrawConsole();
void UpdateConsole();
void NewLineConsole();
void NewLineCommand();
void ConSer(const char *str, ...);
void ConOut(const char *str, ...);
void ConOutEx(int font, const char *str, ...);
void ConErr(const char *str, ...);
void ConPut(int linenum, const char *str, ...);
void ConPar(const char *str, ...);

int SetConsoleBorder(const char *borderset);
void ConsoleAlpha(unsigned char alpha);
int ConsoleBackground(const char *image, int x, int y);


/* ########################################################## */

void DrawCursor();
/* ########################################################## */


#endif
