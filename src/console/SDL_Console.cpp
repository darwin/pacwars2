/*  SDL_Console.c
*  Written By: Garrett Banuk <mongoose@wpi.edu>
*  This is free, just be sure to give me credit when using it
*  in any of your programs.
*/


#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdarg.h>
#include "SDL.h"
#include "SDL_Console.h"
#include "SDL_DrawText.h"
#include "SDL_image.h"

static char **ConsoleLines = NULL;
static char **CommandLines = NULL;
static int TotalConsoleLines = 0;	/* Total number of lines in the console */
static int ConsoleScrollBack = 0;	/* How much the users scrolled back in the console */
static int TotalCommands = 0;	/* Number of commands in the Back Commands */
static int FontNumber;			/* This is the number of the font for the console */
static int Line_Buffer;			/* The number of lines in the console */
static int BackX, BackY;		/* Background images x and y coords */
static SDL_Surface *ConsoleSurface;	/* Surface for the console text */
static SDL_Surface *OutputScreen;	/* This is the screen to draw to */
static SDL_Surface *Border = NULL;	/* Sides, bottom and corners of the console border */
static SDL_Surface *BackgroundImage = NULL;	/* Background image for the console */
static SDL_Surface *InputBackground;	/* Dirty rectangle to draw over behind the users background */

/* ########################################################## */
static char *CursorLine = NULL;
#define					CursorChar '_'
/* ########################################################## */

extern void SV_RemoteViewer(Uint8, char *);	// defined in server.cpp

/* Takes keys from the keyboard and inputs them to the console */
void ConsoleEvents(SDL_Event * event)
{

	static int StringLocation = 0;	/* Current character location in the current string */
	static int CommandScrollBack = 0;	/* How much the users scrolled back in the command lines */
	SDL_Rect inputbackground;

	switch (event->type) {
	case SDL_KEYDOWN:
		switch (event->key.keysym.sym) {
		case SDLK_PAGEUP:
			if (ConsoleScrollBack < TotalConsoleLines
				&& ConsoleScrollBack < Line_Buffer) {
				ConsoleScrollBack++;
				UpdateConsole();
			}
			break;
		case SDLK_PAGEDOWN:
			if (ConsoleScrollBack > 0) {
				ConsoleScrollBack--;
				UpdateConsole();
			}
			break;
		case SDLK_END:
			ConsoleScrollBack = 0;
			UpdateConsole();
			break;
		case SDLK_UP:
			if (CommandScrollBack < TotalCommands) {
				/* move back a line in the command strings and copy the command to the current input string */
				CommandScrollBack++;
				memset(ConsoleLines[0], 0, CHARS_PER_LINE);
				strcpy(ConsoleLines[0], CommandLines[CommandScrollBack]);
				StringLocation = strlen(CommandLines[CommandScrollBack]);
				UpdateConsole();
			}
			break;
		case SDLK_DOWN:
			if (CommandScrollBack > 0) {
				/* move forward a line in the command strings and copy the command to the current input string */
				CommandScrollBack--;
				memset(ConsoleLines[0], 0, CHARS_PER_LINE);
				strcpy(ConsoleLines[0], CommandLines[CommandScrollBack]);
				StringLocation = strlen(ConsoleLines[CommandScrollBack]);
				UpdateConsole();
			}
			break;
		case SDLK_BACKSPACE:
			if (StringLocation > 0) {
				ConsoleLines[0][StringLocation - 1] = '\0';
				StringLocation--;
				inputbackground.x = 0;
				inputbackground.y =
					ConsoleSurface->h - FontHeight(FontNumber);
				inputbackground.w = ConsoleSurface->w;
				inputbackground.h = FontHeight(FontNumber);
				SDL_BlitSurface(InputBackground, NULL, ConsoleSurface,
								&inputbackground);

				//DrawText(ConsoleLines[0], ConsoleSurface, FontNumber, 4, ConsoleSurface->h-FontHeight(FontNumber));
				/* ########################################################## */
				DrawCursor();
				/* ########################################################## */

			}
			break;
		case SDLK_TAB:
			TabCompletion(ConsoleLines[0], &StringLocation);
			break;
		case SDLK_RETURN:
			NewLineCommand();
			/* copy the input into the past commands strings */
			strcpy(CommandLines[0], ConsoleLines[0]);
			strcpy(ConsoleLines[1], ConsoleLines[0]);
			SV_RemoteViewer(0, ConsoleLines[0]);
			NewLineConsole();
			CommandExecute(ConsoleLines[0]);
			/* zero out the current string and get it ready for new input */
			memset(ConsoleLines[0], 0, CHARS_PER_LINE);
			CommandScrollBack = -1;
			StringLocation = 0;
			UpdateConsole();
			break;
		case SDLK_LSHIFT:
		case SDLK_RSHIFT:
		case SDLK_LCTRL:
		case SDLK_RCTRL:
		case SDLK_LALT:
		case SDLK_RALT:
		case SDLK_CAPSLOCK:
		case SDLK_NUMLOCK:
		case SDLK_SCROLLOCK:

			break;
		default:
			if (StringLocation < CHARS_PER_LINE - 1
				&& event->key.keysym.unicode) {
				ConsoleLines[0][StringLocation] =
					(char) event->key.keysym.unicode;
				StringLocation++;
				inputbackground.x = 0;
				inputbackground.y =
					ConsoleSurface->h - FontHeight(FontNumber);
				inputbackground.w = ConsoleSurface->w;
				inputbackground.h = FontHeight(FontNumber);
				SDL_BlitSurface(InputBackground, NULL, ConsoleSurface,
								&inputbackground);

				//DrawText(ConsoleLines[0], ConsoleSurface, FontNumber, 4, ConsoleSurface->h-FontHeight(FontNumber));
				/* ########################################################## */
				DrawCursor();
				/* ########################################################## */

			}
			break;
		}
		break;
	default:
		break;
	}
}


