//###########################################################################
//## GUI methods
//## 
//## 
//###########################################################################

#include "gui.h"

#include "gmath.h"
#include "pac_player.h"
#include "mouse.h"

#include "server.h"
#include "client.h"
#include "pw2.h"
#include "mapman.h"

extern void App_Quit();
extern void CEndServer(char *string);

GUI_BaseMenu* GUI_menu = NULL;
int GUI_id = 0;

extern GUI_OKDialog1 *OKD1;
extern GUI_OKDialog2 *OKD2;
extern GUI_YNDialog *YND;

// Menu fonts
TTF_Font* TextFont;
TTF_Font* MainFont;
TTF_Font* MainFont2;
TTF_Font* BtnFont;
TTF_Font* BtnFont2;

SDL_Color GUI_BtnTextColor;
SDL_Color GUI_BtnATextColor;
SDL_Color GUI_LabelColor;
SDL_Color GUI_ResultLineColor;

SDL_Color GUI_Black;
SDL_Color GUI_Gray64;
SDL_Color GUI_Blue80;
SDL_Color GUI_Blue100;
SDL_Color GUI_Blue120;
SDL_Color GUI_Red80;
SDL_Color GUI_Red100;
SDL_Color GUI_Red120;
SDL_Color GUI_Green80;
SDL_Color GUI_Green100;
SDL_Color GUI_Green120;
SDL_Color GUI_InputActive;
SDL_Color GUI_InputInactive;
SDL_Color GUI_SmBtColor;
SDL_Color GUI_UnselectedItem;
SDL_Color GUI_SelectedItem;

void GUI_InitColors(SDL_Surface* screen)
{
  GUI_Black.r=0;
  GUI_Black.g=0;
  GUI_Black.b=0;

  GUI_BtnTextColor.r=255;
  GUI_BtnTextColor.g=255;
  GUI_BtnTextColor.b=255;
  
  GUI_BtnATextColor.r=255;
  GUI_BtnATextColor.g=255;
  GUI_BtnATextColor.b=64;
  
  GUI_LabelColor.r=255;
  GUI_LabelColor.g=255;
  GUI_LabelColor.b=255;

  GUI_ResultLineColor.r=255;
  GUI_ResultLineColor.g=255;
  GUI_ResultLineColor.b=255;
/*  
  GUI_Gray64.r=64;
  GUI_Gray64.g=64;
  GUI_Gray64.b=64;
*/
  
  GUI_Gray64.r=15;
  GUI_Gray64.g=32;
  GUI_Gray64.b=52;
/*  
  GUI_Gray64.r=26;
  GUI_Gray64.g=50;
  GUI_Gray64.b=74;
*/
  GUI_Blue80.r=0;
  GUI_Blue80.g=0;
  GUI_Blue80.b=80;
  
  GUI_Blue100.r=0;
  GUI_Blue100.g=0;
  GUI_Blue100.b=100;
  
  GUI_Blue120.r=0;
  GUI_Blue120.g=0;
  GUI_Blue120.b=120;
  
  GUI_Green80.r=0;
  GUI_Green80.g=60;
  GUI_Green80.b=0;

  GUI_Green100.r=0;
  GUI_Green100.g=100;
  GUI_Green100.b=0;
  
  GUI_Green120.r=0;
  GUI_Green120.g=130;
  GUI_Green120.b=0;
  
  GUI_Red80.r=76;
  GUI_Red80.g=10;
  GUI_Red80.b=0;
/*
  GUI_Red80.r=80;
  GUI_Red80.g=0;
  GUI_Red80.b=0;
*/  
  GUI_Red100.r=76;
  GUI_Red100.g=10;
  GUI_Red100.b=0;
  
  GUI_Red120.r=120;
  GUI_Red120.g=0;
  GUI_Red120.b=0;

  GUI_SmBtColor.r=76;
  GUI_SmBtColor.g=10;
  GUI_SmBtColor.b=0;

  GUI_InputActive.r = 255;
  GUI_InputActive.g = 255;
  GUI_InputActive.b = 128;

  GUI_InputInactive.r = 255;
  GUI_InputInactive.g = 255;
  GUI_InputInactive.b = 255;

  GUI_UnselectedItem.r=255;
  GUI_UnselectedItem.g=255;
  GUI_UnselectedItem.b=255;

  GUI_SelectedItem.r=255;
  GUI_SelectedItem.g=255;
  GUI_SelectedItem.b=128;
}

/////////////////////////////////////////////////////////////////////////////
// Board
/////////////////////////////////////////////////////////////////////////////

GUI_Board::GUI_Board(SDLWidget* parent, SDL_Rect& r,  bool storebackground, char* theme):
SDLGradientWidget(parent,r)
{
//  drawbackground = true;
  if (!theme)
    LoadThemeStyle("GUI_Board","GradientWidget");
  else
    LoadThemeStyle(theme,"GradientWidget");
}


/////////////////////////////////////////////////////////////////////////////
// Bitmap
/////////////////////////////////////////////////////////////////////////////

GUI_Bitmap::GUI_Bitmap(SDLWidget* parent, SDL_Rect& r,  bool storebackground, SDL_Surface* s):
SDLWidget(parent,r,storebackground)
{
  bitmap = s;
}

void GUI_Bitmap::eventDraw(SDL_Surface* surface, SDL_Rect* rect)
{
  SDL_Rect sr;
  sr.x = 0;
  sr.y = 0;
  sr.w = MIN(rect->w, bitmap->w);
  sr.h = MIN(rect->h, bitmap->h);
  SDL_BlitSurface(bitmap, &sr, surface, rect);
}



/////////////////////////////////////////////////////////////////////////////
// Label
/////////////////////////////////////////////////////////////////////////////


GUI_Label::GUI_Label(SDLWidget* parent, SDL_Rect& r, char* text, bool storebackground):
SDLLabel(parent,r,text,storebackground)
{
  SetFont(TextFont);
//  SetColor(GUI_LabelColor, GUI_LabelColor);
  c = GUI_Gray64;
  shifty = 2;
  shiftx = 0;
  bgmode = 2;
  SetAlignment(SDL_TA_CENTER); 
}

void GUI_Label::SetColor(SDL_Color c1, SDL_Color c2)
{

}


GUI_LabelC::GUI_LabelC(SDLWidget* parent, SDL_Rect& r, char* text, bool storebackground):
SDLLabel(parent,r,text,storebackground)
{
  SetFont(TextFont);
//  SetColor(GUI_LabelColor, GUI_LabelColor);
  c = GUI_Blue100;
  shifty = 0;
  shiftx = 0;
  bgmode = 0;
  SetAlignment(SDL_TA_LEFT); 
}


/////////////////////////////////////////////////////////////////////////////
// Label
/////////////////////////////////////////////////////////////////////////////

GUI_LabelL::GUI_LabelL(SDLWidget* parent, SDL_Rect& r, char* text, GUI_LabelL** iselected, CSMapInfo* si, SDL_Color ic1, SDL_Color ic2, void (*cb)(GUI_LabelL*)):
SDLLabel(parent,r,text,false)
{
  SetFont(TextFont);
  c1 = ic1;
  c2 = ic2;
  shifty = 4;
  shiftx = 5;
  bgmode = 2;
  selected = iselected;
  msi = si;
  callback = cb;
  SetAlignment(SDL_TA_LEFT); 
//  SetColor(GUI_LabelColor, GUI_LabelColor);
}

bool GUI_LabelL::eventMouseButtonDown(const SDL_MouseButtonEvent* button)
{
  GUI_LabelL* last = *selected;
  if (last!=this)
  {
    *selected = this;
    last->Redraw();
    Redraw();
    if (callback) (*callback)(last);
  }
  return true;
}

void GUI_LabelL::eventDraw(SDL_Surface* surface, SDL_Rect* rect)
{
  SDL_Color c;
  if (*selected==this) SetTextColor(c2); else SetTextColor(c1);
  SDLLabel::eventDraw(surface, rect);  
}


/////////////////////////////////////////////////////////////////////////////
// ResultLine
/////////////////////////////////////////////////////////////////////////////

GUI_ResultLine::GUI_ResultLine():
GUI_Label(NULL, SDLWidget::mkrect(0,0,1,1), "x", false)
{
  SetFont(BtnFont);
//  SetColor(GUI_ResultLineColor, GUI_ResultLineColor);
  c = GUI_Red100;
  shifty = 2;
  shiftx = 0;
  bgmode = 2;
  SetAlignment(SDL_TA_LEFT); 
}

void GUI_ResultLine::eventDraw(SDL_Surface* surface, SDL_Rect* rect)
{
  GUI_Label::eventDraw(surface, rect);
}


/////////////////////////////////////////////////////////////////////////////
// Buttons
/////////////////////////////////////////////////////////////////////////////

GUI_Button::GUI_Button(SDLWidget* parent, int btnid, SDL_Rect& r, char* text):
SDLButton(parent,btnid,r,text)
{
  SetBorderSize(0,0,0);
  SetTextColor(GUI_BtnTextColor);
  c = GUI_Red100;
  shifty = 0;//2;
  drawbackground = false;
}


GUI_ButtonBig::GUI_ButtonBig(SDLWidget* parent, int btnid, SDL_Rect& r, char* text):
GUI_Button(parent,btnid,r,text)
{
  SetFont(MainFont);
  //  drawbackground = false;
  drawbackground = true;
  enabled = true;
	LoadThemeStyle("GUI_MenuButton", "Button");
}

void GUI_ButtonBig::eventMouseLeave() {
    SetFont(MainFont);
    if (enabled)
    {
      SetTextColor(GUI_BtnTextColor);
      Redraw();
    }
}

void GUI_ButtonBig::eventMouseEnter() {
    SetFont(MainFont2);
    if (enabled)
    {
      SetTextColor(GUI_BtnATextColor);
      Redraw();
    }
}

bool GUI_ButtonBig::eventMouseButtonDown(const SDL_MouseButtonEvent* button) {
    if (enabled)
      return GUI_Button::eventMouseButtonDown(button);

    return true;
}

bool GUI_ButtonBig::eventMouseButtonUp(const SDL_MouseButtonEvent* button) {
    if (enabled)
      return GUI_Button::eventMouseButtonUp(button);

    return true;
}


GUI_ButtonSmall::GUI_ButtonSmall(SDLWidget* parent, int btnid, SDL_Rect& r, char* text):
GUI_Button(parent,btnid,r,text)
{
  SetFont(BtnFont);
  drawbackground = true;
  c = GUI_SmBtColor;
	LoadThemeStyle("GUI_NormalButton", "Button");
}

void GUI_ButtonSmall::eventMouseLeave() {
	/*SetFont(BtnFont);*/
	SetTextColor(GUI_BtnTextColor); Redraw();
}

void GUI_ButtonSmall::eventMouseEnter() {
	/*SetFont(BtnFont2);*/
	SetTextColor(GUI_BtnATextColor); Redraw();
}


/////////////////////////////////////////////////////////////////////////////
// Checkbox
/////////////////////////////////////////////////////////////////////////////

GUI_CheckBox::GUI_CheckBox(SDLWidget* parent, SDL_Rect& r, char* text, bool ipressed, SDL_Color bg) :
SDLCheckButton(parent,99, r, text)
{
	if (ipressed) {
		SetPressed();
	}

	my_widgetLabel->SetFont(TextFont);

	LoadThemeStyle("GUI_CheckButton");
	my_widgetButton->LoadThemeStyle("GUI_CheckButton", "CheckWidget");
	SetTransparency(0);
}

GUI_CheckBox::~GUI_CheckBox()
{
}

void GUI_CheckBox::eventMouseEnter() {
	SDLGradientWidget::eventMouseEnter();
}

void GUI_CheckBox::eventMouseLeave() {
	SDLGradientWidget::eventMouseLeave();
}


/////////////////////////////////////////////////////////////////////////////
// TextEdit
/////////////////////////////////////////////////////////////////////////////

GUI_TextEdit::GUI_TextEdit(SDLWidget* parent, SDL_Rect& r):
SDLLineEdit(parent, r)
{
	SetFont(TextFont);
	LoadThemeStyle("GUI_LineEdit", "GradientWidget");
}

/////////////////////////////////////////////////////////////////////////////
// NumEdit
/////////////////////////////////////////////////////////////////////////////

GUI_NumEdit::GUI_NumEdit(SDLWidget* parent, SDL_Rect& r, int imin, int imax):
GUI_TextEdit(parent, r)
{
	SetFont(TextFont);
	min = imin;
	max = imax;
}


bool GUI_NumEdit::eventFilterKey(const SDL_KeyboardEvent* key)
{
  switch(key->keysym.sym){
    
    case SDLK_0:
    case SDLK_1:
    case SDLK_2:
    case SDLK_3:
    case SDLK_4:
    case SDLK_5:
    case SDLK_6:
    case SDLK_7:
    case SDLK_8:
    case SDLK_9:
    case SDLK_KP0:
	  case SDLK_KP1:
	  case SDLK_KP2:
	  case SDLK_KP3:
	  case SDLK_KP4:
	  case SDLK_KP5:
	  case SDLK_KP6:
	  case SDLK_KP7:
	  case SDLK_KP8:
	  case SDLK_KP9:
			return false;
      break;
      
    default:
      return true;
  }
  
  return true;
}

/////////////////////////////////////////////////////////////////////////////
// FloatEdit
/////////////////////////////////////////////////////////////////////////////

GUI_FloatEdit::GUI_FloatEdit(SDLWidget* parent, SDL_Rect& r, float imin, float imax):
GUI_TextEdit(parent, r)
{
  SetFont(TextFont);
  min = imin;
  max = imax;
}


bool GUI_FloatEdit::eventFilterKey(const SDL_KeyboardEvent* key)
{
  switch(key->keysym.sym){
    
    case SDLK_0:
    case SDLK_1:
    case SDLK_2:
    case SDLK_3:
    case SDLK_4:
    case SDLK_5:
    case SDLK_6:
    case SDLK_7:
    case SDLK_8:
    case SDLK_9:
    case SDLK_KP0:
	  case SDLK_KP1:
	  case SDLK_KP2:
	  case SDLK_KP3:
	  case SDLK_KP4:
	  case SDLK_KP5:
	  case SDLK_KP6:
	  case SDLK_KP7:
	  case SDLK_KP8:
	  case SDLK_KP9:
	  case SDLK_KP_PERIOD:
		case SDLK_PERIOD:
		case SDLK_MINUS:
	  case SDLK_KP_MINUS:
		return false;
      break;
      
    default:
      return true;
  }
  
  return true;
}

/////////////////////////////////////////////////////////////////////////////
// Scrollbar
/////////////////////////////////////////////////////////////////////////////

GUI_ScrollBar::GUI_ScrollBar(SDLWidget* parent, int id, SDL_Rect& r, int direction) : SDLScrollBar(parent, id, r, direction) {
}

GUI_WidgetList::GUI_WidgetList(SDLWidget* parent, SDL_Rect& r) : SDLWidgetList(parent, r/*, true*/) {
  LoadThemeStyle("GUI_WidgetList", "GradientWidget");
}

GUI_WidgetList::~GUI_WidgetList() {
}

void GUI_WidgetList::AddWidget(SDLWidget* w) {
	SDLWidgetList::AddWidget(w);
}

	
/////////////////////////////////////////////////////////////////////////////
// OK dialog widgets
/////////////////////////////////////////////////////////////////////////////

void GUI_OKDialog1::Reset(char* title, char* line1)
{
	OKDialog1->SetText(title);
	lPrompt1->SetText(line1);
}

GUI_OKDialog1::GUI_OKDialog1(char* title, char* line1, char* line2) : GUI_BaseMenu(GUI_OKDIALOG1, mkrect(OKD1_PX,OKD1_PY,OKD1_VX,OKD1_VY)){

	OKDialog1 = new GUI_Label(this, SDLWidget::mkrect(1,4,OKD1_VX-2,25), title, false);
	lPrompt1 = new GUI_Label(this, SDLWidget::mkrect(1,35, OKD1_VX-2,20), line1, false);
	bOK = new GUI_ButtonSmall(this, 1, SDLWidget::mkrect(105,65,150,25), "OK");

	Board1 = new GUI_Board(this, SDLWidget::mkrect(0,0,OKD1_VX,100), false);

	Default();
  
	OKDialog1->SetColor(GUI_BtnTextColor, GUI_BtnATextColor);
	OKDialog1->SetFont(MainFont);
  
	OKDialog1->bgmode = 2;
  
	lPrompt1->SetAlignment(SDL_TA_CENTER);
	lPrompt1->bgmode = 2;
}

void GUI_OKDialog1::Default()
{
}

