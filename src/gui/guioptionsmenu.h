#ifndef GUIOPTIONSMENU_H
#define GUIOPTIONSMENU_H

#include "guibasemenu.h"

class GUI_ButtonBig;


class GUI_OptionsMenu : public GUI_BaseMenu {
public:
  GUI_OptionsMenu();

  GUI_ButtonBig* lOptionsMenu;
  GUI_ButtonBig* bServer;
  GUI_ButtonBig* bClient;
  GUI_ButtonBig* bVideo;
  GUI_ButtonBig* bSound;
  GUI_ButtonBig* bPaths;
  GUI_ButtonBig* bBack;

  void Default();

protected:

  bool eventButtonClick(int id, PG_Widget* widget);
  void eventShow();
  void eventHide();
};

#endif	// GUIOPTIONSMENU_H
