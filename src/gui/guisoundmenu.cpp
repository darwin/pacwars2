
#include "guisoundmenu.h"
#include "gui.h"
#include "pw2.h"
#include "sampleman.h"
#include "SDL_Console.h"

extern GUI_OKDialog2 *OKD2;

/////////////////////////////////////////////////////////////////////////////
// Sound menu widgets
/////////////////////////////////////////////////////////////////////////////

PARAGUI_CALLBACK(handler_slider_music) {
  PG_Slider* s = (PG_Slider*)clientdata;
  CommandExecute("music_volume %i", s->GetPosition());
	return true;
}

PARAGUI_CALLBACK(handler_slider_sound) {
  PG_Slider* s = (PG_Slider*)clientdata;
  CommandExecute("sound_volume %i", s->GetPosition());
  smPlayVoice(SM_MESSENGER, 100, 11);
	return true;
}

void GUI_SoundMenu::Return() {
	GUI_BaseMenu::Return();
}

GUI_SoundMenu::GUI_SoundMenu() : GUI_BaseMenu(GUI_SOUND, PG_Rect(SM_PX,SM_PY,SM_VX,SM_VY)) {

	SoundMenu = new PG_Label(this, PG_Rect(1,4,SM_VX-2,25), "SOUND OPTIONS", "GUI_Label");
	lRange = new PG_Label(this, PG_Rect(1,30,SM_VX-2,20), "Edit sound properties", "GUI_Label");
	lMusicVolume = new PG_Label(this, PG_Rect(1,65, 119,20), "Music volume:", "GUI_Label");
	eMusicVolume = new PG_Slider(this, 4, PG_Rect(120,65+5,215,20),  false);
	lSoundVolume = new PG_Label(this, PG_Rect(1,90, 119,20), "Sound volume:", "GUI_Label");
	eSoundVolume = new PG_Slider(this, 5, PG_Rect(120,90+5,215,20), 0);
	b3ds = new GUI_ButtonSmall(this, 3, PG_Rect(25,120,310,25), "ENABLE/DISABLE 3D sound engine");
	cSwap = new GUI_CheckBox(this, PG_Rect(25,147,310,CB_SIZEY), "swap left/right sound channel", true, GUI_Gray64);
	l3dDist = new PG_Label(this, PG_Rect(1,170, 119,20), "3D distance step:", "GUI_Label");
	e3dDist = new GUI_FloatEdit(this, PG_Rect(120,170,215,20), 0, 50);
	l3dRoll = new PG_Label(this, PG_Rect(1,195, 119,20), "3D sound rollover:", "GUI_Label");
	e3dRoll = new GUI_FloatEdit(this, PG_Rect(120,195,215,20), 0, 50);
	bSet = new GUI_ButtonSmall(this, 1, PG_Rect(25,225,150, 25), "APPLY");
	bCancel = new GUI_ButtonSmall(this, 2, PG_Rect(25+150+10,225,150,25), "BACK");

	Default();

	SoundMenu->SetFont(MainFont);

	// there are some troubles with following the slider in double buffered mode.
	// sliding blocks the surface flip
	eMusicVolume->SetEventCallback(MSG_SLIDEEND, handler_slider_music, eMusicVolume);
	eSoundVolume->SetEventCallback(MSG_SLIDEEND, handler_slider_sound, eSoundVolume);

  lMusicVolume->SetAlignment(PG_TA_RIGHT);
	l3dDist->SetAlignment(PG_TA_RIGHT);
	l3dRoll->SetAlignment(PG_TA_RIGHT);
	lSoundVolume->SetAlignment(PG_TA_RIGHT);

	LoadThemeStyle("GUI_Board");
}

void GUI_SoundMenu::Default()
{
	eMusicVolume->SetRange(0,64);
	eMusicVolume->SetPosition((int)music_volume.value);
	eSoundVolume->SetRange(0,64);
	eSoundVolume->SetPosition((int)sound_volume.value);
	e3dDist->SetTextFormat("%.3g", snd_dist.value);
	e3dRoll->SetTextFormat("%.3g", snd_roll.value);

	if (snd_swap.value) {
		cSwap->SetPressed();
	}
	else {
		cSwap->SetUnpressed();
	}
}

bool GUI_SoundMenu::eventButtonClick(int id, PG_Widget* widget)
{
  switch (id) {
  case 1:
    ConOut("");
    ConOutEx(MISC_FONT, "> Sound menu sequence <");

    ConOut("sound_volume %i", eSoundVolume->GetPosition());
    ConOut("music_volume %i", eMusicVolume->GetPosition());

    CommandExecuteOut("snd_dist %s", e3dDist->GetText());
    CommandExecuteOut("snd_roll %s", e3dRoll->GetText());

    if (cSwap->GetPressed())
    {
      CommandExecuteOut("snd_swap 1");
    }
    else
    {
      CommandExecuteOut("snd_swap 0");
    }

    ConOutEx(MISC_FONT, "> end of sequence <");
    ConOut("");

    GUI_Return();
    break;
  case 2:
    Return();
    break;
  case 3:
    if (snd_3don.value==1.0)
    {
      Cvar_SetValue("snd_3don", 0);
      OKD2->Reset("3D SOUND", "3D sound was disabled.", "This setting will take effect after restarting program.");
      GUI_OpenMenu(GUI_OKDIALOG2);
    }
    else
    {
      Cvar_SetValue("snd_3don", 1);
      OKD2->Reset("3D SOUND", "3D sound was enabled.", "This setting will take effect after restarting program.");
      GUI_OpenMenu(GUI_OKDIALOG2);
    }

    break;
  }
  return true;
}

void GUI_SoundMenu::eventShow() {
	Clear();
}

void GUI_SoundMenu::eventHide() {
}

