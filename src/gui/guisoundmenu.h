#ifndef GUISOUNDMENU_H
#define GUISOUNDMENU_H

#include "guibasemenu.h"
#include "sdlslider.h"

class GUI_Board;
class GUI_Label;
class GUI_NumEdit;
class GUI_CheckBox;
class GUI_ButtonSmall;
class GUI_FloatEdit;

/////////////////////////////////////////////////////////////////////////////
// Sound menu widgets
/////////////////////////////////////////////////////////////////////////////
#define SM_PX 140
#define SM_PY 110
#define SM_VX 360
#define SM_VY 255


class GUI_SoundMenu : public GUI_BaseMenu {
public:
  GUI_SoundMenu();

  GUI_Board* Board1;
  GUI_Label* SoundMenu;
  GUI_Label* lRange;

  GUI_Label* lMusicVolume;
  SDLSlider* eMusicVolume;
  GUI_Label* lSoundVolume;
  SDLSlider* eSoundVolume;

  GUI_CheckBox* cSwap;
  GUI_ButtonSmall* b3ds;

  GUI_Label* l3dDist;
  GUI_FloatEdit* e3dDist;
  GUI_Label* l3dRoll;
  GUI_FloatEdit* e3dRoll;

  GUI_ButtonSmall* bSet;
  GUI_ButtonSmall* bCancel;

  void eventShow();
  void eventHide();

  void Default();
  void Return();

  bool eventButtonClick(int id, SDLWidget* widget);
};


#endif GUISOUNDMENU_H
