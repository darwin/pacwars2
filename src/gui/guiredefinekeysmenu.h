#ifndef GUIREDEFINEKEYSMENU_H
#define GUIREDEFINEKEYSMENU_H

#include "guibasemenu.h"
#include "pglabel.h"

#include "cvar.h"

class GUI_TextEdit;
class GUI_SkinCtrl;
class GUI_Input;
class GUI_ButtonSmall;
class CGame;

class GUI_RedefineKeysMenu : public GUI_BaseMenu {
public:
	GUI_RedefineKeysMenu();

	PG_Label* RedefineKeysMenu;
  PG_Label* lLine1;

	GUI_Input* iConsole;
	GUI_Input* iMenu;
	GUI_Input* iQuit;
	GUI_Input* iMessage;
	GUI_Input* iNetstat;
	GUI_Input* iSview;
	GUI_Input* iInfo;

	PG_Label* lConsole;
	PG_Label* lMenu;
	PG_Label* lMessage;
	PG_Label* lQuit;
	PG_Label* lNetstat;
	PG_Label* lSview;
	PG_Label* lInfo;

	GUI_ButtonSmall* bOK;
	GUI_ButtonSmall* bCancel;

	void Default();
	void Return();

	void NextBox();

protected:

	bool eventButtonClick(int id, PG_Widget* widget);
	bool eventNextInput(int id, PG_Widget* widget);
	bool eventMessage(MSG_MESSAGE* msg);
	void eventShow();
	void eventHide();

  void GUI_RedefineKeysMenu::SetKBDefaults(
								cvar_t* p_key_Console,
								cvar_t* p_key_Menu,
								cvar_t* p_key_Message,
								cvar_t* p_key_Quit,
								cvar_t* p_key_Netstat,
								cvar_t* p_key_Info,
								cvar_t* p_key_Sview
								);

};

#endif	// GUIREDEFINEKEYSMENU_H
