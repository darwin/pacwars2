//###########################################################################
//## GUI methods
//## 
//## 
//###########################################################################

#ifndef GUI_H
#define GUI_H

#include "paragui.h"
#include "pggradientwidget.h"
#include "pglineedit.h"
#include "pgbutton.h"
#include "pglabel.h"
#include "pgcheckbutton.h"
#include "pgscrollbar.h"
#include "pgwidgetlist.h"
#include "pgslider.h"

#include "guibasemenu.h"
#include "guisoundmenu.h"
#include "guinewgamemenu.h"
#include "guiservermenu.h"
#include "guiclientmenu.h"
#include "guivideomenu.h"
#include "guipathsmenu.h"
#include "guimainmenu.h"
#include "guioptionsmenu.h"
#include "guijoingamemenu.h"
#include "guicreditsmenu.h"
#include "guicreateplayermenu.h"
#include "guideleteplayermenu.h"
#include "guiresultsmenu.h"
#include "guicreateplayerselmenu.h"
#include "guiredefinekeysmenu.h"

#include "guiyndialog.h"

#include "smapman.h"
#include "sprite.h"

#define GUI_NOMENU        0
#define GUI_MAINMENU      1
#define GUI_NEWGAME       2
#define GUI_JOINGAME      3
#define GUI_CREATEPLAYER  4
#define GUI_DELETEPLAYER  5
#define GUI_OPTIONS       6
#define GUI_PATHS         8
#define GUI_SOUND         9
#define GUI_QUIT          10
#define GUI_CREDITS       11
#define GUI_CREATEPLAYER1 12
#define GUI_CREATEPLAYER2 13
#define GUI_CREATEPLAYER3 14
#define GUI_CREATEPLAYER4 15
#define GUI_OKDIALOG1     16
#define GUI_OKDIALOG2     17
#define GUI_YNDIALOG      18
#define GUI_HELP1         19
#define GUI_HELP2         20
#define GUI_HELP3         21

#define GUI_SERVER        30
#define GUI_VIDEO         31
#define GUI_CLIENT        32
#define GUI_REDEFINEKEYS  33

#define GUI_RESULTS       200

// forward declarations
void GUI_Show(int id);
void GUI_Return();
void GUI_OpenMenu(int id);
void GUI_InitColors(SDL_Surface* screen);

class GUI_BaseMenu;

// Menu fonts
extern TTF_Font* MainFont;
extern TTF_Font* MainFont2;
extern TTF_Font* BtnFont;
extern TTF_Font* BtnFont2;

extern SDL_Color GUI_BtnTextColor;
extern SDL_Color GUI_BtnATextColor;
extern SDL_Color GUI_LabelColor;
extern SDL_Color GUI_ResultLineColor;

extern SDL_Color GUI_Black;
extern SDL_Color GUI_Gray64;
extern SDL_Color GUI_Blue80;
extern SDL_Color GUI_Blue100;
extern SDL_Color GUI_Blue120;
extern SDL_Color GUI_Red80;
extern SDL_Color GUI_Red100;
extern SDL_Color GUI_Red120;
extern SDL_Color GUI_Green80;
extern SDL_Color GUI_Green100;
extern SDL_Color GUI_Green120;
extern SDL_Color GUI_InputActive;
extern SDL_Color GUI_InputInactive;
extern SDL_Color GUI_SmBtColor;
extern SDL_Color GUI_UnselectedItem;
extern SDL_Color GUI_SelectedItem;

class GUI_OKDialog1;
class GUI_OKDialog2;
class GUI_YNDialog;

extern GUI_OKDialog1 *OKD1;
extern GUI_OKDialog2 *OKD2;
extern GUI_YNDialog *YND;

/////////////////////////////////////////////////////////////////////////////
// Board
/////////////////////////////////////////////////////////////////////////////

class GUI_Board : public PG_GradientWidget {
public:
  GUI_Board(PG_Widget* parent, const PG_Rect& r, char* theme=NULL);
};

/////////////////////////////////////////////////////////////////////////////
// Bitmap
/////////////////////////////////////////////////////////////////////////////

