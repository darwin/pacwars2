//###########################################################################
//## GUI methods
//## 
//## 
//###########################################################################

#ifndef GUI_H
#define GUI_H

#include "paragui.h"
#include "sdlgradientwidget.h"
#include "sdllineedit.h"
#include "sdlbutton.h"
#include "sdllabel.h"
#include "sdlcheckbutton.h"
#include "sdlscrollbar.h"
#include "sdlwidgetlist.h"

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

#include "smapman.h"
#include "sprite.h"

#define GUI_NOMENU        0
#define GUI_MAINMENU      1
#define GUI_NEWGAME       2
#define GUI_JOINGAME      3
#define GUI_CREATEPLAYER  4
#define GUI_DELETEPLAYER  5
#define GUI_OPTIONS       6
#define GUI_DISCONNECT    7
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

#define GUI_RESULTS       200

// forward declarations
void GUI_Show(int id);
void GUI_Return();
void GUI_OpenMenu(int id);
void GUI_InitColors(SDL_Surface* screen);

class GUI_BaseMenu;

// Menu fonts
extern TTF_Font* TextFont;
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

/////////////////////////////////////////////////////////////////////////////
// Board
/////////////////////////////////////////////////////////////////////////////

class GUI_Board : public SDLGradientWidget {
public:
  GUI_Board(SDLWidget* parent, SDL_Rect& r, bool storebackground = false, char* theme=NULL);
//  void eventDraw(SDL_Surface* surface, SDL_Rect* rect);
  
  bool drawbackground;
};

/////////////////////////////////////////////////////////////////////////////
// Bitmap
/////////////////////////////////////////////////////////////////////////////

class GUI_Bitmap : public SDLWidget {
public:
  GUI_Bitmap(SDLWidget* parent, SDL_Rect& r, bool storebackground = false, SDL_Surface* s=NULL);
  void eventDraw(SDL_Surface* surface, SDL_Rect* rect);
  
  SDL_Surface *bitmap;
  bool drawbackground;
};

/////////////////////////////////////////////////////////////////////////////
// Label
/////////////////////////////////////////////////////////////////////////////

class GUI_Label : public SDLLabel {
public:
  GUI_Label(SDLWidget* parent, SDL_Rect& r, char* text, bool storebackground = false);
 // void eventDraw(SDL_Surface* surface, SDL_Rect* rect);

  void SetColor(SDL_Color c1, SDL_Color c2);
  
  SDL_Color c;
  int shifty;
  int shiftx;
  int bgmode;
};

class GUI_LabelC : public SDLLabel {
public:
  GUI_LabelC(SDLWidget* parent, SDL_Rect& r, char* text, bool storebackground = false);
//  void eventDraw(SDL_Surface* surface, SDL_Rect* rect);
  
  SDL_Color c;
  int shifty;
  int shiftx;
  int bgmode;
};


/////////////////////////////////////////////////////////////////////////////
// Label
/////////////////////////////////////////////////////////////////////////////

class GUI_LabelL : public SDLLabel {
public:
  GUI_LabelL(SDLWidget* parent, SDL_Rect& r, char* text, GUI_LabelL** iselected, CSMapInfo* si, SDL_Color ic1, SDL_Color ic2, void (*cb)(GUI_LabelL*)=NULL);
  void eventDraw(SDL_Surface* surface, SDL_Rect* rect);
  bool eventMouseButtonDown(const SDL_MouseButtonEvent* button);


  CSMapInfo* msi;
  SDL_Color c1;
  SDL_Color c2;
  int shifty;
  int shiftx;
  int bgmode;

  void (*callback)(GUI_LabelL*);
  
  GUI_LabelL** selected;
};


/////////////////////////////////////////////////////////////////////////////
// ResultLine
/////////////////////////////////////////////////////////////////////////////

class GUI_ResultLine : public GUI_Label {
public:
  GUI_ResultLine();
  void eventDraw(SDL_Surface* surface, SDL_Rect* rect);
};


/////////////////////////////////////////////////////////////////////////////
// Buttons
/////////////////////////////////////////////////////////////////////////////

class GUI_Button : public SDLButton {
public:
  GUI_Button(SDLWidget* parent, int btnid, SDL_Rect& r, char* text);
//  void eventDraw(SDL_Surface* surface, SDL_Rect* rect);
  
