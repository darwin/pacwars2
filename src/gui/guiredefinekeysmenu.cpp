#include "guiredefinekeysmenu.h"
#include "gui.h"
#include "client.h"
#include "pw2.h"

GUI_RedefineKeysMenu::GUI_RedefineKeysMenu() : GUI_BaseMenu(GUI_REDEFINEKEYS, PG_Rect(RK_PX,RK_PY,RK_VX,RK_VY)) {

	RedefineKeysMenu = new PG_Label(this, PG_Rect(1,4,RK_VX-2,25), "CREATE PLAYER", "GUI_Label");
	lLine1 = new PG_Label(this, PG_Rect(1,30,RK_VX-2,20), "Redefine game keys.", "GUI_Label");
	lConsole = new PG_Label(this, PG_Rect(RK_INPUT_X1    ,RK_INPUT_Y1    , RKSX,20), "Console:", "GUI_Label");
	iConsole = new GUI_Input(this, PG_Rect(RK_INPUT_X1+RKSX,RK_INPUT_Y1    , 100 ,20),1);
	lMenu = new PG_Label(this, PG_Rect(RK_INPUT_X1    ,RK_INPUT_Y1+20 , RKSX,20), "Main menu:", "GUI_Label");
	iMenu = new GUI_Input(this, PG_Rect(RK_INPUT_X1+RKSX,RK_INPUT_Y1+20 , 100 ,20),2);
	lMessage = new PG_Label(this, PG_Rect(RK_INPUT_X1    ,RK_INPUT_Y1+40 , RKSX,20), "Chat message:", "GUI_Label");
	iMessage = new GUI_Input(this, PG_Rect(RK_INPUT_X1+RKSX,RK_INPUT_Y1+40 , 100 ,20),3);
	lQuit = new PG_Label(this, PG_Rect(RK_INPUT_X1    ,RK_INPUT_Y1+60 , RKSX,20), "Fast quit:", "GUI_Label");
	iQuit = new GUI_Input(this, PG_Rect(RK_INPUT_X1+RKSX,RK_INPUT_Y1+60 , 100 ,20),4);
	lNetstat = new PG_Label(this, PG_Rect(RK_INPUT_X1    ,RK_INPUT_Y1+80 , RKSX,20), "Network stats:", "GUI_Label");
	iNetstat = new GUI_Input(this, PG_Rect(RK_INPUT_X1+RKSX,RK_INPUT_Y1+80 , 100 ,20),5);
	lSview = new PG_Label(this, PG_Rect(RK_INPUT_X1    ,RK_INPUT_Y1+100, RKSX,20), "Server view:", "GUI_Label");
	iSview = new GUI_Input(this, PG_Rect(RK_INPUT_X1+RKSX,RK_INPUT_Y1+100, 100 ,20),6);
	lInfo = new PG_Label(this, PG_Rect(RK_INPUT_X1    ,RK_INPUT_Y1+120, RKSX,20), "Debug info:", "GUI_Label");
	iInfo = new GUI_Input(this, PG_Rect(RK_INPUT_X1+RKSX,RK_INPUT_Y1+120, 100 ,20),7);
	bOK = new GUI_ButtonSmall(this, 1, PG_Rect(25,220,150,25), "OK");
	bCancel = new GUI_ButtonSmall(this, 2, PG_Rect(25+150+10,220,150,25), "BACK");

	Default();

	RedefineKeysMenu->SetFont(MainFont);
	RedefineKeysMenu->SetText("REDEFINE KEYS");

	lConsole->SetAlignment(PG_TA_RIGHT);
	lMenu->SetAlignment(PG_TA_RIGHT);
	lMessage->SetAlignment(PG_TA_RIGHT);
	lQuit->SetAlignment(PG_TA_RIGHT);
	lNetstat->SetAlignment(PG_TA_RIGHT);
	lSview->SetAlignment(PG_TA_RIGHT);
	lInfo->SetAlignment(PG_TA_RIGHT);

	LoadThemeStyle("GUI_Board");
}

void GUI_RedefineKeysMenu::Return() {

	if (iConsole->waiting || iMenu->waiting || iMessage->waiting || iQuit->waiting ||
			iInfo->waiting || iSview->waiting || iNetstat->waiting) {
		
		iConsole->InputEnd();
		iMenu->InputEnd();
		iMessage->InputEnd();
		iQuit->InputEnd();
		iNetstat->InputEnd();
		iSview->InputEnd();
		iInfo->InputEnd();
		return;
	}

	GUI_BaseMenu::Return();
}

