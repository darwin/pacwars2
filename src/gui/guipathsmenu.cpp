#include "guipathsmenu.h"
#include "gui.h"
#include "SDL_Console.h"

GUI_PathsMenu::GUI_PathsMenu() : GUI_BaseMenu(GUI_PATHS, mkrect(PM_PX,PM_PY,PM_VX,PM_VY)) {

	lPathsMenu = new GUI_Label(this, SDLWidget::mkrect(1,4,PM_VX-2,25), "PATHS FOR GAME", false);
	lEffect = new GUI_Label(this, SDLWidget::mkrect(1,30,PM_VX-2,20), "Changes will take effect after restarting program.", false);

	lScriptDir = new GUI_Label(this, SDLWidget::mkrect(1,PM_BASE, 120,20), "Scripts:", false);
	eScriptDir = new GUI_TextEdit(this, SDLWidget::mkrect(120,PM_BASE,215,20));
	lMapDir = new GUI_Label(this, SDLWidget::mkrect(1,PM_BASE+20, 119,20), "Maps:", false);
	eMapDir = new GUI_TextEdit(this, SDLWidget::mkrect(120,PM_BASE+20,215,20));
	lSkinDir = new GUI_Label(this, SDLWidget::mkrect(1,PM_BASE+40, 119,20), "Skins:", false);
	eSkinDir = new GUI_TextEdit(this, SDLWidget::mkrect(120,PM_BASE+40,215,20));
	lSpriteDir = new GUI_Label(this, SDLWidget::mkrect(1,PM_BASE+60, 119,20), "Sprites:", false);
	eSpriteDir = new GUI_TextEdit(this, SDLWidget::mkrect(120,PM_BASE+60,215,20));
	lGfxDir = new GUI_Label(this, SDLWidget::mkrect(1,PM_BASE+80, 119,20), "Graphics:", false);
	eGfxDir = new GUI_TextEdit(this, SDLWidget::mkrect(120,PM_BASE+80,215,20));
	lGuiDir = new GUI_Label(this, SDLWidget::mkrect(1,PM_BASE+100, 119,20), "GUI:", false);
	eGuiDir = new GUI_TextEdit(this, SDLWidget::mkrect(120,PM_BASE+100,215,20));
	lSndDir = new GUI_Label(this, SDLWidget::mkrect(1,PM_BASE+120, 119,20), "Sound:", false);
	eSndDir = new GUI_TextEdit(this, SDLWidget::mkrect(120,PM_BASE+120,215,20));

	bChange = new GUI_ButtonSmall(this, 1, SDLWidget::mkrect(25,PM_BASE+150,150,25), "APPLY");
	bCancel = new GUI_ButtonSmall(this, 2, SDLWidget::mkrect(25+150+10,PM_BASE+150,150,25), "BACK");

	Default();

	lPathsMenu->SetColor(GUI_BtnTextColor, GUI_BtnATextColor);
	lPathsMenu->SetFont(MainFont);

	lScriptDir->SetAlignment(SDL_TA_RIGHT);
	lMapDir->SetAlignment(SDL_TA_RIGHT);
	lSkinDir->SetAlignment(SDL_TA_RIGHT);
	lSpriteDir->SetAlignment(SDL_TA_RIGHT);
	lGfxDir->SetAlignment(SDL_TA_RIGHT);
	lGuiDir->SetAlignment(SDL_TA_RIGHT);
	lSndDir->SetAlignment(SDL_TA_RIGHT);

	LoadThemeStyle("GUI_Board");
}

void GUI_PathsMenu::Return() {
	eScriptDir->EditEnd();
	eSpriteDir->EditEnd();
	eMapDir->EditEnd();
	eSndDir->EditEnd();
	eSkinDir->EditEnd();
	eGuiDir->EditEnd();
	eGfxDir->EditEnd();
	GUI_BaseMenu::Return();
}

void GUI_PathsMenu::Default() {
	eScriptDir->SetText(script_dir.string);
	eMapDir->SetText(map_dir.string);
	eSkinDir->SetText(skin_dir.string);
	eSpriteDir->SetText(sprite_dir.string);
	eGuiDir->SetText(gui_dir.string);
	eGfxDir->SetText(gfx_dir.string);
	eSndDir->SetText(snd_dir.string);
}

bool GUI_PathsMenu::eventButtonClick(int id, SDLWidget* widget) {

	switch (id) {
	case 1:
		ConOut("");
		ConOut("> Paths menu sequence <");
		ConOut("script_dir %s", eScriptDir->GetText());
		CommandExecute("script_dir %s", eScriptDir->GetText());
		ConOut("map_dir %s", eMapDir->GetText());
		CommandExecute("map_dir %s", eMapDir->GetText());
		ConOut("skin_dir %s", eSkinDir->GetText());
		CommandExecute("skin_dir %s", eSkinDir->GetText());
		ConOut("sprite_dir %s", eSpriteDir->GetText());
		CommandExecute("sprite_dir %s", eSpriteDir->GetText());
		ConOut("gfx_dir %s", eGfxDir->GetText());
		CommandExecute("gfx_dir %s", eGfxDir->GetText());
		ConOut("gui_dir %s", eGuiDir->GetText());
		CommandExecute("gui_dir %s", eGuiDir->GetText());
		ConOut("snd_dir %s", eSndDir->GetText());
		CommandExecute("snd_dir %s", eSndDir->GetText());
		ConOut("> end of sequence <");
		ConOut("");
		break;
		
	case 2:
		Return();
		break;
	}
	
	return true;
}

void GUI_PathsMenu::eventShow() {
  Clear();
}

void GUI_PathsMenu::eventHide() {
}

