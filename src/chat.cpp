/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   Chat line
//## author:  Antonin Hildebrand
//## 
//## started: 3.3.2001
//## revised: ???
//## 
//## ingame chat line implementation
//## 
//###########################################################################

#include <stdarg.h>
#include "chat.h"
#include "console/SDL_DrawText.h"
#include "sampleman.h"
#include "client.h"

cvar_t chat_x = { "chat_x", STD_CHAT_X, true };
cvar_t chat_y = { "chat_y", STD_CHAT_Y, true };

static char chat_text[MAX_CHAT_TEXT] = "";
static char chat_history[MAX_CHAT_HISTORY][MAX_CHAT_TEXT] = { "", "", "", "", "", "", "", "", "", "" };
int chat_history_index = 0;

int chat_font = 0;
bool chat_on = false;

void DrawChat(SDL_Surface * s)
{
  if (chat_on)
  {
    char t[1000];
    sprintf(t, "say> %s_", chat_text);
	  DrawText(t, s, chat_font, (int) chat_x.value, (int) chat_y.value);
  }
}

void SetChat(const char *str, ...)
{
	va_list marker;

	va_start(marker, str);
	vsprintf(chat_text, str, marker);
	va_end(marker);
}

void ResetChat()
{
  chat_text[0] = 0;
  chat_on = false;
  chat_history_index = -1;
}

void ChatMoveHistory()
{
  for (int i=MAX_CHAT_HISTORY-2; i>=0; i--)
  {
    strcpy(chat_history[i+1], chat_history[i]);
  }
}

void ChatEvents(SDL_Event * event)
{
	switch (event->type) {
	case SDL_KEYDOWN:
		switch (event->key.keysym.sym) {
    case SDLK_UP:
      if (chat_history_index<MAX_CHAT_HISTORY-1) chat_history_index++;
      strcpy(chat_text, chat_history[chat_history_index]);
			break;
		case SDLK_DOWN:
      if (chat_history_index>0) chat_history_index--;
      strcpy(chat_text, chat_history[chat_history_index]);
			break;
		case SDLK_BACKSPACE:
			if (strlen(chat_text)>0) 
				chat_text[strlen(chat_text)-1] = 0;
			break;
		case SDLK_RETURN:
      if (strlen(chat_text))
      {
	      ChatMessage(chat_text);
      }
      ChatMoveHistory();
      strcpy(chat_history[0], chat_text);
      ResetChat();
      smPlaySample(SM_MESSENGER);
			break;
		case SDLK_ESCAPE:
      ResetChat();
      break;
		case SDLK_PAGEUP:
		case SDLK_PAGEDOWN:
		case SDLK_END:
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
			if (strlen(chat_text)<MAX_CHAT_TEXT-1	&& 6*strlen(chat_text)<440 && event->key.keysym.unicode) 
      {
        chat_text[strlen(chat_text)+1] = 0;
				chat_text[strlen(chat_text)] = (char) event->key.keysym.unicode;
			}
			break;
		}
		break;
	default:
		break;
	}
}