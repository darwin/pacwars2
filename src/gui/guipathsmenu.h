#ifndef GUIPATHSMENU_H
#define GUIPATHSMENU_H

#include "guibasemenu.h"

class GUI_Label;
class GUI_TextEdit;
class GUI_ButtonSmall;


class GUI_PathsMenu : public GUI_BaseMenu {
public:
  GUI_PathsMenu();

  GUI_Label* lPathsMenu;
  GUI_Label* lEffect;

  GUI_Label* lScriptDir;
  GUI_TextEdit* eScriptDir;
  GUI_Label* lSpriteDir;
  GUI_TextEdit* eSpriteDir;
  GUI_Label* lMapDir;
  GUI_TextEdit* eMapDir;
  GUI_Label* lSndDir;
  GUI_TextEdit* eSndDir;
  GUI_Label* lSkinDir;
  GUI_TextEdit* eSkinDir;
  GUI_Label* lGuiDir;
  GUI_TextEdit* eGuiDir;
  GUI_Label* lGfxDir;
  GUI_TextEdit* eGfxDir;

  GUI_ButtonSmall* bChange;
  GUI_ButtonSmall* bCancel;

  void Default();
  void Return();

protected:

  bool eventButtonClick(int id, PG_Widget* widget);
  void eventShow();
  void eventHide();
};

#endif	// GUIPATHSMENU_H
