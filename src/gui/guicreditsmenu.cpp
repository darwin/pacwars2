#include "guicreditsmenu.h"
#include "gui.h"
#include "pw2.h"

/////////////////////////////////////////////////////////////////////////////
// Credits menu widgets
/////////////////////////////////////////////////////////////////////////////

GUI_CreditsMenu::GUI_CreditsMenu() : GUI_BaseMenu(GUI_CREDITS, PG_Rect(CM_PX,CM_PY,CM_VX,CM_VY)) {

	mBack1 = new GUI_Bitmap(this, PG_Rect(1, 1, credits->w, credits->h), credits),
	CreditsMenu = new PG_Label(mBack1, PG_Rect(1,4,CM_VX-2,25), "Credits", "GUI_Label");
	lLine[0] = new PG_Label(mBack1, PG_Rect(CM_G1_PX,CM_G1_PY+35, CM_G1_VX-2,20), "Code by Antonin Hildebrand aka Woid", "GUI_LabelL");
	lLine[1] = new PG_Label(mBack1, PG_Rect(CM_G1_PX,CM_G1_PY+55, CM_G1_VX-2,20), "Graphics by Petr Stastny aka Raist", "GUI_LabelL");
	lLine[2] = new PG_Label(mBack1, PG_Rect(CM_G1_PX,CM_G1_PY+75, CM_G1_VX-2,20), "Music by Ondrej Broukal aka GameOver", "GUI_LabelL");
	lLine[3] = new PG_Label(mBack1, PG_Rect(CM_G1X_PX,CM_G1_PY+95, CM_G1_VX-50,20), "Petr Stastny aka Raist", "GUI_LabelL");
	lLine[4] = new PG_Label(mBack1, PG_Rect(CM_G1X_PX,CM_G1_PY+115, CM_G1_VX-50,20), "Pavel Mirejovsky aka Dusty", "GUI_LabelL");
	lLine[6] = new PG_Label(mBack1, PG_Rect(CM_G2_PX,CM_G2_PY+155, CM_G2_VX-2,20), "SDL library by Sam Latinga", "GUI_LabelL");
	lLine[7] = new PG_Label(mBack1, PG_Rect(CM_G2_PX,CM_G2_PY+175, CM_G2_VX-2,20), "SeeR scripting engine", "GUI_LabelL");
	lLine[5] = new PG_Label(mBack1, PG_Rect(CM_G2_PX,CM_G2_PY+195, CM_G2_VX-2,20), "by Przemek Podsiadly aka Theur", "GUI_LabelL");
	lLine[8] = new PG_Label(mBack1, PG_Rect(CM_G2_PX,CM_G2_PY+215, CM_G2_VX-2,20), "BASS sound system by Ian Luck", "GUI_LabelL");
	lLine[9] = new PG_Label(mBack1, PG_Rect(CM_G2_PX,CM_G2_PY+235, CM_G2_VX-2,20), "Mappy by Robin Burrows", "GUI_LabelL");
	lLine[10] = new PG_Label(mBack1, PG_Rect(CM_G2_PX,CM_G2_PY+255, CM_G2_VX-2,20), "ParaGUI by Alexander Pipelka", "GUI_LabelL");
	lLine[11] = new PG_Label(mBack1, PG_Rect(CM_G2_PX,CM_G2_PY+275, CM_G2_VX-2,20), "ImageMagick library by ImageMagick Studio", "GUI_LabelL");
	lLine[12] = new PG_Label(mBack1, PG_Rect(CM_G2_PX,CM_G2_PY+295, CM_G2_VX-2,20), "SDL: net, console, ttf, mixer and image libs", "GUI_LabelL");
	lLine[13] = new PG_Label(mBack1, PG_Rect(CM_G2_PX,CM_G2_PY+315, CM_G2_VX-2,20), "zlib, libpng, UPX packer", "GUI_LabelL");
	lLine[14] = new PG_Label(mBack1, PG_Rect(CM_G1_PX,CM_G1_PY+135, CM_G1_VX-2,20), "Linux & BeOS port by Alex Pipelka aka Braindead", "GUI_LabelL");
	lLine[15] = new PG_Label(mBack1, PG_Rect(4,235, CM_VX-2,20), "Thanks to SLiK, Mem, Lada, Tessien, Farey, Kerim, Garett Banuk, Nula, ...", "GUI_Label");
	lLine[17] = new PG_Label(mBack1, PG_Rect(4,315, CM_VX-2,25), "visit PW2 site at", "GUI_Label");
	lLine[16] = new PG_Label(mBack1, PG_Rect(CM_G1_PX,CM_G1_PY+175, CM_G1_VX-2,20), "Iain Syme author of the orignal PacWars", "GUI_LabelL");
	lLine[18] = new PG_Label(mBack1, PG_Rect(4,345, CM_VX-2,25), PW_WEBADDRESS, "GUI_Label");
	bOK = new GUI_ButtonSmall(this, 1, PG_Rect(155,390,150,25), "COOL");

	Default();

	CreditsMenu->SetFont(MainFont);
	lLine[17]->SetFont(MainFont);
	lLine[18]->SetFont(MainFont);
}

void GUI_CreditsMenu::Default() {
}

bool GUI_CreditsMenu::eventButtonClick(int id, PG_Widget* widget) {

	switch (id) {
	case 1:
		Return();
		break;
	}
	
	return true;
}

void GUI_CreditsMenu::eventShow() {
  Clear();
}

void GUI_CreditsMenu::eventHide() {
}

