
#include "gui.h"
#include "net.h"
#include "server.h"
#include "mapman.h"

extern GUI_OKDialog1 *OKD1;

/////////////////////////////////////////////////////////////////////////////
// NewGame menu widgets
/////////////////////////////////////////////////////////////////////////////

GUI_NewGameMenu* NGMenu;

GUI_NewGameMenu::GUI_NewGameMenu(): GUI_BaseMenu(GUI_NEWGAME, mkrect(NG_PX,NG_PY,NG_VX,NG_VY)) {

	NGMenu = this;

	NewGameMenu = new GUI_Label(this, SDLWidget::mkrect(1,4,NG_VX-2,25), "NEW GAME", false);
	lIP = new GUI_Label(this, SDLWidget::mkrect(1,30,NG_VX-2,20), "<HERE COMES IP ADDRESS>", false);
	lHost = new GUI_Label(this, SDLWidget::mkrect(1,51,NG_VX-2,20), "<HERE COMES HOST NAME>", false);
	lServerName = new GUI_Label(this, SDLWidget::mkrect(1,90, 119,20), "Server name:", false);
	eServerName = new GUI_TextEdit(this, SDLWidget::mkrect(120,90,215,20));
	lWelcomeMsg = new GUI_Label(this, SDLWidget::mkrect(1,120,119,20), "Welcome msg:", false);
	eWelcomeMsg = new GUI_TextEdit(this, SDLWidget::mkrect(120,120,215,20));
	lMaxClients = new GUI_Label(this, SDLWidget::mkrect(1,150,119,20), "Max clients:", false);
	eMaxClients = new GUI_NumEdit(this, SDLWidget::mkrect(120,150,80,20), 1, PWP_TOTALMAX_CLIENTS);

	lChoose1 = new GUI_Label(this, SDLWidget::mkrect(25,179, 150,20), "Choose map:", false);
	WidgetList1 = new GUI_WidgetList(this, SDLWidget::mkrect(26, 201, 148, 64));

	lChoose2 = new GUI_Label(this, SDLWidget::mkrect(25+150+10,179, 150,20), "Choose script:", false);
	WidgetList2 = new GUI_WidgetList(this, SDLWidget::mkrect(26+150+10, 201, 148, 64));

	Board3 = new GUI_Board(this, SDLWidget::mkrect(25, 273, NG_VX-50, 54), false, "GUI_MapInfoBoard");
	lAuthFile = new GUI_Label(Board3, SDLWidget::mkrect(2,2, NG_VX-50-8,12), "file", false);
	lDesc1 = new GUI_Label(Board3, SDLWidget::mkrect(2,14, NG_VX-50-20,12), "d1", false);
	lDesc2 = new GUI_Label(Board3, SDLWidget::mkrect(2,26, NG_VX-50-20,12), "d2", false);
	lDesc3 = new GUI_Label(Board3, SDLWidget::mkrect(2,38, NG_VX-50-20,12), "d3", false);

	cJoin = new GUI_CheckBox(this, SDLWidget::mkrect(25,333,NG_VX-50, CB_SIZEY), "join the game after starting server", true, GUI_Gray64);

	bStartIt = new GUI_ButtonSmall(this, 1, SDLWidget::mkrect(25,360,150,25), "START SERVER");
	bCancel = new GUI_ButtonSmall(this, 2, SDLWidget::mkrect(25+150+10,360,150,25), "BACK");

	NewGameMenu->SetFont(MainFont);

	lWelcomeMsg->SetAlignment(SDL_TA_RIGHT);
	lMaxClients->SetAlignment(SDL_TA_RIGHT);
	lServerName->SetAlignment(SDL_TA_RIGHT);

	lChoose1->SetAlignment(SDL_TA_LEFT);
	lChoose2->SetAlignment(SDL_TA_LEFT);

	lDesc1->SetAlignment(SDL_TA_LEFT);
	lDesc2->SetAlignment(SDL_TA_LEFT);
	lDesc3->SetAlignment(SDL_TA_LEFT);
	lAuthFile->SetAlignment(SDL_TA_LEFT);

	IPaddress serverIP;
	SDLNet_ResolveHost(&serverIP, "localhost", PWP_MSG_SPORT);

	if (serverIP.host != INADDR_NONE ) {
		lIP->SetTextFormat("IP %s", AddrToS(&serverIP));
		char * remote_host_name = SDLNet_ResolveIP(&serverIP);
		
		if (remote_host_name) {
			// try to resolve found host - that because of localhost doesn't return full IP address
			SDLNet_ResolveHost(&serverIP, remote_host_name, PWP_MSG_SPORT);
			lIP->SetTextFormat("IP %s", AddrToS(&serverIP));
			lHost->SetTextFormat("HOST NAME %s", /*_strlwr(*/remote_host_name/*)*/);
		}
		else
			lHost->SetText("Unknown host name");
	}
	else {
		lIP->SetText("Error resolving IP");
		lHost->SetText("Unknown host name");
	}

	Default();
	LoadThemeStyle("GUI_Board");
}

