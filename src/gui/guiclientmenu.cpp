#include "guiclientmenu.h"
#include "gui.h"
#include "client.h"

GUI_ClientMenu::GUI_ClientMenu() : GUI_BaseMenu(GUI_CLIENT, PG_Rect(CMS_PX,CMS_PY,CMS_VX,CMS_VY)) {

	ClientMenu = new PG_Label(this, PG_Rect(1,4,CMS_VX-2,25), "CLIENT OPTIONS", "GUI_Label");
	lEffect = new PG_Label(this, PG_Rect(1,30,CMS_VX-2,20), "This settings will affect the client side of the game.", "GUI_Label");
	lClientName = new PG_Label(this, PG_Rect(1,65, 119,20), "Station name:", "GUI_Label");
	eClientName = new GUI_TextEdit(this, PG_Rect(120,65,215,20));
	lClientDesc = new PG_Label(this, PG_Rect(1,90, 119,20), "Station finger:", "GUI_Label");
	eClientDesc  = new GUI_TextEdit(this, PG_Rect(120,90,215,20));
	cDelTmps = new GUI_CheckBox(this, PG_Rect(25,115,310,CB_SIZEY), "delete temporary files after transmission", false, GUI_Gray64);
	cDRqs = new GUI_CheckBox(this, PG_Rect(25,140,310,CB_SIZEY), "allow download requests", false, GUI_Gray64);
	cURqs = new GUI_CheckBox(this, PG_Rect(25,165,310,CB_SIZEY), "allow upload requests", false, GUI_Gray64);
	bSet = new GUI_ButtonSmall(this, 1, PG_Rect(25,190,150, 25), "APPLY");
	bCancel = new GUI_ButtonSmall(this, 2, PG_Rect(25+150+10,190,150,25), "BACK");

	Default();

	ClientMenu->SetFont(MainFont);

	lClientName->SetAlignment(PG_TA_RIGHT);
	lClientDesc->SetAlignment(PG_TA_RIGHT);

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

bool GUI_ClientMenu::eventButtonClick(int id, PG_Widget* widget) {

	switch (id) {
	case 1:
		ConOut("");
		ConOutEx(MISC_FONT, "> Client menu sequence <");

		CommandExecuteOut("c_name %s", eClientName->GetText());
		CommandExecuteOut("c_desc %s", eClientDesc->GetText());

		if (cDelTmps->GetPressed()) CommandExecuteOut("c_deltmps 1");	else CommandExecuteOut("c_deltmps 0");
		if (cDRqs->GetPressed()) CommandExecuteOut("c_downloading 1"); else CommandExecuteOut("c_downloading 0");
		if (cURqs->GetPressed()) CommandExecuteOut("c_uploading 1"); else CommandExecuteOut("c_uploading 0");

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

void GUI_ClientMenu::eventShow() {
  Clear();
}

void GUI_ClientMenu::eventHide() {
}
