#include "guiclientmenu.h"
#include "gui.h"
#include "client.h"

GUI_ClientMenu::GUI_ClientMenu() : GUI_BaseMenu(GUI_CLIENT, mkrect(CMS_PX,CMS_PY,CMS_VX,CMS_VY)) {

	ClientMenu = new GUI_Label(this, SDLWidget::mkrect(1,4,CMS_VX-2,25), "CLIENT OPTIONS", false);
	lEffect = new GUI_Label(this, SDLWidget::mkrect(1,30,CMS_VX-2,20), "This settings will affect the client side of the game.", false);
	lClientName = new GUI_Label(this, SDLWidget::mkrect(1,65, 119,20), "Station name:", false);
	eClientName = new GUI_TextEdit(this, SDLWidget::mkrect(120,65,215,20));
	lClientDesc = new GUI_Label(this, SDLWidget::mkrect(1,90, 119,20), "Station finger:", false);
	eClientDesc  = new GUI_TextEdit(this, SDLWidget::mkrect(120,90,215,20));
	cDelTmps = new GUI_CheckBox(this, SDLWidget::mkrect(25,115,310,CB_SIZEY), "delete temporary files after transmission", false, GUI_Gray64);
	cDRqs = new GUI_CheckBox(this, SDLWidget::mkrect(25,140,310,CB_SIZEY), "allow download requests", false, GUI_Gray64);
	cURqs = new GUI_CheckBox(this, SDLWidget::mkrect(25,165,310,CB_SIZEY), "allow upload requests", false, GUI_Gray64);
	bSet = new GUI_ButtonSmall(this, 1, SDLWidget::mkrect(25,190,150, 25), "APPLY");
	bCancel = new GUI_ButtonSmall(this, 2, SDLWidget::mkrect(25+150+10,190,150,25), "BACK");

	Default();

	ClientMenu->SetColor(GUI_BtnTextColor, GUI_BtnATextColor);
	ClientMenu->SetFont(MainFont);

	lClientName->SetAlignment(SDL_TA_RIGHT);
	lClientDesc->SetAlignment(SDL_TA_RIGHT);

	LoadThemeStyle("GUI_Board");
}

void GUI_ClientMenu::Return() {
	eClientName->EditEnd();
	eClientDesc->EditEnd();
	GUI_BaseMenu::Return();
}


void GUI_ClientMenu::Default() {
	eClientName->SetText(c_name.string);
	eClientDesc->SetText(c_desc.string);

	if (c_deletetmps.value) cDelTmps->SetPressed(); else cDelTmps->SetUnpressed();
	if (c_downloading.value) cDRqs->SetPressed(); else cDRqs->SetUnpressed();
	if (c_uploading.value) cURqs->SetPressed(); else cURqs->SetUnpressed();
}

bool GUI_ClientMenu::eventButtonClick(int id, SDLWidget* widget) {

	switch (id) {
	case 1:
		ConOut("");
		ConOut("> Client menu sequence <");

		ConOut("c_name %s", eClientName->GetText());
		CommandExecute("c_name %s", eClientName->GetText());
		ConOut("c_desc %s", eClientDesc->GetText());
		CommandExecute("c_desc %s", eClientDesc->GetText());

		if (cDelTmps->GetPressed()) {
			ConOut("c_deltmps 1");
			CommandExecute("c_deltmps 1");
		}
		else {
			ConOut("c_deltmps 0");
			CommandExecute("c_deltmps 0");
		}

		if (cDRqs->GetPressed()) {
			ConOut("c_downloading 1");
			CommandExecute("c_downloading 1");
		}
		else {
			ConOut("c_downloading 0");
			CommandExecute("c_downloading 0");
		}

		if (cURqs->GetPressed()) {
			ConOut("c_uploading 1");
			CommandExecute("c_uploading 1");
		}
		else {
			ConOut("c_uploading 0");
			CommandExecute("c_uploading 0");
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

void GUI_ClientMenu::eventShow() {
  Clear();
}

void GUI_ClientMenu::eventHide() {
}