/* ########################################################## */
/* Draws the last line with cursor */
void DrawCursor()
{
	memset(CursorLine, 0, CHARS_PER_LINE);
	strcpy(CursorLine, ConsoleLines[0]);
	if (strlen(CursorLine) < CHARS_PER_LINE)
		CursorLine[strlen(CursorLine)] = CursorChar;

	DrawText(CursorLine, ConsoleSurface, FontNumber, 4,
			 ConsoleSurface->h - FontHeight(FontNumber));
}

/* ########################################################## */

/* Updates the console buffer */
void UpdateConsole()
{
	int loop;
	int Screenline = OutputScreen->h / 2 / FontHeight(FontNumber);
	SDL_Rect DestRect;


	SDL_FillRect(ConsoleSurface, NULL, 0);

	/* draw the background image if there is one */
	if (BackgroundImage) {
		DestRect.x = BackX;
		DestRect.y = BackY;
		DestRect.w = BackgroundImage->w;
		DestRect.h = BackgroundImage->h;
		SDL_BlitSurface(BackgroundImage, NULL, ConsoleSurface, &DestRect);
	}

	/* Draw the text */
	for (loop = 0; loop < Screenline; loop++)
		DrawText(ConsoleLines[Screenline - loop + ConsoleScrollBack],
				 ConsoleSurface, FontNumber, 4,
				 loop * FontHeight(FontNumber));

	if (ConsoleScrollBack > 0)
		for (loop = 4; loop < OutputScreen->w - FontWidth(FontNumber) * 2;
			 loop += FontWidth(FontNumber) * 2)
			DrawText("^ ", ConsoleSurface, FontNumber, loop,
					 ConsoleSurface->h - FontHeight(FontNumber));
	else
		//DrawText(ConsoleLines[0], ConsoleSurface, FontNumber, 4, ConsoleSurface->h-FontHeight(FontNumber));
		/* ########################################################## */
		DrawCursor();
	/* ########################################################## */


}

/* Draws the console buffer to the screen */
void DrawConsole()
{
	int loop;
	SDL_Rect DestRect = { 0, 0, ConsoleSurface->w, ConsoleSurface->h };


	SDL_BlitSurface(ConsoleSurface, NULL, OutputScreen, &DestRect);

	/* Now draw a border */
	if (Border) {
		DestRect.x = 0;
		DestRect.y = ConsoleSurface->h;
		DestRect.w = Border->w;
		DestRect.h = Border->h;

		for (loop = 0; loop <= OutputScreen->w / Border->w; loop++) {
			DestRect.x = Border->w * loop;
			SDL_BlitSurface(Border, NULL, OutputScreen, &DestRect);
		}
	}


}

int DoneConsole()
{
	int loop;
	if (ConsoleLines) {
		for (loop = 0; loop <= Line_Buffer - 1; loop++)
			free(ConsoleLines[loop]);
		free(ConsoleLines);
		ConsoleLines = NULL;
	}
	if (CommandLines) {
		for (loop = 0; loop <= Line_Buffer - 1; loop++)
			free(CommandLines[loop]);
		free(CommandLines);
		CommandLines = NULL;
	}
	if (CursorLine) {
		free(CursorLine);
		CursorLine = NULL;
	}
	if (ConsoleSurface) {
		SDL_FreeSurface(ConsoleSurface);
		ConsoleSurface = NULL;
	}
	if (InputBackground) {
		SDL_FreeSurface(InputBackground);
		InputBackground = NULL;
	}
	if (Border) {
		SDL_FreeSurface(Border);
		Border = NULL;
	}
	if (BackgroundImage) {
		SDL_FreeSurface(BackgroundImage);
		BackgroundImage = NULL;
	}

	FreeFonts();
	return 0;
}

