#ifndef GUINEWGAMEMENU_H
#define GUINEWGAMEMENU_H

#include "guibasemenu.h"
#include "pgwidgetlist.h"
#include "pglabel.h"

class GUI_Board;
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

  PG_Label* NewGameMenu;
  PG_Label* lServerName;
  GUI_TextEdit* eServerName;
  PG_Label* lWelcomeMsg;
  GUI_TextEdit* eWelcomeMsg;
  PG_Label* lMaxClients;
  GUI_NumEdit* eMaxClients;
  PG_Label* lIP;
  PG_Label* lHost;
  GUI_ButtonSmall* bStartIt;
  GUI_ButtonSmall* bCancel;

  PG_Label* lChoose1;
  PG_Label* lChoose2;

  GUI_Board* Board3;

  PG_Label* lAuthFile;
  PG_Label* lDesc1;
  PG_Label* lDesc2;
  PG_Label* lDesc3;

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
