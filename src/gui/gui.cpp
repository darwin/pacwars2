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

GUI_BaseMenu* GUI_menu = NULL;
int GUI_id = 0;

extern GUI_OKDialog1 *OKD1;
extern GUI_OKDialog2 *OKD2;

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

GUI_Board::GUI_Board(SDLWidget* parent, SDL_Rect& r,  bool storebackground, char* theme) : SDLGradientWidget(parent,r) {
	if (!theme) {
		LoadThemeStyle("GUI_Board","GradientWidget");
	}
	else {
		LoadThemeStyle(theme,"GradientWidget");
	}
}


/////////////////////////////////////////////////////////////////////////////
// Bitmap
/////////////////////////////////////////////////////////////////////////////

GUI_Bitmap::GUI_Bitmap(SDLWidget* parent, SDL_Rect& r,  bool storebackground, SDL_Surface* s) : SDLWidget(parent,r,storebackground) {
	bitmap = s;
}

void GUI_Bitmap::eventDraw(SDL_Surface* surface, SDL_Rect* rect) {
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


GUI_Label::GUI_Label(SDLWidget* parent, SDL_Rect& r, char* text, bool storebackground, char* style) : SDLLabel(parent,r,text) {
	SetFont(TextFont);
	SetAlignment(SDL_TA_CENTER);
	SDLWidget::LoadThemeStyle(style, "GUI_Label");
}

GUI_LabelC::GUI_LabelC(SDLWidget* parent, SDL_Rect& r, char* text, bool storebackground) : SDLLabel(parent,r,text) {
	SetFont(TextFont);
	SetAlignment(SDL_TA_LEFT);
}


/////////////////////////////////////////////////////////////////////////////
// Label
/////////////////////////////////////////////////////////////////////////////

GUI_LabelL::GUI_LabelL(SDLWidget* parent, SDL_Rect& r, char* text, GUI_LabelL** iselected, CSMapInfo* si, SDL_Color ic1, SDL_Color ic2, void (*cb)(GUI_LabelL*)) : SDLLabel(parent,r,text) {
	SetFont(TextFont);

	c1 = ic1;
	c2 = ic2;
	selected = iselected;
	msi = si;
	callback = cb;

	SetAlignment(SDL_TA_LEFT);
}

bool GUI_LabelL::eventMouseButtonDown(const SDL_MouseButtonEvent* button) {
	GUI_LabelL* last = *selected;

	if (last!=this) {
		*selected = this;
		last->Redraw();
		Redraw();
		if (callback) (*callback)(last);
	}

	return true;
}

void GUI_LabelL::eventDraw(SDL_Surface* surface, SDL_Rect* rect) {

	if (*selected==this) {
		SetTextColor(c2);
	}
	else {
		SetTextColor(c1);
	}

	SDLLabel::eventDraw(surface, rect);
}

/////////////////////////////////////////////////////////////////////////////
// Buttons
/////////////////////////////////////////////////////////////////////////////

GUI_Button::GUI_Button(SDLWidget* parent, int btnid, SDL_Rect& r, char* text) : SDLButton(parent,btnid,r,text) {
	SetBorderSize(0,0,0);
	SetTextColor(GUI_BtnTextColor);
	c = GUI_Red100;
	drawbackground = false;
}


GUI_ButtonBig::GUI_ButtonBig(SDLWidget* parent, int btnid, SDL_Rect& r, char* text) : GUI_Button(parent,btnid,r,text) {
	SDLTheme* theme = SDLApplication::GetTheme();

	SetFont(MainFont);
	drawbackground = true;
	enabled = true;

	LoadThemeStyle("GUI_MenuButton", "Button");
	SDL_Color* color = theme->FindColor("GUI_MenuButton", "Button", "textcolor2");

	if(color != NULL) {
		GUI_BtnATextColor = *color;
	}

	GUI_BtnTextColor = GetTextColor();
}

void GUI_ButtonBig::eventMouseLeave() {
	SetFont(MainFont);
	if (enabled) {
		SetTextColor(GUI_BtnTextColor);
		Update();
	}
}

void GUI_ButtonBig::eventMouseEnter() {
    SetFont(MainFont2);
    if (enabled)
    {
      SetTextColor(GUI_BtnATextColor);
      Update();
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


GUI_ButtonSmall::GUI_ButtonSmall(SDLWidget* parent, int btnid, SDL_Rect& r, char* text) : GUI_Button(parent,btnid,r,text) {
	SDLTheme* theme = SDLApplication::GetTheme();

	SetFont(BtnFont);
	drawbackground = true;
	c = GUI_SmBtColor;

	LoadThemeStyle("GUI_NormalButton", "Button");

	GUI_BtnTextColor = GetTextColor();
	SDL_Color* color = theme->FindColor("GUI_MenuButton", "Button", "textcolor2");

	if(color != NULL) {
		GUI_BtnATextColor = *color;
	}

	GUI_BtnTextColor = GetTextColor();
}

void GUI_ButtonSmall::eventMouseLeave() {
	/*SetFont(BtnFont);*/
	SetTextColor(GUI_BtnTextColor);
	Update();
}

void GUI_ButtonSmall::eventMouseEnter() {
	/*SetFont(BtnFont2);*/
	SetTextColor(GUI_BtnATextColor);
	Update();
}


/////////////////////////////////////////////////////////////////////////////
// Checkbox
/////////////////////////////////////////////////////////////////////////////

GUI_CheckBox::GUI_CheckBox(SDLWidget* parent, SDL_Rect& r, char* text, bool ipressed, SDL_Color bg) : SDLCheckButton(parent,99, r, text) {
	if (ipressed) {
		SetPressed();
	}

	my_widgetLabel->SetFont(TextFont);

	LoadThemeStyle("GUI_CheckButton");
	my_widgetButton->LoadThemeStyle("GUI_CheckButton", "CheckWidget");
	SetTransparency(0);
}

GUI_CheckBox::~GUI_CheckBox() {
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

GUI_TextEdit::GUI_TextEdit(SDLWidget* parent, SDL_Rect& r) : SDLLineEdit(parent, r) {
	SetFont(TextFont);
	LoadThemeStyle("GUI_TextEdit");
}

/////////////////////////////////////////////////////////////////////////////
// NumEdit
/////////////////////////////////////////////////////////////////////////////

GUI_NumEdit::GUI_NumEdit(SDLWidget* parent, SDL_Rect& r, int imin, int imax) : GUI_TextEdit(parent, r) {
	SetFont(TextFont);
	min = imin;
	max = imax;
	SetValidKeys("0123456789");
}

/////////////////////////////////////////////////////////////////////////////
// FloatEdit
/////////////////////////////////////////////////////////////////////////////

GUI_FloatEdit::GUI_FloatEdit(SDLWidget* parent, SDL_Rect& r, float imin, float imax) : GUI_TextEdit(parent, r) {
	SetFont(TextFont);
	min = imin;
	max = imax;
	SetValidKeys("0123456789.-");
}

/////////////////////////////////////////////////////////////////////////////
// Scrollbar
/////////////////////////////////////////////////////////////////////////////

GUI_ScrollBar::GUI_ScrollBar(SDLWidget* parent, int id, SDL_Rect& r, int direction) : SDLScrollBar(parent, id, r, direction) {
}

GUI_WidgetList::GUI_WidgetList(SDLWidget* parent, SDL_Rect& r) : SDLWidgetList(parent, r, "GUI_WidgetList") {
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

	Default();
  
	OKDialog1->SetFont(MainFont);
  
	lPrompt1->SetAlignment(SDL_TA_CENTER);
	LoadThemeStyle("GUI_Board");
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
  OKDialog2->SetText(title);
  lPrompt1->SetText(line1);
  lPrompt2->SetText(line2);
}

GUI_OKDialog2::GUI_OKDialog2(char* title, char* line1, char* line2):
GUI_BaseMenu(GUI_OKDIALOG2, mkrect(OKD2_PX,OKD2_PY,OKD2_VX,OKD2_VY)) {

  OKDialog2 = new GUI_Label(this, SDLWidget::mkrect(1,4,OKD2_VX-2,25), title, false);
  lPrompt1 = new GUI_Label(this, SDLWidget::mkrect(1,35, OKD2_VX-2,20), line1, false);
  lPrompt2 = new GUI_Label(this, SDLWidget::mkrect(1,55, OKD2_VX-2,20), line2, false);
  bOK = new GUI_ButtonSmall(this, 1, SDLWidget::mkrect(105,90,150,25), "OK");

  Default();
  
  OKDialog2->SetFont(MainFont);
  
  lPrompt1->SetAlignment(SDL_TA_CENTER); 
  lPrompt2->SetAlignment(SDL_TA_CENTER);

  LoadThemeStyle("GUI_Board");
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
  
	//Redraw();
	Update();
	return true;
}

bool GUI_ButtonS::eventMouseButtonUp(const SDL_MouseButtonEvent* button){
  ReleaseCapture();
  Update();
  
  if(!IsMouseInside())
    return false;
  SDLMessageObject::SendMessage(GetParent(), MSG_BUTTONCLICK, id, 0);
  
  return true;
}

bool GUI_ButtonS::SetBackground(const char* filename, int mode){
  background = IMG_Load(SDLApplication::GetRelativePath((char*)filename));
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

GUI_SkinCtrl::GUI_SkinCtrl(SDLWidget* parent, SDL_Rect& r,  bool storebackground) : SDLWidget(parent,r,storebackground) {
	SkinPic = new GUI_SkinPic(this, SDLWidget::mkrect(1, 1, 32, 32));
	bNext = new SDLButton(this, 2, SDLWidget::mkrect(17, 33, 16, 16));
	bPrev = new SDLButton(this, 1, SDLWidget::mkrect(1, 33, 16, 16));

	c = GUI_Gray64;
	drawbackground = true;

	bPrev->LoadThemeStyle("GUI_PrevSkinButton");
	bNext->LoadThemeStyle("GUI_NextSkinButton");
}

void GUI_SkinCtrl::eventDraw(SDL_Surface* surface, SDL_Rect* rect)
{
	if (drawbackground) {
		SDL_FillRect(surface, rect, SDL_MapRGB(surface->format, c.r, c.g, c.b));
		SDLWidget::DrawBorder(surface, rect, 1, false);
	}
	else {
		SDL_FillRect(surface, rect, 0);
	}
  
	SDLWidget::eventDraw(surface, rect);
}

bool GUI_SkinCtrl::eventButtonClick(int id, SDLWidget* widget)
{
	switch (id) {

	case 1:
		if (SkinPic->PrevSkin()) {
			bPrev->Hide();
		}
		else {
			bPrev->Show();
		}
		bNext->Show();
		break;

	case 2:
		if (SkinPic->NextSkin()) {
			bNext->Hide();
		}
		else {
			bNext->Show();
		}
		bPrev->Show();
		break;
	}

	return true;
}


GUI_Input::GUI_Input(SDLWidget* parent, SDL_Rect& r, int iid) : SDLGradientWidget(parent,r) {
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


void GUI_Input::eventDraw(SDL_Surface* surface, SDL_Rect* rect) {
	if (waiting) {
		textcolor = GUI_InputActive;
	}
	else {
		textcolor = GUI_InputInactive;
	}

	SDLGradientWidget::eventDraw(surface, rect);

	DrawText(surface, rect);
	DrawBorder(surface, rect, 1, false);
}

void GUI_Input::DrawText(SDL_Surface* surface, SDL_Rect* rect) {
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
//bPrev(NULL, 2, SDLWidget::mkrect(HM1_PX+5,HM1_PY+330,70,20), "PREV"),
bReturn(NULL, 1, SDLWidget::mkrect(HM1_PX+(HM1_VX-100)/2,HM1_PY+330,100,20), "RETURN"),
bNext(NULL, 3, SDLWidget::mkrect(HM1_PX+HM1_VX-5-70,HM1_PY+330,70,20), "NEXT")
{
  Default();
  
  Help1Menu.SetAlignment(SDL_TA_CENTER); 
  Help1Menu.SetFont(MainFont);
  
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
  //AddChild(&bPrev);

  AddChild(&mBack1);
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
  //bPrev.Hide();
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
lK7a(NULL, SDLWidget::mkrect(HM2_K7_PX,HM2_K7_PY, HM2_K7_VX-2, 20), " ", false),
lK7b(NULL, SDLWidget::mkrect(HM2_K7_PX,HM2_K7_PY+HM2_LY3, HM2_K7_VX-2, 20), " ", false),
bPrev(NULL, 2, SDLWidget::mkrect(HM2_PX+5,HM2_PY+365,70,20), "PREV"),
bReturn(NULL, 1, SDLWidget::mkrect(HM2_PX+(HM2_VX-100)/2,HM2_PY+365,100,20), "RETURN")
//bNext(NULL, 3, SDLWidget::mkrect(HM2_PX+HM2_VX-5-70,HM2_PY+365,70,20), "NEXT")
{
  Default();
  
  Help2Menu.SetAlignment(SDL_TA_CENTER); 
  Help2Menu.SetFont(MainFont);
  
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
 // AddChild(&bNext);
  AddChild(&bPrev);

  AddChild(&mBack1);
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
  //bNext.Hide();
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