/* Initializes the console strings */
int InitConsole(const char *FontName, SDL_Surface * DisplayScreen,
				int lines)
{
	int loop;
	SDL_Surface *Temp;

	OutputScreen = DisplayScreen;
	Line_Buffer = lines;

	/* malloc memory for the console lines. */
	ConsoleLines = (char **) malloc(sizeof(char *) * Line_Buffer);
	CommandLines = (char **) malloc(sizeof(char *) * Line_Buffer);

	for (loop = 0; loop <= Line_Buffer - 1; loop++) {
		ConsoleLines[loop] = (char *) calloc(CHARS_PER_LINE, sizeof(char));
		CommandLines[loop] = (char *) calloc(CHARS_PER_LINE, sizeof(char));
	}

	/* ##########################################################  */
	CursorLine = (char *) calloc(CHARS_PER_LINE, sizeof(char));
	/* ##########################################################  */

	/* load the console surface */
	Temp =
		SDL_AllocSurface(SDL_SWSURFACE, OutputScreen->w,
						 OutputScreen->h / 2,
						 OutputScreen->format->BitsPerPixel, 0, 0, 0, 0);
	if (Temp == NULL) {
		SDL_SetError
			("Error Console.c:ConsoleInit()\n\tCouldn't create the ConsoleSurface\n");
		return 1;
	}
	ConsoleSurface =
		SDL_ConvertSurface(Temp, OutputScreen->format, SDL_SWSURFACE);
	SDL_FreeSurface(Temp);
	SDL_FillRect(ConsoleSurface, NULL, 0);

	/* Load the consoles font */
	if (-1 == (FontNumber = LoadFont(FontName, TRANS_FONT))) {
		SDL_SetError("Could not load the font \"%s\" for the console!\n",
					 FontName);
		return 1;
	}

	/* Load the dirty rectangle for user input */
	Temp =
		SDL_AllocSurface(SDL_SWSURFACE, OutputScreen->w,
						 FontHeight(FontNumber),
						 OutputScreen->format->BitsPerPixel, 0, 0, 0, 0);
	if (Temp == NULL) {
		SDL_SetError
			("Error Console.c:ConsoleInit()\n\tCouldn't create the input background\n");
		return 1;
	}
	InputBackground =
		SDL_ConvertSurface(Temp, OutputScreen->format, SDL_SWSURFACE);
	SDL_FreeSurface(Temp);
	SDL_FillRect(InputBackground, NULL, 0);


	ConOut("Console initialised.");
	//  ListCommands();

	/* ########################################################## */
	SDL_EnableKeyRepeat(200, 25);
	/* ########################################################## */

	return 0;
}


/* Increments the console line */
void NewLineConsole()
{
	int loop;
	char *temp = ConsoleLines[Line_Buffer - 1];

	for (loop = Line_Buffer - 1; loop > 1; loop--)
		ConsoleLines[loop] = ConsoleLines[loop - 1];

	ConsoleLines[1] = temp;

	memset(ConsoleLines[1], 0, CHARS_PER_LINE);
	TotalConsoleLines++;
}


/* Increments the command lines */
void NewLineCommand()
{
	int loop;
	char *temp = CommandLines[Line_Buffer - 1];

	for (loop = Line_Buffer - 1; loop > 0; loop--)
		CommandLines[loop] = CommandLines[loop - 1];

	CommandLines[0] = temp;

	memset(CommandLines[0], 0, CHARS_PER_LINE);
	TotalCommands++;
}


/* Outputs error text to the console (in game and stderr), up to 256 chars can be entered
* '\n' characters only take effect on stderr
*/
void ConErr(const char *str, ...)
{
	va_list marker;
	char temp[256];

	va_start(marker, str);
	vsprintf(temp, str, marker);
	va_end(marker);

	SV_RemoteViewer(0, temp);

	if (ConsoleLines) {
		strncpy(ConsoleLines[1], temp, CHARS_PER_LINE);
		ConsoleLines[1][CHARS_PER_LINE - 1] = '\0';
		NewLineConsole();
		UpdateConsole();
	}

	/* And print to stdout */
	fprintf(stderr, "%s\n", temp);
}


