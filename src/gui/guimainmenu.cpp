#include "guimainmenu.h"
#include "gui.h"
#include "client.h"
#include "server.h"

extern GUI_YNDialog *YND;

extern void QuitCB(int res);
extern void ShutdownCB(int res);
extern void DisconnectCB(int res);


GUI_MainMenu::GUI_MainMenu() : GUI_BaseMenu(GUI_MAINMENU, mkrect(MM_PX,MM_PY,MM_VX,MM_VY)) {

	lMainMenu = new GUI_ButtonBig(this, 0, SDLWidget::mkrect(0,0,MM_VX,40), "Main Menu");
	bNewGame = new GUI_ButtonBig(this, 1, SDLWidget::mkrect(0,40,MM_VX,40), "Start Server");
	bEndGame = new GUI_ButtonBig(this, 9, SDLWidget::mkrect(0,40,MM_VX,40), "Shutdown Server");
	bJoinDiscGame = new GUI_ButtonBig(this, 2, SDLWidget::mkrect(0,80,MM_VX,40), "Join Game");
	bCreatePlayer = new GUI_ButtonBig(this, 3, SDLWidget::mkrect(0,120,MM_VX,40), "Create Player");
	bDeletePlayer = new GUI_ButtonBig(this, 4, SDLWidget::mkrect(0,160,MM_VX,40), "Delete Player");
	bOptions = new GUI_ButtonBig(this, 5, SDLWidget::mkrect(0,200,MM_VX,40), "Options");
	bCredits = new GUI_ButtonBig(this, 6, SDLWidget::mkrect(0,240,MM_VX,40), "Credits");
	bHelp = new GUI_ButtonBig(this, 7, SDLWidget::mkrect(0,280,MM_VX,40), "Info");
	bQuit = new GUI_ButtonBig(this, 8, SDLWidget::mkrect(0,320,MM_VX,40), "Quit");
	bGame = new GUI_ButtonBig(this, 10, SDLWidget::mkrect(0,360,MM_VX,40), "Back To Game");

	Default();

	lMainMenu->enabled = false;
	lMainMenu->LoadThemeStyle("GUI_MenuTitle", "Button");
}

void GUI_MainMenu::Default(){
}

bool GUI_MainMenu::eventButtonClick(int id, SDLWidget* widget) {
	char* a[]={"I'M SURE", "YES", "OK", "DO IT", "LEAVE"};

	switch (id) {
	
	case 1:
		GUI_OpenMenu(GUI_NEWGAME);
		break;
		
	case 2:
		if (net_client_status==NS_CONNECTED) 
    {
		  YND->Reset(DisconnectCB, "DISCONNECT", "Do you really want disconenct from the game ?");
		  GUI_OpenMenu(GUI_YNDIALOG);
		}
		else {
			GUI_OpenMenu(GUI_JOINGAME);
		}
		break;
		
	case 3:
		GUI_OpenMenu(GUI_CREATEPLAYER);
		break;
		
	case 4:
		GUI_OpenMenu(GUI_DELETEPLAYER);
		break;
		
	case 5:
		GUI_OpenMenu(GUI_OPTIONS);
		break;
		
	case 6:
		GUI_OpenMenu(GUI_CREDITS);
		break;
		
	case 7:
		GUI_OpenMenu(GUI_HELP1);
		break;
		
	case 8:
		YND->Reset(QuitCB, "QUIT PACWARS2", "Do you really want to quit the game ?", a[rand()%5]);
		GUI_OpenMenu(GUI_YNDIALOG);
		break;
		
	case 9:
		YND->Reset(ShutdownCB, "END SERVER GAME", "Do you want shutdown server ?");
		GUI_OpenMenu(GUI_YNDIALOG);
		break;
		
	case 10:
		Return();
		break;
	}
	return true;
}

void GUI_MainMenu::eventShow() {
	Clear();

	bNewGame->Hide();
	bEndGame->Hide();

	if (net_client_status==NS_CONNECTED) {
		bJoinDiscGame->SetText("Disconnect");
		bCreatePlayer->MoveWindow(0,120);
		bDeletePlayer->MoveWindow(0,160);
		bOptions->MoveWindow(0,200);
		bCredits->MoveWindow(0,240);
		bHelp->MoveWindow(0,280);
		bQuit->MoveWindow(0,320);
		bGame->MoveWindow(0,360);
	}
	else {
		bCreatePlayer->Hide();
		bDeletePlayer->Hide();
		bGame->Hide();

		bJoinDiscGame->SetText("Join Game");
		bOptions->MoveWindow(0,120);
		bCredits->MoveWindow(0,160);
		bHelp->MoveWindow(0,200);
		bQuit->MoveWindow(0,240);
	}

	if (net_server_status==NS_UNINITED) {
		bNewGame->Show();
	}
	else {
		bEndGame->Show();
	}

	//Redraw();
}

void GUI_MainMenu::eventHide() {
}

void GUI_MainMenu::Return() {
	GUI_BaseMenu::Return();
	GUI_id = GUI_NOMENU;
	GUI_menu = NULL;

	smPlaySample(SM_MENU2, menu_volume);
}
