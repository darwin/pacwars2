#include "guivideomenu.h"
#include "gui.h"
#include "pw2.h"
#include "SDL_Console.h"

GUI_VideoMenu::GUI_VideoMenu() : GUI_BaseMenu(GUI_VIDEO, PG_Rect(VM_PX,VM_PY,VM_VX,VM_VY)) {

	lEffect = new GUI_Label(this, PG_Rect(1,30,VM_VX-2,20), "[r] marked changes will take effect after restarting program.", false);
	VideoMenu = new GUI_Label(this, PG_Rect(1,4,VM_VX-2,25), "VIDEO OPTIONS", false);
	cFullscreen = new GUI_CheckBox(this, PG_Rect(25,60,310,CB_SIZEY), "run game in fullscreen mode [r]", true, GUI_Gray64);
	cDB = new GUI_CheckBox(this, PG_Rect(25,85,310,CB_SIZEY), "double buffering mode [r]", true, GUI_Gray64);
	cGLBlit = new GUI_CheckBox(this, PG_Rect(25,110,310,CB_SIZEY), "use OpenGL for blitting [r]", true, GUI_Gray64);
	cAlphaMenu = new GUI_CheckBox(this, PG_Rect(25,135,310,CB_SIZEY), "display menu with alpha (slow!)", true, GUI_Gray64);
	lGamma_r = new GUI_Label(this, PG_Rect(1,160, 119,20), "red gamma:", false);
	eGamma_r = new GUI_FloatEdit(this, PG_Rect(120,160,150,20), 0, 1);
	lGamma_g = new GUI_Label(this, PG_Rect(1,185, 119,20), "green gamma:", false);
	eGamma_g = new GUI_FloatEdit(this, PG_Rect(120,185,150,20), 0, 1),
	lGamma_b = new GUI_Label(this, PG_Rect(1,210, 119,20), "blue gamma:", false);
	eGamma_b = new GUI_FloatEdit(this, PG_Rect(120,210,150,20), 0, 1);
	bSet = new GUI_ButtonSmall(this, 1, PG_Rect(25,240,150, 25), "APPLY");
	bCancel = new GUI_ButtonSmall(this, 2, PG_Rect(25+150+10,240,150,25), "BACK");

	Default();

	VideoMenu->SetFont(MainFont);

	lGamma_r->SetAlignment(PG_TA_RIGHT);
	lGamma_g->SetAlignment(PG_TA_RIGHT);
	lGamma_b->SetAlignment(PG_TA_RIGHT);

	LoadThemeStyle("GUI_Board");
}

void GUI_VideoMenu::Return() {
	eGamma_r->EditEnd();
	eGamma_g->EditEnd();
	eGamma_b->EditEnd();
	GUI_BaseMenu::Return();
}

void GUI_VideoMenu::Default() {

	if (fullscreen.value) cFullscreen->SetPressed(); else cFullscreen->SetUnpressed();
	if (glblit.value) cGLBlit->SetPressed(); else cGLBlit->SetUnpressed();
	if (alphamenu.value) cAlphaMenu->SetPressed(); else cAlphaMenu->SetUnpressed();
	if (doublebuf.value) cDB->SetPressed(); else cDB->SetUnpressed();

	eGamma_r->SetTextFormat("%.3g", pw2_gamma_r.value);
	eGamma_g->SetTextFormat("%.3g", pw2_gamma_g.value);
	eGamma_b->SetTextFormat("%.3g", pw2_gamma_b.value);
}

bool GUI_VideoMenu::eventButtonClick(int id, PG_Widget* widget) {

	switch (id) {
	case 1:
		ConOut("");
		ConOutEx(MISC_FONT, "> Video menu sequence <");

		CommandExecuteOut("gamma_r %s", eGamma_r->GetText());
		CommandExecuteOut("gamma_g %s", eGamma_g->GetText());
		CommandExecuteOut("gamma_b %s", eGamma_b->GetText());

		if (cFullscreen->GetPressed()) CommandExecuteOut("fullscreen 1");	else CommandExecuteOut("fullscreen 0");
		if (cGLBlit->GetPressed()) CommandExecuteOut("glblit 1");	else CommandExecuteOut("glblit 0");
		if (cAlphaMenu->GetPressed()) CommandExecuteOut("alphamenu 1");	else CommandExecuteOut("alphamenu 0");
		if (cDB->GetPressed()) CommandExecuteOut("doublebuf 1"); else CommandExecuteOut("doublebuf 0");

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

void GUI_VideoMenu::eventShow() {
  Clear();
}

void GUI_VideoMenu::eventHide() {
}