/* Outputs text to the console (in game and stdout), up to 256 chars can be entered
* '\n' characters only take effect on stdout.
*/
void ConOut(const char *str, ...)
{
	va_list marker;
	char temp[256];

	va_start(marker, str);
	vsprintf(temp, str, marker);
	va_end(marker);

	SV_RemoteViewer(0, temp);

	if (ConsoleLines) {
		strncpy(ConsoleLines[1], temp, CHARS_PER_LINE);
		ConsoleLines[1][CHARS_PER_LINE - 1] = '\0';
		NewLineConsole();
		UpdateConsole();
	}

	/* And print to stdout */
	fprintf(stdout, "%s\n", temp);
	fflush(stdout);
}

/* Breaks text into MAX_LINE_CHARS long lines and 
* outputs text to the console (in game and stdout), up to 1024 chars can be entered
* '\n' characters only take effect on stdout.
*/
void ConPar(const char *str, ...)
{
	va_list marker;
	char temp[1024];
	char *t;
	char saved;

	va_start(marker, str);
	vsprintf(temp, str, marker);
	va_end(marker);

	SV_RemoteViewer(1, temp);
	t = temp;
	while (strlen(t) > MAX_LINE_CHARS) {
		saved = t[MAX_LINE_CHARS];
		t[MAX_LINE_CHARS] = 0;
		ConOut("%s", t);
		t += MAX_LINE_CHARS;
		t[0] = saved;
	}
	// output the rest of the string
	ConOut("%s", t);

	/* And print to stdout */
	fprintf(stdout, "%s\n", temp);
}

/* Outputs text directly to the console line, up to 256 chars can be entered
* 
*/
void ConPut(int linenum, const char *str, ...)
{
	va_list marker;
	char temp[256];


	va_start(marker, str);
	vsprintf(temp, str, marker);
	va_end(marker);

	SV_RemoteViewer(2, temp);
	if (ConsoleLines) {
		strncpy(ConsoleLines[linenum], temp, CHARS_PER_LINE);
		ConsoleLines[linenum][CHARS_PER_LINE - 1] = '\0';
		UpdateConsole();
	}
}

/* Sets a border for the console, the parameter is NULL the border will 
* be unloaded */
int SetConsoleBorder(const char *border)
{
	SDL_Surface *temp;


	if (border == NULL && Border != NULL) {
		SDL_FreeSurface(Border);
		Border = NULL;
		return 0;
	}

	if (NULL == (temp = IMG_Load(border))) {
		SDL_SetError("Cannot load border %s.", border);
		return 1;
	}
	Border = SDL_ConvertSurface(temp, OutputScreen->format, SDL_SWSURFACE);
	SDL_FreeSurface(temp);

	SDL_SetColorKey(Border, SDL_SRCCOLORKEY,
					Border->format->Rmask | Border->format->Bmask);

	if (ConsoleSurface->format->alpha)
		SDL_SetAlpha(Border, SDL_SRCALPHA, ConsoleSurface->format->alpha);


	return 0;
}


/* Sets the alpha level of the console, 0 turns off alpha blending */
void ConsoleAlpha(unsigned char alpha)
{

	if (alpha > 0 && alpha <= 200) {
		SDL_SetAlpha(ConsoleSurface, SDL_SRCALPHA, alpha);
		if (Border)
			SDL_SetAlpha(Border, SDL_SRCALPHA, alpha);
	} else if (alpha == 0) {
		SDL_SetAlpha(ConsoleSurface, 0, alpha);
		if (Border)
			SDL_SetAlpha(Border, 0, alpha);
	}
	UpdateConsole();
}


/* Adds  background image to the console */
int ConsoleBackground(const char *image, int x, int y)
{
	SDL_Surface *temp;
	SDL_Rect backgroundsrc, backgrounddest;


	if (image == NULL && BackgroundImage != NULL) {
		SDL_FreeSurface(BackgroundImage);
		BackgroundImage = NULL;
		SDL_FillRect(InputBackground, NULL, 0);
		return 0;
	}

	if (NULL == (temp = IMG_Load(image))) {
		SDL_SetError("Cannot load background %s.", image);
		return 1;
	}
	BackgroundImage =
		SDL_ConvertSurface(temp, OutputScreen->format, SDL_SWSURFACE);
	SDL_FreeSurface(temp);
	BackX = x;
	BackY = y;

	backgroundsrc.x = 0;
	backgroundsrc.y = ConsoleSurface->h - FontHeight(FontNumber) - BackY;
	backgroundsrc.w = BackgroundImage->w;
	backgroundsrc.h = InputBackground->h;

	backgrounddest.x = BackX;
	backgrounddest.y = 0;
	backgrounddest.w = BackgroundImage->w;
	backgrounddest.h = FontHeight(FontNumber);

	SDL_FillRect(InputBackground, NULL, 0);
	SDL_BlitSurface(BackgroundImage, &backgroundsrc, InputBackground,
					&backgrounddest);

	return 0;
}