void GUI_NewGameMenu::Return()
{
	eServerName->EditEnd();
	eWelcomeMsg->EditEnd();
	eMaxClients->EditEnd();
	GUI_BaseMenu::Return();
}

void scriptchCB(GUI_LabelL* l)
{
	if(l == NULL) return;
	if (l->msi)   {
		NGMenu->lAuthFile->SetTextFormat(" File: %s.msc, Author: %s", NGMenu->selected2->msi->name, NGMenu->selected2->msi->author);
		NGMenu->lDesc1->SetTextFormat(" %s", NGMenu->selected2->msi->desc[0]);
		NGMenu->lDesc2->SetTextFormat(" %s", NGMenu->selected2->msi->desc[1]);
		NGMenu->lDesc3->SetTextFormat(" %s", NGMenu->selected2->msi->desc[2]);
	}
}


void mapchCB(GUI_LabelL* l)
{
  NGMenu->GenerateScriptSelection();
  NGMenu->WidgetList2->Hide();
  NGMenu->WidgetList2->Show();
}

void GUI_NewGameMenu::GenerateScriptSelection()
{
  WidgetList2->DeleteAll();
  SMapMan.Scan(script_dir.string, smap_ext.string);
  CSMapInfo* a = SMapMan.Scripts;
  int i=0;
  while (a) {
    i++;
    if (selected1 && strcmp(selected1->GetText()+1, a->map)==0)  // +1 because, we add one space before na when inserting label into widgetlist
    {
      char s[200];
      sprintf(s, " %s", a->sname);
      WidgetList2->AddWidget(new GUI_LabelL(NULL, SDLWidget::mkrect(0,0,150-14,12), s, &selected2, a, GUI_UnselectedItem, GUI_SelectedItem, scriptchCB));
    }
    a = a->next;
  }

  if (WidgetList2->GetWidgetCount())
    selected2 = (GUI_LabelL*)WidgetList2->FindWidget(0);
  else
    selected2 = NULL;

	WidgetList2->Redraw();
	scriptchCB(selected2);
}

void GUI_NewGameMenu::Default()
{
  eServerName->SetText(s_name.string);
  eMaxClients->SetText(s_maxclients.string);
  eWelcomeMsg->SetText(s_welcome_msg.string);

  WidgetList1->DeleteAll();
  MapMan.Scan(map_dir.string, map_ext.string);
  CMapInfo* a = MapMan.Maps;
  int i=0;
  while (a) {
    i++;
    char s[200];
    sprintf(s, " %s", a->name);
		WidgetList1->AddWidget(new GUI_LabelL(NULL, SDLWidget::mkrect(0,0,150-14,12), s, &selected1, 0, GUI_UnselectedItem, GUI_SelectedItem, mapchCB));
    a = a->next;
  }

  if (WidgetList1->GetWidgetCount())
    selected1 = (GUI_LabelL*)WidgetList1->FindWidget(0);
  else
    selected1 = NULL;
  GenerateScriptSelection();
}

bool GUI_NewGameMenu::eventButtonClick(int id, SDLWidget* widget)
{
  switch (id) {
  case 1:
    if (selected1 && selected2)
    {
      ConOut("");
      ConOutEx(MISC_FONT, "> NewGame menu sequence <");
      if (net_server_status)
      {
        CommandExecuteOut("es");
      }

      CommandExecuteOut("s_name %s", eServerName->GetText());
      CommandExecuteOut("s_welcome_msg %s", eWelcomeMsg->GetText());
      CommandExecuteOut("s_maxclients %s", eMaxClients->GetText());
      CommandExecuteOut("ss");
      CommandExecuteOut("s_next_map%s", selected1->GetText());
      CommandExecuteOut("s_next_script %s", selected2->msi->name);
      CommandExecuteOut("rs");
      ConOutEx(MISC_FONT, "> end of sequence <");
      ConOut("");

      if (cJoin->GetPressed())
      {
        Cvar_Set("last_server", "localhost");
        JGMenu->Default();
        GUI_Return();
        SendMessage(JGMenu, MSG_BUTTONCLICK, 1, 0);
        GUI_OpenMenu(GUI_MAINMENU);
        GUI_OpenMenu(GUI_CREATEPLAYER);
      }
      else
        while (GUI_id!=GUI_MAINMENU) GUI_Return(); // return from all menus
    }
    else
    {
      if (!selected1)
      {
        OKD1->Reset("MAP REQUIRED", "Map name must be selected !");
        GUI_OpenMenu(GUI_OKDIALOG1);
        return false;
      }

      if (!selected2)
      {
        OKD1->Reset("SCRIPT REQUIRED", "Script name must be selected !");
        GUI_OpenMenu(GUI_OKDIALOG1);
        return false;
      }
    }

    break;
  case 2:
    Return();
    break;
  }
  return true;
}

void GUI_NewGameMenu::eventShow() {
  Clear();
}

void GUI_NewGameMenu::eventHide() {
}