class GUI_Bitmap : public PG_Widget {
public:
  GUI_Bitmap(PG_Widget* parent, const PG_Rect& r, SDL_Surface* s=NULL);
  void eventDraw(SDL_Surface* surface, const PG_Rect& rect);
  
  SDL_Surface *bitmap;
  bool drawbackground;
};

/////////////////////////////////////////////////////////////////////////////
// Label
/////////////////////////////////////////////////////////////////////////////

class GUI_LabelL : public PG_Label {
public:
  GUI_LabelL(PG_Widget* parent, const PG_Rect& r, char* text, GUI_LabelL** iselected, CSMapInfo* si, SDL_Color ic1, SDL_Color ic2, void (*cb)(GUI_LabelL*)=NULL);
  void eventDraw(SDL_Surface* surface, const PG_Rect& rect);
  bool eventMouseButtonDown(const SDL_MouseButtonEvent* button);


  CSMapInfo* msi;
  SDL_Color c1;
  SDL_Color c2;
  void (*callback)(GUI_LabelL*);
  
  GUI_LabelL** selected;
};

/////////////////////////////////////////////////////////////////////////////
// Buttons
/////////////////////////////////////////////////////////////////////////////

class GUI_Button : public PG_Button {
public:
  GUI_Button(PG_Widget* parent, int btnid, const PG_Rect& r, char* text);
  
  SDL_Color c;
  bool drawbackground;
};

class GUI_ButtonBig : public GUI_Button {
public:
  GUI_ButtonBig(PG_Widget* parent, int btnid, const PG_Rect& r, char* text);
  void eventMouseLeave();
  void eventMouseEnter();
	bool eventMouseButtonDown(const SDL_MouseButtonEvent* button);
	bool eventMouseButtonUp(const SDL_MouseButtonEvent* button);

public:
  bool enabled;

};

class GUI_ButtonSmall : public GUI_Button {
public:
  GUI_ButtonSmall(PG_Widget* parent, int btnid, const PG_Rect& r, char* text);
  void eventMouseLeave();
  void eventMouseEnter();
};


/////////////////////////////////////////////////////////////////////////////
// Checkbox
/////////////////////////////////////////////////////////////////////////////

#define CB_SIZEY 20
class DECLSPEC GUI_CheckBox : public PG_CheckButton {
public: 
	
	GUI_CheckBox(PG_Widget* parent, const PG_Rect& r, char* text, bool ipressed=false, SDL_Color bg = GUI_Black);
	~GUI_CheckBox();

  void eventMouseEnter();
  void eventMouseLeave();
};


/////////////////////////////////////////////////////////////////////////////
// TextEdit
/////////////////////////////////////////////////////////////////////////////

class GUI_TextEdit : public PG_LineEdit {
public:
  GUI_TextEdit(PG_Widget* parent, const PG_Rect& r);
};


/////////////////////////////////////////////////////////////////////////////
// NumEdit
/////////////////////////////////////////////////////////////////////////////

class GUI_NumEdit : public GUI_TextEdit {
public:
  GUI_NumEdit(PG_Widget* parent, const PG_Rect& r, int imin, int imax);
  
  int min, max;
};


/////////////////////////////////////////////////////////////////////////////
// FloatEdit
/////////////////////////////////////////////////////////////////////////////

class GUI_FloatEdit : public GUI_TextEdit {
public:
  GUI_FloatEdit(PG_Widget* parent, const PG_Rect& r, float imin, float max);
  
  float min, max;
};


/////////////////////////////////////////////////////////////////////////////
// OK dialog widgets
/////////////////////////////////////////////////////////////////////////////
#define OKD1_PX 140
#define OKD1_PY 140
#define OKD1_VX 360
#define OKD1_VY 95


class GUI_OKDialog1 : public GUI_BaseMenu {
public:
  GUI_OKDialog1(char* title, char* line1="", char* line2="");

  void Reset(char* title, char* line1="");
  
  PG_Label* OKDialog1;
  PG_Label* lPrompt1;
  GUI_ButtonSmall* bOK;

  GUI_Board* Board1;

  void Default();
  
  bool eventButtonClick(int id, PG_Widget* widget);
};


