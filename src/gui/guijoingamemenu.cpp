#include "guijoingamemenu.h"
#include "gui.h"
#include "pw2.h"
#include "client.h"

// bah,.. an ugly global instance
GUI_JoinGameMenu* JGMenu;


GUI_JoinGameMenu::GUI_JoinGameMenu() : GUI_BaseMenu(GUI_JOINGAME, PG_Rect(JG_PX,JG_PY,JG_VX,JG_VY)) {
	JGMenu = this;

	JoinGameMenu = new PG_Label(this, PG_Rect(1,4,JG_VX-2,25), "JOIN GAME", "GUI_Label");
	lServerAddress = new PG_Label(this, PG_Rect(1,50, 119,20), "Server address:", "GUI_Label");
	eServerAddress = new GUI_TextEdit(this, PG_Rect(120,50,215,20));
	lClientName = new PG_Label(this, PG_Rect(1,80, 119,20), "client name:", "GUI_Label");
	eClientName = new GUI_TextEdit(this, PG_Rect(120,80,215,20));
	lClientDesc = new PG_Label(this, PG_Rect(1,110, 119,20), "client finger:", "GUI_Label");
	eClientDesc = new GUI_TextEdit(this, PG_Rect(120,110,215,20));
	bConnect = new GUI_ButtonSmall(this, 1, PG_Rect(25,140,150,25), "CONNECT");
	bCancel = new GUI_ButtonSmall(this, 2, PG_Rect(25+150+10,140,150,25), "BACK");

	Default();

	JoinGameMenu->SetFont(MainFont);

	lServerAddress->SetAlignment(PG_TA_RIGHT);
	lClientName->SetAlignment(PG_TA_RIGHT);
	lClientDesc->SetAlignment(PG_TA_RIGHT);

	LoadThemeStyle("GUI_Board");
}

void GUI_JoinGameMenu::Return() {
	eServerAddress->EditEnd();
	eClientName->EditEnd();
	eClientDesc->EditEnd();
	GUI_BaseMenu::Return();
}

void GUI_JoinGameMenu::Default() {
	eServerAddress->SetText(last_server.string);
	eClientName->SetText(c_name.string);
	eClientDesc->SetText(c_desc.string);
}

bool GUI_JoinGameMenu::eventButtonClick(int id, PG_Widget* widget) {

	switch (id) {

	case 1:
		ConOut("");
		ConOutEx(MISC_FONT, "> JoinGame menu sequence <");
		
		if (net_client_status) {
			CommandExecuteOut("ec");
		}

		CommandExecuteOut("c_name %s", eClientName->GetText());
		CommandExecuteOut("c_desc %s", eClientDesc->GetText());
		CommandExecuteOut("last_server %s", eServerAddress->GetText());

		CommandExecuteOut("sc");
		CommandExecuteOut("connect %s", eServerAddress->GetText());
		ConOutEx(MISC_FONT, "> end of sequence <");
		ConOut("");

		while (GUI_id) {
			GUI_Return(); // return from all menus
		}
		break;
		
	case 2:
		Return();
		break;
	}
	
	return true;
}

void GUI_JoinGameMenu::eventShow() {
}

void GUI_JoinGameMenu::eventHide() {
}