bool GUI_OKDialog1::eventButtonClick(int id, SDLWidget* widget)
{
	switch (id) {
		case 1:
			Return();
			break;
		case 2:
			Return();
			break;
	}
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// OK dialog widgets
/////////////////////////////////////////////////////////////////////////////

void GUI_OKDialog2::Reset(char* title, char* line1, char* line2)
{
  OKDialog2.SetText(title);
  lPrompt1.SetText(line1);
  lPrompt2.SetText(line2);
}

GUI_OKDialog2::GUI_OKDialog2(char* title, char* line1, char* line2):
GUI_BaseMenu(GUI_OKDIALOG2, mkrect(OKD2_PX,OKD2_PY,OKD2_VX,OKD2_VY)),
Board1(NULL, SDLWidget::mkrect(OKD2_PX,OKD2_PY,OKD2_VX,120), false),
OKDialog2(NULL, SDLWidget::mkrect(OKD2_PX+1,OKD2_PY+4,OKD2_VX-2,25), title, false),
lPrompt1(NULL, SDLWidget::mkrect(OKD2_PX+1,OKD2_PY+35, OKD2_VX-2,20), line1, false),
lPrompt2(NULL, SDLWidget::mkrect(OKD2_PX+1,OKD2_PY+55, OKD2_VX-2,20), line2, false),
bOK(NULL, 1, SDLWidget::mkrect(OKD2_PX+105,OKD2_PY+90,150,25), "OK")
{
  Default();
  
  OKDialog2.SetColor(GUI_BtnTextColor, GUI_BtnATextColor);
  OKDialog2.SetFont(MainFont);
  
  OKDialog2.bgmode = 2;
  
  lPrompt1.SetAlignment(SDL_TA_CENTER); 
  lPrompt1.bgmode = 2;
  lPrompt2.SetAlignment(SDL_TA_CENTER); 
  lPrompt2.bgmode = 2;
  
  AddChild(&OKDialog2);
  AddChild(&lPrompt1);
  AddChild(&lPrompt2);
  AddChild(&bOK);

  AddChild(&Board1);
}

void GUI_OKDialog2::Default()
{
}

bool GUI_OKDialog2::eventButtonClick(int id, SDLWidget* widget)
{
  switch (id) {
  case 1:
    Return(); 
    break;
  case 2:
    Return();
    break;
  }
  return true;
}


/////////////////////////////////////////////////////////////////////////////
// OK dialog widgets
/////////////////////////////////////////////////////////////////////////////

void GUI_YNDialog::Reset(void (*cb)(int res), char* title, char* line1, char* tla, char* tlb)
{
  YNDialog->SetText(title);
  lPrompt1->SetText(line1);
  bA->SetText(tla);
  bB->SetText(tlb);
  callback = cb;
}

GUI_YNDialog::GUI_YNDialog() : GUI_BaseMenu(GUI_YNDIALOG, mkrect(YN_PX,YN_PY,YN_VX,YN_VY)) {

	Default();

	YNDialog = new GUI_Label(this, SDLWidget::mkrect(1,4,YN_VX-2,25), "", false);
	lPrompt1 = new GUI_Label(this, SDLWidget::mkrect(1,35, YN_VX-2,20), "", false);
	bA = new GUI_ButtonSmall(this, 1, SDLWidget::mkrect(25,65,150,25), "YES");
	bB = new GUI_ButtonSmall(this, 2, SDLWidget::mkrect(25+150+10,65,150,25), "NO");

	YNDialog->SetColor(GUI_BtnTextColor, GUI_BtnATextColor);
	YNDialog->SetFont(MainFont);
  
	YNDialog->bgmode = 2;
  
	lPrompt1->SetAlignment(SDL_TA_CENTER);
	lPrompt1->bgmode = 2;

	LoadThemeStyle("GUI_Board");
}

void GUI_YNDialog::Default()
{
}

bool GUI_YNDialog::eventButtonClick(int id, SDLWidget* widget)
{
  switch (id) {
  case 1:
    callback(1);
//    Return(); 
    break;
  case 2:
    result = 0;
    callback(0);
//    Return(); 
    break;
  }
  return true;
}



/////////////////////////////////////////////////////////////////////////////
// Main menu widgets
/////////////////////////////////////////////////////////////////////////////

GUI_MainMenu::GUI_MainMenu() : GUI_BaseMenu(GUI_MAINMENU, mkrect(MM_PX,MM_PY,MM_VX,MM_VY)) {

	lMainMenu = new GUI_ButtonBig(this, 0, SDLWidget::mkrect(0,0,MM_VX,40), "Main Menu");
	bNewGame = new GUI_ButtonBig(this, 1, SDLWidget::mkrect(0,40,MM_VX,40), "Start Server");
	bEndGame = new GUI_ButtonBig(this, 9, SDLWidget::mkrect(0,40,MM_VX,40), "Shutdown Server");
	bJoinDiscGame = new GUI_ButtonBig(this, 2, SDLWidget::mkrect(0,80,MM_VX,40), "Join Game");
	bCreatePlayer = new GUI_ButtonBig(this, 3, SDLWidget::mkrect(0,120,MM_VX,40), "Create Player");
	bDeletePlayer = new GUI_ButtonBig(this, 4, SDLWidget::mkrect(0,160,MM_VX,40), "Delete Player");
	bOptions = new GUI_ButtonBig(this, 5, SDLWidget::mkrect(0,200,MM_VX,40), "Options");
	bCredits = new GUI_ButtonBig(this, 6, SDLWidget::mkrect(0,240,MM_VX,40), "Credits");
	bHelp = new GUI_ButtonBig(this, 7, SDLWidget::mkrect(0,280,MM_VX,40), "Info");
	bQuit = new GUI_ButtonBig(this, 8, SDLWidget::mkrect(0,320,MM_VX,40), "Quit");
	bGame = new GUI_ButtonBig(this, 10, SDLWidget::mkrect(0,360,MM_VX,40), "Back To Game");

	Default();
  
	lMainMenu->enabled = false;
	lMainMenu->LoadThemeStyle("GUI_MenuTitle", "Button");
}

void GUI_MainMenu::Default()
{
}

void QuitCB(int res)
{
  if (res) 
  {
    App_Quit();
    while (GUI_id) GUI_Return();
    return;
  }
  GUI_Return();
}

void ShutdownCB(int res)
{
  if (res) 
  {
    ConOut("es");
    CommandExecute("es");
  }
  while (GUI_id) GUI_Return();
  GUI_OpenMenu(GUI_MAINMENU);
}

void EndGameCB(int res)
{
  if (res) CEndServer("");
  GUI_Return();
}

bool GUI_MainMenu::eventButtonClick(int id, SDLWidget* widget)
{
  char* a[]={"I'M SURE", "YES", "OK", "DO IT", "LEAVE"};

  switch (id) {
  case 1:
    GUI_OpenMenu(GUI_NEWGAME);
    break;
  case 2:
    if (net_client_status==NS_CONNECTED)
      GUI_OpenMenu(GUI_DISCONNECT);
    else
      GUI_OpenMenu(GUI_JOINGAME);
    break;
  case 3:
    GUI_OpenMenu(GUI_CREATEPLAYER);
    break;
  case 4:
    GUI_OpenMenu(GUI_DELETEPLAYER);
    break;
  case 5:
    GUI_OpenMenu(GUI_OPTIONS);
    break;
  case 6:
    GUI_OpenMenu(GUI_CREDITS);
    break;
  case 7:
    GUI_OpenMenu(GUI_HELP1);
    break;
  case 8:
    YND->Reset(QuitCB, "QUIT PACWARS2", "Do you really want to quit the game ?", a[rand()%5]);
    GUI_OpenMenu(GUI_YNDIALOG);
    break;
  case 9:
    YND->Reset(ShutdownCB, "END SERVER GAME", "Do you want shutdown server ?");
    GUI_OpenMenu(GUI_YNDIALOG);
    break;
  case 10:
    Return();
    break;
  }
  return true;
}

void GUI_MainMenu::eventShow()
{
  Clear();
  
  bNewGame->Hide();
  bEndGame->Hide();

//  mBack1.Show();
  if (net_client_status==NS_CONNECTED)
  {
//    objDisplayRect.y = (480-400)/2;
//    objDisplayRect.h = 400;
//    objClipRect = objDisplayRect;

    bJoinDiscGame->SetText("Disconnect");
    bCreatePlayer->MoveWindow(0,120);
    bDeletePlayer->MoveWindow(0,160);
    bOptions->MoveWindow(0,200);
    bCredits->MoveWindow(0,240);
    bHelp->MoveWindow(0,280);
    bQuit->MoveWindow(0,320);
    bGame->MoveWindow(0,360);
  }
  else
  {
//    objDisplayRect.y = (480-280)/2;
//    objDisplayRect.h = 280;
//    objClipRect = objDisplayRect;

    bCreatePlayer->Hide();
    bDeletePlayer->Hide();
    bGame->Hide();

    bJoinDiscGame->SetText("Join Game");
    bOptions->MoveWindow(0,120);
    bCredits->MoveWindow(0,160);
    bHelp->MoveWindow(0,200);
    bQuit->MoveWindow(0,240);

//    bGame.MoveRect(MM_PX,MM_PY+280);

  }

  if (net_server_status==NS_UNINITED) {
    bNewGame->Show();
  }
  else {
    bEndGame->Show();
  }

	Redraw();
}

void GUI_MainMenu::eventHide()
{
/*  lMainMenu.Hide();
  bNewGame.Hide();  
  bEndGame.Hide();  
  bJoinDiscGame.Hide();  
  bCreatePlayer.Hide();  
  bDeletePlayer.Hide();  
  bOptions.Hide();  
  bCredits.Hide();  
  bHelp.Hide();  
  bQuit.Hide();
  bGame.Hide();
*/
}

void GUI_MainMenu::Return()
{
  GUI_BaseMenu::Return();
  GUI_id = GUI_NOMENU;
  GUI_menu = NULL;

  smPlaySample(SM_MENU2, menu_volume);
}


/////////////////////////////////////////////////////////////////////////////
// JoinGame menu widgets
/////////////////////////////////////////////////////////////////////////////

GUI_JoinGameMenu* JGMenu;

void GUI_JoinGameMenu::Return()
{
  eServerAddress.EditEnd();
  eClientName.EditEnd();
  eClientDesc.EditEnd();
  GUI_BaseMenu::Return();
}

GUI_JoinGameMenu::GUI_JoinGameMenu():
GUI_BaseMenu(GUI_JOINGAME, mkrect(JG_PX,JG_PY,JG_VX,JG_VY)),
JoinGameMenu(NULL, SDLWidget::mkrect(JG_PX+1,JG_PY+4,JG_VX-2,25), "JOIN GAME", false),
lServerAddress(NULL, SDLWidget::mkrect(JG_PX+1,JG_PY+50, 119,20), "Server address:", false),
eServerAddress(NULL, SDLWidget::mkrect(JG_PX+120,JG_PY+50,215,20)),
lClientName(NULL, SDLWidget::mkrect(JG_PX+1,JG_PY+80, 119,20), "client name:", false),
eClientName(NULL, SDLWidget::mkrect(JG_PX+120,JG_PY+80,215,20)),
lClientDesc(NULL, SDLWidget::mkrect(JG_PX+1,JG_PY+110, 119,20), "client finger:", false),
eClientDesc(NULL, SDLWidget::mkrect(JG_PX+120,JG_PY+110,215,20)),
bConnect(NULL, 1, SDLWidget::mkrect(JG_PX+25,JG_PY+140,150,25), "CONNECT"),
bCancel(NULL, 2, SDLWidget::mkrect(JG_PX+25+150+10,JG_PY+140,150,25), "BACK"),
Board1(NULL, SDLWidget::mkrect(JG_PX,JG_PY,JG_VX, JG_VY), false)
{
  JGMenu = this;
  Default();
  
  JoinGameMenu.SetColor(GUI_BtnTextColor, GUI_BtnATextColor);
  JoinGameMenu.SetFont(MainFont);
  
  JoinGameMenu.bgmode = 2;
  
  lServerAddress.SetAlignment(SDL_TA_RIGHT); 
  lServerAddress.shiftx = -4;
  lClientName.SetAlignment(SDL_TA_RIGHT); 
  lClientName.shiftx = -4;
  lClientDesc.SetAlignment(SDL_TA_RIGHT); 
  lClientDesc.shiftx = -4;
  
  AddChild(&JoinGameMenu);
  AddChild(&lServerAddress);
  AddChild(&eServerAddress);
  AddChild(&lClientName);
  AddChild(&eClientName);
  AddChild(&lClientDesc);
  AddChild(&eClientDesc);
  AddChild(&bConnect);
  AddChild(&bCancel);
  AddChild(&Board1);
}

void GUI_JoinGameMenu::Default()
{
  eServerAddress.SetText(last_server.string);
  eClientName.SetText(c_name.string);
  eClientDesc.SetText(c_desc.string);
}

bool GUI_JoinGameMenu::eventButtonClick(int id, SDLWidget* widget)
{
  switch (id) {
  case 1:
    ConOut("");
    ConOut("> JoinGame menu sequence <");
    if (net_client_status)
    {
      ConOut("ec");
      CommandExecute("ec");
    }
    
    ConOut("c_name %s", eClientName.GetText());
    CommandExecute("c_name %s", eClientName.GetText());
    ConOut("c_desc %s", eClientDesc.GetText());
    CommandExecute("c_desc %s", eClientDesc.GetText());
    ConOut("last_server %s", eServerAddress.GetText());
    CommandExecute("last_server %s", eServerAddress.GetText());
    
    ConOut("sc");
    CommandExecute("sc");
    ConOut("connect %s", eServerAddress.GetText());
    CommandExecute("connect %s", eServerAddress.GetText());
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



/////////////////////////////////////////////////////////////////////////////
// Disconnect menu widgets
/////////////////////////////////////////////////////////////////////////////

GUI_DisconnectMenu::GUI_DisconnectMenu():
GUI_BaseMenu(GUI_DISCONNECT, mkrect(DG_PX,DG_PY,DG_VX,DG_VY)),
Board1(NULL, SDLWidget::mkrect(DG_PX,DG_PY,DG_VX,125), false),
DisconnectMenu(NULL, SDLWidget::mkrect(DG_PX+1,DG_PY+4,DG_VX-2,25), "DISCONNECT", false),
lPrompt1(NULL, SDLWidget::mkrect(DG_PX+1,DG_PY+35, DG_VX-2,20), "Do you really want", false),
lPrompt2(NULL, SDLWidget::mkrect(DG_PX+1,DG_PY+55, DG_VX-2,20), "disconnect from this game ?", false),
bOK(NULL, 1, SDLWidget::mkrect(DG_PX+25,DG_PY+90,150,25), "OK"),
bCancel(NULL, 2, SDLWidget::mkrect(DG_PX+25+150+10,DG_PY+90,150,25), "BACK")
{
  Default();
  
  DisconnectMenu.SetColor(GUI_BtnTextColor, GUI_BtnATextColor);
  DisconnectMenu.SetFont(MainFont);
  
  DisconnectMenu.bgmode = 2;
  
  lPrompt1.SetAlignment(SDL_TA_CENTER); 
  lPrompt1.bgmode = 2;
  lPrompt2.SetAlignment(SDL_TA_CENTER); 
  lPrompt2.bgmode = 2;
  
  AddChild(&DisconnectMenu);
  AddChild(&lPrompt1);
  AddChild(&lPrompt2);
  AddChild(&bOK);
  AddChild(&bCancel);

  AddChild(&Board1);
}

void GUI_DisconnectMenu::Default()
{
}

bool GUI_DisconnectMenu::eventButtonClick(int id, SDLWidget* widget)
{
  switch (id) {
  case 1:
    ConOut("");
    ConOut("> Disconnect menu sequence <");
    if (net_client_status)
    {
      ConOut("disconnect");
      CommandExecute("disconnect");
    }
    ConOut("> end of sequence <");
    ConOut("");
    
    while (GUI_id!=GUI_MAINMENU) GUI_Return(); // return from all menus
    break;
  case 2:
    Return();
    break;
  }
  return true;
}

void GUI_DisconnectMenu::Show()
{
  Clear();
  
  Board1.Show();  
  DisconnectMenu.Show();
  
  lPrompt1.Show();  
  lPrompt2.Show();  
  
  bOK.Show();  
  bCancel.Show();  
}

void GUI_DisconnectMenu::Hide()
{
  Board1.Hide();  
  DisconnectMenu.Hide();
  
  lPrompt1.Hide();  
  lPrompt2.Hide();  
  
  bOK.Hide();  
  bCancel.Hide();  
}

/////////////////////////////////////////////////////////////////////////////
// Credits menu widgets
/////////////////////////////////////////////////////////////////////////////

GUI_CreditsMenu::GUI_CreditsMenu():
GUI_BaseMenu(GUI_CREDITS, mkrect(CM_PX,CM_PY,CM_VX,CM_VY)),
Board1(NULL, SDLWidget::mkrect(CM_PX,CM_PY,CM_VX,420), false),
mBack1(NULL, SDLWidget::mkrect(CM_PX+1, CM_PY+1, credits->w, credits->h), false, credits),
CreditsMenu(NULL, SDLWidget::mkrect(CM_PX+1,CM_PY+4,CM_VX-2,25), "Credits", false),
lLine1(NULL, SDLWidget::mkrect(CM_G1_PX,CM_G1_PY+35, CM_G1_VX-2,20), "Code by Antonin Hildebrand aka Woid", false),
lLine2(NULL, SDLWidget::mkrect(CM_G1_PX,CM_G1_PY+55, CM_G1_VX-2,20), "Graphics by Petr Stastny aka Raist", false),
lLine3(NULL, SDLWidget::mkrect(CM_G1_PX,CM_G1_PY+75, CM_G1_VX-2,20), "Music by Ondrej Broukal aka GameOver", false),
lLine4(NULL, SDLWidget::mkrect(CM_G1X_PX,CM_G1_PY+95, CM_G1_VX-2,20), "Petr Stastny aka Raist", false),
lLine5(NULL, SDLWidget::mkrect(CM_G1X_PX,CM_G1_PY+115, CM_G1_VX-2,20), "Pavel Mirejovsky aka Dusty", false),
lLine20(NULL, SDLWidget::mkrect(CM_G1_PX,CM_G1_PY+155, CM_G1_VX-2,20), "Iain Syme author of the orignal PacWars", false),
lLine7(NULL, SDLWidget::mkrect(CM_G2_PX,CM_G2_PY+155, CM_G2_VX-2,20), "SDL library by Sam Latinga", false),
lLine8(NULL, SDLWidget::mkrect(CM_G2_PX,CM_G2_PY+175, CM_G2_VX-2,20), "SeeR scripting engine", false),
lLine6(NULL, SDLWidget::mkrect(CM_G2_PX,CM_G2_PY+195, CM_G2_VX-2,20), "by Przemek Podsiadly aka Theur", false),
lLine9(NULL, SDLWidget::mkrect(CM_G2_PX,CM_G2_PY+215, CM_G2_VX-2,20), "BASS sound system by Ian Luck", false),
lLine10(NULL, SDLWidget::mkrect(CM_G2_PX,CM_G2_PY+235, CM_G2_VX-2,20), "Mappy by Robin Burrows", false),
lLine11(NULL, SDLWidget::mkrect(CM_G2_PX,CM_G2_PY+255, CM_G2_VX-2,20), "ParaGUI by Alexander Pipelka", false),
lLine12(NULL, SDLWidget::mkrect(CM_G2_PX,CM_G2_PY+275, CM_G2_VX-2,20), "ImageMagick library by ImageMagick Studio", false),
lLine13(NULL, SDLWidget::mkrect(CM_G2_PX,CM_G2_PY+295, CM_G2_VX-2,20), "SDL: net, console, ttf, mixer and image libs", false),
lLine14(NULL, SDLWidget::mkrect(CM_G2_PX,CM_G2_PY+315, CM_G2_VX-2,20), "zlib, libpng, libjpeg, UPX packer", false),
lLine16(NULL, SDLWidget::mkrect(CM_PX+4,CM_PY+235, CM_VX-2,20), "Thanks to SLiK, Mem, Lada, Tessien, Dusty, Farey, Kerim, Garett Banuk, Nula, ...", false),
lLine17(NULL, SDLWidget::mkrect(CM_PX+4,CM_PY+255, CM_VX-8,20), "See credits.txt for full list.", false),
lLine18(NULL, SDLWidget::mkrect(CM_PX+4,CM_PY+315, CM_VX-2,25), "visit pacComunity at", false),
lLine19(NULL, SDLWidget::mkrect(CM_PX+4,CM_PY+345, CM_VX-2,25), "http://pw2.tsx.org", false),
lLine15(NULL, SDLWidget::mkrect(CM_PX+4,CM_PY+285, CM_VX-4,25), "PacWars2 rulezzz !!!", false),
bOK(NULL, 1, SDLWidget::mkrect(CM_PX+155,CM_PY+390,150,25), "COOL")
{
  Default();
  
  CreditsMenu.SetAlignment(SDL_TA_CENTER); 
//  CreditsMenu.SetColor(GUI_BtnTextColor, GUI_BtnATextColor);
  CreditsMenu.SetFont(MainFont);
  
  lLine15.SetFont(MainFont);
  lLine18.SetFont(MainFont);
  lLine19.SetFont(MainFont);

  CreditsMenu.bgmode = 0;
  
  AddChild(&CreditsMenu);

  lLine1.SetAlignment(SDL_TA_LEFT); 
  lLine1.bgmode = 0;
  AddChild(&lLine1);
  lLine2.SetAlignment(SDL_TA_LEFT); 
  lLine2.bgmode = 0;
  AddChild(&lLine2);
  lLine3.SetAlignment(SDL_TA_LEFT); 
  lLine3.bgmode = 0;
  AddChild(&lLine3);
  lLine4.SetAlignment(SDL_TA_LEFT); 
  lLine4.bgmode = 0;
  AddChild(&lLine4);
  lLine5.SetAlignment(SDL_TA_LEFT); 
  lLine5.bgmode = 0;
  AddChild(&lLine5);
  lLine6.SetAlignment(SDL_TA_LEFT); 
  lLine6.bgmode = 0;
  AddChild(&lLine6);
  lLine7.SetAlignment(SDL_TA_LEFT); 
  lLine7.bgmode = 0;
  AddChild(&lLine7);
  lLine8.SetAlignment(SDL_TA_LEFT); 
  lLine8.bgmode = 0;
  AddChild(&lLine8);
  lLine9.SetAlignment(SDL_TA_LEFT); 
  lLine9.bgmode = 0;
  AddChild(&lLine9);
  lLine20.SetAlignment(SDL_TA_LEFT); 
  lLine20.bgmode = 0;
  AddChild(&lLine20);
  lLine10.SetAlignment(SDL_TA_LEFT); 
  lLine10.bgmode = 0;
  AddChild(&lLine10);
  lLine11.SetAlignment(SDL_TA_LEFT); 
  lLine11.bgmode = 0;
  AddChild(&lLine11);
  lLine12.SetAlignment(SDL_TA_LEFT); 
  lLine12.bgmode = 0;
  AddChild(&lLine12);
  lLine13.SetAlignment(SDL_TA_LEFT); 
  lLine13.bgmode = 0;
  AddChild(&lLine13);
  lLine14.SetAlignment(SDL_TA_LEFT); 
  lLine14.bgmode = 0;
  AddChild(&lLine14);
  lLine15.SetAlignment(SDL_TA_CENTER); 
  lLine15.bgmode = 0;
  AddChild(&lLine15);
  lLine16.SetAlignment(SDL_TA_CENTER); 
  lLine16.bgmode = 0;
  AddChild(&lLine16);
  lLine17.SetAlignment(SDL_TA_RIGHT); 
  lLine17.bgmode = 0;
  AddChild(&lLine17);
  lLine18.SetAlignment(SDL_TA_CENTER); 
  lLine18.bgmode = 0;
  AddChild(&lLine18);
  lLine19.SetAlignment(SDL_TA_CENTER); 
  lLine19.bgmode = 0;
  AddChild(&lLine19);
  
  AddChild(&bOK);

  AddChild(&mBack1);
  AddChild(&Board1);
}

void GUI_CreditsMenu::Default()
{
}

bool GUI_CreditsMenu::eventButtonClick(int id, SDLWidget* widget)
{
  switch (id) {
  case 1:
    Return();
    break;
  }
  return true;
}

void GUI_CreditsMenu::Show()
{
  Clear();
  
  Board1.Show();  
  mBack1.Show();  
  CreditsMenu.Show();
  
  lLine1.Show();
  lLine2.Show();
  lLine3.Show();
  lLine4.Show();
  lLine5.Show();
  lLine6.Show();
  lLine7.Show();
  lLine8.Show();
  lLine9.Show();
  lLine10.Show();
  lLine11.Show();
  lLine12.Show();
  lLine13.Show();
  lLine14.Show();
  lLine15.Show();
  lLine16.Show();
  lLine17.Show();
  lLine18.Show();
  lLine19.Show();
  lLine20.Show();
  
  bOK.Show();  
}

void GUI_CreditsMenu::Hide()
{
  Board1.Hide();  
  mBack1.Hide();  
  CreditsMenu.Hide();
  
  lLine1.Hide();
  lLine2.Hide();
  lLine3.Hide();
  lLine4.Hide();
  lLine5.Hide();
  lLine6.Hide();
  lLine6.Hide();
  lLine7.Hide();
  lLine8.Hide();
  lLine9.Hide();
  lLine10.Hide();
  lLine11.Hide();
  lLine12.Hide();
  lLine13.Hide();
  lLine14.Hide();
  lLine15.Hide();
  lLine16.Hide();
  lLine17.Hide();
  lLine18.Hide();
  lLine19.Hide();
  lLine20.Hide();
  
  bOK.Hide();  
}


/////////////////////////////////////////////////////////////////////////////
// Options menu widgets
/////////////////////////////////////////////////////////////////////////////

GUI_OptionsMenu::GUI_OptionsMenu():
GUI_BaseMenu(GUI_OPTIONS, mkrect(OM_PX,OM_PY,OM_VX,OM_VY)),
lOptionsMenu(NULL,0, SDLWidget::mkrect(OM_PX,OM_PY,OM_VX,40), "Options Menu"),
bServer(NULL, 1, SDLWidget::mkrect(OM_PX,OM_PY+40,OM_VX,40), "Server"),
bClient(NULL, 5, SDLWidget::mkrect(OM_PX,OM_PY+80,OM_VX,40), "Client"),
bVideo(NULL, 2, SDLWidget::mkrect(OM_PX,OM_PY+120,OM_VX,40), "Video"),
bSound(NULL, 3, SDLWidget::mkrect(OM_PX,OM_PY+160,OM_VX,40), "Sound"),
bPaths(NULL, 4, SDLWidget::mkrect(OM_PX,OM_PY+200,OM_VX,40), "Paths"),
bBack(NULL, 10, SDLWidget::mkrect(OM_PX,OM_PY+240,OM_VX,40), "Back")
{
  Default();
  
  lOptionsMenu.enabled = false;
	lOptionsMenu.LoadThemeStyle("GUI_MenuTitle", "Button");
   
  AddChild(&lOptionsMenu);
  AddChild(&bPaths);
  AddChild(&bSound);
  AddChild(&bVideo);
  AddChild(&bServer);
  AddChild(&bClient);
  AddChild(&bBack);
}

void GUI_OptionsMenu::Default()
{
}

bool GUI_OptionsMenu::eventButtonClick(int id, SDLWidget* widget)
{
  switch (id) {
  case 1:
    GUI_OpenMenu(GUI_SERVER);
    break;
  case 2:
    GUI_OpenMenu(GUI_VIDEO);
    break;
  case 3:
    GUI_OpenMenu(GUI_SOUND);
    break;
  case 4:
    GUI_OpenMenu(GUI_PATHS);
    break;
  case 5:
    GUI_OpenMenu(GUI_CLIENT);
    break;
  case 10:
    Return();
    break;
  }
  return true;
}

void GUI_OptionsMenu::Show()
{
  Clear();
  
  lOptionsMenu.Show();
  bPaths.Show();  
  bVideo.Show();  
  bServer.Show();  
  bClient.Show();  
  bSound.Show();  
  bBack.Show();  
}

void GUI_OptionsMenu::Hide()
{
  lOptionsMenu.Hide();
  bPaths.Hide();  
  bVideo.Hide();  
  bServer.Hide();  
  bClient.Hide();  
  bSound.Hide();  
  bBack.Hide();  
}

/////////////////////////////////////////////////////////////////////////////
// CreatePlayers menu widgets
/////////////////////////////////////////////////////////////////////////////

GUI_CreatePlayerSelMenu::GUI_CreatePlayerSelMenu():
GUI_BaseMenu(GUI_CREATEPLAYER, mkrect(CPS_PX,CPS_PY,CPS_VX,CPS_VY)),
lCreatePlayerSelMenu(NULL, 0, SDLWidget::mkrect(CPS_PX,CPS_PY,CPS_VX,40), "Players Menu"),
bCP1(NULL, 1, SDLWidget::mkrect(CPS_PX,CPS_PY+40,CPS_VX,40), "Player1"),
bCP2(NULL, 2, SDLWidget::mkrect(CPS_PX,CPS_PY+80,CPS_VX,40), "Player2"),
bCP3(NULL, 3, SDLWidget::mkrect(CPS_PX,CPS_PY+120,CPS_VX,40), "Player3"),
bCP4(NULL, 4, SDLWidget::mkrect(CPS_PX,CPS_PY+160,CPS_VX,40), "Player4"),
bBack(NULL, 10, SDLWidget::mkrect(CPS_PX,CPS_PY+200,CPS_VX,40), "Back")
{
  Default();
  
  lCreatePlayerSelMenu.enabled = false;
	lCreatePlayerSelMenu.LoadThemeStyle("GUI_MenuTitle", "Button");

  AddChild(&lCreatePlayerSelMenu);
  AddChild(&bCP1);
  AddChild(&bCP2);
  AddChild(&bCP3);
  AddChild(&bCP4);
  AddChild(&bBack);
}

void GUI_CreatePlayerSelMenu::Default()
{
}

bool GUI_CreatePlayerSelMenu::eventButtonClick(int id, SDLWidget* widget)
{
  switch (id) {
  case 1:
    GUI_OpenMenu(GUI_CREATEPLAYER1);
    break;
  case 2:
    GUI_OpenMenu(GUI_CREATEPLAYER2);
    break;
  case 3:
    GUI_OpenMenu(GUI_CREATEPLAYER3);
    break;
  case 4:
    GUI_OpenMenu(GUI_CREATEPLAYER4);
    break;
  case 10:
    Return();
    break;
  }
  return true;
}

void GUI_CreatePlayerSelMenu::Show()
{
  Clear();
  
  lCreatePlayerSelMenu.Show();
  bCP1.Show();  
  bCP2.Show();  
  bCP3.Show();  
  bCP4.Show();  
  bBack.Show();  
}

void GUI_CreatePlayerSelMenu::Hide()
{
  lCreatePlayerSelMenu.Hide();
  bCP1.Hide();  
  bCP2.Hide();  
  bCP3.Hide();  
  bCP4.Hide();  
  bBack.Hide();  
}



/////////////////////////////////////////////////////////////////////////////
// Paths menu widgets
/////////////////////////////////////////////////////////////////////////////

void GUI_PathsMenu::Return()
{
  eScriptDir.EditEnd();
  eSpriteDir.EditEnd();
  eMapDir.EditEnd();
  eSndDir.EditEnd();
  eSkinDir.EditEnd();
  eGuiDir.EditEnd();
  eGfxDir.EditEnd();
  GUI_BaseMenu::Return();
}

GUI_PathsMenu::GUI_PathsMenu():
GUI_BaseMenu(GUI_PATHS, mkrect(PM_PX,PM_PY,PM_VX,PM_VY)),
Board1(NULL, SDLWidget::mkrect(PM_PX,PM_PY,PM_VX,PM_VY), false),
lPathsMenu(NULL, SDLWidget::mkrect(PM_PX+1,PM_PY+4,PM_VX-2,25), "PATHS FOR GAME", false),
lEffect(NULL, SDLWidget::mkrect(PM_PX+1,PM_PY+30,PM_VX-2,20), "Changes will take effect after restarting program.", false),

lScriptDir(NULL, SDLWidget::mkrect(PM_PX+1,PM_PY+PM_BASE, 120,20), "Scripts:", false),
eScriptDir(NULL, SDLWidget::mkrect(PM_PX+120,PM_PY+PM_BASE,215,20)),
lMapDir(NULL, SDLWidget::mkrect(PM_PX+1,PM_PY+PM_BASE+20, 119,20), "Maps:", false),
eMapDir(NULL, SDLWidget::mkrect(PM_PX+120,PM_PY+PM_BASE+20,215,20)),
lSkinDir(NULL, SDLWidget::mkrect(PM_PX+1,PM_PY+PM_BASE+40, 119,20), "Skins:", false),
eSkinDir(NULL, SDLWidget::mkrect(PM_PX+120,PM_PY+PM_BASE+40,215,20)),
lSpriteDir(NULL, SDLWidget::mkrect(PM_PX+1,PM_PY+PM_BASE+60, 119,20), "Sprites:", false),
eSpriteDir(NULL, SDLWidget::mkrect(PM_PX+120,PM_PY+PM_BASE+60,215,20)),
lGfxDir(NULL, SDLWidget::mkrect(PM_PX+1,PM_PY+PM_BASE+80, 119,20), "Graphics:", false),
eGfxDir(NULL, SDLWidget::mkrect(PM_PX+120,PM_PY+PM_BASE+80,215,20)),
lGuiDir(NULL, SDLWidget::mkrect(PM_PX+1,PM_PY+PM_BASE+100, 119,20), "GUI:", false),
eGuiDir(NULL, SDLWidget::mkrect(PM_PX+120,PM_PY+PM_BASE+100,215,20)),
lSndDir(NULL, SDLWidget::mkrect(PM_PX+1,PM_PY+PM_BASE+120, 119,20), "Sound:", false),
eSndDir(NULL, SDLWidget::mkrect(PM_PX+120,PM_PY+PM_BASE+120,215,20)),

bChange(NULL, 1, SDLWidget::mkrect(PM_PX+25,PM_PY+PM_BASE+150,150,25), "APPLY"),
bCancel(NULL, 2, SDLWidget::mkrect(PM_PX+25+150+10,PM_PY+PM_BASE+150,150,25), "BACK")
{
  Default();
  
  lPathsMenu.SetColor(GUI_BtnTextColor, GUI_BtnATextColor);
  lPathsMenu.SetFont(MainFont);
  
  lEffect.bgmode = 2;
  lPathsMenu.bgmode = 2;
  
  lScriptDir.SetAlignment(SDL_TA_RIGHT); 
  lScriptDir.shiftx = -4;
  lMapDir.SetAlignment(SDL_TA_RIGHT); 
  lMapDir.shiftx = -4;
  lSkinDir.SetAlignment(SDL_TA_RIGHT); 
  lSkinDir.shiftx = -4;
  lSpriteDir.SetAlignment(SDL_TA_RIGHT); 
  lSpriteDir.shiftx = -4;
  lGfxDir.SetAlignment(SDL_TA_RIGHT); 
  lGfxDir.shiftx = -4;
  lGuiDir.SetAlignment(SDL_TA_RIGHT); 
  lGuiDir.shiftx = -4;
  lSndDir.SetAlignment(SDL_TA_RIGHT); 
  lSndDir.shiftx = -4;
  
  AddChild(&lPathsMenu);
  AddChild(&lEffect);
  
  AddChild(&lScriptDir);
  AddChild(&eScriptDir);
  AddChild(&lMapDir);
  AddChild(&eMapDir);
  AddChild(&lSkinDir);
  AddChild(&eSkinDir);
  AddChild(&lSpriteDir);
  AddChild(&eSpriteDir);
  AddChild(&lGfxDir);
  AddChild(&eGfxDir);
  AddChild(&lGuiDir);
  AddChild(&eGuiDir);
  AddChild(&lSndDir);
  AddChild(&eSndDir);
  
  AddChild(&bChange);
  AddChild(&bCancel);

  AddChild(&Board1);
}

void GUI_PathsMenu::Default()
{
  eScriptDir.SetText(script_dir.string);
  eMapDir.SetText(map_dir.string);
  eSkinDir.SetText(skin_dir.string);
  eSpriteDir.SetText(sprite_dir.string);
  eGuiDir.SetText(gui_dir.string);
  eGfxDir.SetText(gfx_dir.string);
  eSndDir.SetText(snd_dir.string);
}

bool GUI_PathsMenu::eventButtonClick(int id, SDLWidget* widget)
{
  switch (id) {
  case 1:
    ConOut("");
    ConOut("> Paths menu sequence <");
    ConOut("script_dir %s", eScriptDir.GetText());
    CommandExecute("script_dir %s", eScriptDir.GetText());
    ConOut("map_dir %s", eMapDir.GetText());
    CommandExecute("map_dir %s", eMapDir.GetText());
    ConOut("skin_dir %s", eSkinDir.GetText());
    CommandExecute("skin_dir %s", eSkinDir.GetText());
    ConOut("sprite_dir %s", eSpriteDir.GetText());
    CommandExecute("sprite_dir %s", eSpriteDir.GetText());
    ConOut("gfx_dir %s", eGfxDir.GetText());
    CommandExecute("gfx_dir %s", eGfxDir.GetText());
    ConOut("gui_dir %s", eGuiDir.GetText());
    CommandExecute("gui_dir %s", eGuiDir.GetText());
    ConOut("snd_dir %s", eSndDir.GetText());
    CommandExecute("snd_dir %s", eSndDir.GetText());
    ConOut("> end of sequence <");
    ConOut("");
    break;
  case 2:
    Return();
    break;
  }
  return true;
}

void GUI_PathsMenu::Show()
{
  Clear();
  
  Board1.Show();
  lPathsMenu.Show();
  lEffect.Show();
  
  lScriptDir.Show();
  eScriptDir.Show();
  lMapDir.Show();
  eMapDir.Show();
  lSkinDir.Show();
  eSkinDir.Show();
  lSpriteDir.Show();
  eSpriteDir.Show();
  lGfxDir.Show();
  eGfxDir.Show();
  lGuiDir.Show();
  eGuiDir.Show();
  lSndDir.Show();
  eSndDir.Show();
  
  bChange.Show();
  bCancel.Show();
}

void GUI_PathsMenu::Hide()
{
  Board1.Hide();
  lPathsMenu.Hide();
  lEffect.Hide();
  
  lScriptDir.Hide();
  eScriptDir.Hide();
  lMapDir.Hide();
  eMapDir.Hide();
  lSkinDir.Hide();
  eSkinDir.Hide();
  lSpriteDir.Hide();
  eSpriteDir.Hide();
  lGfxDir.Hide();
  eGfxDir.Hide();
  lGuiDir.Hide();
  eGuiDir.Hide();
  lSndDir.Hide();
  eSndDir.Hide();
  
  bChange.Hide();
  bCancel.Hide();
}


/////////////////////////////////////////////////////////////////////////////
// Skin widgets
/////////////////////////////////////////////////////////////////////////////

GUI_ButtonS::GUI_ButtonS(SDLWidget* parent, int btnid, SDL_Rect& r):
SDLWidget(parent, r, false)
{
  //  drawbackground = true;
  SDL_Surface* background = NULL;
  id = btnid;
}

void GUI_ButtonS::eventDraw(SDL_Surface* surface, SDL_Rect* rect)
{
  if(background == NULL){
    return;
  }
  
  SDL_Rect dstrect;
  dstrect.x = 0;
  dstrect.y = 0;
  dstrect.w = background->w;
  dstrect.h = background->h;
  
  SDL_Rect srcrect;
  srcrect.x = rect->x;
  srcrect.y = rect->y;
  srcrect.w = background->w;
  srcrect.h = background->h;
  
  SDL_BlitSurface(background, &srcrect, surface, &dstrect);
}

bool GUI_ButtonS::eventMouseButtonDown(const SDL_MouseButtonEvent* button){
  SetCapture();
  
  Redraw();
  Update();
  return true;
}

bool GUI_ButtonS::eventMouseButtonUp(const SDL_MouseButtonEvent* button){
  ReleaseCapture();
  Redraw();
  
  if(!IsMouseInside())
    return false;
  SDLMessageObject::SendMessage(GetParent(), MSG_BUTTONCLICK, id, 0);
  
  return true;
}

bool GUI_ButtonS::SetBackground(const char* filename, int mode){
  background = SDL_LoadBMP(SDLApplication::GetRelativePath((char*)filename));
  return (background != NULL);
}

GUI_SkinPic::GUI_SkinPic(SDLWidget* parent, SDL_Rect& r):
SDLWidget(parent, r, false)
{
  
  lName = NULL;
  sprite = NULL;
}

GUI_SkinPic::~GUI_SkinPic()
{
  if (sprite) SkinMan.Unregister(sprite);
}

void GUI_SkinPic::eventDraw(SDL_Surface* surface, SDL_Rect* rect)
{
  SDL_Rect sr;
  SDL_Rect dr;
  
  SDL_FillRect(surface, NULL, 255);
  if (sprite)
  {
    // locate rectangle of source sprite
    sr.w = sprite->anims[0]->w;
    sr.h = sprite->anims[0]->h;
    sr.x = 0;
    sr.y = 0;
    dr.w = sprite->anims[0]->w;
    dr.h = sprite->anims[0]->h;
    dr.x = rect->x;
    dr.y = rect->y;
    SDL_BlitSurface(sprite->anims[0]->gfx, &sr, surface, &dr);
  }
}

bool GUI_SkinPic::NextSkin()
{
  CSpriteInfo *curr = SkinMan.sprites;
  
  while (curr && strcmp(lName->GetText(), curr->name)!=0)
  {
    curr = curr->next;
  }
  
  if (!curr) 
  {
    SetSkin(sprite_imp.string);
    return false;
  }
  else 
  {
    if (!curr->next) return true;
    SetSkin(curr->next->name);
    return (curr->next->next==NULL);
  }
}

bool GUI_SkinPic::PrevSkin()
{
  CSpriteInfo *prev1 = NULL;
  CSpriteInfo *prev2 = NULL;
  CSpriteInfo *curr = SkinMan.sprites;
  
  while (curr && strcmp(lName->GetText(), curr->name)!=0)
  {
    prev2 = prev1;
    prev1 = curr;
    curr = curr->next;
  }
  
  if (!curr) 
  {
    SetSkin(sprite_imp.string);
    return false;
  }
  else 
  {
    if (!prev1) return true;
    SetSkin(prev1->name);
    return (prev2==NULL);
  }
}

void GUI_SkinPic::SetSkin(char* ns)
{
  if (sprite) SkinMan.Unregister(sprite);
  sprite = SkinMan.Register(ns);
  lName->SetText(ns);
  GetParent()->Redraw();
}

GUI_SkinCtrl::GUI_SkinCtrl(SDLWidget* parent, SDL_Rect& r,  bool storebackground):
SDLWidget(parent,r,storebackground),
SkinPic(NULL, SDLWidget::mkrect(CP_SK_X+1, CP_SK_Y+1, 32, 32)),
bNext(NULL, 2, SDLWidget::mkrect(CP_SK_X+17, CP_SK_Y+33, 16, 16)),
bPrev(NULL, 1, SDLWidget::mkrect(CP_SK_X+1, CP_SK_Y+33, 16, 16))
{
  c = GUI_Gray64;
  drawbackground = true;
  
  bPrev.SetIcon("./s1l.bmp", "./s1l.bmp");
  bNext.SetIcon("./s1p.bmp", "./s1p.bmp");
  
  AddChild(&SkinPic);
  AddChild(&bPrev);
  AddChild(&bNext);
}

void GUI_SkinCtrl::eventDraw(SDL_Surface* surface, SDL_Rect* rect)
{
  if (drawbackground) {
    SDL_FillRect(surface, rect, SDL_MapRGB(surface->format, c.r, c.g, c.b));
    SDLWidget::DrawBorder(surface, rect, 1, false);
  }
  else
    SDL_FillRect(surface, rect, 0);
  
  SDLWidget::eventDraw(surface, rect);
}

bool GUI_SkinCtrl::eventButtonClick(int id, SDLWidget* widget)
{
  switch (id) {
  case 1:
    if (SkinPic.PrevSkin())
    {
      bPrev.Hide();
    }
    else
    {
      bPrev.Show();
    }
    bNext.Show();
    break;
  case 2:
    if (SkinPic.NextSkin())
    {
      bNext.Hide();
    }
    else
    {
      bNext.Show();
    }
    bPrev.Show();
    break;
  }
  return true;
}


GUI_Input::GUI_Input(SDLWidget* parent, SDL_Rect& r, int iid) :
SDLGradientWidget(parent,r){
  SetFont(TextFont);
  text[0] = 0;
	offset_x = 0;

	textcolor.r = 0xFF;
	textcolor.g = 0xFF;
	textcolor.b = 0xFF;

  id = iid;
	waiting = false;

  LoadThemeStyle("GUI_Input", "GradientWidget");
}

GUI_Input::~GUI_Input(){
}


void GUI_Input::eventDraw(SDL_Surface* surface, SDL_Rect* rect){
	if (waiting)
    textcolor = GUI_InputActive;    
  else
    textcolor = GUI_InputInactive;    

  SDLGradientWidget::eventDraw(surface, rect);

	DrawText(surface, rect);
	DrawBorder(surface, rect, 1, false);
}

void GUI_Input::DrawText(SDL_Surface* surface, SDL_Rect* rect){
	int h;
	int x,y;

	x = rect->x + 3;
	h = TTF_FontHeight(GetFont());
	y = (rect->h - h)/2;
	// draw text
	char* DrawText = GetDrawText();
	if(DrawText[0] != 0){
		SDLDrawObject::DrawText(x+offset_x,y+2, DrawText, textcolor, GetFont());
	}		
}

char* GUI_Input::GetDrawText(){
	return text;	
}

void GUI_Input::InputBegin(){
	SetInputFocus();
	waiting = true;

	Redraw();
}

void GUI_Input::InputEnd(){
	waiting = false;
	Redraw();
	ReleaseInputFocus();
}

bool GUI_Input::eventKeyDown(const SDL_KeyboardEvent* key){

	if(!waiting){
		return false;
	}
	
	switch(key->keysym.sym){
    case SDLK_ESCAPE:
      InputEnd();
			return false;

		// forbidden keys
    case SDLK_F1: 
    case SDLK_F2:
    case SDLK_F3:
    case SDLK_F4:
			return false;

		default:
      InputEnd();
      SetText(SDL_GetKeyName(key->keysym.sym));
      sym=key->keysym.sym;

	    SendMessage(GetParent(), MSG_SIGNAL, id, 0);
			return true;
	}
}

void GUI_Input::eventInputFocusLost(SDLMessageObject* newfocus){
	InputEnd();
}

bool GUI_Input::eventMouseButtonDown(const SDL_MouseButtonEvent* button){
	if(!waiting){
		InputBegin();
	}
	return true;
}

char* GUI_Input::GetText(){
	return text;
}

void GUI_Input::SetText(char* new_text){
	strcpy(text, new_text);
	Redraw();
}



void GUI_CreatePlayerMenu::Return()
{
  if (iUp.waiting || iDown.waiting || iLeft.waiting || iRight.waiting ||
      iWeapon1.waiting || iWeapon2.waiting || iWeapon3.waiting || iWeapon4.waiting || iWeapon5.waiting
      || iShield.waiting || iWarp.waiting || iFire.waiting)
  {
    iUp.eventInputFocusLost(NULL);  
    iDown.eventInputFocusLost(NULL);  
    iLeft.eventInputFocusLost(NULL);  
    iRight.eventInputFocusLost(NULL);  
    iFire.eventInputFocusLost(NULL);  
    iWarp.eventInputFocusLost(NULL);  
    iShield.eventInputFocusLost(NULL);  
    iWeapon1.eventInputFocusLost(NULL);  
    iWeapon2.eventInputFocusLost(NULL);  
    iWeapon3.eventInputFocusLost(NULL);  
    iWeapon4.eventInputFocusLost(NULL);  
    iWeapon5.eventInputFocusLost(NULL);  
    return;
  }
  
  ePName.EditEnd();
  eSkin.EditEnd();

  GUI_BaseMenu::Return();
}

GUI_CreatePlayerMenu::GUI_CreatePlayerMenu(int id, int inum):
GUI_BaseMenu(id, mkrect(CP_PX,CP_PY,CP_VX,CP_VY)),
Board1(NULL, SDLWidget::mkrect(CP_PX,CP_PY,CP_VX,270), false),
CreatePlayerMenu(NULL, SDLWidget::mkrect(CP_PX+1,CP_PY+4,CP_VX-2,25), "CREATE PLAYER", false),
lPName(NULL, SDLWidget::mkrect(CP_PX+1,CP_PY+40,180,20), "Player name:", false),
ePName(NULL, SDLWidget::mkrect(CP_PX+10,CP_PY+60,180,20)),
lSkin(NULL, SDLWidget::mkrect(CP_PX+1+220,CP_PY+40, 110,20), "Player skin:", false),
eSkin(NULL, SDLWidget::mkrect(CP_PX+10+220,CP_PY+60,110,20)),
SkinCtrl(NULL, SDLWidget::mkrect(CP_SK_X, CP_SK_Y, 34, 50), false),
lUp(NULL, SDLWidget::mkrect(CP_INPUT_X1    ,CP_INPUT_Y1    , CCSX,20), "key up:", false),
iUp(NULL, SDLWidget::mkrect(CP_INPUT_X1+CCSX,CP_INPUT_Y1    , 100 ,20),1),
lDown(NULL, SDLWidget::mkrect(CP_INPUT_X1    ,CP_INPUT_Y1+20 , CCSX,20), "key down:", false),
iDown(NULL, SDLWidget::mkrect(CP_INPUT_X1+CCSX,CP_INPUT_Y1+20 , 100 ,20),2),
lLeft(NULL, SDLWidget::mkrect(CP_INPUT_X1    ,CP_INPUT_Y1+40 , CCSX,20), "key left:", false),
iLeft(NULL, SDLWidget::mkrect(CP_INPUT_X1+CCSX,CP_INPUT_Y1+40 , 100 ,20),3),
lRight(NULL, SDLWidget::mkrect(CP_INPUT_X1    ,CP_INPUT_Y1+60 , CCSX,20), "key right:", false),
iRight(NULL, SDLWidget::mkrect(CP_INPUT_X1+CCSX,CP_INPUT_Y1+60 , 100 ,20),4),
lFire(NULL, SDLWidget::mkrect(CP_INPUT_X1    ,CP_INPUT_Y1+80 , CCSX,20), "key fire:", false),
iFire(NULL, SDLWidget::mkrect(CP_INPUT_X1+CCSX,CP_INPUT_Y1+80 , 100 ,20),5),
lShield(NULL, SDLWidget::mkrect(CP_INPUT_X1    ,CP_INPUT_Y1+100, CCSX,20), "shield:", false),
iShield(NULL, SDLWidget::mkrect(CP_INPUT_X1+CCSX,CP_INPUT_Y1+100, 100 ,20),6),
lWarp(NULL, SDLWidget::mkrect(CP_INPUT_X1    ,CP_INPUT_Y1+120, CCSX,20), "warp:", false),
iWarp(NULL, SDLWidget::mkrect(CP_INPUT_X1+CCSX,CP_INPUT_Y1+120, 100 ,20),7),
lWeapon1(NULL, SDLWidget::mkrect(CP_INPUT_X2    ,CP_INPUT_Y2, CCSX,20), "pistolka:", false),
iWeapon1(NULL, SDLWidget::mkrect(CP_INPUT_X2+CCSX,CP_INPUT_Y2, 100 ,20),8),
lWeapon2(NULL, SDLWidget::mkrect(CP_INPUT_X2    ,CP_INPUT_Y2+20, CCSX,20), "becheromet:", false),
iWeapon2(NULL, SDLWidget::mkrect(CP_INPUT_X2+CCSX,CP_INPUT_Y2+20, 100 ,20),9),
lWeapon3(NULL, SDLWidget::mkrect(CP_INPUT_X2    ,CP_INPUT_Y2+40, CCSX,20), "railgun:", false),
iWeapon3(NULL, SDLWidget::mkrect(CP_INPUT_X2+CCSX,CP_INPUT_Y2+40, 100 ,20),10),
lWeapon4(NULL, SDLWidget::mkrect(CP_INPUT_X2    ,CP_INPUT_Y2+60, CCSX,20), "bombs:", false),
iWeapon4(NULL, SDLWidget::mkrect(CP_INPUT_X2+CCSX,CP_INPUT_Y2+60, 100 ,20),11),
lWeapon5(NULL, SDLWidget::mkrect(CP_INPUT_X2    ,CP_INPUT_Y2+80, CCSX,20), "grenades:", false),
iWeapon5(NULL, SDLWidget::mkrect(CP_INPUT_X2+CCSX,CP_INPUT_Y2+80, 100 ,20),12),
bOK(NULL, 1, SDLWidget::mkrect(CP_PX+25,CP_PY+240,150,25), "OK"),
bCancel(NULL, 2, SDLWidget::mkrect(CP_PX+25+150+10,CP_PY+240,150,25), "BACK")
{
  num=inum;
  Default();
  char sx[100];

  CreatePlayerMenu.SetColor(GUI_BtnTextColor, GUI_BtnATextColor);
  CreatePlayerMenu.SetFont(MainFont);
  sprintf(sx, "CREATE PLAYER%d", num);
  CreatePlayerMenu.SetText(sx);
  
  CreatePlayerMenu.bgmode = 2;
  lPName.bgmode = 2;
  lPName.SetAlignment(SDL_TA_LEFT); 
  lPName.shiftx = 4;
  lSkin.bgmode = 2;
  lSkin.SetAlignment(SDL_TA_LEFT); 
  lSkin.shiftx = 4;

  SkinCtrl.SkinPic.lName = &eSkin;
  switch (num) {
    case 1: SkinCtrl.SkinPic.SetSkin(p1_skin.string); break;
    case 2: SkinCtrl.SkinPic.SetSkin(p2_skin.string); break;
    case 3: SkinCtrl.SkinPic.SetSkin(p3_skin.string); break;
    case 4: SkinCtrl.SkinPic.SetSkin(p4_skin.string); break;
  }
   
  
  lUp.SetAlignment(SDL_TA_RIGHT); 
  lDown.SetAlignment(SDL_TA_RIGHT); 
  lLeft.SetAlignment(SDL_TA_RIGHT); 
  lRight.SetAlignment(SDL_TA_RIGHT); 
  lFire.SetAlignment(SDL_TA_RIGHT); 
  lShield.SetAlignment(SDL_TA_RIGHT); 
  lWarp.SetAlignment(SDL_TA_RIGHT); 

  lWeapon1.SetAlignment(SDL_TA_RIGHT); 
  lWeapon2.SetAlignment(SDL_TA_RIGHT); 
  lWeapon3.SetAlignment(SDL_TA_RIGHT); 
  lWeapon4.SetAlignment(SDL_TA_RIGHT); 
  lWeapon5.SetAlignment(SDL_TA_RIGHT); 

  AddChild(&CreatePlayerMenu);
  AddChild(&lPName);
  AddChild(&ePName);
  AddChild(&lSkin);
  AddChild(&eSkin);
  AddChild(&SkinCtrl);

  AddChild(&lUp);
  AddChild(&iUp);
  AddChild(&lDown);
  AddChild(&iDown);
  AddChild(&lLeft);
  AddChild(&iLeft);
  AddChild(&lRight);
  AddChild(&iRight);
  AddChild(&lFire);
  AddChild(&iFire);
  AddChild(&lShield);
  AddChild(&iShield);
  AddChild(&lWarp);
  AddChild(&iWarp);
  AddChild(&lWeapon1);
  AddChild(&iWeapon1);
  AddChild(&lWeapon2);
  AddChild(&iWeapon2);
  AddChild(&lWeapon3);
  AddChild(&iWeapon3);
  AddChild(&lWeapon4);
  AddChild(&iWeapon4);
  AddChild(&lWeapon5);
  AddChild(&iWeapon5);

  AddChild(&bOK);
  AddChild(&bCancel);

  AddChild(&Board1);
}

void GUI_CreatePlayerMenu::Default()
{
  switch (num) {
  case 1:
    ePName.SetText(p1_name.string);

    iUp.SetText(SDL_GetKeyName((SDLKey)(Uint16)p1_key_up.value));
    iDown.SetText(SDL_GetKeyName((SDLKey)(Uint16)p1_key_down.value));
    iLeft.SetText(SDL_GetKeyName((SDLKey)(Uint16)p1_key_left.value));
    iRight.SetText(SDL_GetKeyName((SDLKey)(Uint16)p1_key_right.value));
    iFire.SetText(SDL_GetKeyName((SDLKey)(Uint16)p1_key_fire.value));
    iShield.SetText(SDL_GetKeyName((SDLKey)(Uint16)p1_key_shield.value));
    iWarp.SetText(SDL_GetKeyName((SDLKey)(Uint16)p1_key_warp.value));

    iWeapon1.SetText(SDL_GetKeyName((SDLKey)(Uint16)p1_key_weapon1.value));
    iWeapon2.SetText(SDL_GetKeyName((SDLKey)(Uint16)p1_key_weapon2.value));
    iWeapon3.SetText(SDL_GetKeyName((SDLKey)(Uint16)p1_key_weapon3.value));
    iWeapon4.SetText(SDL_GetKeyName((SDLKey)(Uint16)p1_key_weapon4.value));
    iWeapon5.SetText(SDL_GetKeyName((SDLKey)(Uint16)p1_key_weapon5.value));

    iUp.SetSym((Uint16)p1_key_up.value);
    iDown.SetSym((Uint16)p1_key_down.value);
    iLeft.SetSym((Uint16)p1_key_left.value);
    iRight.SetSym((Uint16)p1_key_right.value);
    iFire.SetSym((Uint16)p1_key_fire.value);
    iShield.SetSym((Uint16)p1_key_shield.value);
    iWarp.SetSym((Uint16)p1_key_warp.value);

    iWeapon1.SetSym((Uint16)p1_key_weapon1.value);
    iWeapon2.SetSym((Uint16)p1_key_weapon2.value);
    iWeapon3.SetSym((Uint16)p1_key_weapon3.value);
    iWeapon4.SetSym((Uint16)p1_key_weapon4.value);
    iWeapon5.SetSym((Uint16)p1_key_weapon5.value);
    break;
  case 2:
    ePName.SetText(p2_name.string);
    iUp.SetText(SDL_GetKeyName((SDLKey)(Uint16)p2_key_up.value));
    iDown.SetText(SDL_GetKeyName((SDLKey)(Uint16)p2_key_down.value));
    iLeft.SetText(SDL_GetKeyName((SDLKey)(Uint16)p2_key_left.value));
    iRight.SetText(SDL_GetKeyName((SDLKey)(Uint16)p2_key_right.value));
    iFire.SetText(SDL_GetKeyName((SDLKey)(Uint16)p2_key_fire.value));
    iShield.SetText(SDL_GetKeyName((SDLKey)(Uint16)p2_key_shield.value));
    iWarp.SetText(SDL_GetKeyName((SDLKey)(Uint16)p2_key_warp.value));

    iWeapon1.SetText(SDL_GetKeyName((SDLKey)(Uint16)p2_key_weapon1.value));
    iWeapon2.SetText(SDL_GetKeyName((SDLKey)(Uint16)p2_key_weapon2.value));
    iWeapon3.SetText(SDL_GetKeyName((SDLKey)(Uint16)p2_key_weapon3.value));
    iWeapon4.SetText(SDL_GetKeyName((SDLKey)(Uint16)p2_key_weapon4.value));
    iWeapon5.SetText(SDL_GetKeyName((SDLKey)(Uint16)p2_key_weapon5.value));

    iUp.SetSym((Uint16)p2_key_up.value);
    iDown.SetSym((Uint16)p2_key_down.value);
    iLeft.SetSym((Uint16)p2_key_left.value);
    iRight.SetSym((Uint16)p2_key_right.value);
    iFire.SetSym((Uint16)p2_key_fire.value);
    iShield.SetSym((Uint16)p2_key_shield.value);
    iWarp.SetSym((Uint16)p2_key_warp.value);

    iWeapon1.SetSym((Uint16)p2_key_weapon1.value);
    iWeapon2.SetSym((Uint16)p2_key_weapon2.value);
    iWeapon3.SetSym((Uint16)p2_key_weapon3.value);
    iWeapon4.SetSym((Uint16)p2_key_weapon4.value);
    iWeapon5.SetSym((Uint16)p2_key_weapon5.value);
    break;
  case 3:
    ePName.SetText(p3_name.string);
    iUp.SetText(SDL_GetKeyName((SDLKey)(Uint16)p3_key_up.value));
    iDown.SetText(SDL_GetKeyName((SDLKey)(Uint16)p3_key_down.value));
    iLeft.SetText(SDL_GetKeyName((SDLKey)(Uint16)p3_key_left.value));
    iRight.SetText(SDL_GetKeyName((SDLKey)(Uint16)p3_key_right.value));
    iFire.SetText(SDL_GetKeyName((SDLKey)(Uint16)p3_key_fire.value));
    iShield.SetText(SDL_GetKeyName((SDLKey)(Uint16)p3_key_shield.value));
    iWarp.SetText(SDL_GetKeyName((SDLKey)(Uint16)p3_key_warp.value));

    iWeapon1.SetText(SDL_GetKeyName((SDLKey)(Uint16)p3_key_weapon1.value));
    iWeapon2.SetText(SDL_GetKeyName((SDLKey)(Uint16)p3_key_weapon2.value));
    iWeapon3.SetText(SDL_GetKeyName((SDLKey)(Uint16)p3_key_weapon3.value));
    iWeapon4.SetText(SDL_GetKeyName((SDLKey)(Uint16)p3_key_weapon4.value));
    iWeapon5.SetText(SDL_GetKeyName((SDLKey)(Uint16)p3_key_weapon5.value));

    iUp.SetSym((Uint16)p3_key_up.value);
    iDown.SetSym((Uint16)p3_key_down.value);
    iLeft.SetSym((Uint16)p3_key_left.value);
    iRight.SetSym((Uint16)p3_key_right.value);
    iFire.SetSym((Uint16)p3_key_fire.value);
    iShield.SetSym((Uint16)p3_key_shield.value);
    iWarp.SetSym((Uint16)p3_key_warp.value);

    iWeapon1.SetSym((Uint16)p3_key_weapon1.value);
    iWeapon2.SetSym((Uint16)p3_key_weapon2.value);
    iWeapon3.SetSym((Uint16)p3_key_weapon3.value);
    iWeapon4.SetSym((Uint16)p3_key_weapon4.value);
    iWeapon5.SetSym((Uint16)p3_key_weapon5.value);
    break;
  case 4:
    ePName.SetText(p4_name.string);
    iUp.SetText(SDL_GetKeyName((SDLKey)(Uint16)p4_key_up.value));
    iDown.SetText(SDL_GetKeyName((SDLKey)(Uint16)p4_key_down.value));
    iLeft.SetText(SDL_GetKeyName((SDLKey)(Uint16)p4_key_left.value));
    iRight.SetText(SDL_GetKeyName((SDLKey)(Uint16)p4_key_right.value));
    iFire.SetText(SDL_GetKeyName((SDLKey)(Uint16)p4_key_fire.value));
    iShield.SetText(SDL_GetKeyName((SDLKey)(Uint16)p4_key_shield.value));
    iWarp.SetText(SDL_GetKeyName((SDLKey)(Uint16)p4_key_warp.value));

    iWeapon1.SetText(SDL_GetKeyName((SDLKey)(Uint16)p4_key_weapon1.value));
    iWeapon2.SetText(SDL_GetKeyName((SDLKey)(Uint16)p4_key_weapon2.value));
    iWeapon3.SetText(SDL_GetKeyName((SDLKey)(Uint16)p4_key_weapon3.value));
    iWeapon4.SetText(SDL_GetKeyName((SDLKey)(Uint16)p4_key_weapon4.value));
    iWeapon5.SetText(SDL_GetKeyName((SDLKey)(Uint16)p4_key_weapon5.value));

    iUp.SetSym((Uint16)p4_key_up.value);
    iDown.SetSym((Uint16)p4_key_down.value);
    iLeft.SetSym((Uint16)p4_key_left.value);
    iRight.SetSym((Uint16)p4_key_right.value);
    iFire.SetSym((Uint16)p4_key_fire.value);
    iShield.SetSym((Uint16)p4_key_shield.value);
    iWarp.SetSym((Uint16)p4_key_warp.value);

    iWeapon1.SetSym((Uint16)p4_key_weapon1.value);
    iWeapon2.SetSym((Uint16)p4_key_weapon2.value);
    iWeapon3.SetSym((Uint16)p4_key_weapon3.value);
    iWeapon4.SetSym((Uint16)p4_key_weapon4.value);
    iWeapon5.SetSym((Uint16)p4_key_weapon5.value);
    break;
  }
}

bool GUI_CreatePlayerMenu::eventButtonClick(int id, SDLWidget* widget)
{
  GPlayer* p;
  CGame& g = client_info.game;
  GAME_MAXOBJS_TYPE i;

  switch (id) {
  case 1:
    ConOut("");
    ConOut("> CreatePlayer menu sequence <");
    
    switch (num) {
    case 1:
      ConOut("p1_name %s", ePName.GetText());
      CommandExecute("p1_name %s", ePName.GetText());
      ConOut("p1_skin %s", eSkin.GetText());
      CommandExecute("p1_skin %s", eSkin.GetText());
      ConOut("p1_key_up %d", iUp.GetSym());
      CommandExecute("p1_key_up %d", iUp.GetSym());
      ConOut("p1_key_down %d", iDown.GetSym());
      CommandExecute("p1_key_down %d", iDown.GetSym());
      ConOut("p1_key_left %d", iLeft.GetSym());
      CommandExecute("p1_key_left %d", iLeft.GetSym());
      ConOut("p1_key_right %d", iRight.GetSym());
      CommandExecute("p1_key_right %d", iRight.GetSym());
      ConOut("p1_key_fire %d", iFire.GetSym());
      CommandExecute("p1_key_fire %d", iFire.GetSym());
      ConOut("p1_key_shield %d", iShield.GetSym());
      CommandExecute("p1_key_shield %d", iShield.GetSym());
      ConOut("p1_key_warp %d", iWarp.GetSym());
      CommandExecute("p1_key_warp %d", iWarp.GetSym());
      ConOut("p1_key_weapon1 %d", iWeapon1.GetSym());
      CommandExecute("p1_key_weapon1 %d", iWeapon1.GetSym());
      ConOut("p1_key_weapon2 %d", iWeapon2.GetSym());
      CommandExecute("p1_key_weapon2 %d", iWeapon2.GetSym());
      ConOut("p1_key_weapon3 %d", iWeapon3.GetSym());
      CommandExecute("p1_key_weapon3 %d", iWeapon3.GetSym());
      ConOut("p1_key_weapon4 %d", iWeapon4.GetSym());
      CommandExecute("p1_key_weapon4 %d", iWeapon4.GetSym());
      ConOut("p1_key_weapon5 %d", iWeapon5.GetSym());
      CommandExecute("p1_key_weapon5 %d", iWeapon5.GetSym());
    
      for (i=0; i<GAME_MAX_OBJS; i++) 
      {
        if ((g.objs[i]->state&OSTATE_ACTIVE) && (g.objs[i]->GetType()==ot_player))
        {
          p = (GPlayer*)g.objs[i]; 
          if (strcmp(p->player_name.GetValRef()->chars, ePName.GetText())==0) 
          {
            ConOut("Player %s found in game, reseting ...", p1_name.string);
            SetKeybindings(p->kbindex, 0);
            ConOut("sp %s %s", p1_name.string, p1_skin.string);
            CommandExecute("sp %s %s", p1_name.string, p1_skin.string);
            ConOut("chase %s", p1_name.string);
            CommandExecute("chase %s", p1_name.string);
            return true;
          }
        }
      }
      strcpy(auto_playername[0], p1_name.string);
      time_playername = ticktime + 0;
      strcpy(auto_skinname[0], p1_skin.string);
      strcpy(auto_skinplayername[0], p1_name.string);
      time_skinname = ticktime + 3000;
      strcpy(auto_chasename[0], p1_name.string);
      time_chasename = ticktime + 3000;
    
      break;
    case 2:
      ConOut("p2_name %s", ePName.GetText());
      CommandExecute("p2_name %s", ePName.GetText());
      ConOut("p2_skin %s", eSkin.GetText());
      CommandExecute("p2_skin %s", eSkin.GetText());
      ConOut("p2_key_up %d", iUp.GetSym());
      CommandExecute("p2_key_up %d", iUp.GetSym());
      ConOut("p2_key_down %d", iDown.GetSym());
      CommandExecute("p2_key_down %d", iDown.GetSym());
      ConOut("p2_key_left %d", iLeft.GetSym());
      CommandExecute("p2_key_left %d", iLeft.GetSym());
      ConOut("p2_key_right %d", iRight.GetSym());
      CommandExecute("p2_key_right %d", iRight.GetSym());
      ConOut("p2_key_fire %d", iFire.GetSym());
      CommandExecute("p2_key_fire %d", iFire.GetSym());
      ConOut("p2_key_shield %d", iShield.GetSym());
      CommandExecute("p2_key_shield %d", iShield.GetSym());
      ConOut("p2_key_warp %d", iWarp.GetSym());
      CommandExecute("p2_key_warp %d", iWarp.GetSym());
      ConOut("p2_key_weapon1 %d", iWeapon1.GetSym());
      CommandExecute("p2_key_weapon1 %d", iWeapon1.GetSym());
      ConOut("p2_key_weapon2 %d", iWeapon2.GetSym());
      CommandExecute("p2_key_weapon2 %d", iWeapon2.GetSym());
      ConOut("p2_key_weapon3 %d", iWeapon3.GetSym());
      CommandExecute("p2_key_weapon3 %d", iWeapon3.GetSym());
      ConOut("p2_key_weapon4 %d", iWeapon4.GetSym());
      CommandExecute("p2_key_weapon4 %d", iWeapon4.GetSym());
      ConOut("p2_key_weapon5 %d", iWeapon5.GetSym());
      CommandExecute("p2_key_weapon5 %d", iWeapon5.GetSym());
    
      for (i=0; i<GAME_MAX_OBJS; i++) 
      {
        if ((g.objs[i]->state&OSTATE_ACTIVE) && (g.objs[i]->GetType()==ot_player))
        {
          p = (GPlayer*)g.objs[i]; 
          if (strcmp(p->player_name.GetValRef()->chars, ePName.GetText())==0) 
          {
            ConOut("Player %s found in game, reseting ...", p2_name.string);
            SetKeybindings(p->kbindex, 1);
            ConOut("sp %s %s", p2_name.string, p2_skin.string);
            CommandExecute("sp %s %s", p2_name.string, p2_skin.string);
            ConOut("chase %s", p2_name.string);
            CommandExecute("chase %s", p2_name.string);
            return true;
          }
        }
      }

      strcpy(auto_playername[1], p2_name.string);
      time_playername = ticktime + 0;
      strcpy(auto_skinname[1], p2_skin.string);
      strcpy(auto_skinplayername[1], p2_name.string);
      time_skinname = ticktime + 3000;
      strcpy(auto_chasename[1], p2_name.string);
      time_chasename = ticktime + 3000;
    
      break;
    case 3:
      ConOut("p3_name %s", ePName.GetText());
      CommandExecute("p3_name %s", ePName.GetText());
      ConOut("p3_skin %s", eSkin.GetText());
      CommandExecute("p3_skin %s", eSkin.GetText());
      ConOut("p3_key_up %d", iUp.GetSym());
      CommandExecute("p3_key_up %d", iUp.GetSym());
      ConOut("p3_key_down %d", iDown.GetSym());
      CommandExecute("p3_key_down %d", iDown.GetSym());
      ConOut("p3_key_left %d", iLeft.GetSym());
      CommandExecute("p3_key_left %d", iLeft.GetSym());
      ConOut("p3_key_right %d", iRight.GetSym());
      CommandExecute("p3_key_right %d", iRight.GetSym());
      ConOut("p3_key_fire %d", iFire.GetSym());
      CommandExecute("p3_key_fire %d", iFire.GetSym());
      ConOut("p3_key_shield %d", iShield.GetSym());
      CommandExecute("p3_key_shield %d", iShield.GetSym());
      ConOut("p3_key_warp %d", iWarp.GetSym());
      CommandExecute("p3_key_warp %d", iWarp.GetSym());
      ConOut("p3_key_weapon1 %d", iWeapon1.GetSym());
      CommandExecute("p3_key_weapon1 %d", iWeapon1.GetSym());
      ConOut("p3_key_weapon2 %d", iWeapon2.GetSym());
      CommandExecute("p3_key_weapon2 %d", iWeapon2.GetSym());
      ConOut("p3_key_weapon3 %d", iWeapon3.GetSym());
      CommandExecute("p3_key_weapon3 %d", iWeapon3.GetSym());
      ConOut("p3_key_weapon4 %d", iWeapon4.GetSym());
      CommandExecute("p3_key_weapon4 %d", iWeapon4.GetSym());
      ConOut("p3_key_weapon5 %d", iWeapon5.GetSym());
      CommandExecute("p3_key_weapon5 %d", iWeapon5.GetSym());
    
      for (i=0; i<GAME_MAX_OBJS; i++) 
      {
        if ((g.objs[i]->state&OSTATE_ACTIVE) && (g.objs[i]->GetType()==ot_player))
        {
          p = (GPlayer*)g.objs[i]; 
          if (strcmp(p->player_name.GetValRef()->chars, ePName.GetText())==0) 
          {
            ConOut("Player %s found in game, reseting ...", p3_name.string);
            SetKeybindings(p->kbindex, 2);
            ConOut("sp %s %s", p3_name.string, p3_skin.string);
            CommandExecute("sp %s %s", p3_name.string, p3_skin.string);
            ConOut("chase %s", p3_name.string);
            CommandExecute("chase %s", p3_name.string);
            return true;
          }
        }
      }
      strcpy(auto_playername[2], p3_name.string);
      time_playername = ticktime + 0;
      strcpy(auto_skinname[2], p3_skin.string);
      strcpy(auto_skinplayername[2], p3_name.string);
      time_skinname = ticktime + 3000;
      strcpy(auto_chasename[2], p3_name.string);
      time_chasename = ticktime + 3000;
    
      break;
    case 4:
      ConOut("p4_name %s", ePName.GetText());
      CommandExecute("p4_name %s", ePName.GetText());
      ConOut("p4_skin %s", eSkin.GetText());
      CommandExecute("p4_skin %s", eSkin.GetText());
      ConOut("p4_key_up %d", iUp.GetSym());
      CommandExecute("p4_key_up %d", iUp.GetSym());
      ConOut("p4_key_down %d", iDown.GetSym());
      CommandExecute("p4_key_down %d", iDown.GetSym());
      ConOut("p4_key_left %d", iLeft.GetSym());
      CommandExecute("p4_key_left %d", iLeft.GetSym());
      ConOut("p4_key_right %d", iRight.GetSym());
      CommandExecute("p4_key_right %d", iRight.GetSym());
      ConOut("p4_key_fire %d", iFire.GetSym());
      CommandExecute("p4_key_fire %d", iFire.GetSym());
      ConOut("p4_key_shield %d", iShield.GetSym());
      CommandExecute("p4_key_shield %d", iShield.GetSym());
      ConOut("p4_key_warp %d", iWarp.GetSym());
      CommandExecute("p4_key_warp %d", iWarp.GetSym());
      ConOut("p4_key_weapon1 %d", iWeapon1.GetSym());
      CommandExecute("p4_key_weapon1 %d", iWeapon1.GetSym());
      ConOut("p4_key_weapon2 %d", iWeapon2.GetSym());
      CommandExecute("p4_key_weapon2 %d", iWeapon2.GetSym());
      ConOut("p4_key_weapon3 %d", iWeapon3.GetSym());
      CommandExecute("p4_key_weapon3 %d", iWeapon3.GetSym());
      ConOut("p4_key_weapon4 %d", iWeapon4.GetSym());
      CommandExecute("p4_key_weapon4 %d", iWeapon4.GetSym());
      ConOut("p4_key_weapon5 %d", iWeapon5.GetSym());
      CommandExecute("p4_key_weapon5 %d", iWeapon5.GetSym());
    
      for (i=0; i<GAME_MAX_OBJS; i++) 
      {
        if ((g.objs[i]->state&OSTATE_ACTIVE) && (g.objs[i]->GetType()==ot_player))
        {
          p = (GPlayer*)g.objs[i]; 
          if (strcmp(p->player_name.GetValRef()->chars, ePName.GetText())==0) 
          {
            ConOut("Player %s found in game, reseting ...", p4_name.string);
            SetKeybindings(p->kbindex, 3);
            ConOut("sp %s %s", p4_name.string, p4_skin.string);
            CommandExecute("sp %s %s", p4_name.string, p4_skin.string);
            ConOut("chase %s", p4_name.string);
            CommandExecute("chase %s", p4_name.string);
            return true;
          }
        }
      }
      strcpy(auto_playername[3], p4_name.string);
      time_playername = ticktime + 0;
      strcpy(auto_skinname[3], p4_skin.string);
      strcpy(auto_skinplayername[3], p4_name.string);
      time_skinname = ticktime + 3000;
      strcpy(auto_chasename[3], p4_name.string);
      time_chasename = ticktime + 3000;
    
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
    iDown.InputBegin();
    break;
  case 2:
    iLeft.InputBegin();
    break;
  case 3:
    iRight.InputBegin();
    break;
  case 4:
    iFire.InputBegin();
    break;
  case 5:
    iShield.InputBegin();
    break;
  case 6:
    iWarp.InputBegin();
    break;
  case 7:
    iWeapon1.InputBegin();
    break;
  case 8:
    iWeapon2.InputBegin();
    break;
  case 9:
    iWeapon3.InputBegin();
    break;
  case 10:
    iWeapon4.InputBegin();
    break;
  case 11:
    iWeapon5.InputBegin();
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


void GUI_CreatePlayerMenu::Show()
{
  Clear();
  
  Board1.Show();  
  CreatePlayerMenu.Show();
  
  lPName.Show();
  ePName.Show();
  lSkin.Show();
  eSkin.Show();
  SkinCtrl.Show();

  lUp.Show();
  iUp.Show();
  lDown.Show();
  iDown.Show();
  lLeft.Show();
  iLeft.Show();
  lRight.Show();
  iRight.Show();
  lFire.Show();
  iFire.Show();
  lShield.Show();
  iShield.Show();
  lWarp.Show();
  iWarp.Show();
  lWeapon1.Show();
  iWeapon1.Show();
  lWeapon2.Show();
  iWeapon2.Show();
  lWeapon3.Show();
  iWeapon3.Show();
  lWeapon4.Show();
  iWeapon4.Show();
  lWeapon5.Show();
  iWeapon5.Show();
  
  bOK.Show();  
  bCancel.Show();  
}

void GUI_CreatePlayerMenu::Hide()
{
  Board1.Hide();  
  CreatePlayerMenu.Hide();
  
  lPName.Hide();
  ePName.Hide();
  lSkin.Hide();
  eSkin.Hide();
  SkinCtrl.Hide();

  lUp.Hide();
  iUp.Hide();
  lDown.Hide();
  iDown.Hide();
  lLeft.Hide();
  iLeft.Hide();
  lRight.Hide();
  iRight.Hide();
  lFire.Hide();
  iFire.Hide();
  lShield.Hide();
  iShield.Hide();
  lWarp.Hide();
  iWarp.Hide();
  lWeapon1.Hide();
  iWeapon1.Hide();
  lWeapon2.Hide();
  iWeapon2.Hide();
  lWeapon3.Hide();
  iWeapon3.Hide();
  lWeapon4.Hide();
  iWeapon4.Hide();
  lWeapon5.Hide();
  iWeapon5.Hide();
  
  bOK.Hide();  
  bCancel.Hide();  
}

/////////////////////////////////////////////////////////////////////////////
// Results menu widgets
/////////////////////////////////////////////////////////////////////////////

GUI_ResultsMenu::GUI_ResultsMenu():
GUI_BaseMenu(GUI_RESULTS, mkrect(RM_PX,RM_PY,RM_VX,RM_VY)),
Board1(NULL, SDLWidget::mkrect(RM_PX,RM_PY,RM_VX,60), false),
lResultsMenu(NULL, SDLWidget::mkrect(RM_PX+1,RM_PY+4,RM_VX-2,25), "RESULTS", false),
lLabel1(NULL, SDLWidget::mkrect(RM_PX+1,RM_PY+30,RM_VX-2,20), "Press SPACE to continue.", false)
{
  Default();
  
  lResultsMenu.SetColor(GUI_BtnTextColor, GUI_BtnATextColor);
  lResultsMenu.SetFont(MainFont);
  
  lLabel1.bgmode = 2;
  lResultsMenu.bgmode = 2;

  AddChild(&lResultsMenu);
  AddChild(&lLabel1);

  px = RM_PX;
  py = RM_PY+59;
 
  lNamex.bgmode = 1;
  lNamex.MoveWindow(px, py);
  lNamex.SizeWindow(RM_VX/2 + 20, 20);
  lFragsx.bgmode = 1;
  lFragsx.MoveWindow(px + 20 + RM_VX/2, py);
  lFragsx.SizeWindow(60, 20);
  lPointsx.bgmode = 1;
  lPointsx.MoveWindow(px + 20+60 + RM_VX/2, py);
  lPointsx.SizeWindow(RM_VX/2-60 - 20, 20);
  AddChild(&lNamex);
  AddChild(&lPointsx);
  AddChild(&lFragsx);

  px = RM_PX;
  py = RM_PY+59+19;
  for (int i=0; i<RM_MAX_NAMES; i++)
  {
    lName[i].bgmode = 1;
    lName[i].MoveWindow(px, py + i*19);
    lName[i].SizeWindow(RM_VX/2 + 20, 20);
    lFrags[i].bgmode = 1;
    lFrags[i].MoveWindow(px + 20 + RM_VX/2, py + i*19);
    lFrags[i].SizeWindow(60, 20);
    lPoints[i].bgmode = 1;
    lPoints[i].MoveWindow(px + 20 + 60 + RM_VX/2, py + i*19);
    lPoints[i].SizeWindow(RM_VX/2 - 60 - 20, 20);

    AddChild(&lName[i]);
    AddChild(&lPoints[i]);
    AddChild(&lFrags[i]);
  }

  AddChild(&Board1);

}

void GUI_ResultsMenu::Default()
{
  CGame *g = &client_info.game;
  index = 0;
  int i=0;
  int s[RM_MAX_NAMES]; int slots[RM_MAX_NAMES];
  for (int f=0; f<RM_MAX_NAMES; f++) { s[f]=-100000; slots[f] = -1; }

  for (i=0; i<MAX_GAMEBAR_PLAYERS; i++)
  {
    if (g->GBSlots[i]==GAME_MAX_OBJS) break;
    if (g->objs[g->GBSlots[i]]->GetType()==ot_player)
    {
      GPlayer *p = (GPlayer*) g->objs[g->GBSlots[i]];
      char line[100];
      if (index<RM_MAX_NAMES)
      {
        sprintf(line, "  %-10s", p->player_name.GetValRef()->chars);
        lName[index].SetText(line);
        sprintf(line, "  %3d", (int)*p->frags.GetValRef());
        lFrags[index].SetText(line);
        sprintf(line, "  %4d", (int)*p->points.GetValRef());
        lPoints[index].SetText(line);
        s[index] = p->points;
        slots[index] = i;
        index++;
      }
    }
  }

  char str[100];
  bool sorted = false;
  while (!sorted)
  {
    sorted = true;
    for (i=0; i<RM_MAX_NAMES-1; i++)
    {
      if (s[i]<s[i+1])
      {
        strcpy(str, lName[i+1].GetText());
        lName[i+1].SetText(lName[i].GetText());
        lName[i].SetText(str);
        strcpy(str, lFrags[i+1].GetText());
        lFrags[i+1].SetText(lFrags[i].GetText());
        lFrags[i].SetText(str);
        strcpy(str, lPoints[i+1].GetText());
        lPoints[i+1].SetText(lPoints[i].GetText());
        lPoints[i].SetText(str);
        int x;
        x = s[i+1];
        s[i+1] = s[i];
        s[i] = x;
        x = slots[i+1];
        slots[i+1] = slots[i];
        slots[i] = x;
        sorted = false;
      }
    }
  }

  if (inloop)
  {
    if (slots[0]!=-1 && ((GPlayer*)g->objs[slots[0]])->brain_owner == client_info.client_num)
    {
      smPlayVoice(SM_WIN1+ rand()%3, 100, 250);
    }
    else
    {
      smPlayVoice(SM_END1+ rand()%10, 100, 250);
    }
  }

  lNamex.SetText("  NAME");
  lPointsx.SetText("  POINTS");
  lFragsx.SetText("  FRAGS");
}

void GUI_ResultsMenu::Show()
{
  Clear();
  SizeWindow(RM_VX, 59+(1+index)*19+1);
  
  Board1.Show();
  lResultsMenu.Show();
  lLabel1.Show();

  for (int i=0; i<index; i++)
  {
    lName[i].Show();
    lPoints[i].Show();
    lFrags[i].Show();
  }
  lNamex.Show();
  lPointsx.Show();
  lFragsx.Show();
}

void GUI_ResultsMenu::Hide()
{
  Board1.Hide();
  lResultsMenu.Hide();
  lLabel1.Hide();
 
  lNamex.Hide();
  lPointsx.Hide();
  lFragsx.Hide();
  for (int i=0; i<index; i++)
  {
    lName[i].Hide();
    lPoints[i].Hide();
    lFrags[i].Hide();
  }
}

/////////////////////////////////////////////////////////////////////////////
// Help1 menu widgets
/////////////////////////////////////////////////////////////////////////////

GUI_Help1Menu::GUI_Help1Menu():
GUI_BaseMenu(GUI_HELP1, mkrect(HM1_PX,HM1_PY,HM1_VX,HM1_VY)),
Board1(NULL, SDLWidget::mkrect(HM1_PX,HM1_PY,HM1_VX,HM1_VY), false),
mBack1(NULL, SDLWidget::mkrect(HM1_PX+1, HM1_PY+1, help1->w, help1->h), false, help1),
Help1Menu(NULL, SDLWidget::mkrect(HM1_PX+1,HM1_PY+4,HM1_VX-2,25), "Weapons & Bonuses", false),
lLine1a(NULL, SDLWidget::mkrect(HM1_G1_PX,HM1_G1_PY, HM1_G1_VX-2, 20), "SMALL PISTOL", false),
lLine1b(NULL, SDLWidget::mkrect(HM1_G1_PX,HM1_G1_PY+HM1_LY1, HM1_G1_VX-2, 20), "fires small slow projectiles", false),
lLine1c(NULL, SDLWidget::mkrect(HM1_G1_PX,HM1_G1_PY+2*HM1_LY1, HM1_G1_VX-2, 20), "bounces with shield and some kind of walls", false),
lLine2a(NULL, SDLWidget::mkrect(HM1_G2_PX,HM1_G2_PY, HM1_G2_VX-2, 20), "BECHEROMET", false),
lLine2b(NULL, SDLWidget::mkrect(HM1_G2_PX,HM1_G2_PY+HM1_LY1, HM1_G2_VX-2, 20), "fires high piercing shots in clusters", false),
lLine2c(NULL, SDLWidget::mkrect(HM1_G2_PX,HM1_G2_PY+2*HM1_LY1, HM1_G2_VX-2, 20), "loaded by becherovka bottles", false),
lLine3a(NULL, SDLWidget::mkrect(HM1_G3_PX,HM1_G3_PY, HM1_G3_VX-2, 20), "RAILGUN", false),
lLine3b(NULL, SDLWidget::mkrect(HM1_G3_PX,HM1_G3_PY+HM1_LY1, HM1_G3_VX-2, 20), "after small delay for charge fires long fireline", false),
lLine3c(NULL, SDLWidget::mkrect(HM1_G3_PX,HM1_G3_PY+2*HM1_LY1, HM1_G3_VX-2, 20), "works on basis of pressed alcohol gas", false),
lLine4a(NULL, SDLWidget::mkrect(HM1_G4_PX,HM1_G4_PY, HM1_G4_VX-2, 20), "RAPID BOMB", false),
lLine4b(NULL, SDLWidget::mkrect(HM1_G4_PX,HM1_G4_PY+HM1_LY1, HM1_G4_VX-2, 20), "droped bomb blows up after a period of time", false),
lLine4c(NULL, SDLWidget::mkrect(HM1_G4_PX,HM1_G4_PY+2*HM1_LY1, HM1_G4_VX-2, 20), "chemical modification of rapid drink", false),
lLine5a(NULL, SDLWidget::mkrect(HM1_G5_PX,HM1_G5_PY, HM1_G5_VX-2, 20), "BETON GRENADE", false),
lLine5b(NULL, SDLWidget::mkrect(HM1_G5_PX,HM1_G5_PY+HM1_LY1, HM1_G5_VX-2, 20), "after throwed detonates when stroked", false),
lLine5c(NULL, SDLWidget::mkrect(HM1_G5_PX,HM1_G5_PY+2*HM1_LY1, HM1_G5_VX-2, 20), "deadly modification of beton mixture", false),
lI1a(NULL, SDLWidget::mkrect(HM1_I1_PX,HM1_I1_PY, HM1_I1_VX-2, 20), "BECHER", false),
lI1b(NULL, SDLWidget::mkrect(HM1_I1_PX,HM1_I1_PY+HM1_LY2, HM1_I1_VX-2, 20), "80 points", false),
lI2a(NULL, SDLWidget::mkrect(HM1_I2_PX,HM1_I2_PY, HM1_I2_VX-2, 20), "RAPID", false),
lI2b(NULL, SDLWidget::mkrect(HM1_I2_PX,HM1_I2_PY+HM1_LY2, HM1_I2_VX-2, 20), "20 points", false),
lI3a(NULL, SDLWidget::mkrect(HM1_I3_PX,HM1_I3_PY, HM1_I3_VX-2, 20), "GOLD", false),
lI3b(NULL, SDLWidget::mkrect(HM1_I3_PX,HM1_I3_PY+HM1_LY2, HM1_I3_VX-2, 20), "6 points", false),
lI4a(NULL, SDLWidget::mkrect(HM1_I4_PX,HM1_I4_PY, HM1_I4_VX-2, 20), "GOLD", false),
lI4b(NULL, SDLWidget::mkrect(HM1_I4_PX,HM1_I4_PY+HM1_LY2, HM1_I4_VX-2, 20), "5 points", false),
lI5a(NULL, SDLWidget::mkrect(HM1_I5_PX,HM1_I5_PY, HM1_I5_VX-2, 20), "DRINK", false),
lI5b(NULL, SDLWidget::mkrect(HM1_I5_PX,HM1_I5_PY+HM1_LY2, HM1_I5_VX-2, 20), "11 points", false),
lI6a(NULL, SDLWidget::mkrect(HM1_I6_PX,HM1_I6_PY, HM1_I6_VX-2, 20), "DRINK", false),
lI6b(NULL, SDLWidget::mkrect(HM1_I6_PX,HM1_I6_PY+HM1_LY2, HM1_I6_VX-2, 20), "13 points", false),
bPrev(NULL, 2, SDLWidget::mkrect(HM1_PX+5,HM1_PY+330,70,20), "PREV"),
bReturn(NULL, 1, SDLWidget::mkrect(HM1_PX+(HM1_VX-100)/2,HM1_PY+330,100,20), "RETURN"),
bNext(NULL, 3, SDLWidget::mkrect(HM1_PX+HM1_VX-5-70,HM1_PY+330,70,20), "NEXT")
{
  Default();
  
  Help1Menu.SetAlignment(SDL_TA_CENTER); 
  Help1Menu.SetFont(MainFont);
  
  Help1Menu.bgmode = 0;
  
  AddChild(&mBack1);
  AddChild(&Help1Menu);

  AddChild(&lLine1a);
  AddChild(&lLine1b);
  AddChild(&lLine1c);

  AddChild(&lLine2a);
  AddChild(&lLine2b);
  AddChild(&lLine2c);

  AddChild(&lLine3a);
  AddChild(&lLine3b);
  AddChild(&lLine3c);

  AddChild(&lLine4a);
  AddChild(&lLine4b);
  AddChild(&lLine4c);

  AddChild(&lLine5a);
  AddChild(&lLine5b);
  AddChild(&lLine5c);
  
  AddChild(&lI1a);
  AddChild(&lI1b);
  AddChild(&lI2a);
  AddChild(&lI2b);
  AddChild(&lI3a);
  AddChild(&lI3b);
  AddChild(&lI4a);
  AddChild(&lI4b);
  AddChild(&lI5a);
  AddChild(&lI5b);
  AddChild(&lI6a);
  AddChild(&lI6b);
  
  AddChild(&bReturn);
  AddChild(&bNext);
  AddChild(&bPrev);

  AddChild(&Board1);
}

void GUI_Help1Menu::Default()
{
}

bool GUI_Help1Menu::eventButtonClick(int id, SDLWidget* widget)
{
  switch (id) {
  case 1:
    Return();
    break;
  case 3:
    Return();
    GUI_OpenMenu(GUI_HELP2);
    break;
  }
  return true;
}

void GUI_Help1Menu::Show()
{
  Clear();
  
  Board1.Show();  
  mBack1.Show();  
  Help1Menu.Show();
  
  lLine1a.Show();
  lLine1b.Show();
  lLine1c.Show();
  
  lLine2a.Show();
  lLine2b.Show();
  lLine2c.Show();
  
  lLine3a.Show();
  lLine3b.Show();
  lLine3c.Show();
  
  lLine4a.Show();
  lLine4b.Show();
  lLine4c.Show();
  
  lLine5a.Show();
  lLine5b.Show();
  lLine5c.Show();

  lI1a.Show();
  lI1b.Show();
  lI2a.Show();
  lI2b.Show();
  lI3a.Show();
  lI3b.Show();
  lI4a.Show();
  lI4b.Show();
  lI5a.Show();
  lI5b.Show();
  lI6a.Show();
  lI6b.Show();
  
  bReturn.Show();  
  bNext.Show();  
//  bPrev.Show();  
}

void GUI_Help1Menu::Hide()
{
  Board1.Hide();  
  mBack1.Hide();  
  Help1Menu.Hide();
  
  lLine1a.Hide();
  lLine1b.Hide();
  lLine1c.Hide();
  
  lLine2a.Hide();
  lLine2b.Hide();
  lLine2c.Hide();
  
  lLine3a.Hide();
  lLine3b.Hide();
  lLine3c.Hide();
  
  lLine4a.Hide();
  lLine4b.Hide();
  lLine4c.Hide();
  
  lLine5a.Hide();
  lLine5b.Hide();
  lLine5c.Hide();

  lI1a.Hide();
  lI1b.Hide();
  lI2a.Hide();
  lI2b.Hide();
  lI3a.Hide();
  lI3b.Hide();
  lI4a.Hide();
  lI4b.Hide();
  lI5a.Hide();
  lI5b.Hide();
  lI6a.Hide();
  lI6b.Hide();
  
  bReturn.Hide();  
  bNext.Hide();  
//  bPrev.Hide();  
}

/////////////////////////////////////////////////////////////////////////////
// Help2 menu widgets
/////////////////////////////////////////////////////////////////////////////

GUI_Help2Menu::GUI_Help2Menu():
GUI_BaseMenu(GUI_HELP2, mkrect(HM2_PX,HM2_PY,HM2_VX,HM2_VY)),
Board1(NULL, SDLWidget::mkrect(HM2_PX,HM2_PY,HM2_VX,HM2_VY), false),
mBack1(NULL, SDLWidget::mkrect(HM2_PX+1, HM2_PY+1, help2->w, help2->h), false, help2),
Help2Menu(NULL, SDLWidget::mkrect(HM2_PX+1,HM2_PY+4,HM2_VX-2,25), "Items & Ammo", false),
lI1a(NULL, SDLWidget::mkrect(HM2_I1_PX,HM2_I1_PY, HM2_I1_VX-2, 20), "SPEED", false),
lI1b(NULL, SDLWidget::mkrect(HM2_I1_PX,HM2_I1_PY+HM2_LY1, HM2_I1_VX-2, 20), "faster moving", false),
lI2a(NULL, SDLWidget::mkrect(HM2_I2_PX,HM2_I2_PY, HM2_I2_VX-2, 20), "POWER", false),
lI2b(NULL, SDLWidget::mkrect(HM2_I2_PX,HM2_I2_PY+HM2_LY1, HM2_I2_VX-2, 20), "full bomb power", false),
lI3a(NULL, SDLWidget::mkrect(HM2_I3_PX,HM2_I3_PY, HM2_I3_VX-2, 20), "SHIELD", false),
lI3b(NULL, SDLWidget::mkrect(HM2_I3_PX,HM2_I3_PY+HM2_LY1, HM2_I3_VX-2, 20), "add shields", false),
lI4a(NULL, SDLWidget::mkrect(HM2_I4_PX,HM2_I4_PY, HM2_I4_VX-2, 20), "INVISIBILITY", false),
lI4b(NULL, SDLWidget::mkrect(HM2_I4_PX,HM2_I4_PY+HM2_LY1, HM2_I4_VX-2, 20), "", false),
lI5a(NULL, SDLWidget::mkrect(HM2_I5_PX,HM2_I5_PY, HM2_I5_VX-2, 20), "PUNCH", false),
lI5b(NULL, SDLWidget::mkrect(HM2_I5_PX,HM2_I5_PY+HM2_LY1, HM2_I5_VX-2, 20), "punching bombs", false),
lI6a(NULL, SDLWidget::mkrect(HM2_I6_PX,HM2_I6_PY, HM2_I6_VX-2, 20), "WARP", false),
lI6b(NULL, SDLWidget::mkrect(HM2_I6_PX,HM2_I6_PY+HM2_LY1, HM2_I6_VX-2, 20), "add warps", false),
lI7a(NULL, SDLWidget::mkrect(HM2_I7_PX,HM2_I7_PY, HM2_I7_VX-2, 20), "GLASSES", false),
lI7b(NULL, SDLWidget::mkrect(HM2_I7_PX,HM2_I7_PY+HM2_LY1, HM2_I7_VX-2, 20), "detecting invisibles", false),
lJ1a(NULL, SDLWidget::mkrect(HM2_J1_PX,HM2_J1_PY, HM2_J1_VX-2, 20), "BECHEROMET", false),
lJ1b(NULL, SDLWidget::mkrect(HM2_J1_PX,HM2_J1_PY+HM2_LY2, HM2_J1_VX-2, 20), "enable weapon", false),
lJ2a(NULL, SDLWidget::mkrect(HM2_J2_PX,HM2_J2_PY, HM2_J2_VX-2, 20), "RAILGUN", false),
lJ2b(NULL, SDLWidget::mkrect(HM2_J2_PX,HM2_J2_PY+HM2_LY2, HM2_J2_VX-2, 20), "enable weapon", false),
lJ3a(NULL, SDLWidget::mkrect(HM2_J3_PX,HM2_J3_PY, HM2_J3_VX-2, 20), "BOMB", false),
lJ3b(NULL, SDLWidget::mkrect(HM2_J3_PX,HM2_J3_PY+HM2_LY2, HM2_J3_VX-2, 20), "enable weapon", false),
lJ4a(NULL, SDLWidget::mkrect(HM2_J4_PX,HM2_J4_PY, HM2_J4_VX-2, 20), "GRENADE", false),
lJ4b(NULL, SDLWidget::mkrect(HM2_J4_PX,HM2_J4_PY+HM2_LY2, HM2_J4_VX-2, 20), "enable weapon", false),
lJ5a(NULL, SDLWidget::mkrect(HM2_J5_PX,HM2_J5_PY, HM2_J5_VX-2, 20), "AMMO", false),
lJ5b(NULL, SDLWidget::mkrect(HM2_J5_PX,HM2_J5_PY+HM2_LY2, HM2_J5_VX-2, 20), "becheromet", false),
lJ6a(NULL, SDLWidget::mkrect(HM2_J6_PX,HM2_J6_PY, HM2_J6_VX-2, 20), "AMMO", false),
lJ6b(NULL, SDLWidget::mkrect(HM2_J6_PX,HM2_J6_PY+HM2_LY2, HM2_J6_VX-2, 20), "railgun", false),
lJ7a(NULL, SDLWidget::mkrect(HM2_J7_PX,HM2_J7_PY, HM2_J7_VX-2, 20), "AMMO", false),
lJ7b(NULL, SDLWidget::mkrect(HM2_J7_PX,HM2_J7_PY+HM2_LY2, HM2_J7_VX-2, 20), "bombs", false),
lK1a(NULL, SDLWidget::mkrect(HM2_K1_PX,HM2_K1_PY, HM2_K1_VX-2, 20), "AMMO", false),
lK1b(NULL, SDLWidget::mkrect(HM2_K1_PX,HM2_K1_PY+HM2_LY3, HM2_K1_VX-2, 20), "grenades", false),
lK2a(NULL, SDLWidget::mkrect(HM2_K2_PX,HM2_K2_PY, HM2_K2_VX-2, 20), "TURTLE", false),
lK2b(NULL, SDLWidget::mkrect(HM2_K2_PX,HM2_K2_PY+HM2_LY3, HM2_K2_VX-2, 20), "slower moving", false),
lK3a(NULL, SDLWidget::mkrect(HM2_K3_PX,HM2_K3_PY, HM2_K3_VX-2, 20), "GLUE", false),
lK3b(NULL, SDLWidget::mkrect(HM2_K3_PX,HM2_K3_PY+HM2_LY3, HM2_K3_VX-2, 20), "not able moving", false),
lK4a(NULL, SDLWidget::mkrect(HM2_K4_PX,HM2_K4_PY, HM2_K4_VX-2, 20), "REVERSE", false),
lK4b(NULL, SDLWidget::mkrect(HM2_K4_PX,HM2_K4_PY+HM2_LY3, HM2_K4_VX-2, 20), "reverse controls", false),
lK5a(NULL, SDLWidget::mkrect(HM2_K5_PX,HM2_K5_PY, HM2_K5_VX-2, 20), "LOST", false),
lK5b(NULL, SDLWidget::mkrect(HM2_K5_PX,HM2_K5_PY+HM2_LY3, HM2_K5_VX-2, 20), "disable weapons", false),
lK6a(NULL, SDLWidget::mkrect(HM2_K6_PX,HM2_K6_PY, HM2_K6_VX-2, 20), "BERSERK", false),
lK6b(NULL, SDLWidget::mkrect(HM2_K6_PX,HM2_K6_PY+HM2_LY3, HM2_K6_VX-2, 20), "kill by touch", false),
lK7a(NULL, SDLWidget::mkrect(HM2_K7_PX,HM2_K7_PY, HM2_K7_VX-2, 20), "", false),
lK7b(NULL, SDLWidget::mkrect(HM2_K7_PX,HM2_K7_PY+HM2_LY3, HM2_K7_VX-2, 20), "", false),
bPrev(NULL, 2, SDLWidget::mkrect(HM2_PX+5,HM2_PY+365,70,20), "PREV"),
bReturn(NULL, 1, SDLWidget::mkrect(HM2_PX+(HM2_VX-100)/2,HM2_PY+365,100,20), "RETURN"),
bNext(NULL, 3, SDLWidget::mkrect(HM2_PX+HM2_VX-5-70,HM2_PY+365,70,20), "NEXT")
{
  Default();
  
  Help2Menu.SetAlignment(SDL_TA_CENTER); 
  Help2Menu.SetFont(MainFont);
  
  Help2Menu.bgmode = 0;
  
  AddChild(&mBack1);
  AddChild(&Help2Menu);
  
  AddChild(&lI1a);
  AddChild(&lI1b);
  AddChild(&lI2a);
  AddChild(&lI2b);
  AddChild(&lI3a);
  AddChild(&lI3b);
  AddChild(&lI4a);
  AddChild(&lI4b);
  AddChild(&lI5a);
  AddChild(&lI5b);
  AddChild(&lI6a);
  AddChild(&lI6b);
  AddChild(&lI7a);
  AddChild(&lI7b);
  
  AddChild(&lJ1a);
  AddChild(&lJ1b);
  AddChild(&lJ2a);
  AddChild(&lJ2b);
  AddChild(&lJ3a);
  AddChild(&lJ3b);
  AddChild(&lJ4a);
  AddChild(&lJ4b);
  AddChild(&lJ5a);
  AddChild(&lJ5b);
  AddChild(&lJ6a);
  AddChild(&lJ6b);
  AddChild(&lJ7a);
  AddChild(&lJ7b);
  
  AddChild(&lK1a);
  AddChild(&lK1b);
  AddChild(&lK2a);
  AddChild(&lK2b);
  AddChild(&lK3a);
  AddChild(&lK3b);
  AddChild(&lK4a);
  AddChild(&lK4b);
  AddChild(&lK5a);
  AddChild(&lK5b);
  AddChild(&lK6a);
  AddChild(&lK6b);
  AddChild(&lK7a);
  AddChild(&lK7b);
  
  AddChild(&bReturn);
  AddChild(&bNext);
  AddChild(&bPrev);

  AddChild(&Board1);
}

void GUI_Help2Menu::Default()
{
}

bool GUI_Help2Menu::eventButtonClick(int id, SDLWidget* widget)
{
  switch (id) {
  case 1:
    Return();
    break;
  case 2:
    Return();
    GUI_OpenMenu(GUI_HELP1);
    break;
  }
  return true;
}

void GUI_Help2Menu::Show()
{
  Clear();
  
  Board1.Show();  
  mBack1.Show();  
  Help2Menu.Show();
  
  lI1a.Show();
  lI1b.Show();
  lI2a.Show();
  lI2b.Show();
  lI3a.Show();
  lI3b.Show();
  lI4a.Show();
  lI4b.Show();
  lI5a.Show();
  lI5b.Show();
  lI6a.Show();
  lI6b.Show();
  lI7a.Show();
  lI7b.Show();
  
  lJ1a.Show();
  lJ1b.Show();
  lJ2a.Show();
  lJ2b.Show();
  lJ3a.Show();
  lJ3b.Show();
  lJ4a.Show();
  lJ4b.Show();
  lJ5a.Show();
  lJ5b.Show();
  lJ6a.Show();
  lJ6b.Show();
  lJ7a.Show();
  lJ7b.Show();
  
  lK1a.Show();
  lK1b.Show();
  lK2a.Show();
  lK2b.Show();
  lK3a.Show();
  lK3b.Show();
  lK4a.Show();
  lK4b.Show();
  lK5a.Show();
  lK5b.Show();
  lK6a.Show();
  lK6b.Show();
  lK7a.Show();
  lK7b.Show();
  
  bReturn.Show();  
//  bNext.Show();  
  bPrev.Show();  
}

void GUI_Help2Menu::Hide()
{
  Board1.Hide();  
  mBack1.Hide();  
  Help2Menu.Hide();
  

  lI1a.Hide();
  lI1b.Hide();
  lI2a.Hide();
  lI2b.Hide();
  lI3a.Hide();
  lI3b.Hide();
  lI4a.Hide();
  lI4b.Hide();
  lI5a.Hide();
  lI5b.Hide();
  lI6a.Hide();
  lI6b.Hide();
  lI7a.Hide();
  lI7b.Hide();
  
  lJ1a.Hide();
  lJ1b.Hide();
  lJ2a.Hide();
  lJ2b.Hide();
  lJ3a.Hide();
  lJ3b.Hide();
  lJ4a.Hide();
  lJ4b.Hide();
  lJ5a.Hide();
  lJ5b.Hide();
  lJ6a.Hide();
  lJ6b.Hide();
  lJ7a.Hide();
  lJ7b.Hide();
  
  lK1a.Hide();
  lK1b.Hide();
  lK2a.Hide();
  lK2b.Hide();
  lK3a.Hide();
  lK3b.Hide();
  lK4a.Hide();
  lK4b.Hide();
  lK5a.Hide();
  lK5b.Hide();
  lK6a.Hide();
  lK6b.Hide();
  lK7a.Hide();
  lK7b.Hide();

  bReturn.Hide();  
//  bNext.Hide();  
  bPrev.Hide();  
}



/////////////////////////////////////////////////////////////////////////////
// DeletePlayer menu widgets
/////////////////////////////////////////////////////////////////////////////

void GUI_DeletePlayerMenu::Return()
{
  GUI_BaseMenu::Return();
}

GUI_DeletePlayerMenu::GUI_DeletePlayerMenu():
GUI_BaseMenu(GUI_DELETEPLAYER, mkrect(DPM_PX,DPM_PY,DPM_VX,DPM_VY)),
Board1(NULL, SDLWidget::mkrect(DPM_PX,DPM_PY,DPM_VX,DPM_VY), false),
DeletePlayerMenu(NULL, SDLWidget::mkrect(DPM_PX+1,DPM_PY+4,DPM_VX-2,25), "DELETE PLAYER", false),
lChoose(NULL, SDLWidget::mkrect(DPM_PX+25,DPM_PY+40, 159,20), "Choose player name to delete:", false),
Board2(NULL, SDLWidget::mkrect(DPM_PX+25+150+10, DPM_PY+39, 150, 66), false),
WidgetList(NULL, SDLWidget::mkrect(DPM_PX+26+150+10, DPM_PY+40, 148, 64)),
bDelete(NULL, 1, SDLWidget::mkrect(DPM_PX+25,DPM_PY+120,150,25), "DELETE"),
bCancel(NULL, 2, SDLWidget::mkrect(DPM_PX+25+150+10,DPM_PY+120,150,25), "BACK")
{
  Default();
  
  DeletePlayerMenu.SetColor(GUI_BtnTextColor, GUI_BtnATextColor);
  DeletePlayerMenu.SetFont(MainFont);
  
  DeletePlayerMenu.bgmode = 2;

  lChoose.bgmode = 2;

  
  AddChild(&DeletePlayerMenu);
  AddChild(&bDelete);
  AddChild(&bCancel);
  AddChild(&lChoose);

  AddChild(&Board2);
  AddChild(&Board1);
}

void GUI_DeletePlayerMenu::Default()
{
  GPlayer* p;
  CGame& g = client_info.game;
  GAME_MAXOBJS_TYPE i;

  WidgetList.DeleteAll();
  int id = 0;
  for (i=0; i<GAME_MAX_OBJS; i++) 
  {
    if ((g.objs[i]->state&OSTATE_ACTIVE) && (g.objs[i]->GetType()==ot_player))
    {
      p = (GPlayer*)g.objs[i]; 
      if (p->brain_owner==client_info.client_num)
      {
        id++;
		    WidgetList.AddWidget(new GUI_LabelL(NULL, SDLWidget::mkrect(0,0,150-14,16), p->player_name.GetValRef()->chars, &selected1, 0, GUI_UnselectedItem, GUI_SelectedItem));
      }
    }
  }
  if (WidgetList.GetWidgetCount())
    selected1 = (GUI_LabelL*)WidgetList.FindWidget(0);
  else
    selected1 = NULL;
}

bool GUI_DeletePlayerMenu::eventButtonClick(int id, SDLWidget* widget)
{
  switch (id) {
  case 1:
    if (selected1)
    {
      ConOut("");
      ConOut("> DeletePlayer menu sequence <");
      ConOut("dp %s", selected1->GetText());
      CommandExecute("dp %s", selected1->GetText());
      ConOut("> end of sequence <");
      ConOut("");
      while (GUI_id!=GUI_MAINMENU) GUI_Return(); // return from all menus
    }
    else
      Return();
    break;
  case 2:
    Return();
    break;
  }
  return true;
}

void GUI_DeletePlayerMenu::Show()
{
  Clear();
  
  Board1.Show();  
  Board2.Show();  
  bDelete.Show();  
  bCancel.Show();  
  DeletePlayerMenu.Show();
  bCancel.Show();  
  WidgetList.Show();
  lChoose.Show();  
}
 

void GUI_DeletePlayerMenu::Hide()
{
  Board1.Hide();  
  Board2.Hide();  
  bDelete.Hide();  
  bCancel.Hide();  
  DeletePlayerMenu.Hide();
  WidgetList.Hide();
  lChoose.Hide();  
}

  
/////////////////////////////////////////////////////////////////////////////
// Video menu widgets
/////////////////////////////////////////////////////////////////////////////

void GUI_VideoMenu::Return()
{
  eGamma_r.EditEnd();
  eGamma_g.EditEnd();
  eGamma_b.EditEnd();
  GUI_BaseMenu::Return();
}

GUI_VideoMenu::GUI_VideoMenu():
GUI_BaseMenu(GUI_VIDEO, mkrect(VM_PX,VM_PY,VM_VX,VM_VY)),
Board1(NULL, SDLWidget::mkrect(VM_PX,VM_PY,VM_VX, VM_VY), false),
lEffect(NULL, SDLWidget::mkrect(VM_PX+1,VM_PY+30,VM_VX-2,20), "[r] marked changes will take effect after restarting program.", false),
VideoMenu(NULL, SDLWidget::mkrect(VM_PX+1,VM_PY+4,VM_VX-2,25), "VIDEO OPTIONS", false),
cFullscreen(NULL, SDLWidget::mkrect(VM_PX+25,VM_PY+60,310,CB_SIZEY), "run game in fullscreen mode [r]", true, GUI_Gray64),
cDB(NULL, SDLWidget::mkrect(VM_PX+25,VM_PY+85,310,CB_SIZEY), "double buffering mode [r]", true, GUI_Gray64),
cGLBlit(NULL, SDLWidget::mkrect(VM_PX+25,VM_PY+110,310,CB_SIZEY), "use OpenGL for blitting [r]", true, GUI_Gray64),
cAlphaMenu(NULL, SDLWidget::mkrect(VM_PX+25,VM_PY+135,310,CB_SIZEY), "display menu with alpha (slow!)", true, GUI_Gray64),
lGamma_r(NULL, SDLWidget::mkrect(VM_PX+1,VM_PY+160, 119,20), "red gamma:", false),
eGamma_r(NULL, SDLWidget::mkrect(VM_PX+120,VM_PY+160,150,20), 0, 1),
lGamma_g(NULL, SDLWidget::mkrect(VM_PX+1,VM_PY+185, 119,20), "green gamma:", false),
eGamma_g(NULL, SDLWidget::mkrect(VM_PX+120,VM_PY+185,150,20), 0, 1),
lGamma_b(NULL, SDLWidget::mkrect(VM_PX+1,VM_PY+210, 119,20), "blue gamma:", false),
eGamma_b(NULL, SDLWidget::mkrect(VM_PX+120,VM_PY+210,150,20), 0, 1),
bSet(NULL, 1, SDLWidget::mkrect(VM_PX+25,VM_PY+240,150, 25), "APPLY"),
bCancel(NULL, 2, SDLWidget::mkrect(VM_PX+25+150+10,VM_PY+240,150,25), "BACK")
{
  Default();
  
  VideoMenu.SetColor(GUI_BtnTextColor, GUI_BtnATextColor);
  VideoMenu.SetFont(MainFont);
  
  VideoMenu.bgmode = 2;

  lGamma_r.SetAlignment(SDL_TA_RIGHT); 
  lGamma_r.shiftx = -4;
  lGamma_g.SetAlignment(SDL_TA_RIGHT); 
  lGamma_g.shiftx = -4;
  lGamma_b.SetAlignment(SDL_TA_RIGHT); 
  lGamma_b.shiftx = -4;
  
  AddChild(&VideoMenu);
  AddChild(&lEffect);
  AddChild(&cFullscreen);
  AddChild(&cDB);
  AddChild(&cGLBlit);
  AddChild(&cAlphaMenu);
  AddChild(&lGamma_r);
  AddChild(&eGamma_r);
  AddChild(&lGamma_g);
  AddChild(&eGamma_g);
  AddChild(&lGamma_b);
  AddChild(&eGamma_b);
  AddChild(&bSet);
  AddChild(&bCancel);

  AddChild(&Board1);
}

void GUI_VideoMenu::Default()
{
  if (fullscreen.value) cFullscreen.SetPressed(); else cFullscreen.SetUnpressed();
  if (glblit.value) cGLBlit.SetPressed(); else cGLBlit.SetUnpressed();
  if (alphamenu.value) cAlphaMenu.SetPressed(); else cAlphaMenu.SetUnpressed();
  if (doublebuf.value) cDB.SetPressed(); else cDB.SetUnpressed();

  char s[50];
  
  sprintf(s, "%.3g", gamma_r.value);
  eGamma_r.SetText(s);
  sprintf(s, "%.3g", gamma_g.value);
  eGamma_g.SetText(s);
  sprintf(s, "%.3g", gamma_b.value);
  eGamma_b.SetText(s);
}

bool GUI_VideoMenu::eventButtonClick(int id, SDLWidget* widget)
{
  switch (id) {
  case 1:
    ConOut("");
    ConOut("> Video menu sequence <");

    ConOut("gamma_r %s", eGamma_r.GetText());
    CommandExecute("gamma_r %s", eGamma_r.GetText());
    ConOut("gamma_g %s", eGamma_g.GetText());
    CommandExecute("gamma_g %s", eGamma_g.GetText());
    ConOut("gamma_b %s", eGamma_b.GetText());
    CommandExecute("gamma_b %s", eGamma_b.GetText());

    if (cFullscreen.GetPressed())
    {
      ConOut("fullscreen 1");
      CommandExecute("fullscreen 1");
    }
    else
    {
      ConOut("fullscreen 0");
      CommandExecute("fullscreen 0");
    }
    if (cGLBlit.GetPressed())
    {
      ConOut("glblit 1");
      CommandExecute("glblit 1");
    }
    else
    {
      ConOut("glblit 0");
      CommandExecute("glblit 0");
    }
    if (cAlphaMenu.GetPressed())
    {
      ConOut("alphamenu 1");
      CommandExecute("alphamenu 1");
    }
    else
    {
      ConOut("alphamenu 0");
      CommandExecute("alphamenu 0");
    }
    if (cDB.GetPressed())
    {
      ConOut("doublebuf 1");
      CommandExecute("doublebuf 1");
    }
    else
    {
      ConOut("doublebuf 0");
      CommandExecute("doublebuf 0");
    }


    ConOut("> end of sequence <");
    ConOut("");
    
    GUI_Return();
    break;
  case 2:
    Return();
    break;
  }
  return true;
}

void GUI_VideoMenu::Show()
{
  Clear();
  
  Board1.Show();  
  VideoMenu.Show();
  lEffect.Show();
  cFullscreen.Show();
  cDB.Show();
  cGLBlit.Show();
  cAlphaMenu.Show();
  lGamma_r.Show();
  eGamma_r.Show();
  lGamma_g.Show();
  eGamma_g.Show();
  lGamma_b.Show();
  eGamma_b.Show();
  
  bSet.Show();  
  bCancel.Show();  
}

void GUI_VideoMenu::Hide()
{
  Board1.Hide();  
  VideoMenu.Hide();
  lEffect.Hide();
  cFullscreen.Hide();
  cDB.Hide();
  cGLBlit.Hide();
  cAlphaMenu.Hide();
  lGamma_r.Hide();
  eGamma_r.Hide();
  lGamma_g.Hide();
  eGamma_g.Hide();
  lGamma_b.Hide();
  eGamma_b.Hide();
  
  bSet.Hide();  
  bCancel.Hide();  
}


/////////////////////////////////////////////////////////////////////////////
// Server menu widgets
/////////////////////////////////////////////////////////////////////////////

void GUI_ServerMenu::Return()
{
  eTimelimit.EditEnd();
  eServerName.EditEnd();
  eWelcomeMsg.EditEnd();
  eMaxClients.EditEnd();
  GUI_BaseMenu::Return();
}

GUI_ServerMenu::GUI_ServerMenu():
GUI_BaseMenu(GUI_SERVER, mkrect(SMS_PX,SMS_PY,SMS_VX,SMS_VY)),
lEffect(NULL, SDLWidget::mkrect(VM_PX+1,VM_PY+30,VM_VX-2,20), "This settings will affect the server side of the game.", false),
ServerMenu(NULL, SDLWidget::mkrect(SMS_PX+1,SMS_PY+4,SMS_VX-2,25), "SERVER OPTIONS", false),
lServerName(NULL, SDLWidget::mkrect(SMS_PX+1,SMS_PY+65, 119,20), "Server name:", false),
eServerName(NULL, SDLWidget::mkrect(SMS_PX+120,SMS_PY+65,215,20)),
lWelcomeMsg(NULL, SDLWidget::mkrect(SMS_PX+1,SMS_PY+90,119,20), "Welcome msg:", false),
eWelcomeMsg(NULL, SDLWidget::mkrect(SMS_PX+120,SMS_PY+90,215,20)),
lMaxClients(NULL, SDLWidget::mkrect(SMS_PX+1,SMS_PY+115,119,20), "Max clients:", false),
eMaxClients(NULL, SDLWidget::mkrect(SMS_PX+120,SMS_PY+115,80,20), 1, PWP_TOTALMAX_CLIENTS),
lTimelimit(NULL, SDLWidget::mkrect(SMS_PX+1,SMS_PY+140, 119,20), "timelimit:", false),
eTimelimit(NULL, SDLWidget::mkrect(SMS_PX+120,SMS_PY+140,150,20), 20, 10000),
cDelTmps(NULL, SDLWidget::mkrect(SMS_PX+25,SMS_PY+165,310,CB_SIZEY), "delete temporary files after transmission", false, GUI_Gray64),
cRDRqs(NULL, SDLWidget::mkrect(SMS_PX+25,SMS_PY+190,310,CB_SIZEY), "receive download requests", false, GUI_Gray64),
cRURqs(NULL, SDLWidget::mkrect(SMS_PX+25,SMS_PY+215,310,CB_SIZEY), "receive upload requests", false, GUI_Gray64),
bSet(NULL, 1, SDLWidget::mkrect(SMS_PX+25,SMS_PY+240,150, 25), "APPLY"),
bCancel(NULL, 2, SDLWidget::mkrect(SMS_PX+25+150+10,SMS_PY+240,150,25), "BACK"),
Board1(NULL, SDLWidget::mkrect(SMS_PX,SMS_PY,SMS_VX, SMS_VY), false)
{
  Default();
  
  ServerMenu.SetColor(GUI_BtnTextColor, GUI_BtnATextColor);
  ServerMenu.SetFont(MainFont);
  ServerMenu.bgmode = 2;
  
  lTimelimit.SetAlignment(SDL_TA_RIGHT); 
  lTimelimit.shiftx = -4;
  lWelcomeMsg.SetAlignment(SDL_TA_RIGHT); 
  lWelcomeMsg.shiftx = -4;
  lMaxClients.SetAlignment(SDL_TA_RIGHT); 
  lMaxClients.shiftx = -4;
  lServerName.SetAlignment(SDL_TA_RIGHT);
  lServerName.shiftx = -4;

  AddChild(&ServerMenu);
  AddChild(&lEffect);
  AddChild(&lServerName);
  AddChild(&eServerName);
  AddChild(&lWelcomeMsg);
  AddChild(&eWelcomeMsg);
  AddChild(&lMaxClients);
  AddChild(&eMaxClients);
  AddChild(&lTimelimit);
  AddChild(&eTimelimit);
  AddChild(&cDelTmps);
  AddChild(&cRDRqs);
  AddChild(&cRURqs);
  AddChild(&bSet);
  AddChild(&bCancel);
  AddChild(&Board1);
}

void GUI_ServerMenu::Default()
{
  char s[50];
  int i = (int)s_timelimit.value;
  sprintf(s, "%i", i);
  eTimelimit.SetText(s);

  eServerName.SetText(s_name.string);
  eMaxClients.SetText(s_maxclients.string);
  eWelcomeMsg.SetText(s_welcome_msg.string);

  if (s_deletetmps.value) cDelTmps.SetPressed(); else cDelTmps.SetUnpressed();
  if (s_downloading.value) cRDRqs.SetPressed(); else cRDRqs.SetUnpressed();
  if (s_uploading.value) cRURqs.SetPressed(); else cRURqs.SetUnpressed();

}

bool GUI_ServerMenu::eventButtonClick(int id, SDLWidget* widget)
{
  switch (id) {
  case 1:
    ConOut("");
    ConOut("> Server menu sequence <");

    ConOut("s_name %s", eServerName.GetText());
    CommandExecute("s_name %s", eServerName.GetText());
    ConOut("s_welcome_msg %s", eWelcomeMsg.GetText());
    CommandExecute("s_welcome_msg %s", eWelcomeMsg.GetText());
    ConOut("s_maxclients %s", eMaxClients.GetText());
    CommandExecute("s_maxclients %s", eMaxClients.GetText());
    ConOut("s_timelimit %s", eTimelimit.GetText());
    CommandExecute("s_timelimit %s", eTimelimit.GetText());

    if (cDelTmps.GetPressed())
    {
      ConOut("s_deltmps 1");
      CommandExecute("s_deltmps 1");
    }
    else
    {
      ConOut("s_deltmps 0");
      CommandExecute("s_deltmps 0");
    }
    if (cRDRqs.GetPressed())
    {
      ConOut("s_downloading 1");
      CommandExecute("s_downloading 1");
    }
    else
    {
      ConOut("s_downloading 0");
      CommandExecute("s_downloading 0");
    }
    if (cRURqs.GetPressed())
    {
      ConOut("s_uploading 1");
      CommandExecute("s_uploading 1");
    }
    else
    {
      ConOut("s_uploading 0");
      CommandExecute("s_uploading 0");
    }

    ConOut("> end of sequence <");
    ConOut("");
    
    GUI_Return();
    break;
  case 2:
    Return();
    break;
  }
  return true;
}

void GUI_ServerMenu::Show()
{
  Clear();
  
  Board1.Show();  
  ServerMenu.Show();

  lEffect.Show();
  eServerName.Show();  
  lServerName.Show();  
  eWelcomeMsg.Show();  
  lWelcomeMsg.Show();  
  eMaxClients.Show();  
  lMaxClients.Show();  
  eTimelimit.Show();  
  lTimelimit.Show();  

  cDelTmps.Show();  
  cRDRqs.Show();  
  cRURqs.Show();  
  
  bSet.Show();  
  bCancel.Show();  
}

void GUI_ServerMenu::Hide()
{
  Board1.Hide();  
  ServerMenu.Hide();
  
  lEffect.Hide();
  eServerName.Hide();  
  lServerName.Hide();  
  eWelcomeMsg.Hide();  
  lWelcomeMsg.Hide();  
  eMaxClients.Hide();  
  lMaxClients.Hide();  
  eTimelimit.Hide();  
  lTimelimit.Hide();  

  cDelTmps.Hide();  
  cRDRqs.Hide();  
  cRURqs.Hide();  

  bSet.Hide();  
  bCancel.Hide();  
}

/////////////////////////////////////////////////////////////////////////////
// Client menu widgets
/////////////////////////////////////////////////////////////////////////////

void GUI_ClientMenu::Return()
{
  eClientName.EditEnd();
  eClientDesc.EditEnd();
  GUI_BaseMenu::Return();
}

GUI_ClientMenu::GUI_ClientMenu():
GUI_BaseMenu(GUI_CLIENT, mkrect(CMS_PX,CMS_PY,CMS_VX,CMS_VY)),
Board1(NULL, SDLWidget::mkrect(CMS_PX,CMS_PY,CMS_VX, CMS_VY), false),
ClientMenu(NULL, SDLWidget::mkrect(CMS_PX+1,CMS_PY+4,CMS_VX-2,25), "CLIENT OPTIONS", false),
lEffect(NULL, SDLWidget::mkrect(CMS_PX+1,CMS_PY+30,CMS_VX-2,20), "This settings will affect the client side of the game.", false),
lClientName(NULL, SDLWidget::mkrect(CMS_PX+1,CMS_PY+65, 119,20), "Station name:", false),
eClientName(NULL, SDLWidget::mkrect(CMS_PX+120,CMS_PY+65,215,20)),
lClientDesc(NULL, SDLWidget::mkrect(CMS_PX+1,CMS_PY+90, 119,20), "Station finger:", false),
eClientDesc(NULL, SDLWidget::mkrect(CMS_PX+120,CMS_PY+90,215,20)),
cDelTmps(NULL, SDLWidget::mkrect(CMS_PX+25,CMS_PY+115,310,CB_SIZEY), "delete temporary files after transmission", false, GUI_Gray64),
cDRqs(NULL, SDLWidget::mkrect(CMS_PX+25,CMS_PY+140,310,CB_SIZEY), "allow download requests", false, GUI_Gray64),
cURqs(NULL, SDLWidget::mkrect(CMS_PX+25,CMS_PY+165,310,CB_SIZEY), "allow upload requests", false, GUI_Gray64),
bSet(NULL, 1, SDLWidget::mkrect(CMS_PX+25,CMS_PY+190,150, 25), "APPLY"),
bCancel(NULL, 2, SDLWidget::mkrect(CMS_PX+25+150+10,CMS_PY+190,150,25), "BACK")
{
  Default();
  
  ClientMenu.SetColor(GUI_BtnTextColor, GUI_BtnATextColor);
  ClientMenu.SetFont(MainFont);
  ClientMenu.bgmode = 2;
  
  lClientName.SetAlignment(SDL_TA_RIGHT); 
  lClientName.shiftx = -4;
  lClientDesc.SetAlignment(SDL_TA_RIGHT); 
  lClientDesc.shiftx = -4;

  AddChild(&ClientMenu);
  AddChild(&lEffect);
  AddChild(&lClientName);
  AddChild(&eClientName);
  AddChild(&lClientDesc);
  AddChild(&eClientDesc);
  AddChild(&cDelTmps);
  AddChild(&cDRqs);
  AddChild(&cURqs);
  AddChild(&bSet);
  AddChild(&bCancel);

  AddChild(&Board1);
}

void GUI_ClientMenu::Default()
{
  eClientName.SetText(c_name.string);
  eClientDesc.SetText(c_desc.string);

  if (c_deletetmps.value) cDelTmps.SetPressed(); else cDelTmps.SetUnpressed();
  if (c_downloading.value) cDRqs.SetPressed(); else cDRqs.SetUnpressed();
  if (c_uploading.value) cURqs.SetPressed(); else cURqs.SetUnpressed();

}

bool GUI_ClientMenu::eventButtonClick(int id, SDLWidget* widget)
{
  switch (id) {
  case 1:
    ConOut("");
    ConOut("> Client menu sequence <");

    ConOut("c_name %s", eClientName.GetText());
    CommandExecute("c_name %s", eClientName.GetText());
    ConOut("c_desc %s", eClientDesc.GetText());
    CommandExecute("c_desc %s", eClientDesc.GetText());

    if (cDelTmps.GetPressed())
    {
      ConOut("c_deltmps 1");
      CommandExecute("c_deltmps 1");
    }
    else
    {
      ConOut("c_deltmps 0");
      CommandExecute("c_deltmps 0");
    }
    if (cDRqs.GetPressed())
    {
      ConOut("c_downloading 1");
      CommandExecute("c_downloading 1");
    }
    else
    {
      ConOut("c_downloading 0");
      CommandExecute("c_downloading 0");
    }
    if (cURqs.GetPressed())
    {
      ConOut("c_uploading 1");
      CommandExecute("c_uploading 1");
    }
    else
    {
      ConOut("c_uploading 0");
      CommandExecute("c_uploading 0");
    }

    ConOut("> end of sequence <");
    ConOut("");
    
    GUI_Return();
    break;
  case 2:
    Return();
    break;
  }
  return true;
}

void GUI_ClientMenu::Show()
{
  Clear();
  
  Board1.Show();  
  ClientMenu.Show();
  lEffect.Show();

  eClientName.Show();  
  lClientName.Show();  
  eClientDesc.Show();  
  lClientDesc.Show();  

  cDelTmps.Show();  
  cDRqs.Show();  
  cURqs.Show();  
  
  bSet.Show();  
  bCancel.Show();  
}

void GUI_ClientMenu::Hide()
{
  Board1.Hide();  
  ClientMenu.Hide();

  lEffect.Hide();
  
  eClientName.Hide();  
  lClientName.Hide();  
  eClientDesc.Hide();  
  lClientDesc.Hide();  

  cDelTmps.Hide();  
  cDRqs.Hide();  
  cURqs.Hide();  

  bSet.Hide();  
  bCancel.Hide();  
}





/////////////////////////////////////////////////////////////////////////////
// GUI wrappers
/////////////////////////////////////////////////////////////////////////////

GUI_BaseMenu *GUI_System = NULL;

GUI_BaseMenu* GUI_Find(int id)
{
  GUI_BaseMenu* p = GUI_System;  
  while (p)
  {
    if (p->mid == id) return p;
    p = p->next;
  }
  return NULL;
}

void GUI_Return()
{
    GUI_BaseMenu* o = GUI_Find(GUI_id);
    if (o) o->Return();
}

void GUI_OpenMenu(int id)
{
  if (GUI_menu) GUI_menu->Hide();
  
  GUI_BaseMenu* xnew = GUI_Find(id);
  if (xnew) 
  {
    xnew->Hide();
    xnew->Default();
    xnew->Show();
    blocked_inputs = 1;
    ShowMouse();
    xnew->parentmid = GUI_id;
    smPlaySample(SM_MENU1, menu_volume);

    // message refresh hack
    int x, y;
    SDL_GetMouseState(&x, &y);
    SDL_WarpMouse(x,y);
  }
  GUI_id = id;
  GUI_menu = xnew;
}

void GUI_Refresh()
{
  if (GUI_menu)
  {
    GUI_menu->Hide();
    GUI_menu->Default();
    GUI_menu->Show();
  }
}

void GUI_Show(int id)
{
  GUI_BaseMenu* o = GUI_Find(id);
  if (o) 
  {  
    o->Hide();
    o->Default();
    o->Show();
    blocked_inputs = 1;
    ShowMouse();
  }
  GUI_menu = o;
  GUI_id = id;
}

bool GUI_Init(SDL_Surface* s)
{
  GUI_InitColors(s);
  MainFont = TTF_OpenFont(SDLApplication::GetRelativePath("fonts/futuraxk.ttf"), 24);
  if (!MainFont)
  {
    fprintf(stderr, "GUI: Couldn't load font %s", SDLApplication::GetRelativePath("fonts/futuraxk.ttf"));
    return false;
  }
  MainFont2 = TTF_OpenFont(SDLApplication::GetRelativePath("fonts/futuraxk.ttf"), 28);
  if (!MainFont2)
  {
    fprintf(stderr, "GUI: Couldn't load font %s", SDLApplication::GetRelativePath("fonts/futuraxk.ttf"));
    return false;
  }
  TextFont = TTF_OpenFont(SDLApplication::GetRelativePath("fonts/font2.ttf"), 10);
  if (!TextFont)
  {
    fprintf(stderr, "GUI: Couldn't load font %s", SDLApplication::GetRelativePath("fonts/font2.ttf"));
    return false;
  }
  BtnFont = TTF_OpenFont(SDLApplication::GetRelativePath("fonts/futurab.ttf"), 12);
  if (!BtnFont)
  {
    fprintf(stderr, "GUI: Couldn't load font %s", SDLApplication::GetRelativePath("fonts/futurab.ttf"));
    return false;
  }
  BtnFont2 = TTF_OpenFont(SDLApplication::GetRelativePath("fonts/futurab.ttf"), 14);
  if (!BtnFont2)
  {
    fprintf(stderr, "GUI: Couldn't load font %s", SDLApplication::GetRelativePath("fonts/futurab.ttf"));
    return false;
  }
  
  return true;
}

bool GUI_Done()
{
  if (MainFont) TTF_CloseFont(MainFont);
  if (MainFont2) TTF_CloseFont(MainFont2);
  if (TextFont) TTF_CloseFont(TextFont);
  if (BtnFont) TTF_CloseFont(BtnFont);
  if (BtnFont2) TTF_CloseFont(BtnFont2);
  return true;
}

void RenderResults(SDL_Surface *screen)
{

}

