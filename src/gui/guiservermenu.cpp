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
	cRemote = new GUI_CheckBox(this, SDLWidget::mkrect(25,240,310,CB_SIZEY), "remote console", false, GUI_Gray64);
	lPassword = new GUI_Label(this, SDLWidget::mkrect(1,265, 119,20), "Remote password:", false);
	ePassword = new GUI_TextEdit(this, SDLWidget::mkrect(120,265,215,20));
	bSet = new GUI_ButtonSmall(this, 1, SDLWidget::mkrect(25,290,150, 25), "APPLY");
	bCancel = new GUI_ButtonSmall(this, 2, SDLWidget::mkrect(25+150+10,290,150,25), "BACK");

	Default();

	ServerMenu->SetFont(MainFont);

	lTimelimit->SetAlignment(SDL_TA_RIGHT);
	lWelcomeMsg->SetAlignment(SDL_TA_RIGHT);
	lMaxClients->SetAlignment(SDL_TA_RIGHT);
	lServerName->SetAlignment(SDL_TA_RIGHT);
	lPassword->SetAlignment(SDL_TA_RIGHT);

	LoadThemeStyle("GUI_Board");
}

void GUI_ServerMenu::Return() {
	eTimelimit->EditEnd();
	eServerName->EditEnd();
	ePassword->EditEnd();
	eWelcomeMsg->EditEnd();
	eMaxClients->EditEnd();
	GUI_BaseMenu::Return();
}

void GUI_ServerMenu::Default()
{
	eTimelimit->SetTextFormat("%i", (int)s_timelimit.value);

	eServerName->SetText(s_name.string);
	ePassword->SetText(s_acpass.string);
	eMaxClients->SetText(s_maxclients.string);
	eWelcomeMsg->SetText(s_welcome_msg.string);

	if (s_deletetmps.value) cDelTmps->SetPressed(); else cDelTmps->SetUnpressed();
	if (s_downloading.value) cRDRqs->SetPressed(); else cRDRqs->SetUnpressed();
	if (s_uploading.value) cRURqs->SetPressed(); else cRURqs->SetUnpressed();
	if (s_remote.value) cRemote->SetPressed(); else cRemote->SetUnpressed();
}

bool GUI_ServerMenu::eventButtonClick(int id, SDLWidget* widget)
{
	switch (id) {
	case 1:
		ConOut("");
		ConOutEx(MISC_FONT, "> Server menu sequence <");

		CommandExecuteOut("s_name %s", eServerName->GetText());
		CommandExecuteOut("s_welcome_msg %s", eWelcomeMsg->GetText());
		CommandExecuteOut("s_maxclients %s", eMaxClients->GetText());
		CommandExecuteOut("s_timelimit %s", eTimelimit->GetText());
		CommandExecuteOut("s_acpass %s", ePassword->GetText());

		if (cDelTmps->GetPressed()) CommandExecuteOut("s_deltmps 1");	else CommandExecuteOut("s_deltmps 0");
		if (cRDRqs->GetPressed()) CommandExecuteOut("s_downloading 1");	else CommandExecuteOut("s_downloading 0");
		if (cRURqs->GetPressed()) CommandExecuteOut("s_uploading 1");	else CommandExecuteOut("s_uploading 0");
		if (cRemote->GetPressed()) CommandExecuteOut("s_remote 1");	else CommandExecuteOut("s_remote 0");

    ConOutEx(MISC_FONT, "> end of sequence <");
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

