#include "guicreateplayermenu.h"
#include "gui.h"
#include "client.h"
#include "pw2.h"

GUI_CreatePlayerMenu::GUI_CreatePlayerMenu(int id, int inum) : GUI_BaseMenu(id, mkrect(CP_PX,CP_PY,CP_VX,CP_VY)) {

	CreatePlayerMenu = new GUI_Label(this, SDLWidget::mkrect(1,4,CP_VX-2,25), "CREATE PLAYER", false);
	lPName = new GUI_Label(this, SDLWidget::mkrect(1,40,180,20), "Player name:", false);
	ePName = new GUI_TextEdit(this, SDLWidget::mkrect(10,60,180,20));
	lSkin = new GUI_Label(this, SDLWidget::mkrect(1+220,40, 110,20), "Player skin:", false);
	eSkin = new GUI_TextEdit(this, SDLWidget::mkrect(10+220,+60,110,20));
	SkinCtrl = new GUI_SkinCtrl(this, SDLWidget::mkrect(CP_SK_X, CP_SK_Y, 34, 50), false);
	lUp = new GUI_Label(this, SDLWidget::mkrect(CP_INPUT_X1    ,CP_INPUT_Y1    , CCSX,20), "key up:", false);
	iUp = new GUI_Input(this, SDLWidget::mkrect(CP_INPUT_X1+CCSX,CP_INPUT_Y1    , 100 ,20),1);
	lDown = new GUI_Label(this, SDLWidget::mkrect(CP_INPUT_X1    ,CP_INPUT_Y1+20 , CCSX,20), "key down:", false);
	iDown = new GUI_Input(this, SDLWidget::mkrect(CP_INPUT_X1+CCSX,CP_INPUT_Y1+20 , 100 ,20),2);
	lLeft = new GUI_Label(this, SDLWidget::mkrect(CP_INPUT_X1    ,CP_INPUT_Y1+40 , CCSX,20), "key left:", false);
	iLeft = new GUI_Input(this, SDLWidget::mkrect(CP_INPUT_X1+CCSX,CP_INPUT_Y1+40 , 100 ,20),3);
	lRight = new GUI_Label(this, SDLWidget::mkrect(CP_INPUT_X1    ,CP_INPUT_Y1+60 , CCSX,20), "key right:", false);
	iRight = new GUI_Input(this, SDLWidget::mkrect(CP_INPUT_X1+CCSX,CP_INPUT_Y1+60 , 100 ,20),4);
	lFire = new GUI_Label(this, SDLWidget::mkrect(CP_INPUT_X1    ,CP_INPUT_Y1+80 , CCSX,20), "key fire:", false);
	iFire = new GUI_Input(this, SDLWidget::mkrect(CP_INPUT_X1+CCSX,CP_INPUT_Y1+80 , 100 ,20),5);
	lShield = new GUI_Label(this, SDLWidget::mkrect(CP_INPUT_X1    ,CP_INPUT_Y1+100, CCSX,20), "shield:", false);
	iShield = new GUI_Input(this, SDLWidget::mkrect(CP_INPUT_X1+CCSX,CP_INPUT_Y1+100, 100 ,20),6);
	lWarp = new GUI_Label(this, SDLWidget::mkrect(CP_INPUT_X1    ,CP_INPUT_Y1+120, CCSX,20), "warp:", false);
	iWarp = new GUI_Input(this, SDLWidget::mkrect(CP_INPUT_X1+CCSX,CP_INPUT_Y1+120, 100 ,20),7);
	lWeapon1 = new GUI_Label(this, SDLWidget::mkrect(CP_INPUT_X2    ,CP_INPUT_Y2, CCSX,20), "pistolka:", false);
	iWeapon1 = new GUI_Input(this, SDLWidget::mkrect(CP_INPUT_X2+CCSX,CP_INPUT_Y2, 100 ,20),8);
	lWeapon2 = new GUI_Label(this, SDLWidget::mkrect(CP_INPUT_X2    ,CP_INPUT_Y2+20, CCSX,20), "becheromet:", false);
	iWeapon2 = new GUI_Input(this, SDLWidget::mkrect(CP_INPUT_X2+CCSX,CP_INPUT_Y2+20, 100 ,20),9);
	lWeapon3 = new GUI_Label(this, SDLWidget::mkrect(CP_INPUT_X2    ,CP_INPUT_Y2+40, CCSX,20), "railgun:", false);
	iWeapon3 = new GUI_Input(this, SDLWidget::mkrect(CP_INPUT_X2+CCSX,CP_INPUT_Y2+40, 100 ,20),10);
	lWeapon4 = new GUI_Label(this, SDLWidget::mkrect(CP_INPUT_X2    ,CP_INPUT_Y2+60, CCSX,20), "bombs:", false);
	iWeapon4 = new GUI_Input(this, SDLWidget::mkrect(CP_INPUT_X2+CCSX,CP_INPUT_Y2+60, 100 ,20),11);
	lWeapon5 = new GUI_Label(this, SDLWidget::mkrect(CP_INPUT_X2    ,CP_INPUT_Y2+80, CCSX,20), "grenades:", false);
	iWeapon5 = new GUI_Input(this, SDLWidget::mkrect(CP_INPUT_X2+CCSX,CP_INPUT_Y2+80, 100 ,20),12);
	bOK = new GUI_ButtonSmall(this, 1, SDLWidget::mkrect(25,240,150,25), "OK");
	bCancel = new GUI_ButtonSmall(this, 2, SDLWidget::mkrect(25+150+10,240,150,25), "BACK");

	num=inum;
	Default();
	char sx[100];

	CreatePlayerMenu->SetColor(GUI_BtnTextColor, GUI_BtnATextColor);
	CreatePlayerMenu->SetFont(MainFont);
	CreatePlayerMenu->SetTextFormat("CREATE PLAYER%d", num);

	CreatePlayerMenu->bgmode = 2;
	lPName->bgmode = 2;
	lPName->SetAlignment(SDL_TA_LEFT);
	lPName->shiftx = 4;
	lSkin->bgmode = 2;
	lSkin->SetAlignment(SDL_TA_LEFT);
	lSkin->shiftx = 4;

	SkinCtrl->SkinPic->lName = eSkin;
	switch (num) {
	case 1: SkinCtrl->SkinPic->SetSkin(p1_skin.string); break;
	case 2: SkinCtrl->SkinPic->SetSkin(p2_skin.string); break;
	case 3: SkinCtrl->SkinPic->SetSkin(p3_skin.string); break;
	case 4: SkinCtrl->SkinPic->SetSkin(p4_skin.string); break;
	}

	lUp->SetAlignment(SDL_TA_RIGHT);
	lDown->SetAlignment(SDL_TA_RIGHT);
	lLeft->SetAlignment(SDL_TA_RIGHT);
	lRight->SetAlignment(SDL_TA_RIGHT);
	lFire->SetAlignment(SDL_TA_RIGHT);
	lShield->SetAlignment(SDL_TA_RIGHT);
	lWarp->SetAlignment(SDL_TA_RIGHT);

	lWeapon1->SetAlignment(SDL_TA_RIGHT);
	lWeapon2->SetAlignment(SDL_TA_RIGHT);
	lWeapon3->SetAlignment(SDL_TA_RIGHT);
	lWeapon4->SetAlignment(SDL_TA_RIGHT);
	lWeapon5->SetAlignment(SDL_TA_RIGHT);

	LoadThemeStyle("GUI_Board");
}

