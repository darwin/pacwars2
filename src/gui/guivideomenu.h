#ifndef GUIVIDEOMENU_H
#define GUIVIDEOMENU_H

#include "guibasemenu.h"
#include "pglabel.h"

class GUI_CheckBox;
class GUI_ButtonSmall;
class GUI_FloatEdit;

class GUI_VideoMenu : public GUI_BaseMenu {
public:
  GUI_VideoMenu();

  PG_Label* VideoMenu;
  PG_Label* lEffect;

  GUI_CheckBox* cFullscreen;
  GUI_CheckBox* cDB;
  GUI_CheckBox* cGLBlit;
  GUI_CheckBox* cAlphaMenu;

  PG_Label* lGamma_r;
  GUI_FloatEdit* eGamma_r;
  PG_Label* lGamma_g;
  GUI_FloatEdit* eGamma_g;
  PG_Label* lGamma_b;
  GUI_FloatEdit* eGamma_b;

  GUI_ButtonSmall* bSet;
  GUI_ButtonSmall* bCancel;

  void Default();
  void Return();

protected:

  bool eventButtonClick(int id, PG_Widget* widget);
  void eventShow();
  void eventHide();
};

#endif	// GUIVIDEOMENU_H