/////////////////////////////////////////////////////////////////////////////
// OK dialog widgets
/////////////////////////////////////////////////////////////////////////////
#define OKD2_PX 140
#define OKD2_PY 140
#define OKD2_VX 360
#define OKD2_VY 120


class GUI_OKDialog2 : public GUI_BaseMenu {
public:
  GUI_OKDialog2(char* title, char* line1="", char* line2="");

  void Reset(char* title, char* line1="", char* line2="");
  
  PG_Label* OKDialog2;
  PG_Label* lPrompt1;
  PG_Label* lPrompt2;
  GUI_ButtonSmall* bOK;

  GUI_Board* Board1;

  void Default();
  
  bool eventButtonClick(int id, PG_Widget* widget);
};


/////////////////////////////////////////////////////////////////////////////
// OK dialog widgets
/////////////////////////////////////////////////////////////////////////////
#define YN_PX 140
#define YN_PY 140
#define YN_VX 360
#define YN_VY 95


/////////////////////////////////////////////////////////////////////////////
// Main menu widgets
/////////////////////////////////////////////////////////////////////////////
#define MM_PX 180
#define MM_PY 40
#define MM_VX 280
#define MM_VY 400


/////////////////////////////////////////////////////////////////////////////
// JoinGame menu widgets
/////////////////////////////////////////////////////////////////////////////
#define JG_PX 140
#define JG_PY 165
#define JG_VX 360
#define JG_VY 170

extern GUI_JoinGameMenu* JGMenu;


/////////////////////////////////////////////////////////////////////////////
// Credits menu widgets
/////////////////////////////////////////////////////////////////////////////
#define CM_PX 90
#define CM_PY 30
#define CM_VX 460
#define CM_VY 420

#define CM_G1_PX (8)
#define CM_G1_PY (0)
#define CM_G1_VX (CM_VX/2)

#define CM_G1X_PX (52)
#define CM_G1X_PY (0)


#define CM_G2_PX (10+CM_VX/2+10)
#define CM_G2_PY (-120)
#define CM_G2_VX (CM_VX/2)


/////////////////////////////////////////////////////////////////////////////
// Options menu widgets
/////////////////////////////////////////////////////////////////////////////
#define OM_PX 180
#define OM_PY 80
#define OM_VX 280
#define OM_VY 320


/////////////////////////////////////////////////////////////////////////////
// CreatePlayers menu widgets
/////////////////////////////////////////////////////////////////////////////
#define CPS_PX 180
#define CPS_PY 130
#define CPS_VX 280
#define CPS_VY 240


/////////////////////////////////////////////////////////////////////////////
// Paths menu widgets
/////////////////////////////////////////////////////////////////////////////
#define PM_PX 140
#define PM_PY 120
#define PM_VX 360
#define PM_VY 245
#define PM_BASE 65


/////////////////////////////////////////////////////////////////////////////
// CreatePlayer menu widgets
/////////////////////////////////////////////////////////////////////////////
#define CP_PX 120
#define CP_PY 90
#define CP_VX 400
#define CP_VY 270

#define CP_SK_X    (CP_VX-34-10)
#define CP_SK_Y    (40)

/////////////////////////////////////////////////////////////////////////////
// Skin widgets
/////////////////////////////////////////////////////////////////////////////

class GUI_ButtonS : public PG_Widget {
public:
  GUI_ButtonS(PG_Widget* parent, int btnid, const PG_Rect& r);
  
  void eventDraw(SDL_Surface* surface, const PG_Rect& rect);
  bool eventMouseButtonDown(const SDL_MouseButtonEvent* button);
  bool eventMouseButtonUp(const SDL_MouseButtonEvent* button);
  
  bool SetBackground(const char* filename, int mode);
  
  SDL_Surface* background;
  
  int id;
};


class GUI_SkinPic : public PG_Widget {
public:
  GUI_SkinPic(PG_Widget* parent, const PG_Rect& r);
  ~GUI_SkinPic();

  void eventDraw(SDL_Surface* surface, const PG_Rect& rect);

  GUI_TextEdit *lName;
  CSpriteInfo* sprite;

  bool NextSkin();
  bool PrevSkin();
  void SetSkin(char* name);
};