void GUI_CreatePlayerMenu::Return() {

	if (iUp->waiting || iDown->waiting || iLeft->waiting || iRight->waiting ||
		iWeapon1->waiting || iWeapon2->waiting || iWeapon3->waiting || iWeapon4->waiting || iWeapon5->waiting
		|| iShield->waiting || iWarp->waiting || iFire->waiting) {
		
		iUp->InputEnd();
		iDown->InputEnd();
		iLeft->InputEnd();
		iRight->InputEnd();
		iFire->InputEnd();
		iWarp->InputEnd();
		iShield->InputEnd();
		iWeapon1->InputEnd();
		iWeapon2->InputEnd();
		iWeapon3->InputEnd();
		iWeapon4->InputEnd();
		iWeapon5->InputEnd();
		return;
	}

	ePName->EditEnd();
	eSkin->EditEnd();

	GUI_BaseMenu::Return();
}

void GUI_CreatePlayerMenu::SetPlayerDefaults(
								cvar_t* p_name,
								cvar_t* p_key_up,
								cvar_t* p_key_down,
								cvar_t* p_key_left,
								cvar_t* p_key_right,
								cvar_t* p_key_fire,
								cvar_t* p_key_shield,
								cvar_t* p_key_warp,
								cvar_t* p_key_weapon1,
								cvar_t* p_key_weapon2,
								cvar_t* p_key_weapon3,
								cvar_t* p_key_weapon4,
								cvar_t* p_key_weapon5
								)
{
	ePName->SetText(p_name->string);

	iUp->SetText(SDL_GetKeyName((SDLKey)(Uint16)p_key_up->value));
	iDown->SetText(SDL_GetKeyName((SDLKey)(Uint16)p_key_down->value));
	iLeft->SetText(SDL_GetKeyName((SDLKey)(Uint16)p_key_left->value));
	iRight->SetText(SDL_GetKeyName((SDLKey)(Uint16)p_key_right->value));
	iFire->SetText(SDL_GetKeyName((SDLKey)(Uint16)p_key_fire->value));
	iShield->SetText(SDL_GetKeyName((SDLKey)(Uint16)p_key_shield->value));
	iWarp->SetText(SDL_GetKeyName((SDLKey)(Uint16)p_key_warp->value));

	iWeapon1->SetText(SDL_GetKeyName((SDLKey)(Uint16)p_key_weapon1->value));
	iWeapon2->SetText(SDL_GetKeyName((SDLKey)(Uint16)p_key_weapon2->value));
	iWeapon3->SetText(SDL_GetKeyName((SDLKey)(Uint16)p_key_weapon3->value));
	iWeapon4->SetText(SDL_GetKeyName((SDLKey)(Uint16)p_key_weapon4->value));
	iWeapon5->SetText(SDL_GetKeyName((SDLKey)(Uint16)p_key_weapon5->value));

	iUp->SetSym((Uint16)p_key_up->value);
	iDown->SetSym((Uint16)p_key_down->value);
	iLeft->SetSym((Uint16)p_key_left->value);
	iRight->SetSym((Uint16)p_key_right->value);
	iFire->SetSym((Uint16)p_key_fire->value);
	iShield->SetSym((Uint16)p_key_shield->value);
	iWarp->SetSym((Uint16)p_key_warp->value);

	iWeapon1->SetSym((Uint16)p_key_weapon1->value);
	iWeapon2->SetSym((Uint16)p_key_weapon2->value);
	iWeapon3->SetSym((Uint16)p_key_weapon3->value);
	iWeapon4->SetSym((Uint16)p_key_weapon4->value);
	iWeapon5->SetSym((Uint16)p_key_weapon5->value);
}

