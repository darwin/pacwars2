
#include "guisoundmenu.h"
#include "gui.h"
#include "pw2.h"
#include "sampleman.h"
#include "SDL_Console.h"

extern GUI_OKDialog2 *OKD2;

/////////////////////////////////////////////////////////////////////////////
// Sound menu widgets
/////////////////////////////////////////////////////////////////////////////

void GUI_SoundMenu::Return()
{
  //eMusicVolume->EditEnd();
  //eSoundVolume->EditEnd();
  GUI_BaseMenu::Return();
}

GUI_SoundMenu::GUI_SoundMenu():
GUI_BaseMenu(GUI_SOUND, mkrect(SM_PX,SM_PY,SM_VX,SM_VY)) /*,
Board1(NULL, SDLWidget::mkrect(SM_PX,SM_PY,SM_VX, SM_VY), false),
lRange(NULL, SDLWidget::mkrect(SM_PX+1,SM_PY+30,SM_VX-2,20), "Enter volume in range [0-64].", false),
SoundMenu(NULL, SDLWidget::mkrect(SM_PX+1,SM_PY+4,SM_VX-2,25), "SOUND OPTIONS", false),
lMusicVolume(NULL, SDLWidget::mkrect(SM_PX+1,SM_PY+65, 119,20), "Music volume:", false),
eMusicVolume(NULL, SDLWidget::mkrect(SM_PX+120,SM_PY+65,215,20), 0, 64),
lSoundVolume(NULL, SDLWidget::mkrect(SM_PX+1,SM_PY+90, 119,20), "Sound volume:", false),
eSoundVolume(NULL, SDLWidget::mkrect(SM_PX+120,SM_PY+90,215,20), 0, 64),
b3ds(NULL, 3, SDLWidget::mkrect(SM_PX+25,SM_PY+120,310,25), "ENABLE/DISABLE 3D sound engine"),
cSwap(NULL, SDLWidget::mkrect(SM_PX+25,SM_PY+147,310,CB_SIZEY), "swap left/right sound channel", true, GUI_Gray64),
l3dDist(NULL, SDLWidget::mkrect(SM_PX+1,SM_PY+170, 119,20), "3D distance step:", false),
e3dDist(NULL, SDLWidget::mkrect(SM_PX+120,SM_PY+170,215,20), 0, 50),
l3dRoll(NULL, SDLWidget::mkrect(SM_PX+1,SM_PY+195, 119,20), "3D sound rollover:", false),
e3dRoll(NULL, SDLWidget::mkrect(SM_PX+120,SM_PY+195,215,20), 0, 50),
bSet(NULL, 1, SDLWidget::mkrect(SM_PX+25,SM_PY+225,150, 25), "APPLY"),
bCancel(NULL, 2, SDLWidget::mkrect(SM_PX+25+150+10,SM_PY+225,150,25), "BACK")
*/
{

	lRange = new GUI_Label(this, SDLWidget::mkrect(1,30,SM_VX-2,20), "Enter volume in range [0-64].", false);
	SoundMenu = new GUI_Label(this, SDLWidget::mkrect(1,4,SM_VX-2,25), "SOUND OPTIONS", false);
	lMusicVolume = new GUI_Label(this, SDLWidget::mkrect(1,65, 119,20), "Music volume:", false);
	eMusicVolume = new SDLSlider(this, 4, SDLWidget::mkrect(120,65,215,20),  false);
	lSoundVolume = new GUI_Label(this, SDLWidget::mkrect(1,90, 119,20), "Sound volume:", false);
	eSoundVolume = new SDLSlider(this, 5, SDLWidget::mkrect(120,90,215,20), 0);
	b3ds = new GUI_ButtonSmall(this, 3, SDLWidget::mkrect(25,120,310,25), "ENABLE/DISABLE 3D sound engine");
	cSwap = new GUI_CheckBox(this, SDLWidget::mkrect(25,147,310,CB_SIZEY), "swap left/right sound channel", true, GUI_Gray64);
	l3dDist = new GUI_Label(this, SDLWidget::mkrect(1,170, 119,20), "3D distance step:", false);
	e3dDist = new GUI_FloatEdit(this, SDLWidget::mkrect(120,170,215,20), 0, 50);
	l3dRoll = new GUI_Label(this, SDLWidget::mkrect(1,195, 119,20), "3D sound rollover:", false);
	e3dRoll = new GUI_FloatEdit(this, SDLWidget::mkrect(120,195,215,20), 0, 50);
	bSet = new GUI_ButtonSmall(this, 1, SDLWidget::mkrect(25,225,150, 25), "APPLY");
	bCancel = new GUI_ButtonSmall(this, 2, SDLWidget::mkrect(25+150+10,225,150,25), "BACK");

	Board1 = new GUI_Board(this, SDLWidget::mkrect(0,0,SM_VX, SM_VY), false);

  Default();

  SoundMenu->SetColor(GUI_BtnTextColor, GUI_BtnATextColor);
  SoundMenu->SetFont(MainFont);

  SoundMenu->bgmode = 2;

  lRange->bgmode = 2;
  lMusicVolume->SetAlignment(SDL_TA_RIGHT);
  lMusicVolume->shiftx = -4;
  l3dDist->SetAlignment(SDL_TA_RIGHT);
  l3dDist->shiftx = -4;
  l3dRoll->SetAlignment(SDL_TA_RIGHT);
  l3dRoll->shiftx = -4;
  lSoundVolume->SetAlignment(SDL_TA_RIGHT);
  lSoundVolume->shiftx = -4;

	/*
  AddChild(&Board1);
  AddChild(&SoundMenu);
  AddChild(&lRange);
  AddChild(&lMusicVolume);
  AddChild(&eMusicVolume);
  AddChild(&lSoundVolume);
  AddChild(&eSoundVolume);
  AddChild(&b3ds);
  AddChild(&cSwap);
  AddChild(&l3dDist);
  AddChild(&e3dDist);
  AddChild(&l3dRoll);
  AddChild(&e3dRoll);
  AddChild(&bSet);
  AddChild(&bCancel);
	*/
}

