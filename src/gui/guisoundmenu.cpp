
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
  SDLSlider* s = (SDLSlider*)clientdata;
  CommandExecute("music_volume %i", s->GetPosition());
	return true;
}

PARAGUI_CALLBACK(handler_slider_sound) {
  SDLSlider* s = (SDLSlider*)clientdata;
  CommandExecute("sound_volume %i", s->GetPosition());
  smPlayVoice(SM_MESSENGER, 100, 11);
	return true;
}

void GUI_SoundMenu::Return() {
	GUI_BaseMenu::Return();
}

GUI_SoundMenu::GUI_SoundMenu() : GUI_BaseMenu(GUI_SOUND, mkrect(SM_PX,SM_PY,SM_VX,SM_VY)) {

	SoundMenu = new GUI_Label(this, SDLWidget::mkrect(1,4,SM_VX-2,25), "SOUND OPTIONS", false);
	lRange = new GUI_Label(this, SDLWidget::mkrect(1,30,SM_VX-2,20), "Edit sound properties", false);
	lMusicVolume = new GUI_Label(this, SDLWidget::mkrect(1,65, 119,20), "Music volume:", false);
	eMusicVolume = new SDLSlider(this, 4, SDLWidget::mkrect(120,65+5,215,20),  false);
	lSoundVolume = new GUI_Label(this, SDLWidget::mkrect(1,90, 119,20), "Sound volume:", false);
	eSoundVolume = new SDLSlider(this, 5, SDLWidget::mkrect(120,90+5,215,20), 0);
	b3ds = new GUI_ButtonSmall(this, 3, SDLWidget::mkrect(25,120,310,25), "ENABLE/DISABLE 3D sound engine");
	cSwap = new GUI_CheckBox(this, SDLWidget::mkrect(25,147,310,CB_SIZEY), "swap left/right sound channel", true, GUI_Gray64);
	l3dDist = new GUI_Label(this, SDLWidget::mkrect(1,170, 119,20), "3D distance step:", false);
	e3dDist = new GUI_FloatEdit(this, SDLWidget::mkrect(120,170,215,20), 0, 50);
	l3dRoll = new GUI_Label(this, SDLWidget::mkrect(1,195, 119,20), "3D sound rollover:", false);
	e3dRoll = new GUI_FloatEdit(this, SDLWidget::mkrect(120,195,215,20), 0, 50);
	bSet = new GUI_ButtonSmall(this, 1, SDLWidget::mkrect(25,225,150, 25), "APPLY");
	bCancel = new GUI_ButtonSmall(this, 2, SDLWidget::mkrect(25+150+10,225,150,25), "BACK");

	Default();

	SoundMenu->SetFont(MainFont);

	// there are some troubles with following the slider in double buffered mode.
	// sliding blocks the surface flip
	eMusicVolume->SetEventCallback(MSG_SLIDEEND, handler_slider_music, eMusicVolume);
	eSoundVolume->SetEventCallback(MSG_SLIDEEND, handler_slider_sound, eSoundVolume);

  lMusicVolume->SetAlignment(SDL_TA_RIGHT);
	l3dDist->SetAlignment(SDL_TA_RIGHT);
	l3dRoll->SetAlignment(SDL_TA_RIGHT);
	lSoundVolume->SetAlignment(SDL_TA_RIGHT);

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

bool GUI_SoundMenu::eventButtonClick(int id, SDLWidget* widget)
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

