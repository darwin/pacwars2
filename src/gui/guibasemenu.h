#ifndef GUIBASEMENU_H
#define GUIBASEMENU_H

#include "pggradientwidget.h"

/////////////////////////////////////////////////////////////////////////////
// Base menu widgets
/////////////////////////////////////////////////////////////////////////////

class GUI_BaseMenu : public PG_GradientWidget {
public:
  int mid;
  int parentmid;
  GUI_BaseMenu *next;

  GUI_BaseMenu(int iid, const PG_Rect& rect);

  void eventDraw(SDL_Surface* surface, const PG_Rect& rect);
  void Clear();

  virtual void Default() { }
  virtual void Execute() { }
  //virtual void Show()=0;
  //virtual void Hide()=0;
  virtual void Return();
};

#endif GUIBASEMENU_H
