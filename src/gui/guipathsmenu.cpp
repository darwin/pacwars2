#include "guipathsmenu.h"
#include "gui.h"
#include "SDL_Console.h"

GUI_PathsMenu::GUI_PathsMenu() : GUI_BaseMenu(GUI_PATHS, PG_Rect(PM_PX,PM_PY,PM_VX,PM_VY)) {

	lPathsMenu = new PG_Label(this, PG_Rect(1,4,PM_VX-2,25), "PATHS FOR GAME", "GUI_Label");
	lEffect = new PG_Label(this, PG_Rect(1,30,PM_VX-2,20), "Changes will take effect after restarting program.", "GUI_Label");

	lScriptDir = new PG_Label(this, PG_Rect(1,PM_BASE, 118,20), "Scripts:", "GUI_LabelR");
	eScriptDir = new GUI_TextEdit(this, PG_Rect(120,PM_BASE,215,20));

	lMapDir = new PG_Label(this, PG_Rect(1,PM_BASE+20, 118,20), "Maps:", "GUI_LabelR");
	eMapDir = new GUI_TextEdit(this, PG_Rect(120,PM_BASE+20,215,20));

	lSkinDir = new PG_Label(this, PG_Rect(1,PM_BASE+40, 118,20), "Skins:", "GUI_LabelR");
	eSkinDir = new GUI_TextEdit(this, PG_Rect(120,PM_BASE+40,215,20));

	lSpriteDir = new PG_Label(this, PG_Rect(1,PM_BASE+60, 118,20), "Sprites:", "GUI_LabelR");
	eSpriteDir = new GUI_TextEdit(this, PG_Rect(120,PM_BASE+60,215,20));

	lGfxDir = new PG_Label(this, PG_Rect(1,PM_BASE+80, 118,20), "Graphics:", "GUI_LabelR");
	eGfxDir = new GUI_TextEdit(this, PG_Rect(120,PM_BASE+80,215,20));

	lGuiDir = new PG_Label(this, PG_Rect(1,PM_BASE+100, 118,20), "GUI:", "GUI_LabelR");
	eGuiDir = new GUI_TextEdit(this, PG_Rect(120,PM_BASE+100,215,20));

	lSndDir = new PG_Label(this, PG_Rect(1,PM_BASE+120, 118,20), "Sound:", "GUI_LabelR");
	eSndDir = new GUI_TextEdit(this, PG_Rect(120,PM_BASE+119,215,20));

	bChange = new GUI_ButtonSmall(this, 1, PG_Rect(25,PM_BASE+150,150,25), "APPLY");
	bCancel = new GUI_ButtonSmall(this, 2, PG_Rect(25+150+10,PM_BASE+150,150,25), "BACK");

	Default();

	lPathsMenu->SetFont(MainFont);

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

bool GUI_PathsMenu::eventButtonClick(int id, PG_Widget* widget) {

	switch (id) {
	case 1:
		ConOut("");
		ConOutEx(MISC_FONT, "> Paths menu sequence <");
		CommandExecuteOut("script_dir %s", eScriptDir->GetText());
		CommandExecuteOut("map_dir %s", eMapDir->GetText());
		CommandExecuteOut("skin_dir %s", eSkinDir->GetText());
		CommandExecuteOut("sprite_dir %s", eSpriteDir->GetText());
		CommandExecuteOut("gfx_dir %s", eGfxDir->GetText());
		CommandExecuteOut("gui_dir %s", eGuiDir->GetText());
		CommandExecuteOut("snd_dir %s", eSndDir->GetText());
		ConOutEx(MISC_FONT, "> end of sequence <");
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