  SDL_Color c;
  int shifty;
  bool drawbackground;
};

class GUI_ButtonBig : public GUI_Button {
public:
  GUI_ButtonBig(SDLWidget* parent, int btnid, SDL_Rect& r, char* text);
  void eventMouseLeave();
  void eventMouseEnter();
	bool eventMouseButtonDown(const SDL_MouseButtonEvent* button);
	bool eventMouseButtonUp(const SDL_MouseButtonEvent* button);

public:
  bool enabled;

};

class GUI_ButtonSmall : public GUI_Button {
public:
  GUI_ButtonSmall(SDLWidget* parent, int btnid, SDL_Rect& r, char* text);
  void eventMouseLeave();
  void eventMouseEnter();
};


/////////////////////////////////////////////////////////////////////////////
// Checkbox
/////////////////////////////////////////////////////////////////////////////

#define CB_SIZEY 20
class DECLSPEC GUI_CheckBox : public SDLCheckButton {
public: 
	
	GUI_CheckBox(SDLWidget* parent, SDL_Rect& r, char* text, bool ipressed=false, SDL_Color bg = GUI_Black);
	~GUI_CheckBox();

  void eventMouseEnter();
  void eventMouseLeave();
};


/////////////////////////////////////////////////////////////////////////////
// TextEdit
/////////////////////////////////////////////////////////////////////////////

class GUI_TextEdit : public SDLLineEdit {
public:
  GUI_TextEdit(SDLWidget* parent, SDL_Rect& r);
};


/////////////////////////////////////////////////////////////////////////////
// NumEdit
/////////////////////////////////////////////////////////////////////////////

class GUI_NumEdit : public GUI_TextEdit {
public:
  GUI_NumEdit(SDLWidget* parent, SDL_Rect& r, int imin, int imax);
  
  bool eventFilterKey(const SDL_KeyboardEvent* key);

  int min, max;
};


/////////////////////////////////////////////////////////////////////////////
// FloatEdit
/////////////////////////////////////////////////////////////////////////////

class GUI_FloatEdit : public GUI_TextEdit {
public:
  GUI_FloatEdit(SDLWidget* parent, SDL_Rect& r, float imin, float max);
  
  bool eventFilterKey(const SDL_KeyboardEvent* key);
  
  float min, max;
};


/////////////////////////////////////////////////////////////////////////////
// Scrollbar
/////////////////////////////////////////////////////////////////////////////

class GUI_ScrollBar : public SDLScrollBar {
public:
  GUI_ScrollBar(SDLWidget* parent, int id, SDL_Rect& r, int direction);
};

#define LB_POSX 5

class GUI_WidgetList : public SDLWidgetList {
public: 
  GUI_WidgetList(SDLWidget* parent, SDL_Rect& r);
  ~GUI_WidgetList();

  void AddWidget(SDLWidget* w);
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
  
  GUI_Label* OKDialog1;
  GUI_Label* lPrompt1;
  GUI_ButtonSmall* bOK;

  GUI_Board* Board1;

  void Default();
  
  bool eventButtonClick(int id, SDLWidget* widget);
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
  
  GUI_Label OKDialog2;
  GUI_Label lPrompt1;
  GUI_Label lPrompt2;
  GUI_ButtonSmall bOK;

  GUI_Board Board1;

  void Default();
  
  bool eventButtonClick(int id, SDLWidget* widget);
};


/////////////////////////////////////////////////////////////////////////////
// OK dialog widgets
/////////////////////////////////////////////////////////////////////////////
#define YN_PX 140
#define YN_PY 140
#define YN_VX 360
#define YN_VY 95


class GUI_YNDialog : public GUI_BaseMenu {
public:
  GUI_YNDialog();

  void Reset(void (*cb)(int res), char* title, char* line1="", char* tla="YES", char* tlb="NO");
  
  GUI_Label* YNDialog;
  GUI_Label* lPrompt1;
  
  GUI_ButtonSmall* bA;
  GUI_ButtonSmall* bB;

  int result;
  void (*callback)(int res);

  //void Show();
  //void Hide();
  void Default();
  
