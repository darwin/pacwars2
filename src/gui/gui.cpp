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

GUI_Board::GUI_Board(PG_Widget* parent, const PG_Rect& r, char* theme) : PG_GradientWidget(parent,r) {
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

GUI_Bitmap::GUI_Bitmap(PG_Widget* parent, const PG_Rect& r, SDL_Surface* s) : PG_Widget(parent,r, true) {
	bitmap = s;
}

void GUI_Bitmap::eventDraw(SDL_Surface* surface, const PG_Rect& rect) {
	SDL_Rect sr;

	sr.x = 0;
	sr.y = 0;
	sr.w = MIN(rect.w, bitmap->w);
	sr.h = MIN(rect.h, bitmap->h);

	SDL_BlitSurface(bitmap, &sr, surface, (PG_Rect*)&rect);
}



/////////////////////////////////////////////////////////////////////////////
// Label
/////////////////////////////////////////////////////////////////////////////

GUI_LabelL::GUI_LabelL(PG_Widget* parent, const PG_Rect& r, char* text, GUI_LabelL** iselected, CSMapInfo* si, SDL_Color ic1, SDL_Color ic2, void (*cb)(GUI_LabelL*)) : PG_Label(parent,r,text) {
	c1 = ic1;
	c2 = ic2;
	selected = iselected;
	msi = si;
	callback = cb;

	SetAlignment(PG_TA_LEFT);
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

void GUI_LabelL::eventDraw(SDL_Surface* surface, const PG_Rect& rect) {
	if (*selected==this) {
		SetTextColor(c2);
	}
	else {
		SetTextColor(c1);
	}
	PG_Label::eventDraw(surface, rect);
}

/////////////////////////////////////////////////////////////////////////////
// Buttons
/////////////////////////////////////////////////////////////////////////////

GUI_Button::GUI_Button(PG_Widget* parent, int btnid, const PG_Rect& r, char* text) : PG_Button(parent,btnid,r,text) {
	SetBorderSize(0,0,0);
	SetTextColor(GUI_BtnTextColor);
	c = GUI_Red100;
	drawbackground = false;
}


GUI_ButtonBig::GUI_ButtonBig(PG_Widget* parent, int btnid, const PG_Rect& r, char* text) : GUI_Button(parent,btnid,r,text) {
	PG_Theme* theme = PG_Application::GetTheme();

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
	GUI_Button::eventMouseLeave();
}

void GUI_ButtonBig::eventMouseEnter() {
    SetFont(MainFont2);
    if (enabled)
    {
      SetTextColor(GUI_BtnATextColor);
      Update();
    }
	GUI_Button::eventMouseEnter();
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


GUI_ButtonSmall::GUI_ButtonSmall(PG_Widget* parent, int btnid, const PG_Rect& r, char* text) : GUI_Button(parent,btnid,r,text) {
	PG_Theme* theme = PG_Application::GetTheme();

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
	GUI_Button::eventMouseLeave();
}

void GUI_ButtonSmall::eventMouseEnter() {
	/*SetFont(BtnFont2);*/
	SetTextColor(GUI_BtnATextColor);
	Update();
	GUI_Button::eventMouseEnter();
}


/////////////////////////////////////////////////////////////////////////////
// Checkbox
/////////////////////////////////////////////////////////////////////////////

GUI_CheckBox::GUI_CheckBox(PG_Widget* parent, const PG_Rect& r, char* text, bool ipressed, SDL_Color bg) : PG_CheckButton(parent,99, r, text) {
	if (ipressed) {
		SetPressed();
	}

	LoadThemeStyle("GUI_CheckButton");
	my_widgetButton->LoadThemeStyle("GUI_CheckButton", "CheckWidget");
	//SetTransparency(0);
}

GUI_CheckBox::~GUI_CheckBox() {
}

void GUI_CheckBox::eventMouseEnter() {
	//PG_CheckButton::eventMouseEnter();
}

void GUI_CheckBox::eventMouseLeave() {
	//PG_CheckButton::eventMouseLeave();
}


/////////////////////////////////////////////////////////////////////////////
// TextEdit
/////////////////////////////////////////////////////////////////////////////

GUI_TextEdit::GUI_TextEdit(PG_Widget* parent, const PG_Rect& r) : PG_LineEdit(parent, r) {
	LoadThemeStyle("GUI_TextEdit");
}

/////////////////////////////////////////////////////////////////////////////
// NumEdit
/////////////////////////////////////////////////////////////////////////////

GUI_NumEdit::GUI_NumEdit(PG_Widget* parent, const PG_Rect& r, int imin, int imax) : GUI_TextEdit(parent, r) {
	min = imin;
	max = imax;
	SetValidKeys("0123456789");
}

/////////////////////////////////////////////////////////////////////////////
// FloatEdit
/////////////////////////////////////////////////////////////////////////////

GUI_FloatEdit::GUI_FloatEdit(PG_Widget* parent, const PG_Rect& r, float imin, float imax) : GUI_TextEdit(parent, r) {
	min = imin;
	max = imax;
	SetValidKeys("0123456789.-");
}

/////////////////////////////////////////////////////////////////////////////
// OK dialog widgets
/////////////////////////////////////////////////////////////////////////////

void GUI_OKDialog1::Reset(char* title, char* line1)
{
	OKDialog1->SetText(title);
	lPrompt1->SetText(line1);
}

GUI_OKDialog1::GUI_OKDialog1(char* title, char* line1, char* line2) : GUI_BaseMenu(GUI_OKDIALOG1, PG_Rect(OKD1_PX,OKD1_PY,OKD1_VX,OKD1_VY)){

	OKDialog1 = new PG_Label(this, PG_Rect(1,4,OKD1_VX-2,25), title, "GUI_Label");
	lPrompt1 = new PG_Label(this, PG_Rect(1,35, OKD1_VX-2,20), line1, "GUI_Label");
	bOK = new GUI_ButtonSmall(this, 1, PG_Rect(105,65,150,25), "OK");

	Default();
  
	OKDialog1->SetFont(MainFont);
  
	lPrompt1->SetAlignment(PG_TA_CENTER);
	LoadThemeStyle("GUI_Board");
}

void GUI_OKDialog1::Default()
{
}

bool GUI_OKDialog1::eventButtonClick(int id, PG_Widget* widget)
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

GUI_OKDialog2::GUI_OKDialog2(char* title, char* line1, char* line2) : GUI_BaseMenu(GUI_OKDIALOG2, PG_Rect(OKD2_PX,OKD2_PY,OKD2_VX,OKD2_VY)) {

  OKDialog2 = new PG_Label(this, PG_Rect(1,4,OKD2_VX-2,25), title, "GUI_Label");
  lPrompt1 = new PG_Label(this, PG_Rect(1,35, OKD2_VX-2,20), line1, "GUI_Label");
  lPrompt2 = new PG_Label(this, PG_Rect(1,55, OKD2_VX-2,20), line2, "GUI_Label");
  bOK = new GUI_ButtonSmall(this, 1, PG_Rect(105,90,150,25), "OK");

  Default();
  
  OKDialog2->SetFont(MainFont);
  
  lPrompt1->SetAlignment(PG_TA_CENTER);
  lPrompt2->SetAlignment(PG_TA_CENTER);

  LoadThemeStyle("GUI_Board");
}

void GUI_OKDialog2::Default()
{
}

bool GUI_OKDialog2::eventButtonClick(int id, PG_Widget* widget)
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

GUI_ButtonS::GUI_ButtonS(PG_Widget* parent, int btnid, const PG_Rect& r):
PG_Widget(parent, r, false)
{
  SDL_Surface* background = NULL;
  id = btnid;
}

void GUI_ButtonS::eventDraw(SDL_Surface* surface, const PG_Rect& rect)
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
  srcrect.x = rect.x;
  srcrect.y = rect.y;
  srcrect.w = background->w;
  srcrect.h = background->h;

  SDL_BlitSurface(background, &srcrect, surface, &dstrect);
}

bool GUI_ButtonS::eventMouseButtonDown(const SDL_MouseButtonEvent* button){
	SetCapture();

	Update();
	return true;
}

bool GUI_ButtonS::eventMouseButtonUp(const SDL_MouseButtonEvent* button){
  ReleaseCapture();
  Update();

  if(!IsMouseInside())
    return false;
  PG_MessageObject::SendMessage(GetParent(), MSG_BUTTONCLICK, id, 0);

  return true;
}

bool GUI_ButtonS::SetBackground(const char* filename, int mode){
  background = IMG_Load(PG_Application::GetRelativePath((char*)filename));
  return (background != NULL);
}

GUI_SkinPic::GUI_SkinPic(PG_Widget* parent, const PG_Rect& r):
PG_Widget(parent, r, true)
{
  lName = NULL;
  sprite = NULL;
}

GUI_SkinPic::~GUI_SkinPic()
{
  if (sprite) SkinMan.Unregister(sprite);
}

void GUI_SkinPic::eventDraw(SDL_Surface* surface, const PG_Rect& rect)
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
    dr.x = rect.x;
    dr.y = rect.y;
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

GUI_SkinCtrl::GUI_SkinCtrl(PG_Widget* parent, const PG_Rect& r) : PG_Widget(parent, r, true) {
	bNext = new PG_Button(this, 2, PG_Rect(17, 33, 16, 16));
	bPrev = new PG_Button(this, 1, PG_Rect(1, 33, 16, 16));
	SkinPic = new GUI_SkinPic(this, PG_Rect(1, 1, 32, 32));

	c = GUI_Gray64;
	drawbackground = true;

	bPrev->LoadThemeStyle("GUI_PrevSkinButton");
	bNext->LoadThemeStyle("GUI_NextSkinButton");
}
/*
void GUI_SkinCtrl::eventDraw(SDL_Surface* surface, const PG_Rect& rect)
{
	if (drawbackground) {
		SDL_FillRect(surface, (PG_Rect*)&rect, SDL_MapRGB(surface->format, c.r, c.g, c.b));
		PG_Widget::DrawBorder(rect, 1, false);
	}
	else {
		SDL_FillRect(surface, (PG_Rect*)&rect, 0);
	}

	PG_Widget::eventDraw(surface, rect);
}
*/
bool GUI_SkinCtrl::eventButtonClick(int id, PG_Widget* widget)
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


GUI_Input::GUI_Input(PG_Widget* parent, const PG_Rect& r, int iid) : PG_StaticFrame(parent,r) {
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


void GUI_Input::eventBlit(SDL_Surface* surface, const PG_Rect& src, const PG_Rect& dst) {
	if (waiting) {
		textcolor = GUI_InputActive;
	}
	else {
		textcolor = GUI_InputInactive;
	}

	PG_StaticFrame::eventBlit(surface, src, dst);

	DrawText();
	DrawBorder(PG_Rect(0,0,my_width,my_height), 1, false);
}

char* my_strupr(char* text) {
	while(*text != 0) {
		*text=toupper(*text);
		text++;
	}
	return text;
}

void GUI_Input::DrawText() {
	int h;
	int x,y;

	x = 3;
	h = TTF_FontHeight(GetFont());
	y = (my_height - h)/2;

	// draw text
	char DrawText[10];
	strcpy(DrawText, GetDrawText());
	my_strupr(DrawText);

	if(DrawText[0] != 0){
		PG_DrawObject::DrawText(x+offset_x,y+2, DrawText, textcolor, GetFont());
	}		
}

char* GUI_Input::GetDrawText(){
	return text;	
}

void GUI_Input::InputBegin(){
	SetInputFocus();
	waiting = true;

	Update();
}

void GUI_Input::InputEnd(){
	waiting = false;
	Update();
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

void GUI_Input::eventInputFocusLost(PG_MessageObject* newfocus){
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
	Update();
}


/////////////////////////////////////////////////////////////////////////////
// Help1 menu widgets
/////////////////////////////////////////////////////////////////////////////

GUI_Help1Menu::GUI_Help1Menu():
GUI_BaseMenu(GUI_HELP1, PG_Rect(HM1_PX,HM1_PY,HM1_VX,HM1_VY)),
Board1(NULL, PG_Rect(HM1_PX,HM1_PY,HM1_VX,HM1_VY)),
mBack1(NULL, PG_Rect(HM1_PX+1, HM1_PY+1, help1->w, help1->h), help1),
Help1Menu(NULL, PG_Rect(HM1_PX+1,HM1_PY+4,HM1_VX-2,25), "Weapons & Bonuses", "GUI_Label"),
lLine1a(NULL, PG_Rect(HM1_G1_PX,HM1_G1_PY, HM1_G1_VX-2, 20), "SMALL PISTOL", "GUI_LabelL"),
lLine1b(NULL, PG_Rect(HM1_G1_PX,HM1_G1_PY+HM1_LY1, HM1_G1_VX-2, 20), "fires small slow projectiles", "GUI_LabelL"),
lLine1c(NULL, PG_Rect(HM1_G1_PX,HM1_G1_PY+2*HM1_LY1, HM1_G1_VX-2, 20), "bounces with shield and some kind of walls", "GUI_LabelL"),
lLine2a(NULL, PG_Rect(HM1_G2_PX,HM1_G2_PY, HM1_G2_VX-2, 20), "BECHEROMET", "GUI_LabelL"),
lLine2b(NULL, PG_Rect(HM1_G2_PX,HM1_G2_PY+HM1_LY1, HM1_G2_VX-2, 20), "fires high piercing shots in clusters", "GUI_LabelL"),
lLine2c(NULL, PG_Rect(HM1_G2_PX,HM1_G2_PY+2*HM1_LY1, HM1_G2_VX-2, 20), "loaded by becherovka bottles", "GUI_LabelL"),
lLine3a(NULL, PG_Rect(HM1_G3_PX,HM1_G3_PY, HM1_G3_VX-2, 20), "RAILGUN", "GUI_LabelL"),
lLine3b(NULL, PG_Rect(HM1_G3_PX,HM1_G3_PY+HM1_LY1, HM1_G3_VX-2, 20), "after small delay for charge fires long fireline", "GUI_LabelL"),
lLine3c(NULL, PG_Rect(HM1_G3_PX,HM1_G3_PY+2*HM1_LY1, HM1_G3_VX-2, 20), "works on basis of pressed alcohol gas", "GUI_LabelL"),
lLine4a(NULL, PG_Rect(HM1_G4_PX,HM1_G4_PY, HM1_G4_VX-2, 20), "RAPID BOMB", "GUI_LabelL"),
lLine4b(NULL, PG_Rect(HM1_G4_PX,HM1_G4_PY+HM1_LY1, HM1_G4_VX-2, 20), "droped bomb blows up after a period of time", "GUI_LabelL"),
lLine4c(NULL, PG_Rect(HM1_G4_PX,HM1_G4_PY+2*HM1_LY1, HM1_G4_VX-2, 20), "chemical modification of rapid drink", "GUI_LabelL"),
lLine5a(NULL, PG_Rect(HM1_G5_PX,HM1_G5_PY, HM1_G5_VX-2, 20), "BETON GRENADE", "GUI_LabelL"),
lLine5b(NULL, PG_Rect(HM1_G5_PX,HM1_G5_PY+HM1_LY1, HM1_G5_VX-2, 20), "after throwed detonates when stroked", "GUI_LabelL"),
lLine5c(NULL, PG_Rect(HM1_G5_PX,HM1_G5_PY+2*HM1_LY1, HM1_G5_VX-2, 20), "deadly modification of beton mixture", "GUI_LabelL"),
lI1a(NULL, PG_Rect(HM1_I1_PX,HM1_I1_PY, HM1_I1_VX-2, 20), "BECHER", "GUI_LabelL"),
lI1b(NULL, PG_Rect(HM1_I1_PX,HM1_I1_PY+HM1_LY2, HM1_I1_VX-2, 20), "80 points", "GUI_LabelL"),
lI2a(NULL, PG_Rect(HM1_I2_PX,HM1_I2_PY, HM1_I2_VX-2, 20), "RAPID", "GUI_LabelL"),
lI2b(NULL, PG_Rect(HM1_I2_PX,HM1_I2_PY+HM1_LY2, HM1_I2_VX-2, 20), "20 points", "GUI_LabelL"),
lI3a(NULL, PG_Rect(HM1_I3_PX,HM1_I3_PY, HM1_I3_VX-2, 20), "GOLD", "GUI_LabelL"),
lI3b(NULL, PG_Rect(HM1_I3_PX,HM1_I3_PY+HM1_LY2, HM1_I3_VX-2, 20), "6 points", "GUI_LabelL"),
lI4a(NULL, PG_Rect(HM1_I4_PX,HM1_I4_PY, HM1_I4_VX-2, 20), "GOLD", "GUI_LabelL"),
lI4b(NULL, PG_Rect(HM1_I4_PX,HM1_I4_PY+HM1_LY2, HM1_I4_VX-2, 20), "5 points", "GUI_LabelL"),
lI5a(NULL, PG_Rect(HM1_I5_PX,HM1_I5_PY, HM1_I5_VX-2, 20), "DRINK", "GUI_LabelL"),
lI5b(NULL, PG_Rect(HM1_I5_PX,HM1_I5_PY+HM1_LY2, HM1_I5_VX-2, 20), "11 points", "GUI_LabelL"),
lI6a(NULL, PG_Rect(HM1_I6_PX,HM1_I6_PY, HM1_I6_VX-2, 20), "DRINK", "GUI_LabelL"),
lI6b(NULL, PG_Rect(HM1_I6_PX,HM1_I6_PY+HM1_LY2, HM1_I6_VX-2, 20), "13 points", "GUI_LabelL"),
//bPrev(NULL, 2, PG_Rect(HM1_PX+5,HM1_PY+330,70,20), "PREV"),
bReturn(NULL, 1, PG_Rect(HM1_PX+(HM1_VX-100)/2,HM1_PY+330,100,20), "RETURN"),
bNext(NULL, 3, PG_Rect(HM1_PX+HM1_VX-5-70,HM1_PY+330,70,20), "NEXT")
{
  Default();

  Help1Menu.SetFont(MainFont);

  AddChild(&Board1);
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
}

void GUI_Help1Menu::Default()
{
}

bool GUI_Help1Menu::eventButtonClick(int id, PG_Widget* widget)
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
GUI_BaseMenu(GUI_HELP2, PG_Rect(HM2_PX,HM2_PY,HM2_VX,HM2_VY)),
Board1(NULL, PG_Rect(HM2_PX,HM2_PY,HM2_VX,HM2_VY)),
mBack1(NULL, PG_Rect(HM2_PX+1, HM2_PY+1, help2->w, help2->h), help2),
Help2Menu(NULL, PG_Rect(HM2_PX+1,HM2_PY+4,HM2_VX-2,25), "Items & Ammo", "GUI_Label"),
lI1a(NULL, PG_Rect(HM2_I1_PX,HM2_I1_PY, HM2_I1_VX-2, 20), "SPEED", "GUI_LabelL"),
lI1b(NULL, PG_Rect(HM2_I1_PX,HM2_I1_PY+HM2_LY1, HM2_I1_VX-2, 20), "faster moving", "GUI_LabelL"),
lI2a(NULL, PG_Rect(HM2_I2_PX,HM2_I2_PY, HM2_I2_VX-2, 20), "POWER", "GUI_LabelL"),
lI2b(NULL, PG_Rect(HM2_I2_PX,HM2_I2_PY+HM2_LY1, HM2_I2_VX-2, 20), "full bomb power", "GUI_LabelL"),
lI3a(NULL, PG_Rect(HM2_I3_PX,HM2_I3_PY, HM2_I3_VX-2, 20), "SHIELD", "GUI_LabelL"),
lI3b(NULL, PG_Rect(HM2_I3_PX,HM2_I3_PY+HM2_LY1, HM2_I3_VX-2, 20), "add shields", "GUI_LabelL"),
lI4a(NULL, PG_Rect(HM2_I4_PX,HM2_I4_PY, HM2_I4_VX-2, 20), "INVISIBILITY", "GUI_LabelL"),
lI4b(NULL, PG_Rect(HM2_I4_PX,HM2_I4_PY+HM2_LY1, HM2_I4_VX-2, 20), "", "GUI_LabelL"),
lI5a(NULL, PG_Rect(HM2_I5_PX,HM2_I5_PY, HM2_I5_VX-2, 20), "PUNCH", "GUI_LabelL"),
lI5b(NULL, PG_Rect(HM2_I5_PX,HM2_I5_PY+HM2_LY1, HM2_I5_VX-2, 20), "punching bombs", "GUI_LabelL"),
lI6a(NULL, PG_Rect(HM2_I6_PX,HM2_I6_PY, HM2_I6_VX-2, 20), "WARP", "GUI_LabelL"),
lI6b(NULL, PG_Rect(HM2_I6_PX,HM2_I6_PY+HM2_LY1, HM2_I6_VX-2, 20), "add warps", "GUI_LabelL"),
lI7a(NULL, PG_Rect(HM2_I7_PX,HM2_I7_PY, HM2_I7_VX-2, 20), "GLASSES", "GUI_LabelL"),
lI7b(NULL, PG_Rect(HM2_I7_PX,HM2_I7_PY+HM2_LY1, HM2_I7_VX-2, 20), "detecting invisibles", "GUI_LabelL"),
lJ1a(NULL, PG_Rect(HM2_J1_PX,HM2_J1_PY, HM2_J1_VX-2, 20), "BECHEROMET", "GUI_LabelL"),
lJ1b(NULL, PG_Rect(HM2_J1_PX,HM2_J1_PY+HM2_LY2, HM2_J1_VX-2, 20), "enable weapon", "GUI_LabelL"),
lJ2a(NULL, PG_Rect(HM2_J2_PX,HM2_J2_PY, HM2_J2_VX-2, 20), "RAILGUN", "GUI_LabelL"),
lJ2b(NULL, PG_Rect(HM2_J2_PX,HM2_J2_PY+HM2_LY2, HM2_J2_VX-2, 20), "enable weapon", "GUI_LabelL"),
lJ3a(NULL, PG_Rect(HM2_J3_PX,HM2_J3_PY, HM2_J3_VX-2, 20), "BOMB", "GUI_LabelL"),
lJ3b(NULL, PG_Rect(HM2_J3_PX,HM2_J3_PY+HM2_LY2, HM2_J3_VX-2, 20), "enable weapon", "GUI_LabelL"),
lJ4a(NULL, PG_Rect(HM2_J4_PX,HM2_J4_PY, HM2_J4_VX-2, 20), "GRENADE", "GUI_LabelL"),
lJ4b(NULL, PG_Rect(HM2_J4_PX,HM2_J4_PY+HM2_LY2, HM2_J4_VX-2, 20), "enable weapon", "GUI_LabelL"),
lJ5a(NULL, PG_Rect(HM2_J5_PX,HM2_J5_PY, HM2_J5_VX-2, 20), "AMMO", "GUI_LabelL"),
lJ5b(NULL, PG_Rect(HM2_J5_PX,HM2_J5_PY+HM2_LY2, HM2_J5_VX-2, 20), "becheromet", "GUI_LabelL"),
lJ6a(NULL, PG_Rect(HM2_J6_PX,HM2_J6_PY, HM2_J6_VX-2, 20), "AMMO", "GUI_LabelL"),
lJ6b(NULL, PG_Rect(HM2_J6_PX,HM2_J6_PY+HM2_LY2, HM2_J6_VX-2, 20), "railgun", "GUI_LabelL"),
lJ7a(NULL, PG_Rect(HM2_J7_PX,HM2_J7_PY, HM2_J7_VX-2, 20), "AMMO", "GUI_LabelL"),
lJ7b(NULL, PG_Rect(HM2_J7_PX,HM2_J7_PY+HM2_LY2, HM2_J7_VX-2, 20), "bombs", "GUI_LabelL"),
lK1a(NULL, PG_Rect(HM2_K1_PX,HM2_K1_PY, HM2_K1_VX-2, 20), "AMMO", "GUI_LabelL"),
lK1b(NULL, PG_Rect(HM2_K1_PX,HM2_K1_PY+HM2_LY3, HM2_K1_VX-2, 20), "grenades", "GUI_LabelL"),
lK2a(NULL, PG_Rect(HM2_K2_PX,HM2_K2_PY, HM2_K2_VX-2, 20), "TURTLE", "GUI_LabelL"),
lK2b(NULL, PG_Rect(HM2_K2_PX,HM2_K2_PY+HM2_LY3, HM2_K2_VX-2, 20), "slower moving", "GUI_LabelL"),
lK3a(NULL, PG_Rect(HM2_K3_PX,HM2_K3_PY, HM2_K3_VX-2, 20), "GLUE", "GUI_LabelL"),
lK3b(NULL, PG_Rect(HM2_K3_PX,HM2_K3_PY+HM2_LY3, HM2_K3_VX-2, 20), "not able moving", "GUI_LabelL"),
lK4a(NULL, PG_Rect(HM2_K4_PX,HM2_K4_PY, HM2_K4_VX-2, 20), "REVERSE", "GUI_LabelL"),
lK4b(NULL, PG_Rect(HM2_K4_PX,HM2_K4_PY+HM2_LY3, HM2_K4_VX-2, 20), "reverse controls", "GUI_LabelL"),
lK5a(NULL, PG_Rect(HM2_K5_PX,HM2_K5_PY, HM2_K5_VX-2, 20), "LOST", "GUI_LabelL"),
lK5b(NULL, PG_Rect(HM2_K5_PX,HM2_K5_PY+HM2_LY3, HM2_K5_VX-2, 20), "disable weapons", "GUI_LabelL"),
lK6a(NULL, PG_Rect(HM2_K6_PX,HM2_K6_PY, HM2_K6_VX-2, 20), "BERSERK", "GUI_LabelL"),
lK6b(NULL, PG_Rect(HM2_K6_PX,HM2_K6_PY+HM2_LY3, HM2_K6_VX-2, 20), "kill by touch", "GUI_LabelL"),
lK7a(NULL, PG_Rect(HM2_K7_PX,HM2_K7_PY, HM2_K7_VX-2, 20), " ", "GUI_LabelL"),
lK7b(NULL, PG_Rect(HM2_K7_PX,HM2_K7_PY+HM2_LY3, HM2_K7_VX-2, 20), " ", "GUI_LabelL"),
bPrev(NULL, 2, PG_Rect(HM2_PX+5,HM2_PY+365,70,20), "PREV"),
bReturn(NULL, 1, PG_Rect(HM2_PX+(HM2_VX-100)/2,HM2_PY+365,100,20), "RETURN")
//bNext(NULL, 3, PG_Rect(HM2_PX+HM2_VX-5-70,HM2_PY+365,70,20), "NEXT")
{
  Default();

  Help2Menu.SetFont(MainFont);
  
  AddChild(&Board1);
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
  AddChild(&bPrev);
}

void GUI_Help2Menu::Default()
{
}

bool GUI_Help2Menu::eventButtonClick(int id, PG_Widget* widget)
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
  MainFont = TTF_OpenFont(PG_Application::GetRelativePath("fonts/futuraxk.ttf"), 24);
  if (!MainFont)
  {
    fprintf(stderr, "GUI: Couldn't load font %s", PG_Application::GetRelativePath("fonts/futuraxk.ttf"));
    return false;
  }
  MainFont2 = TTF_OpenFont(PG_Application::GetRelativePath("fonts/futuraxk.ttf"), 28);
  if (!MainFont2)
  {
    fprintf(stderr, "GUI: Couldn't load font %s", PG_Application::GetRelativePath("fonts/futuraxk.ttf"));
    return false;
  }
  BtnFont = TTF_OpenFont(PG_Application::GetRelativePath("fonts/futurab.ttf"), 12);
  if (!BtnFont)
  {
    fprintf(stderr, "GUI: Couldn't load font %s", PG_Application::GetRelativePath("fonts/futurab.ttf"));
    return false;
  }
  BtnFont2 = TTF_OpenFont(PG_Application::GetRelativePath("fonts/futurab.ttf"), 14);
  if (!BtnFont2)
  {
    fprintf(stderr, "GUI: Couldn't load font %s", PG_Application::GetRelativePath("fonts/futurab.ttf"));
    return false;
  }
  
  return true;
}

bool GUI_Done()
{
  if (MainFont) TTF_CloseFont(MainFont);
  if (MainFont2) TTF_CloseFont(MainFont2);
  if (BtnFont) TTF_CloseFont(BtnFont);
  if (BtnFont2) TTF_CloseFont(BtnFont2);
  return true;
}

void RenderResults(SDL_Surface *screen)
{

}

