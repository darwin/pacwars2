#ifndef GUICLIENTMENU_H
#define GUICLIENTMENU_H

#include "guibasemenu.h"
#include "pglabel.h"

class GUI_CheckBox;
class GUI_ButtonSmall;
class GUI_TextEdit;

class GUI_ClientMenu : public GUI_BaseMenu {
public:
  GUI_ClientMenu();

  PG_Label* ClientMenu;

  PG_Label* lClientName;
  GUI_TextEdit* eClientName;
  PG_Label* lClientDesc;
  GUI_TextEdit* eClientDesc;

  GUI_CheckBox* cDelTmps;
  GUI_CheckBox* cDRqs;
  GUI_CheckBox* cURqs;

  PG_Label* lEffect;

  GUI_ButtonSmall* bSet;
  GUI_ButtonSmall* bCancel;

  void Default();
  void Return();

protected:

  bool eventButtonClick(int id, PG_Widget* widget);
  void eventShow();
  void eventHide();
};

#endif	// GUICLIENTMENU_H
