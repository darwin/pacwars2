#ifndef GUIDELETEPLAYERMENU_H
#define GUIDELETEPLAYERMENU_H

#include "guibasemenu.h"
#include "cvar.h"
#include "pgwidgetlist.h"

class GUI_Label;
class GUI_LabelL;
class GUI_ButtonSmall;

class GUI_DeletePlayerMenu : public GUI_BaseMenu {
public:
  GUI_DeletePlayerMenu();
  
  GUI_Label* DeletePlayerMenu;
  GUI_ButtonSmall* bDelete;
  GUI_ButtonSmall* bCancel;
  GUI_Label* lChoose;
  PG_WidgetList* WidgetList;

  GUI_LabelL* selected1; // pointer to actualy selected item
  
  void Default();
  void Return();

protected:  
  bool eventButtonClick(int id, PG_Widget* widget);
  void eventShow();
  void eventHide();
};

#endif	// GUIDELETEPLAYERMENU_H
