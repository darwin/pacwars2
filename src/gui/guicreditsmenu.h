#ifndef GUICREDITSMENU_H
#define GUICREDITSMENU_H

#include "guibasemenu.h"
#include "pglabel.h"

class GUI_Board;
class GUI_Bitmap;
class GUI_ButtonSmall;


class GUI_CreditsMenu : public GUI_BaseMenu {
public:
  GUI_CreditsMenu();

  GUI_Bitmap* mBack1;
  PG_Label* CreditsMenu;

  PG_Label* lLine[19];

  GUI_ButtonSmall* bOK;

  void Default();

protected:

  bool eventButtonClick(int id, PG_Widget* widget);
  void eventShow();
  void eventHide();
};

#endif	// GUICREDITSMENU_H
