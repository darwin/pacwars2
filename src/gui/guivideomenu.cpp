#include "guivideomenu.h"
#include "gui.h"
#include "pw2.h"
#include "SDL_Console.h"

GUI_VideoMenu::GUI_VideoMenu() : GUI_BaseMenu(GUI_VIDEO, mkrect(VM_PX,VM_PY,VM_VX,VM_VY)) {

	lEffect = new GUI_Label(this, SDLWidget::mkrect(1,30,VM_VX-2,20), "[r] marked changes will take effect after restarting program.", false);
	VideoMenu = new GUI_Label(this, SDLWidget::mkrect(1,4,VM_VX-2,25), "VIDEO OPTIONS", false);
	cFullscreen = new GUI_CheckBox(this, SDLWidget::mkrect(25,60,310,CB_SIZEY), "run game in fullscreen mode [r]", true, GUI_Gray64);
	cDB = new GUI_CheckBox(this, SDLWidget::mkrect(25,85,310,CB_SIZEY), "double buffering mode [r]", true, GUI_Gray64);
	cGLBlit = new GUI_CheckBox(this, SDLWidget::mkrect(25,110,310,CB_SIZEY), "use OpenGL for blitting [r]", true, GUI_Gray64);
	cAlphaMenu = new GUI_CheckBox(this, SDLWidget::mkrect(25,135,310,CB_SIZEY), "display menu with alpha (slow!)", true, GUI_Gray64);
	lGamma_r = new GUI_Label(this, SDLWidget::mkrect(1,160, 119,20), "red gamma:", false);
	eGamma_r = new GUI_FloatEdit(this, SDLWidget::mkrect(120,160,150,20), 0, 1);
	lGamma_g = new GUI_Label(this, SDLWidget::mkrect(1,185, 119,20), "green gamma:", false);
	eGamma_g = new GUI_FloatEdit(this, SDLWidget::mkrect(120,185,150,20), 0, 1),
	lGamma_b = new GUI_Label(this, SDLWidget::mkrect(1,210, 119,20), "blue gamma:", false);
	eGamma_b = new GUI_FloatEdit(this, SDLWidget::mkrect(120,210,150,20), 0, 1);
	bSet = new GUI_ButtonSmall(this, 1, SDLWidget::mkrect(25,240,150, 25), "APPLY");
	bCancel = new GUI_ButtonSmall(this, 2, SDLWidget::mkrect(25+150+10,240,150,25), "BACK");

	Default();

	VideoMenu->SetColor(GUI_BtnTextColor, GUI_BtnATextColor);
	VideoMenu->SetFont(MainFont);
	VideoMenu->bgmode = 2;

	lGamma_r->SetAlignment(SDL_TA_RIGHT);
	lGamma_r->shiftx = -4;
	lGamma_g->SetAlignment(SDL_TA_RIGHT);
	lGamma_g->shiftx = -4;
	lGamma_b->SetAlignment(SDL_TA_RIGHT);
	lGamma_b->shiftx = -4;

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

	eGamma_r->SetTextFormat("%.3g", gamma_r.value);
	eGamma_g->SetTextFormat("%.3g", gamma_g.value);
	eGamma_b->SetTextFormat("%.3g", gamma_b.value);
}

bool GUI_VideoMenu::eventButtonClick(int id, SDLWidget* widget) {

	switch (id) {
	case 1:
		ConOut("");
		ConOut("> Video menu sequence <");

		ConOut("gamma_r %s", eGamma_r->GetText());
		CommandExecute("gamma_r %s", eGamma_r->GetText());
		ConOut("gamma_g %s", eGamma_g->GetText());
		CommandExecute("gamma_g %s", eGamma_g->GetText());
		ConOut("gamma_b %s", eGamma_b->GetText());
		CommandExecute("gamma_b %s", eGamma_b->GetText());

		if (cFullscreen->GetPressed()) {
			ConOut("fullscreen 1");
			CommandExecute("fullscreen 1");
		}
		else {
			ConOut("fullscreen 0");
			CommandExecute("fullscreen 0");
		}
		
		if (cGLBlit->GetPressed()) {
			ConOut("glblit 1");
			CommandExecute("glblit 1");
		}
		else {
			ConOut("glblit 0");
			CommandExecute("glblit 0");
		}
		
		if (cAlphaMenu->GetPressed()) {
			ConOut("alphamenu 1");
			CommandExecute("alphamenu 1");
		}
		else {
			ConOut("alphamenu 0");
			CommandExecute("alphamenu 0");
		}
		
		if (cDB->GetPressed()) {
			ConOut("doublebuf 1");
			CommandExecute("doublebuf 1");
		}
		else {
			ConOut("doublebuf 0");
			CommandExecute("doublebuf 0");
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

void GUI_VideoMenu::eventShow() {
  Clear();
}

void GUI_VideoMenu::eventHide() {
}