class GUI_SkinCtrl : public PG_Widget {
public:
  GUI_SkinCtrl(PG_Widget* parent, const PG_Rect& r);
  
//  void eventDraw(SDL_Surface* surface, const PG_Rect& rect);
  bool eventButtonClick(int id, PG_Widget* widget);
  
  GUI_SkinPic* SkinPic;
  PG_Button* bNext;
  PG_Button* bPrev;
  
  SDL_Color c;
  bool drawbackground;
};


class GUI_Input : public PG_StaticFrame{
public:
	GUI_Input(PG_Widget* parent, const PG_Rect& r, int iid);
	~GUI_Input();

	void InputBegin();
	void InputEnd();
	char* GetText();
	void SetText(char* new_text);

  SDL_Color c_color;

	void eventBlit(SDL_Surface* surface, const PG_Rect& src, const PG_Rect& dst);
	bool eventKeyDown(const SDL_KeyboardEvent* key);
	bool eventMouseButtonDown(const SDL_MouseButtonEvent* button);
	void eventInputFocusLost(PG_MessageObject* newfocus);

	void DrawText();
	char* GetDrawText();

  Uint16 GetSym() { return sym; }
  void SetSym(Uint16 s) { sym=s; }
	
	char text[256];
	bool waiting;
	int offset_x;
  int id;

  Uint16 sym;

	SDL_Color textcolor;
};

#define CP_INPUT_X1 (5)
#define CP_INPUT_Y1 (90)

#define CP_INPUT_X2 (170)
#define CP_INPUT_Y2 (90)

#define CCSX 60

/////////////////////////////////////////////////////////////////////////////
// Redefine Keys menu widgets
/////////////////////////////////////////////////////////////////////////////
#define RK_PX 140
#define RK_PY 100
#define RK_VX 360
#define RK_VY 250

#define RK_SK_X    (RK_VX-34-10)
#define RK_SK_Y    (40)

#define RK_INPUT_X1 (60)
#define RK_INPUT_Y1 (60)

#define RK_INPUT_X2 (170)
#define RK_INPUT_Y2 (90)

#define RKSX 80

/////////////////////////////////////////////////////////////////////////////
// Results menu widgets
/////////////////////////////////////////////////////////////////////////////
#define RM_PX 280
#define RM_PY 20
#define RM_VX 280
#define RM_VY 250

/////////////////////////////////////////////////////////////////////////////
// Help1 menu widgets
/////////////////////////////////////////////////////////////////////////////
#define HM1_PX 114
#define HM1_PY 60
#define HM1_VX 412
#define HM1_VY 357


class GUI_Help1Menu : public GUI_BaseMenu {
public:
  GUI_Help1Menu();
  
  GUI_Board Board1;
  GUI_Bitmap mBack1;
  PG_Label Help1Menu;
  
  PG_Label lLine1a;
  PG_Label lLine1b;
  PG_Label lLine1c;

  PG_Label lLine2a;
  PG_Label lLine2b;
  PG_Label lLine2c;

  PG_Label lLine3a;
  PG_Label lLine3b;
  PG_Label lLine3c;
  
  PG_Label lLine4a;
  PG_Label lLine4b;
  PG_Label lLine4c;

  PG_Label lLine5a;
  PG_Label lLine5b;
  PG_Label lLine5c;

  GUI_ButtonSmall bReturn;
  GUI_ButtonSmall bNext;
  //GUI_ButtonSmall bPrev;

  PG_Label lI1a;
  PG_Label lI1b;
  PG_Label lI2a;
  PG_Label lI2b;
  PG_Label lI3a;
  PG_Label lI3b;
  PG_Label lI4a;
  PG_Label lI4b;
  PG_Label lI5a;
  PG_Label lI5b;
  PG_Label lI6a;
  PG_Label lI6b;
  

  void Show();
  void Hide();
  void Default();
  
  bool eventButtonClick(int id, PG_Widget* widget);
};

#define HM1_LY1 12

#define HM1_G1_PX (HM1_PX+76)
#define HM1_G1_PY (HM1_PY+48)
#define HM1_G1_VX (210)

