
#include "guicreateplayerselmenu.h"
#include "gui.h"

/////////////////////////////////////////////////////////////////////////////
// CreatePlayers menu widgets
/////////////////////////////////////////////////////////////////////////////

GUI_CreatePlayerSelMenu::GUI_CreatePlayerSelMenu() : GUI_BaseMenu(GUI_CREATEPLAYER, mkrect(CPS_PX,CPS_PY,CPS_VX,CPS_VY)) {
	lCreatePlayerSelMenu = new GUI_ButtonBig(this, 0, SDLWidget::mkrect(0, 0, CPS_VX, 40), "Players Menu");
	bCP1 = new GUI_ButtonBig(this, 1, SDLWidget::mkrect(0, 40, CPS_VX, 40), "Player1");
	bCP2 = new GUI_ButtonBig(this, 2, SDLWidget::mkrect(0, 80, CPS_VX, 40), "Player2");
	bCP3 = new GUI_ButtonBig(this, 3, SDLWidget::mkrect(0, 120, CPS_VX, 40), "Player3");
	bCP4 = new GUI_ButtonBig(this, 4, SDLWidget::mkrect(0, 160, CPS_VX, 40), "Player4");
	bBack = new GUI_ButtonBig(this, 10, SDLWidget::mkrect(0, 200, CPS_VX, 40), "Back");
	
	Default();

	lCreatePlayerSelMenu->enabled = false;
	lCreatePlayerSelMenu->LoadThemeStyle("GUI_MenuTitle");
	
	LoadThemeStyle("GUI_Board");	
}

void GUI_CreatePlayerSelMenu::Default() {
}

bool GUI_CreatePlayerSelMenu::eventButtonClick(int id, SDLWidget* widget) {
  switch (id) {
  case 1:
    GUI_OpenMenu(GUI_CREATEPLAYER1);
    break;
  case 2:
    GUI_OpenMenu(GUI_CREATEPLAYER2);
    break;
  case 3:
    GUI_OpenMenu(GUI_CREATEPLAYER3);
    break;
  case 4:
    GUI_OpenMenu(GUI_CREATEPLAYER4);
    break;
  case 10:
    Return();
    break;
  }
  return true;
}

void GUI_CreatePlayerSelMenu::eventShow() {
  Clear();
}

void GUI_CreatePlayerSelMenu::eventHide() {
}