void GUI_RedefineKeysMenu::SetKBDefaults(
								cvar_t* p_key_Console,
								cvar_t* p_key_Menu,
								cvar_t* p_key_Message,
								cvar_t* p_key_Quit,
								cvar_t* p_key_Netstat,
								cvar_t* p_key_Info,
								cvar_t* p_key_Sview
								)
{
  if (!p_key_Console->string[0]) Cvar_SetValue(p_key_Console->name, STD_KEY_CONSOLE);
  if (!p_key_Message->string[0]) Cvar_SetValue(p_key_Message->name, STD_KEY_CHAT);
  if (!p_key_Menu->string[0]) Cvar_SetValue(p_key_Menu->name, STD_KEY_MENU);
  if (!p_key_Quit->string[0]) Cvar_SetValue(p_key_Quit->name, STD_KEY_FASTQUIT);
  if (!p_key_Netstat->string[0]) Cvar_SetValue(p_key_Netstat->name, STD_KEY_NETSTAT);
  if (!p_key_Info->string[0]) Cvar_SetValue(p_key_Info->name, STD_KEY_INFO);
  if (!p_key_Sview->string[0]) Cvar_SetValue(p_key_Sview->name, STD_KEY_SERVERVIEW);

	iConsole->SetText(SDL_GetKeyName((SDLKey)(Uint16)p_key_Console->value));
	iMenu->SetText(SDL_GetKeyName((SDLKey)(Uint16)p_key_Menu->value));
	iMessage->SetText(SDL_GetKeyName((SDLKey)(Uint16)p_key_Message->value));
	iQuit->SetText(SDL_GetKeyName((SDLKey)(Uint16)p_key_Quit->value));
	iNetstat->SetText(SDL_GetKeyName((SDLKey)(Uint16)p_key_Netstat->value));
	iInfo->SetText(SDL_GetKeyName((SDLKey)(Uint16)p_key_Info->value));
	iSview->SetText(SDL_GetKeyName((SDLKey)(Uint16)p_key_Sview->value));

	iConsole->SetSym((Uint16)p_key_Console->value);
	iMenu->SetSym((Uint16)p_key_Menu->value);
	iMessage->SetSym((Uint16)p_key_Message->value);
	iQuit->SetSym((Uint16)p_key_Quit->value);
	iNetstat->SetSym((Uint16)p_key_Netstat->value);
	iInfo->SetSym((Uint16)p_key_Info->value);
	iSview->SetSym((Uint16)p_key_Sview->value);
}

void GUI_RedefineKeysMenu::Default() {
  
		SetKBDefaults(
      &key_console,
      &key_menu,
      &key_chat,
      &key_quit,
      &key_netstat,
      &key_info,
      &key_sview
      );
}

bool GUI_RedefineKeysMenu::eventButtonClick(int id, PG_Widget* widget) {
	CGame& g = client_info.game;

	switch (id) {
	case 1:

    ConOut("");
    ConOutEx(MISC_FONT, "> Keyboard menu sequence <");
    
    CommandExecuteOut("key_console %d", iConsole->GetSym());
    CommandExecuteOut("key_menu %d", iMenu->GetSym());
    CommandExecuteOut("key_chat %d", iMessage->GetSym());
    CommandExecuteOut("key_quit %d", iQuit->GetSym());
    CommandExecuteOut("key_netstat %d", iNetstat->GetSym());
    CommandExecuteOut("key_info %d", iInfo->GetSym());
    CommandExecuteOut("key_sview %d", iSview->GetSym());
	
		ConOutEx(MISC_FONT, "> end of sequence <");
		ConOut("");

		while (GUI_id) GUI_Return(); // return from all menus
		break;
		
	case 2:
		Return();
		break;
	}
	
	return true;
}

bool GUI_RedefineKeysMenu::eventNextInput(int id, PG_Widget* widget)
{
	switch (id) {
	case 1:
		iMenu->InputBegin();
		break;
	case 2:
		iMessage->InputBegin();
		break;
	case 3:
		iQuit->InputBegin();
		break;
	case 4:
		iNetstat->InputBegin();
		break;
	case 5:
		iInfo->InputBegin();
		break;
	case 6:
		iSview->InputBegin();
		break;
	}
	
	return true;
}



bool GUI_RedefineKeysMenu::eventMessage(MSG_MESSAGE* msg){
	bool rc = false;

	if((msg->to != this) && (msg->to != NULL)){
		return false;
	}

	// dispatch user message
	switch(msg->type) {
		case MSG_BUTTONCLICK:
			rc = eventButtonClick(msg->widget_id, (PG_Widget*)(msg->from));
			break;

		case MSG_SIGNAL:
			rc = eventNextInput(msg->widget_id, (PG_Widget*)(msg->from));
			break;
		
		default:
			rc = false;
			break;
	}

	return rc;
}


void GUI_RedefineKeysMenu::eventShow() {
  Clear();
}

void GUI_RedefineKeysMenu::eventHide() {
}