#define HM1_G2_PX (HM1_PX+76)
#define HM1_G2_PY (HM1_PY+101)
#define HM1_G2_VX (210)

#define HM1_G3_PX (HM1_PX+76)
#define HM1_G3_PY (HM1_PY+154)
#define HM1_G3_VX (210)

#define HM1_G4_PX (HM1_PX+76)
#define HM1_G4_PY (HM1_PY+207)
#define HM1_G4_VX (210)

#define HM1_G5_PX (HM1_PX+76)
#define HM1_G5_PY (HM1_PY+259)
#define HM1_G5_VX (210)


#define HM1_LY2 12

#define HM1_I1_PX (HM1_PX+338)
#define HM1_I1_PY (HM1_PY+47)
#define HM1_I1_VX (70)

#define HM1_I2_PX (HM1_PX+338)
#define HM1_I2_PY (HM1_PY+94)
#define HM1_I2_VX (70)

#define HM1_I3_PX (HM1_PX+338)
#define HM1_I3_PY (HM1_PY+139)
#define HM1_I3_VX (70)

#define HM1_I4_PX (HM1_PX+338)
#define HM1_I4_PY (HM1_PY+185)
#define HM1_I4_VX (70)

#define HM1_I5_PX (HM1_PX+338)
#define HM1_I5_PY (HM1_PY+231)
#define HM1_I5_VX (70)

#define HM1_I6_PX (HM1_PX+338)
#define HM1_I6_PY (HM1_PY+278)
#define HM1_I6_VX (70)


/////////////////////////////////////////////////////////////////////////////
// Help2 menu widgets
/////////////////////////////////////////////////////////////////////////////
#define HM2_PX 110
#define HM2_PY 45
#define HM2_VX 422
#define HM2_VY 392


class GUI_Help2Menu : public GUI_BaseMenu {
public:
  GUI_Help2Menu();
  
  GUI_Board Board1;
  GUI_Bitmap mBack1;
  PG_Label Help2Menu;
  
  GUI_ButtonSmall bReturn;
  //GUI_ButtonSmall bNext;
  GUI_ButtonSmall bPrev;

  PG_Label lI1a;
  PG_Label lI1b;
  PG_Label lI2a;
  PG_Label lI2b;
  PG_Label lI3a;
  PG_Label lI3b;
  PG_Label lI4a;
  PG_Label lI4b;
  PG_Label lI5a;
  PG_Label lI5b;
  PG_Label lI6a;
  PG_Label lI6b;
  PG_Label lI7a;
  PG_Label lI7b;

  PG_Label lJ1a;
  PG_Label lJ1b;
  PG_Label lJ2a;
  PG_Label lJ2b;
  PG_Label lJ3a;
  PG_Label lJ3b;
  PG_Label lJ4a;
  PG_Label lJ4b;
  PG_Label lJ5a;
  PG_Label lJ5b;
  PG_Label lJ6a;
  PG_Label lJ6b;
  PG_Label lJ7a;
  PG_Label lJ7b;

  PG_Label lK1a;
  PG_Label lK1b;
  PG_Label lK2a;
  PG_Label lK2b;
  PG_Label lK3a;
  PG_Label lK3b;
  PG_Label lK4a;
  PG_Label lK4b;
  PG_Label lK5a;
  PG_Label lK5b;
  PG_Label lK6a;
  PG_Label lK6b;
  PG_Label lK7a;
  PG_Label lK7b;
  

  void Show();
  void Hide();
  void Default();
  
  bool eventButtonClick(int id, PG_Widget* widget);
};

#define HM2_LY1 12

#define HM2_I1_PX (HM2_PX+52)
#define HM2_I1_PY (HM2_PY+45)
#define HM2_I1_VX (100)

#define HM2_I2_PX (HM2_PX+52)
#define HM2_I2_PY (HM2_PY+90)
#define HM2_I2_VX (100)

#define HM2_I3_PX (HM2_PX+52)
#define HM2_I3_PY (HM2_PY+136)
#define HM2_I3_VX (100)

#define HM2_I4_PX (HM2_PX+52)
#define HM2_I4_PY (HM2_PY+182)
#define HM2_I4_VX (100)

