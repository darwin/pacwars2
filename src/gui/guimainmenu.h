#ifndef GUIMAINMENU_H
#define GUIMAINMENU_H

#include "guibasemenu.h"

class GUI_ButtonBig;


class GUI_MainMenu : public GUI_BaseMenu {
public:
  GUI_MainMenu();

  GUI_ButtonBig* lMainMenu;
  GUI_ButtonBig* bNewGame;
  GUI_ButtonBig* bEndGame;
  GUI_ButtonBig* bJoinDiscGame;
  GUI_ButtonBig* bCreatePlayer;
  GUI_ButtonBig* bDeletePlayer;
  GUI_ButtonBig* bOptions;
  GUI_ButtonBig* bHelp;
  GUI_ButtonBig* bCredits;
  GUI_ButtonBig* bQuit;
  GUI_ButtonBig* bGame;

  void Return();
  void Default();

protected:

  bool eventButtonClick(int id, SDLWidget* widget);
  void eventShow();
  void eventHide();
};

#endif	// GUIMAINMENU_H
