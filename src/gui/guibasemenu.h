#ifndef GUIBASEMENU_H
#define GUIBASEMENU_H

#include "pgstaticframe.h"

/////////////////////////////////////////////////////////////////////////////
// Base menu widgets
/////////////////////////////////////////////////////////////////////////////

class GUI_BaseMenu : public PG_StaticFrame {
public:
  int mid;
  int parentmid;
  GUI_BaseMenu *next;

  GUI_BaseMenu(int iid, const PG_Rect& rect);

  void Clear();

  virtual void Default() { }
  virtual void Execute() { }
  virtual void Return();
};

#endif GUIBASEMENU_H
