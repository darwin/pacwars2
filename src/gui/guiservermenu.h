#ifndef GUISERVERMENU_H
#define GUISERVERMENU_H

#include "guibasemenu.h"

class GUI_Label;
class GUI_TextEdit;
class GUI_NumEdit;
class GUI_CheckBox;
class GUI_ButtonSmall;


class GUI_ServerMenu : public GUI_BaseMenu {
public:
  GUI_ServerMenu();

  GUI_Label* ServerMenu;

  GUI_Label* lServerName;
  GUI_TextEdit* eServerName;
  GUI_Label* lWelcomeMsg;
  GUI_TextEdit* eWelcomeMsg;
  GUI_Label* lMaxClients;
  GUI_NumEdit* eMaxClients;
  GUI_Label* lTimelimit;
  GUI_NumEdit* eTimelimit;
  GUI_Label* lPassword;
  GUI_TextEdit* ePassword;

  GUI_CheckBox* cDelTmps;
  GUI_CheckBox* cRDRqs;
  GUI_CheckBox* cRURqs;
  GUI_CheckBox* cRemote;

  GUI_ButtonSmall* bSet;
  GUI_ButtonSmall* bCancel;

  GUI_Label* lEffect;

  void Default();
  void Return();

protected:

  bool eventButtonClick(int id, PG_Widget* widget);
  void eventShow();
  void eventHide();
};

#endif	// GUISERVERMENU_H
