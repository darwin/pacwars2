#ifndef GUICREATEPLAYERSELMENU_H
#define GUICREATEPLAYERSELMENU_H

#include "guibasemenu.h"

class GUI_ButtonBig;


class GUI_CreatePlayerSelMenu : public GUI_BaseMenu {
public:
  GUI_CreatePlayerSelMenu();

  GUI_ButtonBig* lCreatePlayerSelMenu;
  GUI_ButtonBig* bCP1;
  GUI_ButtonBig* bCP2;
  GUI_ButtonBig* bCP3;
  GUI_ButtonBig* bCP4;
  GUI_ButtonBig* bBack;

  void Default();

protected:

  void eventShow();
  void eventHide();

  bool eventButtonClick(int id, SDLWidget* widget);
};

#endif	// GUICREATEPLAYERSELMENU_H
