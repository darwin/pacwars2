#ifndef GUISOUNDMENU_H
#define GUISOUNDMENU_H

#include "guibasemenu.h"
#include "pgslider.h"
#include "pglabel.h"

class GUI_Board;
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
  PG_Label* SoundMenu;
  PG_Label* lRange;

  PG_Label* lMusicVolume;
  PG_Slider* eMusicVolume;
  PG_Label* lSoundVolume;
  PG_Slider* eSoundVolume;

  GUI_CheckBox* cSwap;
  GUI_ButtonSmall* b3ds;

  PG_Label* l3dDist;
  GUI_FloatEdit* e3dDist;
  PG_Label* l3dRoll;
  GUI_FloatEdit* e3dRoll;

  GUI_ButtonSmall* bSet;
  GUI_ButtonSmall* bCancel;

  void eventShow();
  void eventHide();

  void Default();
  void Return();

  bool eventButtonClick(int id, PG_Widget* widget);
};


#endif // GUISOUNDMENU_H
