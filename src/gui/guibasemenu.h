#ifndef GUIBASEMENU_H
#define GUIBASEMENU_H

#include "sdlwidget.h"

/////////////////////////////////////////////////////////////////////////////
// Base menu widgets
/////////////////////////////////////////////////////////////////////////////

class GUI_BaseMenu : public SDLWidget {
public:
  int mid;
  int parentmid;
  GUI_BaseMenu *next;

  GUI_BaseMenu(int iid, SDL_Rect& rect);

  void eventDraw(SDL_Surface* surface, SDL_Rect* rect);
  void Clear();

  virtual void Default() { }
  virtual void Execute() { }
  //virtual void Show()=0;
  //virtual void Hide()=0;
  virtual void Return();
};

#endif GUIBASEMENU_H
