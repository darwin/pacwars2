#include "guicreateplayermenu.h"
#include "gui.h"
#include "client.h"
#include "pw2.h"

GUI_CreatePlayerMenu::GUI_CreatePlayerMenu(int id, int inum) : GUI_BaseMenu(id, PG_Rect(CP_PX,CP_PY,CP_VX,CP_VY)) {

	CreatePlayerMenu = new PG_Label(this, PG_Rect(1,4,CP_VX-2,25), "CREATE PLAYER", "GUI_Label");
	lPName = new PG_Label(this, PG_Rect(1,40,180,20), "Player name:", "GUI_Label");
	ePName = new GUI_TextEdit(this, PG_Rect(10,60,180,20));
	lSkin = new PG_Label(this, PG_Rect(1+220,40, 110,20), "Player skin:", "GUI_Label");
	eSkin = new GUI_TextEdit(this, PG_Rect(10+220,+60,110,20));
	SkinCtrl = new GUI_SkinCtrl(this, PG_Rect(CP_SK_X, CP_SK_Y, 34, 50), false);
	lUp = new PG_Label(this, PG_Rect(CP_INPUT_X1    ,CP_INPUT_Y1    , CCSX,20), "key up:", "GUI_Label");
	iUp = new GUI_Input(this, PG_Rect(CP_INPUT_X1+CCSX,CP_INPUT_Y1    , 100 ,20),1);
	lDown = new PG_Label(this, PG_Rect(CP_INPUT_X1    ,CP_INPUT_Y1+20 , CCSX,20), "key down:", "GUI_Label");
	iDown = new GUI_Input(this, PG_Rect(CP_INPUT_X1+CCSX,CP_INPUT_Y1+20 , 100 ,20),2);
	lLeft = new PG_Label(this, PG_Rect(CP_INPUT_X1    ,CP_INPUT_Y1+40 , CCSX,20), "key left:", "GUI_Label");
	iLeft = new GUI_Input(this, PG_Rect(CP_INPUT_X1+CCSX,CP_INPUT_Y1+40 , 100 ,20),3);
	lRight = new PG_Label(this, PG_Rect(CP_INPUT_X1    ,CP_INPUT_Y1+60 , CCSX,20), "key right:", "GUI_Label");
	iRight = new GUI_Input(this, PG_Rect(CP_INPUT_X1+CCSX,CP_INPUT_Y1+60 , 100 ,20),4);
	lFire = new PG_Label(this, PG_Rect(CP_INPUT_X1    ,CP_INPUT_Y1+80 , CCSX,20), "key fire:", "GUI_Label");
	iFire = new GUI_Input(this, PG_Rect(CP_INPUT_X1+CCSX,CP_INPUT_Y1+80 , 100 ,20),5);
	lShield = new PG_Label(this, PG_Rect(CP_INPUT_X1    ,CP_INPUT_Y1+100, CCSX,20), "shield:", "GUI_Label");
	iShield = new GUI_Input(this, PG_Rect(CP_INPUT_X1+CCSX,CP_INPUT_Y1+100, 100 ,20),6);
	lWarp = new PG_Label(this, PG_Rect(CP_INPUT_X1    ,CP_INPUT_Y1+120, CCSX,20), "warp:", "GUI_Label");
	iWarp = new GUI_Input(this, PG_Rect(CP_INPUT_X1+CCSX,CP_INPUT_Y1+120, 100 ,20),7);
	lWeapon1 = new PG_Label(this, PG_Rect(CP_INPUT_X2    ,CP_INPUT_Y2, CCSX,20), "pistolka:", "GUI_Label");
	iWeapon1 = new GUI_Input(this, PG_Rect(CP_INPUT_X2+CCSX,CP_INPUT_Y2, 100 ,20),8);
	lWeapon2 = new PG_Label(this, PG_Rect(CP_INPUT_X2    ,CP_INPUT_Y2+20, CCSX,20), "becheromet:", "GUI_Label");
	iWeapon2 = new GUI_Input(this, PG_Rect(CP_INPUT_X2+CCSX,CP_INPUT_Y2+20, 100 ,20),9);
	lWeapon3 = new PG_Label(this, PG_Rect(CP_INPUT_X2    ,CP_INPUT_Y2+40, CCSX,20), "railgun:", "GUI_Label");
	iWeapon3 = new GUI_Input(this, PG_Rect(CP_INPUT_X2+CCSX,CP_INPUT_Y2+40, 100 ,20),10);
	lWeapon4 = new PG_Label(this, PG_Rect(CP_INPUT_X2    ,CP_INPUT_Y2+60, CCSX,20), "bombs:", "GUI_Label");
	iWeapon4 = new GUI_Input(this, PG_Rect(CP_INPUT_X2+CCSX,CP_INPUT_Y2+60, 100 ,20),11);
	lWeapon5 = new PG_Label(this, PG_Rect(CP_INPUT_X2    ,CP_INPUT_Y2+80, CCSX,20), "grenades:", "GUI_Label");
	iWeapon5 = new GUI_Input(this, PG_Rect(CP_INPUT_X2+CCSX,CP_INPUT_Y2+80, 100 ,20),12);
	bOK = new GUI_ButtonSmall(this, 1, PG_Rect(25,240,150,25), "OK");
	bCancel = new GUI_ButtonSmall(this, 2, PG_Rect(25+150+10,240,150,25), "BACK");

	num=inum;
	Default();

	CreatePlayerMenu->SetFont(MainFont);
	CreatePlayerMenu->SetTextFormat("CREATE PLAYER%d", num);

	lPName->SetAlignment(PG_TA_LEFT);
	lSkin->SetAlignment(PG_TA_LEFT);

	SkinCtrl->SkinPic->lName = eSkin;
	switch (num) {
	case 1: SkinCtrl->SkinPic->SetSkin(p1_skin.string); break;
	case 2: SkinCtrl->SkinPic->SetSkin(p2_skin.string); break;
	case 3: SkinCtrl->SkinPic->SetSkin(p3_skin.string); break;
	case 4: SkinCtrl->SkinPic->SetSkin(p4_skin.string); break;
	}

	lUp->SetAlignment(PG_TA_RIGHT);
	lDown->SetAlignment(PG_TA_RIGHT);
	lLeft->SetAlignment(PG_TA_RIGHT);
	lRight->SetAlignment(PG_TA_RIGHT);
	lFire->SetAlignment(PG_TA_RIGHT);
	lShield->SetAlignment(PG_TA_RIGHT);
	lWarp->SetAlignment(PG_TA_RIGHT);

	lWeapon1->SetAlignment(PG_TA_RIGHT);
	lWeapon2->SetAlignment(PG_TA_RIGHT);
	lWeapon3->SetAlignment(PG_TA_RIGHT);
	lWeapon4->SetAlignment(PG_TA_RIGHT);
	lWeapon5->SetAlignment(PG_TA_RIGHT);

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
  if (!p_key_up->string[0]) Cvar_SetValue(p_key_up->name, STD_KEY_UP);
  if (!p_key_down->string[0]) Cvar_SetValue(p_key_down->name, STD_KEY_DOWN);
  if (!p_key_left->string[0]) Cvar_SetValue(p_key_left->name, STD_KEY_LEFT);
  if (!p_key_right->string[0]) Cvar_SetValue(p_key_right->name, STD_KEY_RIGHT);
  if (!p_key_fire->string[0]) Cvar_SetValue(p_key_fire->name, STD_KEY_FIRE);
  if (!p_key_shield->string[0]) Cvar_SetValue(p_key_shield->name, STD_KEY_SHIELD);
  if (!p_key_warp->string[0]) Cvar_SetValue(p_key_warp->name, STD_KEY_WARP);
  if (!p_key_weapon1->string[0]) Cvar_SetValue(p_key_weapon1->name, STD_KEY_WEAPON1);
  if (!p_key_weapon2->string[0]) Cvar_SetValue(p_key_weapon2->name, STD_KEY_WEAPON2);
  if (!p_key_weapon3->string[0]) Cvar_SetValue(p_key_weapon3->name, STD_KEY_WEAPON3);
  if (!p_key_weapon4->string[0]) Cvar_SetValue(p_key_weapon4->name, STD_KEY_WEAPON4);
  if (!p_key_weapon5->string[0]) Cvar_SetValue(p_key_weapon5->name, STD_KEY_WEAPON5);

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
	
      CommandExecuteOut("p%i_name %s", num, ePName->GetText());
      CommandExecuteOut("p%i_skin %s", num, eSkin->GetText());
      CommandExecuteOut("p%i_key_up %d", num, iUp->GetSym());
      CommandExecuteOut("p%i_key_down %d", num, iDown->GetSym());
      CommandExecuteOut("p%i_key_left %d", num, iLeft->GetSym());
      CommandExecuteOut("p%i_key_right %d", num, iRight->GetSym());
      CommandExecuteOut("p%i_key_fire %d", num, iFire->GetSym());
      CommandExecuteOut("p%i_key_shield %d", num, iShield->GetSym());
      CommandExecuteOut("p%i_key_warp %d", num, iWarp->GetSym());
      CommandExecuteOut("p%i_key_weapon1 %d", num, iWeapon1->GetSym());
      CommandExecuteOut("p%i_key_weapon2 %d", num, iWeapon2->GetSym());
      CommandExecuteOut("p%i_key_weapon3 %d", num, iWeapon3->GetSym());
      CommandExecuteOut("p%i_key_weapon4 %d", num, iWeapon4->GetSym());
      CommandExecuteOut("p%i_key_weapon5 %d", num, iWeapon5->GetSym());

      for (int i=0; i<GAME_MAX_OBJS; i++)
      {
        if ((g->objs[i]->state&OSTATE_ACTIVE) && (g->objs[i]->GetType()==ot_player))
        {
          p = (GPlayer*)g->objs[i];
          if (strcmp(p->player_name.GetValRef()->chars, ePName->GetText())==0)
          {
            ConOut("Player %s found in game, reseting ...", p_name->string);
            SetKeybindings(p->kbindex, 0);
            CommandExecuteOut("sp %s %s", p_name->string, p_skin->string);
            CommandExecuteOut("chase %s", p_name->string);
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

bool GUI_CreatePlayerMenu::eventButtonClick(int id, PG_Widget* widget) {
	CGame& g = client_info.game;

	switch (id) {
	case 1:

		ConOut("");
		ConOutEx(MISC_FONT, "> CreatePlayer menu sequence <");
	
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
	
		ConOutEx(MISC_FONT, "> end of sequence <");
		ConOut("");

		while (GUI_id) GUI_Return(); // return from all menus
		break;
		
	case 2:
		Return();
		break;
	}
	
	return true;
}

bool GUI_CreatePlayerMenu::eventNextInput(int id, PG_Widget* widget)
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
			rc = eventButtonClick(msg->widget_id, (PG_Widget*)(msg->from));
			break;

		case MSG_SIGNAL:
			rc = eventNextInput(msg->widget_id, (PG_Widget*)(msg->from));
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

