#include "guiservermenu.h"
#include "gui.h"
#include "protocol.h"
#include "server.h"

/////////////////////////////////////////////////////////////////////////////
// Server menu widgets
/////////////////////////////////////////////////////////////////////////////

GUI_ServerMenu::GUI_ServerMenu() : GUI_BaseMenu(GUI_SERVER, mkrect(SMS_PX,SMS_PY,SMS_VX,SMS_VY)) {

	lEffect = new GUI_Label(this, SDLWidget::mkrect(1,30,VM_VX-2,20), "This settings will affect the server side of the game.", false);
	ServerMenu = new GUI_Label(this, SDLWidget::mkrect(1,4,SMS_VX-2,25), "SERVER OPTIONS", false);
	lServerName = new GUI_Label(this, SDLWidget::mkrect(1,65, 119,20), "Server name:", false);
	eServerName = new GUI_TextEdit(this, SDLWidget::mkrect(120,65,215,20));
	lWelcomeMsg = new GUI_Label(this, SDLWidget::mkrect(1,90,119,20), "Welcome msg:", false);
	eWelcomeMsg = new GUI_TextEdit(this, SDLWidget::mkrect(120,90,215,20));
	lMaxClients = new GUI_Label(this, SDLWidget::mkrect(1,115,119,20), "Max clients:", false);
	eMaxClients = new GUI_NumEdit(this, SDLWidget::mkrect(120,115,80,20), 1, PWP_TOTALMAX_CLIENTS);
	lTimelimit = new GUI_Label(this, SDLWidget::mkrect(1,140, 119,20), "timelimit:", false);
	eTimelimit = new GUI_NumEdit(this, SDLWidget::mkrect(120,140,150,20), 20, 10000);
	cDelTmps = new GUI_CheckBox(this, SDLWidget::mkrect(25,165,310,CB_SIZEY), "delete temporary files after transmission", false, GUI_Gray64);
	cRDRqs = new GUI_CheckBox(this, SDLWidget::mkrect(25,190,310,CB_SIZEY), "receive download requests", false, GUI_Gray64);
	cRURqs = new GUI_CheckBox(this, SDLWidget::mkrect(25,215,310,CB_SIZEY), "receive upload requests", false, GUI_Gray64);
	bSet = new GUI_ButtonSmall(this, 1, SDLWidget::mkrect(25,240,150, 25), "APPLY");
	bCancel = new GUI_ButtonSmall(this, 2, SDLWidget::mkrect(25+150+10,240,150,25), "BACK");

	Default();

	ServerMenu->SetColor(GUI_BtnTextColor, GUI_BtnATextColor);
	ServerMenu->SetFont(MainFont);
	ServerMenu->bgmode = 2;

	lTimelimit->SetAlignment(SDL_TA_RIGHT);
	lTimelimit->shiftx = -4;
	lWelcomeMsg->SetAlignment(SDL_TA_RIGHT);
	lWelcomeMsg->shiftx = -4;
	lMaxClients->SetAlignment(SDL_TA_RIGHT);
	lMaxClients->shiftx = -4;
	lServerName->SetAlignment(SDL_TA_RIGHT);
	lServerName->shiftx = -4;

	LoadThemeStyle("GUI_Board");
}

void GUI_ServerMenu::Return() {
	eTimelimit->EditEnd();
	eServerName->EditEnd();
	eWelcomeMsg->EditEnd();
	eMaxClients->EditEnd();
	GUI_BaseMenu::Return();
}

void GUI_ServerMenu::Default()
{
	eTimelimit->SetTextFormat("%i", (int)s_timelimit.value);

	eServerName->SetText(s_name.string);
	eMaxClients->SetText(s_maxclients.string);
	eWelcomeMsg->SetText(s_welcome_msg.string);

	if (s_deletetmps.value) {
		cDelTmps->SetPressed();
	}
	else {
		cDelTmps->SetUnpressed();
	}

	if (s_downloading.value) {
		cRDRqs->SetPressed();
	}
	else {
		cRDRqs->SetUnpressed();
	}

	if (s_uploading.value) {
		cRURqs->SetPressed();
	}
	else {
		cRURqs->SetUnpressed();
	}

}

bool GUI_ServerMenu::eventButtonClick(int id, SDLWidget* widget)
{
	switch (id) {
	case 1:
		ConOut("");
		ConOut("> Server menu sequence <");

		ConOut("s_name %s", eServerName->GetText());
		CommandExecute("s_name %s", eServerName->GetText());
		ConOut("s_welcome_msg %s", eWelcomeMsg->GetText());
		CommandExecute("s_welcome_msg %s", eWelcomeMsg->GetText());
		ConOut("s_maxclients %s", eMaxClients->GetText());
		CommandExecute("s_maxclients %s", eMaxClients->GetText());
		ConOut("s_timelimit %s", eTimelimit->GetText());
		CommandExecute("s_timelimit %s", eTimelimit->GetText());

		if (cDelTmps->GetPressed()) {
			ConOut("s_deltmps 1");
			CommandExecute("s_deltmps 1");
		}
		else {
			ConOut("s_deltmps 0");
			CommandExecute("s_deltmps 0");
		}

		if (cRDRqs->GetPressed()) {
			ConOut("s_downloading 1");
			CommandExecute("s_downloading 1");
		}
		else {
			ConOut("s_downloading 0");
			CommandExecute("s_downloading 0");
		}

		if (cRURqs->GetPressed()) {
			ConOut("s_uploading 1");
			CommandExecute("s_uploading 1");
		}
		else {
			ConOut("s_uploading 0");
			CommandExecute("s_uploading 0");
		}

		ConOut("> end of sequence <");
		ConOut("");

		GUI_Return();
		break;

	case 2:
		Return();
		break;
	}

	return true;
}

void GUI_ServerMenu::eventShow() {
  Clear();
}

void GUI_ServerMenu::eventHide() {
}

