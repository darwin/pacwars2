#ifndef GUICREDITSMENU_H
#define GUICREDITSMENU_H

#include "guibasemenu.h"

class GUI_Board;
class GUI_Bitmap;
class GUI_LabelC;
class GUI_ButtonSmall;


class GUI_CreditsMenu : public GUI_BaseMenu {
public:
  GUI_CreditsMenu();

  GUI_Bitmap* mBack1;
  GUI_LabelC* CreditsMenu;

  GUI_LabelC* lLine[20];

  GUI_ButtonSmall* bOK;

  void Default();

protected:

  bool eventButtonClick(int id, SDLWidget* widget);
  void eventShow();
  void eventHide();
};

#endif	// GUICREDITSMENU_H
