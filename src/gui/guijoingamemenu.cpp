#include "guijoingamemenu.h"
#include "gui.h"
#include "pw2.h"
#include "client.h"

// bah,.. an ugly global instance
GUI_JoinGameMenu* JGMenu;


GUI_JoinGameMenu::GUI_JoinGameMenu() : GUI_BaseMenu(GUI_JOINGAME, mkrect(JG_PX,JG_PY,JG_VX,JG_VY)) {
	JGMenu = this;

	JoinGameMenu = new GUI_Label(this, SDLWidget::mkrect(1,4,JG_VX-2,25), "JOIN GAME", false);
	lServerAddress = new GUI_Label(this, SDLWidget::mkrect(1,50, 119,20), "Server address:", false);
	eServerAddress = new GUI_TextEdit(this, SDLWidget::mkrect(120,50,215,20));
	lClientName = new GUI_Label(this, SDLWidget::mkrect(1,80, 119,20), "client name:", false);
	eClientName = new GUI_TextEdit(this, SDLWidget::mkrect(120,80,215,20));
	lClientDesc = new GUI_Label(this, SDLWidget::mkrect(1,110, 119,20), "client finger:", false);
	eClientDesc = new GUI_TextEdit(this, SDLWidget::mkrect(120,110,215,20));
	bConnect = new GUI_ButtonSmall(this, 1, SDLWidget::mkrect(25,140,150,25), "CONNECT");
	bCancel = new GUI_ButtonSmall(this, 2, SDLWidget::mkrect(25+150+10,140,150,25), "BACK");

	Default();

	JoinGameMenu->SetColor(GUI_BtnTextColor, GUI_BtnATextColor);
	JoinGameMenu->SetFont(MainFont);

	lServerAddress->SetAlignment(SDL_TA_RIGHT);
	lClientName->SetAlignment(SDL_TA_RIGHT);
	lClientDesc->SetAlignment(SDL_TA_RIGHT);

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

bool GUI_JoinGameMenu::eventButtonClick(int id, SDLWidget* widget) {

	switch (id) {

	case 1:
		ConOut("");
		ConOut("> JoinGame menu sequence <");
		
		if (net_client_status) {
			ConOut("ec");
			CommandExecute("ec");
		}

		ConOut("c_name %s", eClientName->GetText());
		CommandExecute("c_name %s", eClientName->GetText());
		ConOut("c_desc %s", eClientDesc->GetText());
		CommandExecute("c_desc %s", eClientDesc->GetText());
		ConOut("last_server %s", eServerAddress->GetText());
		CommandExecute("last_server %s", eServerAddress->GetText());

		ConOut("sc");
		CommandExecute("sc");
		ConOut("connect %s", eServerAddress->GetText());
		CommandExecute("connect %s", eServerAddress->GetText());
		ConOut("> end of sequence <");
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