  bool eventButtonClick(int id, SDLWidget* widget);
};


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
// Disconnect menu widgets
/////////////////////////////////////////////////////////////////////////////
#define DG_PX 140
#define DG_PY 140
#define DG_VX 360
#define DG_VY 120


class GUI_DisconnectMenu : public GUI_BaseMenu {
public:
  GUI_DisconnectMenu();
  
  GUI_Board Board1;
  GUI_Label DisconnectMenu;
  
  GUI_Label lPrompt1;
  GUI_Label lPrompt2;
  
  GUI_ButtonSmall bOK;
  GUI_ButtonSmall bCancel;
  
  void Show();
  void Hide();
  void Default();
  
  bool eventButtonClick(int id, SDLWidget* widget);
};


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
#define OM_PY 100
#define OM_VX 280
#define OM_VY 280


/////////////////////////////////////////////////////////////////////////////
// CreatePlayers menu widgets
/////////////////////////////////////////////////////////////////////////////
#define CPS_PX 180
#define CPS_PY 130
#define CPS_VX 280
#define CPS_VY 240


class GUI_CreatePlayerSelMenu : public GUI_BaseMenu {
public:
  GUI_CreatePlayerSelMenu();
  
  GUI_ButtonBig lCreatePlayerSelMenu;
  GUI_ButtonBig bCP1;
  GUI_ButtonBig bCP2;
  GUI_ButtonBig bCP3;
  GUI_ButtonBig bCP4;
  GUI_ButtonBig bBack;
  
  void Show();
  void Hide();
  void Default();
  
  bool eventButtonClick(int id, SDLWidget* widget);
};


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

class GUI_ButtonS : public SDLWidget {
public:
  GUI_ButtonS(SDLWidget* parent, int btnid, SDL_Rect& r);
  
  void eventDraw(SDL_Surface* surface, SDL_Rect* rect);
  bool eventMouseButtonDown(const SDL_MouseButtonEvent* button);
  bool eventMouseButtonUp(const SDL_MouseButtonEvent* button);
  
  bool SetBackground(const char* filename, int mode);
  
  SDL_Surface* background;
  
  int id;
};


class GUI_SkinPic : public SDLWidget {
public:
  GUI_SkinPic(SDLWidget* parent, SDL_Rect& r);
  ~GUI_SkinPic();

  void eventDraw(SDL_Surface* surface, SDL_Rect* rect);

  GUI_TextEdit *lName;
  CSpriteInfo* sprite;

  bool NextSkin();
  bool PrevSkin();
  void SetSkin(char* name);
};



class GUI_SkinCtrl : public SDLWidget {
public:
  GUI_SkinCtrl(SDLWidget* parent, SDL_Rect& r, bool storebackground = false);
  
  void eventDraw(SDL_Surface* surface, SDL_Rect* rect);
  bool eventButtonClick(int id, SDLWidget* widget);
  
  GUI_SkinPic* SkinPic;
  SDLButton* bNext;
  SDLButton* bPrev;
  
  SDL_Color c;
  bool drawbackground;
};


class GUI_Input : public SDLGradientWidget{
public:
	GUI_Input(SDLWidget* parent, SDL_Rect& r, int iid);
	~GUI_Input();

	void InputBegin();
	void InputEnd();
	char* GetText();
	void SetText(char* new_text);

  SDL_Color c_color;

	void eventDraw(SDL_Surface* surface, SDL_Rect* rect);
	bool eventKeyDown(const SDL_KeyboardEvent* key);
	bool eventMouseButtonDown(const SDL_MouseButtonEvent* button);
	void eventInputFocusLost(SDLMessageObject* newfocus);

	void DrawText(SDL_Surface* surface, SDL_Rect* rect);
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
// Results menu widgets
/////////////////////////////////////////////////////////////////////////////
#define RM_PX 280
#define RM_PY 20
#define RM_VX 280
#define RM_VY 250

#define RM_MAX_NAMES 20

class GUI_ResultsMenu : public GUI_BaseMenu {
public:
  int index;

  int px, py;

  GUI_ResultsMenu();
  
  GUI_Board Board1;
  GUI_Label lResultsMenu;
  GUI_Label lLabel1;

  GUI_ResultLine lNamex;
  GUI_ResultLine lFragsx;
  GUI_ResultLine lPointsx;
  
