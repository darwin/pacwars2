#ifndef GUIJOINGAMEMENU_H
#define GUIJOINGAMEMENU_H

#include "guibasemenu.h"
#include "pglabel.h"

class GUI_TextEdit;
class GUI_ButtonSmall;

class GUI_JoinGameMenu : public GUI_BaseMenu {
public:
  GUI_JoinGameMenu();

  PG_Label* JoinGameMenu;

  PG_Label* lServerAddress;
  GUI_TextEdit* eServerAddress;
  PG_Label* lClientName;
  GUI_TextEdit* eClientName;
  PG_Label* lClientDesc;
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