void GUI_SoundMenu::Default()
{
	eMusicVolume->SetRange(0,64);
	eMusicVolume->SetPosition((int)music_volume.value);
	//eMusicVolume->SetTextFormat("%i", (int)music_volume.value);
	eSoundVolume->SetRange(0,64);
	eSoundVolume->SetPosition((int)sound_volume.value);
	//eSoundVolume->SetTextFormat("%i", (int)sound_volume.value);
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
    ConOut("> Sound menu sequence <");

    ConOut("sound_volume %i", eSoundVolume->GetPosition());
    CommandExecute("sound_volume %i", eSoundVolume->GetPosition());
    ConOut("music_volume %i", eMusicVolume->GetPosition());
    CommandExecute("music_volume %i", eMusicVolume->GetPosition());

    ConOut("snd_dist %s", e3dDist->GetText());
    CommandExecute("snd_dist %s", e3dDist->GetText());
    ConOut("snd_roll %s", e3dRoll->GetText());
    CommandExecute("snd_roll %s", e3dRoll->GetText());

    if (cSwap->GetPressed())
    {
      ConOut("snd_swap 1");
      CommandExecute("snd_swap 1");
    }
    else
    {
      ConOut("snd_swap 0");
      CommandExecute("snd_swap 0");
    }

    ConOut("> end of sequence <");
    ConOut("");

    GUI_Return();
    break;
  case 2:
    Return();
    break;
/*  case 4:
    if (snd_swapLR.value==1.0)
    {
      Cvar_SetValue("snd_swap", 0);
    }
    else
    {
      Cvar_SetValue("snd_swap", 1);
    }
    OKD1->Reset("SWAPPED LEFT/RIGHT", "The direction of 3d sound output was reversed.");
    GUI_OpenMenu(GUI_OKDIALOG1);

    break;*/
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

void GUI_SoundMenu::eventShow()
{
  Clear();

/*
  Board1.Show();
  SoundMenu.Show();

  eSoundVolume.Show();
  lSoundVolume.Show();
  eMusicVolume.Show();
  lMusicVolume.Show();
  lRange.Show();
  b3ds.Show();
  cSwap.Show();
  e3dDist.Show();
  l3dDist.Show();
  e3dRoll.Show();
  l3dRoll.Show();

  bSet.Show();
  bCancel.Show();
*/
}

void GUI_SoundMenu::eventHide()
{
/*
  Board1.Hide();
  SoundMenu.Hide();

  eSoundVolume.Hide();
  lSoundVolume.Hide();
  eMusicVolume.Hide();
  lMusicVolume.Hide();
  lRange.Hide();

  cSwap.Hide();
  e3dDist.Hide();
  l3dDist.Hide();
  e3dRoll.Hide();
  l3dRoll.Hide();
  b3ds.Hide();
  bSet.Hide();
  bCancel.Hide();
*/
}

