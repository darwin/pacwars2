#ifndef GUICLIENTMENU_H
#define GUICLIENTMENU_H

#include "guibasemenu.h"

class GUI_Label;
class GUI_CheckBox;
class GUI_ButtonSmall;
class GUI_TextEdit;

class GUI_ClientMenu : public GUI_BaseMenu {
public:
  GUI_ClientMenu();

  GUI_Label* ClientMenu;

  GUI_Label* lClientName;
  GUI_TextEdit* eClientName;
  GUI_Label* lClientDesc;
  GUI_TextEdit* eClientDesc;

  GUI_CheckBox* cDelTmps;
  GUI_CheckBox* cDRqs;
  GUI_CheckBox* cURqs;

  GUI_Label* lEffect;

  GUI_ButtonSmall* bSet;
  GUI_ButtonSmall* bCancel;

  void Default();
  void Return();

protected:

  bool eventButtonClick(int id, SDLWidget* widget);
  void eventShow();
  void eventHide();
};

#endif	// GUICLIENTMENU_H
