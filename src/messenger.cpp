/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   Messenger
//## author:  Antonin Hildebrand
//## 
//## started: 23.6.2000
//## revised: ???
//## 
//## ingame message title implementation
//## 
//###########################################################################

#include <stdarg.h>
#include "messenger.h"
#include "console/SDL_DrawText.h"
#include "sampleman.h"

#define MAX_MSNG_MESSAGES 5

cvar_t messenger_x = { "messenger_x", STD_MESSENGER_X, true };
cvar_t messenger_y = { "messenger_y", STD_MESSENGER_Y, true };
cvar_t messenger_time = { "messenger_time", STD_MESSENGER_TIME, true };

static char messenger_text[MAX_MSNG_MESSAGES][MAX_MESSENGER_TEXT] =
	{ "", "", "", "", "" };
static Uint32 messenger_last_time[MAX_MSNG_MESSAGES] = { 0, 0, 0, 0, 0 };

int messenger_font = 0;

void DrawMessenger(SDL_Surface * s)
{
	Uint32 time = SDL_GetTicks();
	int i = 0;

	for (i = 0; i < MAX_MSNG_MESSAGES; i++)
		if (!messenger_text[i][0]) {
			for (int j = i + 1; j < MAX_MSNG_MESSAGES; j++) {
				strcpy(messenger_text[j - 1], messenger_text[j]);
				messenger_last_time[j - 1] = messenger_last_time[j];
				messenger_text[j][0] = 0;
			}
		}

	for (i = 0; i < MAX_MSNG_MESSAGES; i++) {
		if ((time - messenger_last_time[i]) > messenger_time.value) {
			messenger_text[i][0] = 0;
		}
		if (messenger_text[i][0] != 0) {
			DrawText(messenger_text[i], s, messenger_font,
					 (int) messenger_x.value,
					 (int) messenger_y.value + i * 12);
		}
	}
}

void SetMessenger(const char *str, ...)
{
	int i = 0;

	for (i = 0; i < MAX_MSNG_MESSAGES; i++) {
		if (!messenger_text[i][0])
			break;
	}
  if (i==MAX_MSNG_MESSAGES) 
  {
    for (int j = 1; j < MAX_MSNG_MESSAGES; j++) {
			strcpy(messenger_text[j - 1], messenger_text[j]);
      messenger_last_time[j - 1] = messenger_last_time[j];
      messenger_text[j][0] = 0;
    }
    
    i = MAX_MSNG_MESSAGES-1;
  }

  va_list marker;

	va_start(marker, str);
	vsprintf(messenger_text[i], str, marker);
	va_end(marker);

	messenger_last_time[i] = SDL_GetTicks();
//	smPlaySample(SM_MESSENGER);
}
