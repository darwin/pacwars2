#ifndef GUIPATHSMENU_H
#define GUIPATHSMENU_H

#include "guibasemenu.h"
#include "pglabel.h"

class GUI_TextEdit;
class GUI_ButtonSmall;


class GUI_PathsMenu : public GUI_BaseMenu {
public:
  GUI_PathsMenu();

  PG_Label* lPathsMenu;
  PG_Label* lEffect;

  PG_Label* lScriptDir;
  GUI_TextEdit* eScriptDir;
  PG_Label* lSpriteDir;
  GUI_TextEdit* eSpriteDir;
  PG_Label* lMapDir;
  GUI_TextEdit* eMapDir;
  PG_Label* lSndDir;
  GUI_TextEdit* eSndDir;
  PG_Label* lSkinDir;
  GUI_TextEdit* eSkinDir;
  PG_Label* lGuiDir;
  GUI_TextEdit* eGuiDir;
  PG_Label* lGfxDir;
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