void GUI_CreatePlayerMenu::Default() {

	switch (num) {
	case 1:
		SetPlayerDefaults(
			&p1_name,
			&p1_key_up,
			&p1_key_down,
			&p1_key_left,
			&p1_key_right,
			&p1_key_fire,
			&p1_key_shield,
			&p1_key_warp,
			&p1_key_weapon1,
			&p1_key_weapon2,
			&p1_key_weapon3,
			&p1_key_weapon4,
			&p1_key_weapon5
			);
		break;
		
	case 2:
		SetPlayerDefaults(
   			&p2_name,
   			&p2_key_up,
   			&p2_key_down,
   			&p2_key_left,
   			&p2_key_right,
   			&p2_key_fire,
   			&p2_key_shield,
			&p2_key_warp,
   			&p2_key_weapon1,
   			&p2_key_weapon2,
   			&p2_key_weapon3,
   			&p2_key_weapon4,
   			&p2_key_weapon5
   			);
		break;
		
	case 3:
		SetPlayerDefaults(
			&p3_name,
			&p3_key_up,
			&p3_key_down,
			&p3_key_left,
			&p3_key_right,
			&p3_key_fire,
			&p3_key_shield,
			&p3_key_warp,
			&p3_key_weapon1,
			&p3_key_weapon2,
			&p3_key_weapon3,
			&p3_key_weapon4,
			&p3_key_weapon5
			);
		break;
		
	case 4:
		SetPlayerDefaults(
			&p4_name,
			&p4_key_up,
			&p4_key_down,
			&p4_key_left,
			&p4_key_right,
			&p4_key_fire,
			&p4_key_shield,
			&p4_key_warp,
			&p4_key_weapon1,
			&p4_key_weapon2,
			&p4_key_weapon3,
			&p4_key_weapon4,
			&p4_key_weapon5
			);
		break;
		
	}
}

