#ifndef GUICREATEPLAYERMENU_H
#define GUICREATEPLAYERMENU_H

#include "guibasemenu.h"
#include "cvar.h"

class GUI_Label;
class GUI_TextEdit;
class GUI_SkinCtrl;
class GUI_Input;
class GUI_ButtonSmall;
class CGame;

class GUI_CreatePlayerMenu : public GUI_BaseMenu {
public:
	GUI_CreatePlayerMenu(int id, int inum);
	int num;

	GUI_Label* CreatePlayerMenu;

	GUI_Label* lPName;
	GUI_TextEdit* ePName;
	GUI_Label* lSkin;
	GUI_TextEdit* eSkin;
	GUI_SkinCtrl* SkinCtrl;

	GUI_Input* iUp;
	GUI_Input* iDown;
	GUI_Input* iLeft;
	GUI_Input* iRight;
	GUI_Input* iFire;
	GUI_Input* iWarp;
	GUI_Input* iShield;

	GUI_Input* iWeapon1;
	GUI_Input* iWeapon2;
	GUI_Input* iWeapon3;
	GUI_Input* iWeapon4;
	GUI_Input* iWeapon5;

	GUI_Label* lUp;
	GUI_Label* lDown;
	GUI_Label* lLeft;
	GUI_Label* lRight;
	GUI_Label* lFire;
	GUI_Label* lWarp;
	GUI_Label* lShield;

	GUI_Label* lWeapon1;
	GUI_Label* lWeapon2;
	GUI_Label* lWeapon3;
	GUI_Label* lWeapon4;
	GUI_Label* lWeapon5;

	GUI_ButtonSmall* bOK;
	GUI_ButtonSmall* bCancel;

	void Default();
	void Return();

	void NextBox();

protected:

	bool eventButtonClick(int id, PG_Widget* widget);
	bool eventNextInput(int id, PG_Widget* widget);
	bool eventMessage(MSG_MESSAGE* msg);
	void eventShow();
	void eventHide();

	void SetPlayerDefaults(
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
				);

	bool SetPlayerVals(
				int num,
				CGame* g,
				cvar_t* p_name,
				cvar_t* p_skin
				);
};

#endif	// GUICREATEPLAYERMENU_H
