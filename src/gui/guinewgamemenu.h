#ifndef GUINEWGAMEMENU_H
#define GUINEWGAMEMENU_H

#include "guibasemenu.h"
#include "pgwidgetlist.h"

class GUI_Board;
class GUI_Label;
class GUI_NumEdit;
class GUI_CheckBox;
class GUI_ButtonSmall;
class GUI_TextEdit;
class GUI_LabelL;

/////////////////////////////////////////////////////////////////////////////
// NewGame menu widgets
/////////////////////////////////////////////////////////////////////////////

#define NG_PX 140
#define NG_PY 40
#define NG_VX 360
#define NG_VY 400


class GUI_NewGameMenu : public GUI_BaseMenu {
public:
  GUI_NewGameMenu();

  GUI_Label* NewGameMenu;
  GUI_Label* lServerName;
  GUI_TextEdit* eServerName;
  GUI_Label* lWelcomeMsg;
  GUI_TextEdit* eWelcomeMsg;
  GUI_Label* lMaxClients;
  GUI_NumEdit* eMaxClients;
  GUI_Label* lIP;
  GUI_Label* lHost;
  GUI_ButtonSmall* bStartIt;
  GUI_ButtonSmall* bCancel;

  GUI_Label* lChoose1;
  GUI_Label* lChoose2;

  GUI_Board* Board3;

  GUI_Label* lAuthFile;
  GUI_Label* lDesc1;
  GUI_Label* lDesc2;
  GUI_Label* lDesc3;

  GUI_CheckBox* cJoin;

  PG_WidgetList* WidgetList1;
  PG_WidgetList* WidgetList2;

  GUI_LabelL* selected1;
  GUI_LabelL* selected2;

  void eventShow();
  void eventHide();
  void Default();
  void Return();

  void GenerateScriptSelection();

  bool eventButtonClick(int id, PG_Widget* widget);
};

extern GUI_NewGameMenu* NGMenu;

#endif		// GUINEWGAMEMENU_H