bool GUI_CreatePlayerMenu::SetPlayerVals(
							int num,
							CGame* g,
							cvar_t* p_name,
							cvar_t* p_skin
							)
{
	GPlayer* p = NULL;
	
      ConOut("p%i_name %s", num, ePName->GetText());
      CommandExecute("p%i_name %s", num, ePName->GetText());
      ConOut("p%i_skin %s", num, eSkin->GetText());
      CommandExecute("p%i_skin %s", num, eSkin->GetText());
      ConOut("p%i_key_up %d", num, iUp->GetSym());
      CommandExecute("p%i_key_up %d", num, iUp->GetSym());
      ConOut("p%i_key_down %d", num, iDown->GetSym());
      CommandExecute("p%i_key_down %d", num, iDown->GetSym());
      ConOut("p%i_key_left %d", num, iLeft->GetSym());
      CommandExecute("p%i_key_left %d", num, iLeft->GetSym());
      ConOut("p%i_key_right %d", num, iRight->GetSym());
      CommandExecute("p%i_key_right %d", num, iRight->GetSym());
      ConOut("p%i_key_fire %d", num, iFire->GetSym());
      CommandExecute("p%i_key_fire %d", num, iFire->GetSym());
      ConOut("p%i_key_shield %d", num, iShield->GetSym());
      CommandExecute("p%i_key_shield %d", num, iShield->GetSym());
      ConOut("p%i_key_warp %d", num, iWarp->GetSym());
      CommandExecute("p%i_key_warp %d", num, iWarp->GetSym());
      ConOut("p%i_key_weapon1 %d", num, iWeapon1->GetSym());
      CommandExecute("p%i_key_weapon1 %d", num, iWeapon1->GetSym());
      ConOut("p%i_key_weapon2 %d", num, iWeapon2->GetSym());
      CommandExecute("p%i_key_weapon2 %d", num, iWeapon2->GetSym());
      ConOut("p%i_key_weapon3 %d", num, iWeapon3->GetSym());
      CommandExecute("p%i_key_weapon3 %d", num, iWeapon3->GetSym());
      ConOut("p%i_key_weapon4 %d", num, iWeapon4->GetSym());
      CommandExecute("p%i_key_weapon4 %d", num, iWeapon4->GetSym());
      ConOut("p%i_key_weapon5 %d", num, iWeapon5->GetSym());
      CommandExecute("p%i_key_weapon5 %d", num, iWeapon5->GetSym());

      for (int i=0; i<GAME_MAX_OBJS; i++)
      {
        if ((g->objs[i]->state&OSTATE_ACTIVE) && (g->objs[i]->GetType()==ot_player))
        {
          p = (GPlayer*)g->objs[i];
          if (strcmp(p->player_name.GetValRef()->chars, ePName->GetText())==0)
          {
            ConOut("Player %s found in game, reseting ...", p_name->string);
            SetKeybindings(p->kbindex, 0);
            ConOut("sp %s %s", p_name->string, p_skin->string);
            CommandExecute("sp %s %s", p_name->string, p_skin->string);
            ConOut("chase %s", p_name->string);
            CommandExecute("chase %s", p_name->string);
            return true;
          }
        }
      }
      strcpy(auto_playername[0], p_name->string);
      time_playername = ticktime + 0;
      strcpy(auto_skinname[0], p_skin->string);
      strcpy(auto_skinplayername[0], p_name->string);
      time_skinname = ticktime + 3000;
      strcpy(auto_chasename[0], p_name->string);
      time_chasename = ticktime + 3000;

      return false;
}

bool GUI_CreatePlayerMenu::eventButtonClick(int id, SDLWidget* widget) {
	GPlayer* p;
	CGame& g = client_info.game;
	GAME_MAXOBJS_TYPE i;

	switch (id) {
	case 1:

		ConOut("");
		ConOut("> CreatePlayer menu sequence <");
	
		switch (num) {
		case 1:
			if(SetPlayerVals(1, &g, &p1_name, &p1_skin)) {
				return true;
			}
			break;
		
		case 2:
			if(SetPlayerVals(2, &g, &p2_name, &p2_skin)) {
				return true;
			}
			break;
		
		case 3:
			if(SetPlayerVals(3, &g, &p3_name, &p3_skin)) {
				return true;
			}
			break;
		
		case 4:
			if(SetPlayerVals(4, &g, &p4_name, &p4_skin)) {
				return true;
			}
			break;
		}
	
		ConOut("> end of sequence <");
		ConOut("");

		while (GUI_id) GUI_Return(); // return from all menus
		break;
		
	case 2:
		Return();
		break;
	}
	
	return true;
}

bool GUI_CreatePlayerMenu::eventNextInput(int id, SDLWidget* widget)
{
	switch (id) {
	case 1:
		iDown->InputBegin();
		break;
	case 2:
		iLeft->InputBegin();
		break;
	case 3:
		iRight->InputBegin();
		break;
	case 4:
		iFire->InputBegin();
		break;
	case 5:
		iShield->InputBegin();
		break;
	case 6:
		iWarp->InputBegin();
		break;
	case 7:
		iWeapon1->InputBegin();
		break;
	case 8:
		iWeapon2->InputBegin();
		break;
	case 9:
		iWeapon3->InputBegin();
		break;
	case 10:
		iWeapon4->InputBegin();
		break;
	case 11:
		iWeapon5->InputBegin();
		break;
	}
	
	return true;
}



bool GUI_CreatePlayerMenu::eventMessage(MSG_MESSAGE* msg){
	bool rc = false;

	if((msg->to != this) && (msg->to != NULL)){
		return false;
	}

	// dispatch user message
	switch(msg->type) {
		case MSG_BUTTONCLICK:
			rc = eventButtonClick(msg->widget_id, (SDLWidget*)(msg->from));
			break;

		case MSG_SIGNAL:
			rc = eventNextInput(msg->widget_id, (SDLWidget*)(msg->from));
			break;
		
		default:
			rc = false;
			break;
	}

	return rc;
}


void GUI_CreatePlayerMenu::eventShow() {
  Clear();
}

void GUI_CreatePlayerMenu::eventHide() {
}

