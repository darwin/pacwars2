SDL Console
	A console and text output routine written for SDL.

Garrett Banuk
(mongoose@wpi.edu)
http://www.wpi.edu/~mongoose/SDL_Console

This code containes a pull down console that can be added into any
SDL program. Along with this program are text output routines which
can be used seperately by themselves from the console. The console 
itself requires these text output routines though.


Heres an overview of the CONSOLE API.


#include "SDL_Console.h"

int  ConsoleInit(char *FontName, SDL_Surface *DisplayScreen, int lines);  
Takes in a string of the path to the bitmap fontname, the surface the 
console will be displayed on, and the number of lines in
the back buffer of the console.

void ConsoleEvents(SDL_Event *event);
Send SDL events to this to be processed when the console is
down. Be sure to set SDL_EnableUNICODE(1); for SDL before
the commands are sent to the console so the console can
interpet the commands.

void DrawConsole(void);
Draws the console on the screen.

void ConOut(char *str, ... );
Displays strings to the console, works just like printf()

void CommandExecute(char *BackStrings);
Executes a command string passed to it. Usually only used
internally by the console API.

void AddCommand(void (*CommandCallback)(char *Parameters), char *CommandWord);
The first parameter of this is a pointer to a function that
takes a string pointer as a parameter. The string is the arguments
passed to the command in the console. The second is the string
of the command (no space) the user will type into the console to call
this function.

void ListCommands(void);
Lists the commands in the console.

int ConsoleBackground(char *image, int x, int y);
Loads the file from image to the location (x,y) on the background.
If this function is passed NULL for image the background is unloaded.

void ConsoleAlpha( unsigned char alpha );
Alpha blends the console into the background, value range is 1-200.
If the alpha value passed is 0 alpha blending is turned off.

int SetConsoleBorder(char *borderset);
Loads a boarder to draw at the bottom. Uses transparency in the border
image, RGB(255,0,255)


Page_UP Page_DOWN scroll around the back buffer of the console
TAB tab completes commands
END goes back to the bottom of the console

Heres an overview of the DRAW TEXT API.


#include "SDL_DrawText.h"


int LoadFont( char *BitmapName, int flags );
Loads a font from the BitmapName file and returns the
number of the font. returns -1 as an error. Pass TRANS_FONT
to the flags if the font is transparent. Transparent color is
RGB(255,0,255)

void DrawText( char *string, SDL_Surface *surface, int FontType, int x, int y );
Draws text to the screen. First parameter is the text to
draw, second is the surface to draw too, third is the font
type to draw, last two are the x,y screen location.

int FontHeight( int FontNumber );
returns the font character height.

int FontWidth( int FontNumber );
returns the font character width.


Some sample functions in the console are
drawtextdemo
   An example of the DrawText routines
printme [string]
   Parses the command line passed to printme and prints
   it back out again
quit
   Quits the program.
border
   Adds the border to the bottom of the console. Uses
   a transparent blit.
background [x] [y]
   Displays the background at (x,y) coordinates. Pass -1 for the 
   x or y value to turn the background off.
alpha [value]
   Blends the console into the background, value is the amount
   of blending from 1-200. 0 Turns off alpha blending.

in this demo the '~' key pulls the console down