#define HM2_I5_PX (HM2_PX+52)
#define HM2_I5_PY (HM2_PY+229)
#define HM2_I5_VX (100)

#define HM2_I6_PX (HM2_PX+52)
#define HM2_I6_PY (HM2_PY+274)
#define HM2_I6_VX (100)

#define HM2_I7_PX (HM2_PX+52)
#define HM2_I7_PY (HM2_PY+320)
#define HM2_I7_VX (100)


#define HM2_LY2 12

#define HM2_J1_PX (HM2_PX+194)
#define HM2_J1_PY (HM2_PY+45)
#define HM2_J1_VX (100)

#define HM2_J2_PX (HM2_PX+194)
#define HM2_J2_PY (HM2_PY+90)
#define HM2_J2_VX (100)

#define HM2_J3_PX (HM2_PX+194)
#define HM2_J3_PY (HM2_PY+136)
#define HM2_J3_VX (100)

#define HM2_J4_PX (HM2_PX+194)
#define HM2_J4_PY (HM2_PY+182)
#define HM2_J4_VX (100)

#define HM2_J5_PX (HM2_PX+194)
#define HM2_J5_PY (HM2_PY+229)
#define HM2_J5_VX (100)

#define HM2_J6_PX (HM2_PX+194)
#define HM2_J6_PY (HM2_PY+274)
#define HM2_J6_VX (100)

#define HM2_J7_PX (HM2_PX+194)
#define HM2_J7_PY (HM2_PY+320)
#define HM2_J7_VX (100)

#define HM2_LY3 12

#define HM2_K1_PX (HM2_PX+335)
#define HM2_K1_PY (HM2_PY+45)
#define HM2_K1_VX (100)

#define HM2_K2_PX (HM2_PX+335)
#define HM2_K2_PY (HM2_PY+90)
#define HM2_K2_VX (100)

#define HM2_K3_PX (HM2_PX+335)
#define HM2_K3_PY (HM2_PY+136)
#define HM2_K3_VX (100)

#define HM2_K4_PX (HM2_PX+335)
#define HM2_K4_PY (HM2_PY+182)
#define HM2_K4_VX (100)

#define HM2_K5_PX (HM2_PX+335)
#define HM2_K5_PY (HM2_PY+229)
#define HM2_K5_VX (100)

#define HM2_K6_PX (HM2_PX+335)
#define HM2_K6_PY (HM2_PY+274)
#define HM2_K6_VX (100)

#define HM2_K7_PX (HM2_PX+335)
#define HM2_K7_PY (HM2_PY+320)
#define HM2_K7_VX (100)


/////////////////////////////////////////////////////////////////////////////
// DeletePlayer menu widgets
/////////////////////////////////////////////////////////////////////////////
#define DPM_PX 140
#define DPM_PY 165
#define DPM_VX 360
#define DPM_VY 150

/////////////////////////////////////////////////////////////////////////////
// Video menu widgets
/////////////////////////////////////////////////////////////////////////////
#define VM_PX 140
#define VM_PY 110
#define VM_VX 360
#define VM_VY 270


/////////////////////////////////////////////////////////////////////////////
// Server menu widgets
/////////////////////////////////////////////////////////////////////////////
#define SMS_PX 140
#define SMS_PY 80
#define SMS_VX 360
#define SMS_VY 320



/////////////////////////////////////////////////////////////////////////////
// Client menu widgets
/////////////////////////////////////////////////////////////////////////////
#define CMS_PX 140
#define CMS_PY 130
#define CMS_VX 360
#define CMS_VY 220

/////////////////////////////////////////////////////////////////////////////
// GUI wrappers
/////////////////////////////////////////////////////////////////////////////

extern GUI_BaseMenu *GUI_System;
extern int GUI_id;
extern GUI_BaseMenu* GUI_menu;

GUI_BaseMenu* GUI_Find(int id);
void GUI_Return();
void GUI_OpenMenu(int id);
void GUI_Refresh();
void GUI_Show(int id);
bool GUI_Init(SDL_Surface* s);
bool GUI_Done();
void RenderResults(SDL_Surface *screen);

#endif		// GUI_H
