#ifndef GUIJOINGAMEMENU_H
#define GUIJOINGAMEMENU_H

#include "guibasemenu.h"

class GUI_Label;
class GUI_TextEdit;
class GUI_ButtonSmall;

class GUI_JoinGameMenu : public GUI_BaseMenu {
public:
  GUI_JoinGameMenu();

  GUI_Label* JoinGameMenu;

  GUI_Label* lServerAddress;
  GUI_TextEdit* eServerAddress;
  GUI_Label* lClientName;
  GUI_TextEdit* eClientName;
  GUI_Label* lClientDesc;
  GUI_TextEdit* eClientDesc;

  GUI_ButtonSmall* bConnect;
  GUI_ButtonSmall* bCancel;

  void Default();
  void Return();

protected:

  bool eventButtonClick(int id, PG_Widget* widget);
  void eventShow();
  void eventHide();
};

#endif	// GUIJOINGAMEMENU_H