  GUI_ResultLine lName[RM_MAX_NAMES];
  GUI_ResultLine lPoints[RM_MAX_NAMES];
  GUI_ResultLine lFrags[RM_MAX_NAMES];
  
  void Show();
  void Hide();
  void Default();
};

#define RM_BASE 65


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
  GUI_LabelC Help1Menu;
  
  GUI_LabelC lLine1a;
  GUI_LabelC lLine1b;
  GUI_LabelC lLine1c;

  GUI_LabelC lLine2a;
  GUI_LabelC lLine2b;
  GUI_LabelC lLine2c;

  GUI_LabelC lLine3a;
  GUI_LabelC lLine3b;
  GUI_LabelC lLine3c;
  
  GUI_LabelC lLine4a;
  GUI_LabelC lLine4b;
  GUI_LabelC lLine4c;

  GUI_LabelC lLine5a;
  GUI_LabelC lLine5b;
  GUI_LabelC lLine5c;

  GUI_ButtonSmall bReturn;
  GUI_ButtonSmall bNext;
  GUI_ButtonSmall bPrev;

  GUI_LabelC lI1a;
  GUI_LabelC lI1b;
  GUI_LabelC lI2a;
  GUI_LabelC lI2b;
  GUI_LabelC lI3a;
  GUI_LabelC lI3b;
  GUI_LabelC lI4a;
  GUI_LabelC lI4b;
  GUI_LabelC lI5a;
  GUI_LabelC lI5b;
  GUI_LabelC lI6a;
  GUI_LabelC lI6b;
  

  void Show();
  void Hide();
  void Default();
  
  bool eventButtonClick(int id, SDLWidget* widget);
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
  GUI_LabelC Help2Menu;
  
  GUI_ButtonSmall bReturn;
  GUI_ButtonSmall bNext;
  GUI_ButtonSmall bPrev;

  GUI_LabelC lI1a;
  GUI_LabelC lI1b;
  GUI_LabelC lI2a;
  GUI_LabelC lI2b;
  GUI_LabelC lI3a;
  GUI_LabelC lI3b;
  GUI_LabelC lI4a;
  GUI_LabelC lI4b;
  GUI_LabelC lI5a;
  GUI_LabelC lI5b;
  GUI_LabelC lI6a;
  GUI_LabelC lI6b;
  GUI_LabelC lI7a;
  GUI_LabelC lI7b;

  GUI_LabelC lJ1a;
  GUI_LabelC lJ1b;
  GUI_LabelC lJ2a;
  GUI_LabelC lJ2b;
  GUI_LabelC lJ3a;
  GUI_LabelC lJ3b;
  GUI_LabelC lJ4a;
  GUI_LabelC lJ4b;
  GUI_LabelC lJ5a;
  GUI_LabelC lJ5b;
  GUI_LabelC lJ6a;
  GUI_LabelC lJ6b;
  GUI_LabelC lJ7a;
  GUI_LabelC lJ7b;

  GUI_LabelC lK1a;
  GUI_LabelC lK1b;
  GUI_LabelC lK2a;
  GUI_LabelC lK2b;
  GUI_LabelC lK3a;
  GUI_LabelC lK3b;
  GUI_LabelC lK4a;
  GUI_LabelC lK4b;
  GUI_LabelC lK5a;
  GUI_LabelC lK5b;
  GUI_LabelC lK6a;
  GUI_LabelC lK6b;
  GUI_LabelC lK7a;
  GUI_LabelC lK7b;
  

  void Show();
  void Hide();
  void Default();
  
  bool eventButtonClick(int id, SDLWidget* widget);
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

class GUI_DeletePlayerMenu : public GUI_BaseMenu {
public:
  GUI_DeletePlayerMenu();
  
  GUI_Board Board1;
  GUI_Label DeletePlayerMenu;
  GUI_ButtonSmall bDelete;
  GUI_ButtonSmall bCancel;
  GUI_Label lChoose;
  GUI_Board Board2;
  GUI_WidgetList WidgetList;
  
  void Show();
  void Hide();
  void Default();
  void Return();
  
  bool eventButtonClick(int id, SDLWidget* widget);

  GUI_LabelL* selected1;
};


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
#define SMS_PY 105
#define SMS_VX 360
#define SMS_VY 270



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
