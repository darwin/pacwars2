//###########################################################################
//## GUI methods
//## 
//## 
//###########################################################################

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

class GUI_BaseMenu;

GUI_BaseMenu* GUI_menu = NULL;
int GUI_id = 0;

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

class GUI_Board : public SDLGradientWidget {
public:
  GUI_Board(SDLWidget* parent, SDL_Rect& r, bool storebackground = false, char* theme=NULL);
//  void eventDraw(SDL_Surface* surface, SDL_Rect* rect);
  
  bool drawbackground;
};

GUI_Board::GUI_Board(SDLWidget* parent, SDL_Rect& r,  bool storebackground, char* theme):
SDLGradientWidget(parent,r)
{
//  drawbackground = true;
  if (!theme)
    LoadThemeStyle("GUI_Board","GradientWidget");
  else
    LoadThemeStyle(theme,"GradientWidget");
}

/*
void GUI_Board::eventDraw(SDL_Surface* surface, SDL_Rect* rect)
{
  if (drawbackground) {
    SDL_FillRect(surface, rect, SDL_MapRGB(surface->format, c.r, c.g, c.b));
    SDLWidget::DrawBorder(surface, rect, 1, false);
  }
  else
    SDL_FillRect(surface, rect, 0);
}
*/

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
/*
void GUI_Label::eventDraw(SDL_Surface* surface, SDL_Rect* rect)
{
	SDL_Rect srcrect;

  if (bgmode) {
    if (bgmode>=1 && bgmode<=2)
    {
      SDL_FillRect(surface, rect, SDL_MapRGB(surface->format, c.r, c.g, c.b));
      if (bgmode==1)
      {
        SDLWidget::DrawBorder(surface, rect, 1, false);
      }
    }
    else
    {
      SDL_FillRect(surface, rect, 0);
    }
  }
  
  my_srfText = TTF_RenderText_Blended(GetFont(), my_textLabel, GetTextColor());
  if (my_srfText)
  {
		switch (my_alignment) {
			case SDL_TA_LEFT:
			my_rectLabel.x = rect->x;
			my_rectLabel.y = rect->y + (rect->h - my_srfText->h) / 2;
			break;

			case SDL_TA_RIGHT:
			my_rectLabel.x = rect->x + (rect->w - my_srfText->w);
			my_rectLabel.y = rect->y + (rect->h - my_srfText->h) / 2;
			break;

			case SDL_TA_CENTER:
			my_rectLabel.x = rect->x + (rect->w - my_srfText->w) / 2;
			my_rectLabel.y = rect->y + (rect->h - my_srfText->h) / 2;
			break;
		}

		srcrect.x = 0;
		srcrect.y = 0;
		srcrect.w = my_srfText->w;
		srcrect.h = my_srfText->h;

		my_rectLabel.h = my_srfText->h;
		my_rectLabel.w = my_srfText->w;

		SDL_BlitSurface(my_srfText, &srcrect, surface, &my_rectLabel);
		SDL_FreeSurface(my_srfText);
    my_srfText = false;
  }
}
*/
class GUI_LabelC : public SDLLabel {
public:
  GUI_LabelC(SDLWidget* parent, SDL_Rect& r, char* text, bool storebackground = false);
//  void eventDraw(SDL_Surface* surface, SDL_Rect* rect);
  
  SDL_Color c;
  int shifty;
  int shiftx;
  int bgmode;
};


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

/*
void GUI_LabelC::eventDraw(SDL_Surface* surface, SDL_Rect* rect)
{
  /*if (bgmode) {
    if (bgmode>=1 && bgmode<=2)
    {
      SDL_FillRect(surface, rect, SDL_MapRGB(surface->format, c.r, c.g, c.b));
      if (bgmode==1)
      {
        SDLWidget::DrawBorder(surface, rect, 1, false);
      }
    }
    else
    {
      SDL_FillRect(surface, rect, 0);
    }
  }*/
/*  SDL_FillRect(surface, rect, 0);
  
	SDL_Rect srcrect;
  
  my_srfText = TTF_RenderText_Blended(GetFont(), my_textLabel, GetTextColor());
  if (my_srfText)
  {
		switch (my_alignment) {
			case SDL_TA_LEFT:
			my_rectLabel.x = rect->x;
			my_rectLabel.y = rect->y + (rect->h - my_srfText->h) / 2;
			break;

			case SDL_TA_RIGHT:
			my_rectLabel.x = rect->x + (rect->w - my_srfText->w);
			my_rectLabel.y = rect->y + (rect->h - my_srfText->h) / 2;
			break;

			case SDL_TA_CENTER:
			my_rectLabel.x = rect->x + (rect->w - my_srfText->w) / 2;
			my_rectLabel.y = rect->y + (rect->h - my_srfText->h) / 2;
			break;
		}

		srcrect.x = 0;
		srcrect.y = 0;
		srcrect.w = my_srfText->w;
		srcrect.h = my_srfText->h;

		my_rectLabel.h = my_srfText->h;
		my_rectLabel.w = my_srfText->w;

		SDL_BlitSurface(my_srfText, &srcrect, surface, &my_rectLabel);
		SDL_FreeSurface(my_srfText);
    my_srfText = false;
  }
}
*/
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

bool eventMouseButtonDown(const SDL_MouseButtonEvent* button);


/////////////////////////////////////////////////////////////////////////////
// ResultLine
/////////////////////////////////////////////////////////////////////////////

class GUI_ResultLine : public GUI_Label {
public:
  GUI_ResultLine();
  void eventDraw(SDL_Surface* surface, SDL_Rect* rect);
};


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

class GUI_Button : public SDLButton {
public:
  GUI_Button(SDLWidget* parent, int btnid, SDL_Rect& r, char* text);
//  void eventDraw(SDL_Surface* surface, SDL_Rect* rect);
  
  SDL_Color c;
  int shifty;
  bool drawbackground;
};

GUI_Button::GUI_Button(SDLWidget* parent, int btnid, SDL_Rect& r, char* text):
SDLButton(parent,btnid,r,text)
{
  SetBorderSize(0,0,0);
  SetTextColor(GUI_BtnTextColor);
  c = GUI_Red100;
  shifty = 0;//2;
  drawbackground = false;
}

/*
void GUI_Button::eventDraw(SDL_Surface* surface, SDL_Rect* rect){
  SDL_Rect r;
  
  //  _beep(200,100);
  if (drawbackground) {
    SDL_FillRect(surface, rect, SDL_MapRGB(surface->format, c.r, c.g, c.b));
    SDLWidget::DrawBorder(surface, rect, 1, false);
  }
  else
    SDL_FillRect(surface, rect, 0);
  
  int shift = ((state == BTN_STATE_PRESSED) || (togglemode && isPressed)) ? 4 : 0;
  
  r.x = rect->x + rect->w/2; //+ shift;
  r.y = rect->y + rect->h/2 + shift;
  r.h = 0;
  
  if(labeltext[0] != 0){
    
    SDL_Surface* label_srf = TTF_RenderText_Blended(GetFont(), labeltext, GetTextColor());
    
    SDL_Rect lr;
    lr.x = (rect->w - label_srf->w)/2;// + shift;
    lr.y = r.y + r.h;
    lr.w = label_srf->w;
    lr.h = label_srf->h;
    
    lr.y = rect->y + (rect->h - label_srf->h)/2;
    if((lr.y + lr.h) > (rect->y + rect->h)){
      lr.y = rect->h - (label_srf->h+3) + shift;
    }
    
    lr.y+=shifty;
    SDL_BlitSurface(label_srf, NULL, surface, &lr);
    SDL_FreeSurface(label_srf);
  }
}
*/

class GUI_ButtonBig : public GUI_Button {
public:
  GUI_ButtonBig(SDLWidget* parent, int btnid, SDL_Rect& r, char* text);
  void eventMouseLeave() { 
    //SetFont(MainFont); 
    if (enabled)
    {
      SetTextColor(GUI_BtnTextColor); 
      Redraw(); 
    }
  }
  void eventMouseEnter() { 
    //SetFont(MainFont2); 
    if (enabled)
    {
      SetTextColor(GUI_BtnATextColor); 
      Redraw(); 
    }
  }
	bool eventMouseButtonDown(const SDL_MouseButtonEvent* button)
  {
    if (enabled)
      return GUI_Button::eventMouseButtonDown(button);
    return true;
  }
	bool eventMouseButtonUp(const SDL_MouseButtonEvent* button)
  {
    if (enabled)
      return GUI_Button::eventMouseButtonUp(button);
    return true;
  }

public:
  bool enabled;

};

GUI_ButtonBig::GUI_ButtonBig(SDLWidget* parent, int btnid, SDL_Rect& r, char* text):
GUI_Button(parent,btnid,r,text)
{
  SetFont(MainFont);
  //  drawbackground = false;
  drawbackground = true;
  enabled = true;
	LoadThemeStyle("GUI_MenuButton", "Button");
}

class GUI_ButtonSmall : public GUI_Button {
public:
  GUI_ButtonSmall(SDLWidget* parent, int btnid, SDL_Rect& r, char* text);
  void eventMouseLeave() { /*SetFont(BtnFont);*/ SetTextColor(GUI_BtnTextColor); Redraw(); }
  void eventMouseEnter() { /*SetFont(BtnFont2);*/ SetTextColor(GUI_BtnATextColor); Redraw(); }
};

GUI_ButtonSmall::GUI_ButtonSmall(SDLWidget* parent, int btnid, SDL_Rect& r, char* text):
GUI_Button(parent,btnid,r,text)
{
  SetFont(BtnFont);
  drawbackground = true;
  c = GUI_SmBtColor;
	LoadThemeStyle("GUI_NormalButton", "Button");
}


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

GUI_CheckBox::GUI_CheckBox(SDLWidget* parent, SDL_Rect& r, char* text, bool ipressed, SDL_Color bg) : 
SDLCheckButton(parent,99, r, text)
{
  if (ipressed) SetPressed();
  my_widgetLabel->SetFont(TextFont);

  LoadThemeStyle("GUI_CheckButton", "GradientWidget");
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

class GUI_TextEdit : public SDLLineEdit {
public:
  GUI_TextEdit(SDLWidget* parent, SDL_Rect& r);
};

GUI_TextEdit::GUI_TextEdit(SDLWidget* parent, SDL_Rect& r):
SDLLineEdit(parent, r)
{
  SetFont(TextFont);
  LoadThemeStyle("GUI_LineEdit", "GradientWidget");
}

/////////////////////////////////////////////////////////////////////////////
// NumEdit
/////////////////////////////////////////////////////////////////////////////

class GUI_NumEdit : public GUI_TextEdit {
public:
  GUI_NumEdit(SDLWidget* parent, SDL_Rect& r, int imin, int imax);
  
  bool eventFilterKey(const SDL_KeyboardEvent* key);

  int min, max;
};

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

class GUI_FloatEdit : public GUI_TextEdit {
public:
  GUI_FloatEdit(SDLWidget* parent, SDL_Rect& r, float imin, float max);
  
  bool eventFilterKey(const SDL_KeyboardEvent* key);
  
  float min, max;
};

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

class GUI_ScrollBar : public SDLScrollBar {
public:
  GUI_ScrollBar(SDLWidget* parent, int id, SDL_Rect& r, int direction);
};

GUI_ScrollBar::GUI_ScrollBar(SDLWidget* parent, int id, SDL_Rect& r, int direction) : SDLScrollBar(parent, id, r, direction)
{

}

#define LB_POSX 5

class GUI_WidgetList : public SDLWidgetList {
public: 
  GUI_WidgetList(SDLWidget* parent, SDL_Rect& r);
  ~GUI_WidgetList();

  void AddWidget(SDLWidget* w);
};

GUI_WidgetList::GUI_WidgetList(SDLWidget* parent, SDL_Rect& r) : SDLWidgetList(parent, r/*, true*/){
  
  // TODO: comment SDLWidgetList(...) !!! when download new paragui

	/*
	my_widgetCount = 0;
	my_firstWidget = 0;
	my_widthScrollbar = 12;
	my_height = 0;

  my_rectScrollbar.x = r.w - my_widthScrollbar - 1;
	my_rectScrollbar.y = 1;
	my_rectScrollbar.h = r.h - 2;
	my_rectScrollbar.w = my_widthScrollbar;

	my_objScrollbar = new SDLScrollBar(this, SDL_IDWIDGETLIST_SCROLL, my_rectScrollbar, SDL_SB_VERTICAL);

	my_rectList.x = 0;
	my_rectList.y = 0;
	my_rectList.w = r.w - my_widthScrollbar;
	my_rectList.h = r.h;
 	*/

  LoadThemeStyle("GUI_WidgetList", "GradientWidget");
/*	
  my_rectScrollbar.x = r.w - my_widthScrollbar - 1;
	my_rectScrollbar.y = 0;
	my_rectScrollbar.h = r.h - 2;
	my_rectScrollbar.w = my_widthScrollbar;

	my_rectList.x = 5;
	my_rectList.y = 1;
	my_rectList.w = r.w - my_widthScrollbar - 10;
	my_rectList.h = r.h-2;
*/
}

GUI_WidgetList::~GUI_WidgetList()
{
//	DeleteAll();
//	delete my_objScrollbar;
}

void GUI_WidgetList::AddWidget(SDLWidget* w) {

	SDLWidgetList::AddWidget(w);

	/*
	w->SetVisible(false);
	AddChild(w);
	w->SetStoreBackground(false);

	SDL_Rect r = w->GetWidgetRect();

	if(my_widgetList.empty()) {
		w->MoveWindow(LB_POSX, 0);
		my_height = w->Height();
	} else {
		r = my_widgetList[my_widgetCount-1]->GetWidgetRect();
		r.x -= my_rectDisplay.x;
		r.y -= my_rectDisplay.y;

		w->MoveWindow(r.x, r.y+r.h);
		my_height = r.y + r.h + w->Height();
	}

	my_widgetList.insert(my_widgetList.end(), w);
	my_widgetCount++;

	my_objScrollbar->SetLineSize(my_height / my_widgetCount);
	my_objScrollbar->SetRange(0, my_height - Height());
	*/
}


	
/////////////////////////////////////////////////////////////////////////////
// Base menu widgets
/////////////////////////////////////////////////////////////////////////////

class GUI_BaseMenu : public SDLWidget {
public:
  int mid;
  int parentmid;
  GUI_BaseMenu *next;
  
  GUI_BaseMenu(int iid, SDL_Rect& rect): SDLWidget(NULL, rect) { mid = iid; next = NULL; }
  
  void eventDraw(SDL_Surface* surface, SDL_Rect* rect);
  void Clear();
  
  virtual void Default() { }
  virtual void Execute() { }
  virtual void Show()=0;
  virtual void Hide()=0;
  virtual void Return(); 
};

void GUI_BaseMenu::eventDraw(SDL_Surface* surface, SDL_Rect* rect)
{
  SDLWidget::DrawBorder(surface, rect, 1, false);
}

void GUI_BaseMenu::Clear()
{
  SDL_FillRect(SDLApplication::GetScreen(), &my_rectDisplay, 0);
}

void GUI_BaseMenu::Return()
{
  Hide();
  blocked_inputs = 0;
  HideMouse();
  SDL_FillRect(SDLApplication::GetScreen(), &my_rectDisplay, 0);
  if (parentmid!=-1) 
  {
    GUI_Show(parentmid);
  }
  
  smPlaySample(SM_MENU3, menu_volume);
}

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
  
  GUI_Board Board1;
  GUI_Label OKDialog1;
  
  GUI_Label lPrompt1;
  
  GUI_ButtonSmall bOK;
  
  void Show();
  void Hide();
  void Default();
  
  bool eventButtonClick(int id, SDLWidget* widget);
};

void GUI_OKDialog1::Reset(char* title, char* line1)
{
  OKDialog1.SetText(title);
  lPrompt1.SetText(line1);
}

GUI_OKDialog1::GUI_OKDialog1(char* title, char* line1, char* line2):
GUI_BaseMenu(GUI_OKDIALOG1, mkrect(OKD1_PX,OKD1_PY,OKD1_VX,OKD1_VY)),
Board1(NULL, SDLWidget::mkrect(OKD1_PX,OKD1_PY,OKD1_VX,100), false),
OKDialog1(NULL, SDLWidget::mkrect(OKD1_PX+1,OKD1_PY+4,OKD1_VX-2,25), title, false),
lPrompt1(NULL, SDLWidget::mkrect(OKD1_PX+1,OKD1_PY+35, OKD1_VX-2,20), line1, false),
bOK(NULL, 1, SDLWidget::mkrect(OKD1_PX+105,OKD1_PY+65,150,25), "OK")
{
  Default();
  
  OKDialog1.SetColor(GUI_BtnTextColor, GUI_BtnATextColor);
  OKDialog1.SetFont(MainFont);
  
  OKDialog1.bgmode = 2;
  
  lPrompt1.SetAlignment(SDL_TA_CENTER); 
  lPrompt1.bgmode = 2;
  
  AddChild(&Board1);
  AddChild(&OKDialog1);
  AddChild(&lPrompt1);
  AddChild(&bOK);
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

void GUI_OKDialog1::Show()
{
  Clear();
  
  Board1.Show();  
  OKDialog1.Show();
  
  lPrompt1.Show();  
  
  bOK.Show();  
}

void GUI_OKDialog1::Hide()
{
  Board1.Hide();  
  OKDialog1.Hide();
  
  lPrompt1.Hide();  
  
  bOK.Hide();  
}


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
  
  GUI_Board Board1;
  GUI_Label OKDialog2;
  
  GUI_Label lPrompt1;
  GUI_Label lPrompt2;
  
  GUI_ButtonSmall bOK;
  
  void Show();
  void Hide();
  void Default();
  
  bool eventButtonClick(int id, SDLWidget* widget);
};

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
  
  AddChild(&Board1);
  AddChild(&OKDialog2);
  AddChild(&lPrompt1);
  AddChild(&lPrompt2);
  AddChild(&bOK);
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

void GUI_OKDialog2::Show()
{
  Clear();
  
  Board1.Show();  
  OKDialog2.Show();
  
  lPrompt1.Show();  
  lPrompt2.Show();  
  
  bOK.Show();  
}

void GUI_OKDialog2::Hide()
{
  Board1.Hide();  
  OKDialog2.Hide();
  
  lPrompt1.Hide();  
  lPrompt2.Hide();  
  
  bOK.Hide();  
}


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
  
  GUI_Board Board1;
  GUI_Label YNDialog;
  
  GUI_Label lPrompt1;
  
  GUI_ButtonSmall bA;
  GUI_ButtonSmall bB;

  int result;
  void (*callback)(int res);

  void Show();
  void Hide();
  void Default();
  
  bool eventButtonClick(int id, SDLWidget* widget);
};

void GUI_YNDialog::Reset(void (*cb)(int res), char* title, char* line1, char* tla, char* tlb)
{
  YNDialog.SetText(title);
  lPrompt1.SetText(line1);
  bA.SetText(tla);
  bB.SetText(tlb);
  callback = cb;
}

GUI_YNDialog::GUI_YNDialog():
GUI_BaseMenu(GUI_YNDIALOG, mkrect(YN_PX,YN_PY,YN_VX,YN_VY)),
Board1(NULL, SDLWidget::mkrect(YN_PX,YN_PY,YN_VX,100), false),
YNDialog(NULL, SDLWidget::mkrect(YN_PX+1,YN_PY+4,YN_VX-2,25), "", false),
lPrompt1(NULL, SDLWidget::mkrect(YN_PX+1,YN_PY+35, YN_VX-2,20), "", false),
bA(NULL, 1, SDLWidget::mkrect(YN_PX+25,YN_PY+65,150,25), "YES"),
bB(NULL, 2, SDLWidget::mkrect(YN_PX+25+150+10,YN_PY+65,150,25), "NO")
{
  Default();
  
  YNDialog.SetColor(GUI_BtnTextColor, GUI_BtnATextColor);
  YNDialog.SetFont(MainFont);
  
  YNDialog.bgmode = 2;
  
  lPrompt1.SetAlignment(SDL_TA_CENTER); 
  lPrompt1.bgmode = 2;
  
  AddChild(&Board1);
  AddChild(&YNDialog);
  AddChild(&lPrompt1);
  AddChild(&bA);
  AddChild(&bB);
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

void GUI_YNDialog::Show()
{
  Clear();
  
  Board1.Show();  
  YNDialog.Show();
  
  lPrompt1.Show();  
  
  bA.Show();  
  bB.Show();  
}

void GUI_YNDialog::Hide()
{
  Board1.Hide();  
  YNDialog.Hide();
  
  lPrompt1.Hide();  
  
  bA.Hide();  
  bB.Hide();  
}



/////////////////////////////////////////////////////////////////////////////
// Main menu widgets
/////////////////////////////////////////////////////////////////////////////
#define MM_PX 180
#define MM_PY 40
#define MM_VX 280
#define MM_VY 400


class GUI_MainMenu : public GUI_BaseMenu {
public:
  GUI_MainMenu();

//  GUI_Bitmap mBack1;
  
  GUI_ButtonBig lMainMenu;
  GUI_ButtonBig bNewGame;
  GUI_ButtonBig bEndGame;
  GUI_ButtonBig bJoinDiscGame;
  GUI_ButtonBig bCreatePlayer;
  GUI_ButtonBig bDeletePlayer;
  GUI_ButtonBig bOptions;
  GUI_ButtonBig bHelp;
  GUI_ButtonBig bCredits;
  GUI_ButtonBig bQuit;
  GUI_ButtonBig bGame;
  
  void Show();
  void Hide();
  void Return();
  void Default();
  
  bool eventButtonClick(int id, SDLWidget* widget);
};

GUI_MainMenu::GUI_MainMenu():
GUI_BaseMenu(GUI_MAINMENU, mkrect(MM_PX,MM_PY,MM_VX,MM_VY)),
//mBack1(NULL, SDLWidget::mkrect(MM_PX+30, MM_PY+60, splash->w, splash->h), false, splash),
lMainMenu(NULL, 0, SDLWidget::mkrect(MM_PX,MM_PY,MM_VX,40), "Main Menu"),
bNewGame(NULL, 1, SDLWidget::mkrect(MM_PX,MM_PY+40,MM_VX,40), "Start Server"),
bEndGame(NULL, 9, SDLWidget::mkrect(MM_PX,MM_PY+40,MM_VX,40), "Shutdown Server"),
bJoinDiscGame(NULL, 2, SDLWidget::mkrect(MM_PX,MM_PY+80,MM_VX,40), "Join Game"),
bCreatePlayer(NULL, 3, SDLWidget::mkrect(MM_PX,MM_PY+120,MM_VX,40), "Create Player"),
bDeletePlayer(NULL, 4, SDLWidget::mkrect(MM_PX,MM_PY+160,MM_VX,40), "Delete Player"),
bOptions(NULL, 5, SDLWidget::mkrect(MM_PX,MM_PY+200,MM_VX,40), "Options"),
bCredits(NULL, 6, SDLWidget::mkrect(MM_PX,MM_PY+240,MM_VX,40), "Credits"),
bHelp(NULL, 7, SDLWidget::mkrect(MM_PX,MM_PY+280,MM_VX,40), "Info"),
bQuit(NULL, 8, SDLWidget::mkrect(MM_PX,MM_PY+320,MM_VX,40), "Quit"),
bGame(NULL, 10, SDLWidget::mkrect(MM_PX,MM_PY+360,MM_VX,40), "Back To Game")
{
  Default();
  
  lMainMenu.enabled = false;
	lMainMenu.LoadThemeStyle("GUI_MenuTitle", "Button");
  
//  AddChild(&mBack1);
  AddChild(&lMainMenu);
  AddChild(&bNewGame);
  AddChild(&bEndGame);
  AddChild(&bJoinDiscGame);
  AddChild(&bCreatePlayer);
  AddChild(&bDeletePlayer);
  AddChild(&bOptions);
  AddChild(&bCredits);
  AddChild(&bHelp);
  AddChild(&bQuit);
  AddChild(&bGame);
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

void GUI_MainMenu::Show()
{
  Clear();
  
//  mBack1.Show();
  if (net_client_status==NS_CONNECTED)
  {
//    objDisplayRect.y = (480-400)/2;
//    objDisplayRect.h = 400;
//    objClipRect = objDisplayRect;

    bJoinDiscGame.SetText("Disconnect");
    bCreatePlayer.MoveRect(MM_PX,MM_PY+120);
    bDeletePlayer.MoveRect(MM_PX,MM_PY+160);
    bOptions.MoveRect(MM_PX,MM_PY+200);
    bCredits.MoveRect(MM_PX,MM_PY+240);
    bHelp.MoveRect(MM_PX,MM_PY+280);
    bQuit.MoveRect(MM_PX,MM_PY+320);
    bGame.MoveRect(MM_PX,MM_PY+360);
    
    bCreatePlayer.Show(); 
    bDeletePlayer.Show();  
    bOptions.Show();  
    bCredits.Show();  
    bHelp.Show();  
    bQuit.Show();
    bGame.Show();
  }
  else
  {
//    objDisplayRect.y = (480-280)/2;
//    objDisplayRect.h = 280;
//    objClipRect = objDisplayRect;

    bJoinDiscGame.SetText("Join Game");
    bOptions.MoveRect(MM_PX,MM_PY+120);
    bCredits.MoveRect(MM_PX,MM_PY+160);
    bHelp.MoveRect(MM_PX,MM_PY+200);
    bQuit.MoveRect(MM_PX,MM_PY+240);
//    bGame.MoveRect(MM_PX,MM_PY+280);

    bOptions.Show();  
    bCredits.Show();  
    bHelp.Show();  
    bQuit.Show();
  }
  lMainMenu.Show();
  if (net_server_status==NS_UNINITED)
  {
    bNewGame.Show();  
  }
  else
  {
    bEndGame.Show();  
  }
  
  
  bJoinDiscGame.Show();  
}

void GUI_MainMenu::Hide()
{
//  mBack1.Hide();
  lMainMenu.Hide();
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
#define JG_PX 140
#define JG_PY 165
#define JG_VX 360
#define JG_VY 170


class GUI_JoinGameMenu : public GUI_BaseMenu {
public:
  GUI_JoinGameMenu();
  
  GUI_Board Board1;
  GUI_Label JoinGameMenu;
  
  GUI_Label lServerAddress;
  GUI_TextEdit eServerAddress;
  GUI_Label lClientName;
  GUI_TextEdit eClientName;
  GUI_Label lClientDesc;
  GUI_TextEdit eClientDesc;
  
  GUI_ButtonSmall bConnect;
  GUI_ButtonSmall bCancel;
  
  void Show();
  void Hide();
  void Default();
  void Return();
  
  bool eventButtonClick(int id, SDLWidget* widget);
};

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
Board1(NULL, SDLWidget::mkrect(JG_PX,JG_PY,JG_VX, JG_VY), false),
JoinGameMenu(NULL, SDLWidget::mkrect(JG_PX+1,JG_PY+4,JG_VX-2,25), "JOIN GAME", false),
lServerAddress(NULL, SDLWidget::mkrect(JG_PX+1,JG_PY+50, 119,20), "Server address:", false),
eServerAddress(NULL, SDLWidget::mkrect(JG_PX+120,JG_PY+50,215,20)),
lClientName(NULL, SDLWidget::mkrect(JG_PX+1,JG_PY+80, 119,20), "client name:", false),
eClientName(NULL, SDLWidget::mkrect(JG_PX+120,JG_PY+80,215,20)),
lClientDesc(NULL, SDLWidget::mkrect(JG_PX+1,JG_PY+110, 119,20), "client finger:", false),
eClientDesc(NULL, SDLWidget::mkrect(JG_PX+120,JG_PY+110,215,20)),
bConnect(NULL, 1, SDLWidget::mkrect(JG_PX+25,JG_PY+140,150,25), "CONNECT"),
bCancel(NULL, 2, SDLWidget::mkrect(JG_PX+25+150+10,JG_PY+140,150,25), "BACK")
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
  
  AddChild(&Board1);
  AddChild(&JoinGameMenu);
  AddChild(&lServerAddress);
  AddChild(&eServerAddress);
  AddChild(&lClientName);
  AddChild(&eClientName);
  AddChild(&lClientDesc);
  AddChild(&eClientDesc);
  AddChild(&bConnect);
  AddChild(&bCancel);
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

void GUI_JoinGameMenu::Show()
{
  Clear();
  
  Board1.Show();  
  JoinGameMenu.Show();
  
  eServerAddress.Show();  
  lServerAddress.Show();  
  eClientName.Show();  
  lClientName.Show();  
  eClientDesc.Show();  
  lClientDesc.Show();  
  
  bConnect.Show();  
  bCancel.Show();  
}

void GUI_JoinGameMenu::Hide()
{
  Board1.Hide();  
  JoinGameMenu.Hide();
  
  eServerAddress.Hide();  
  lServerAddress.Hide();  
  eClientName.Hide();  
  lClientName.Hide();  
  eClientDesc.Hide();  
  lClientDesc.Hide();  
  
  bConnect.Hide();  
  bCancel.Hide();  
}


/////////////////////////////////////////////////////////////////////////////
// NewGame menu widgets
/////////////////////////////////////////////////////////////////////////////
#define NG_PX 140
#define NG_PY 40
#define NG_VX 360
#define NG_VY 400

class GUI_NewGameMenu : public GUI_BaseMenu {
public:
  GUI_NewGameMenu();
  
  GUI_Board Board;
  GUI_Label NewGameMenu;
  GUI_Label lServerName;
  GUI_TextEdit eServerName;
  GUI_Label lWelcomeMsg;
  GUI_TextEdit eWelcomeMsg;
  GUI_Label lMaxClients;
  GUI_NumEdit eMaxClients;
  GUI_Label lIP;
  GUI_Label lHost;
  GUI_ButtonSmall bStartIt;
  GUI_ButtonSmall bCancel;

  GUI_Label lChoose1;
  GUI_Label lChoose2;

  GUI_Board Board1;
  GUI_Board Board2;
  GUI_Board Board3;

  GUI_Label lAuthFile;
  GUI_Label lDesc1;
  GUI_Label lDesc2;
  GUI_Label lDesc3;

  GUI_CheckBox cJoin;

  GUI_WidgetList WidgetList1; 
  GUI_WidgetList WidgetList2; 

  GUI_LabelL* selected1;
  GUI_LabelL* selected2;
  
  void Show();
  void Hide();
  void Default();
  void Return();

  void GenerateScriptSelection();
  
  bool eventButtonClick(int id, SDLWidget* widget);
};

GUI_NewGameMenu* NGMenu;

void GUI_NewGameMenu::Return()
{
  eServerName.EditEnd();
  eWelcomeMsg.EditEnd();
  eMaxClients.EditEnd();
  GUI_BaseMenu::Return();
}

GUI_NewGameMenu::GUI_NewGameMenu():
GUI_BaseMenu(GUI_NEWGAME, mkrect(NG_PX,NG_PY,NG_VX,NG_VY)),
Board(NULL, SDLWidget::mkrect(NG_PX,NG_PY,NG_VX,NG_VY), false),
NewGameMenu(NULL, SDLWidget::mkrect(NG_PX+1,NG_PY+4,NG_VX-2,25), "NEW GAME", false),
lIP(NULL, SDLWidget::mkrect(NG_PX+1,NG_PY+30,NG_VX-2,20), "<HERE COMES IP ADDRESS>", false),
lHost(NULL, SDLWidget::mkrect(NG_PX+1,NG_PY+51,NG_VX-2,20), "<HERE COMES HOST NAME>", false),
lServerName(NULL, SDLWidget::mkrect(NG_PX+1,NG_PY+90, 119,20), "Server name:", false),
eServerName(NULL, SDLWidget::mkrect(NG_PX+120,NG_PY+90,215,20)),
lWelcomeMsg(NULL, SDLWidget::mkrect(NG_PX+1,NG_PY+120,119,20), "Welcome msg:", false),
eWelcomeMsg(NULL, SDLWidget::mkrect(NG_PX+120,NG_PY+120,215,20)),
lMaxClients(NULL, SDLWidget::mkrect(NG_PX+1,NG_PY+150,119,20), "Max clients:", false),
eMaxClients(NULL, SDLWidget::mkrect(NG_PX+120,NG_PY+150,80,20), 1, PWP_TOTALMAX_CLIENTS),

lChoose1(NULL, SDLWidget::mkrect(NG_PX+25,NG_PY+179, 150,20), "Choose map:", false),
Board1(NULL, SDLWidget::mkrect(NG_PX+25, NG_PY+200, 150, 66), false),
WidgetList1(NULL, SDLWidget::mkrect(NG_PX+26, NG_PY+201, 148, 64)),

lChoose2(NULL, SDLWidget::mkrect(NG_PX+25+150+10,NG_PY+179, 150,20), "Choose script:", false),
Board2(NULL, SDLWidget::mkrect(NG_PX+25+150+10, NG_PY+200, 150, 66), false),
WidgetList2(NULL, SDLWidget::mkrect(NG_PX+26+150+10, NG_PY+201, 148, 64)),

Board3(NULL, SDLWidget::mkrect(NG_PX+25, NG_PY+273, NG_VX-50, 54), false, "GUI_MapInfoBoard"),
lAuthFile(NULL, SDLWidget::mkrect(NG_PX+25,NG_PY+275, NG_VX-50-8,12), "file", false),
lDesc1(NULL, SDLWidget::mkrect(NG_PX+25,NG_PY+287, NG_VX-50-20,12), "d1", false),
lDesc2(NULL, SDLWidget::mkrect(NG_PX+25,NG_PY+299, NG_VX-50-20,12), "d2", false),
lDesc3(NULL, SDLWidget::mkrect(NG_PX+25,NG_PY+311, NG_VX-50-20,12), "d3", false),

cJoin(NULL, SDLWidget::mkrect(NG_PX+25,NG_PY+333,NG_VX-50, CB_SIZEY), "join the game after starting server", true, GUI_Gray64),

bStartIt(NULL, 1, SDLWidget::mkrect(NG_PX+25,NG_PY+360,150,25), "START SERVER"),
bCancel(NULL, 2, SDLWidget::mkrect(NG_PX+25+150+10,NG_PY+360,150,25), "BACK")
{
  NGMenu = this;

  NewGameMenu.SetColor(GUI_BtnTextColor, GUI_BtnATextColor);
  NewGameMenu.SetFont(MainFont);
  
  
  lIP.bgmode = 2;
  lHost.bgmode = 2;
  NewGameMenu.bgmode = 2;

  
  lWelcomeMsg.SetAlignment(SDL_TA_RIGHT); 
  lWelcomeMsg.shiftx = -4;
  lMaxClients.SetAlignment(SDL_TA_RIGHT); 
  lMaxClients.shiftx = -4;
  lServerName.SetAlignment(SDL_TA_RIGHT);
  lServerName.shiftx = -4;

  lChoose1.bgmode = 2;
  lChoose1.SetAlignment(SDL_TA_LEFT);
  lChoose2.bgmode = 2;
  lChoose2.SetAlignment(SDL_TA_LEFT);

  lDesc1.bgmode = 2;
  lDesc1.shiftx = 10;
  lDesc1.shifty = -3;  
  lDesc1.SetAlignment(SDL_TA_LEFT);
  lDesc2.bgmode = 2;
  lDesc2.SetAlignment(SDL_TA_LEFT);
  lDesc2.shiftx = 10;
  lDesc2.shifty = -3;  
  lDesc3.bgmode = 2;
  lDesc3.SetAlignment(SDL_TA_LEFT);
  lDesc3.shiftx = 10;
  lDesc3.shifty = -3;  
  lAuthFile.bgmode = 2;
  lAuthFile.SetAlignment(SDL_TA_LEFT);
  lAuthFile.shiftx = 10;
  lAuthFile.shifty = -3;  
  
  IPaddress serverIP;
  SDLNet_ResolveHost(&serverIP, "localhost", PWP_MSG_SPORT);
  
  if (serverIP.host != INADDR_NONE ) 
  {
    char s[255];
    sprintf(s, "IP %s", AddrToS(&serverIP));
    lIP.SetText(s);
    
    char * remote_host_name = SDLNet_ResolveIP(&serverIP);
    if (remote_host_name)
    {
      // try to resolve found host - that because of localhost doesn't return full IP address
      SDLNet_ResolveHost(&serverIP, remote_host_name, PWP_MSG_SPORT);
      sprintf(s, "IP %s", AddrToS(&serverIP));
      lIP.SetText(s);
      
      sprintf(s, "HOST NAME %s", /*_strlwr(*/remote_host_name/*)*/);
      lHost.SetText(s);
    }
    else
      lHost.SetText("Unknown host name");
  }
  else
  {
    lIP.SetText("Error resolving IP");
    lHost.SetText("Unknown host name");
  }
  
  AddChild(&Board);
  AddChild(&NewGameMenu);
  AddChild(&lIP);
  AddChild(&lHost);
  AddChild(&lServerName);
  AddChild(&eServerName);
  AddChild(&lWelcomeMsg);
  AddChild(&eWelcomeMsg);
  AddChild(&lMaxClients);
  AddChild(&eMaxClients);
  AddChild(&lChoose1);
  AddChild(&Board1);
  AddChild(&WidgetList1);
  AddChild(&lChoose2);
  AddChild(&Board2);
  AddChild(&WidgetList2);
  AddChild(&Board3);
  AddChild(&lAuthFile);
  AddChild(&lDesc1);
  AddChild(&lDesc2);
  AddChild(&lDesc3);
  AddChild(&cJoin);
  AddChild(&bStartIt);
  AddChild(&bCancel);

  Default();
}

void scriptchCB(GUI_LabelL* l)
{
//  NGMenu->Board3.Hide();
//  NGMenu->Board3.Show();
//  NGMenu->Board3.Show();
  NGMenu->Board3.Redraw();

  if(l == NULL) {
		return;
  }

  if (l->msi)
  {
    char a[300];
    sprintf(a, " File: %s.msc, Author: %s", NGMenu->selected2->msi->name, NGMenu->selected2->msi->author);
    NGMenu->lAuthFile.SetText(a);
    sprintf(a, " %s", NGMenu->selected2->msi->desc[0]);
    NGMenu->lDesc1.SetText(a);
    sprintf(a, " %s", NGMenu->selected2->msi->desc[1]);
    NGMenu->lDesc2.SetText(a);
    sprintf(a, " %s", NGMenu->selected2->msi->desc[2]);
    NGMenu->lDesc3.SetText(a);
  }
}


void mapchCB(GUI_LabelL* l)
{
  NGMenu->GenerateScriptSelection();
  NGMenu->WidgetList2.Hide();
  NGMenu->WidgetList2.Show();
}

void GUI_NewGameMenu::GenerateScriptSelection()
{
  WidgetList2.DeleteAll();
  CSMapInfo* a;
  SMapMan.Scan(script_dir.string, smap_ext.string);
  a = SMapMan.Scripts;
  int i=0;
  while (a) {
    i++;
    if (selected1 && strcmp(selected1->GetText(), a->map)==0)
      WidgetList2.AddWidget(new GUI_LabelL(NULL, SDLWidget::mkrect(0,0,150-14,12), a->sname, &selected2, a, GUI_UnselectedItem, GUI_SelectedItem, scriptchCB));
    a = a->next;
  }

  if (WidgetList2.GetWidgetCount())
    selected2 = (GUI_LabelL*)WidgetList2.FindWidget(0);
  else
    selected2 = NULL;
  
  Board2.Redraw();
  WidgetList2.Redraw();
  scriptchCB(selected2);
}

void GUI_NewGameMenu::Default()
{
  eServerName.SetText(s_name.string);
  eMaxClients.SetText(s_maxclients.string);
  eWelcomeMsg.SetText(s_welcome_msg.string);

  WidgetList1.DeleteAll();
  MapMan.Scan(map_dir.string, map_ext.string);
  CMapInfo* a;
  a = MapMan.Maps;
  int i=0;
  while (a) {
    i++;
		WidgetList1.AddWidget(new GUI_LabelL(NULL, SDLWidget::mkrect(0,0,150-14,12), a->name, &selected1, 0, GUI_UnselectedItem, GUI_SelectedItem, mapchCB));
    a = a->next;
  }

  if (WidgetList1.GetWidgetCount())
    selected1 = (GUI_LabelL*)WidgetList1.FindWidget(0);
  else
    selected1 = NULL;
  GenerateScriptSelection();
}

bool GUI_NewGameMenu::eventButtonClick(int id, SDLWidget* widget)
{
  switch (id) {
  case 1:
    if (selected1 && selected2)
    {
      ConOut("");
      ConOut("> NewGame menu sequence <");
      if (net_server_status)
      {
        ConOut("es");
        CommandExecute("es");
      }
    
      ConOut("s_name %s", eServerName.GetText());
      CommandExecute("s_name %s", eServerName.GetText());
      ConOut("s_welcome_msg %s", eWelcomeMsg.GetText());
      CommandExecute("s_welcome_msg %s", eWelcomeMsg.GetText());
      ConOut("s_maxclients %s", eMaxClients.GetText());
      CommandExecute("s_maxclients %s", eMaxClients.GetText());
      ConOut("ss");
      CommandExecute("ss");
      ConOut("s_next_map %s", selected1->GetText());
      CommandExecute("s_next_map %s", selected1->GetText());
      ConOut("s_next_script %s", selected2->msi->name);
      CommandExecute("s_next_script %s", selected2->msi->name);
      ConOut("rs");
      CommandExecute("rs");
      ConOut("> end of sequence <");
      ConOut("");
      
      if (cJoin.GetPressed())
      {
        Cvar_Set("last_server", "localhost");
        JGMenu->Default();
        JGMenu->eventButtonClick(1, NULL);
        GUI_OpenMenu(GUI_MAINMENU);
        GUI_OpenMenu(GUI_CREATEPLAYER);
      }
      else
        while (GUI_id!=GUI_MAINMENU) GUI_Return(); // return from all menus
    }
    else
    {
      if (!selected1)
      {
        OKD1->Reset("MAP REQUIRED", "Map name must be selected !");
        GUI_OpenMenu(GUI_OKDIALOG1);
        return false;
      }

      if (!selected2)
      {
        OKD1->Reset("SCRIPT REQUIRED", "Script name must be selected !");
        GUI_OpenMenu(GUI_OKDIALOG1);
        return false;
      }
    }

    break;
  case 2:
    Return();
    break;
  }
  return true;
}

void GUI_NewGameMenu::Show()
{
  Clear();
  
  Board.Show();  
  bStartIt.Show();  
  bCancel.Show();  
  eServerName.Show();  
  lServerName.Show();  
  eWelcomeMsg.Show();  
  lWelcomeMsg.Show();  
  eMaxClients.Show();  
  lMaxClients.Show();  
  lIP.Show();  
  lHost.Show();  
  lChoose1.Show();
  Board1.Show();
  WidgetList1.Show();
  lChoose2.Show();
  Board2.Show();
  WidgetList2.Show();
  NewGameMenu.Show();
  cJoin.Show();
  Board3.Show();
  lAuthFile.Show();
  lDesc1.Show();
  lDesc2.Show();
  lDesc3.Show();
}

void GUI_NewGameMenu::Hide()
{
  Board.Hide();  
  bStartIt.Hide();  
  bCancel.Hide();  
  eServerName.Hide();  
  lServerName.Hide();  
  eWelcomeMsg.Hide();  
  lWelcomeMsg.Hide();  
  eMaxClients.Hide();  
  lMaxClients.Hide();  
  lIP.Hide();  
  lHost.Hide();  
  lChoose1.Hide();
  Board1.Hide();
  WidgetList1.Hide();
  lChoose2.Hide();
  Board2.Hide();
  WidgetList2.Hide();
  NewGameMenu.Hide();
  cJoin.Hide();
  Board3.Hide();
  lAuthFile.Hide();
  lDesc1.Hide();
  lDesc2.Hide();
  lDesc3.Hide();
}

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
  
  AddChild(&Board1);
  AddChild(&DisconnectMenu);
  AddChild(&lPrompt1);
  AddChild(&lPrompt2);
  AddChild(&bOK);
  AddChild(&bCancel);
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
#define CM_PX 90
#define CM_PY 30
#define CM_VX 460
#define CM_VY 420


class GUI_CreditsMenu : public GUI_BaseMenu {
public:
  GUI_CreditsMenu();
  
  GUI_Board Board1;
  GUI_Bitmap mBack1;
  GUI_LabelC CreditsMenu;
  
  GUI_LabelC lLine1;
  GUI_LabelC lLine2;
  GUI_LabelC lLine3;
  GUI_LabelC lLine4;
  GUI_LabelC lLine5;
  GUI_LabelC lLine6;
  GUI_LabelC lLine7;
  GUI_LabelC lLine8;
  GUI_LabelC lLine9;
  GUI_LabelC lLine10;
  GUI_LabelC lLine11;
  GUI_LabelC lLine12;
  GUI_LabelC lLine13;
  GUI_LabelC lLine14;
  GUI_LabelC lLine15;
  GUI_LabelC lLine16;
  GUI_LabelC lLine17;
  GUI_LabelC lLine18;
  GUI_LabelC lLine19;
  GUI_LabelC lLine20;

  GUI_ButtonSmall bOK;
  
  void Show();
  void Hide();
  void Default();
  
  bool eventButtonClick(int id, SDLWidget* widget);
};

#define CM_G1_PX (CM_PX+8)
#define CM_G1_PY (CM_PY)
#define CM_G1_VX (CM_VX/2)

#define CM_G1X_PX (CM_PX+52)
#define CM_G1X_PY (CM_PY)


#define CM_G2_PX (CM_PX+10+CM_VX/2+10)
#define CM_G2_PY (CM_PY-120)
#define CM_G2_VX (CM_VX/2)


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
  
  AddChild(&Board1);
  AddChild(&mBack1);
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
#define OM_PX 180
#define OM_PY 100
#define OM_VX 280
#define OM_VY 280


class GUI_OptionsMenu : public GUI_BaseMenu {
public:
  GUI_OptionsMenu();
  
  GUI_ButtonBig lOptionsMenu;
  GUI_ButtonBig bServer;
  GUI_ButtonBig bClient;
  GUI_ButtonBig bVideo;
  GUI_ButtonBig bSound;
  GUI_ButtonBig bPaths;
  GUI_ButtonBig bBack;
  
  void Show();
  void Hide();
  void Default();
  
  bool eventButtonClick(int id, SDLWidget* widget);
};

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
#define PM_PX 140
#define PM_PY 120
#define PM_VX 360
#define PM_VY 245


class GUI_PathsMenu : public GUI_BaseMenu {
public:
  GUI_PathsMenu();
  
  GUI_Board Board1;
  GUI_Label lPathsMenu;
  GUI_Label lEffect;
  
  GUI_Label lScriptDir;
  GUI_TextEdit eScriptDir;
  GUI_Label lSpriteDir;
  GUI_TextEdit eSpriteDir;
  GUI_Label lMapDir;
  GUI_TextEdit eMapDir;
  GUI_Label lSndDir;
  GUI_TextEdit eSndDir;
  GUI_Label lSkinDir;
  GUI_TextEdit eSkinDir;
  GUI_Label lGuiDir;
  GUI_TextEdit eGuiDir;
  GUI_Label lGfxDir;
  GUI_TextEdit eGfxDir;
  
  GUI_ButtonSmall bChange;
  GUI_ButtonSmall bCancel;
  
  void Show();
  void Hide();
  void Default();
  void Return();
  
  bool eventButtonClick(int id, SDLWidget* widget);
};

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

#define PM_BASE 65

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
  
  AddChild(&Board1);
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
// CreatePlayer menu widgets
/////////////////////////////////////////////////////////////////////////////
#define CP_PX 120
#define CP_PY 90
#define CP_VX 400
#define CP_VY 270

#define CP_SK_X    (CP_PX+CP_VX-34-10)
#define CP_SK_Y    (CP_PY+40)

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


class GUI_SkinCtrl : public SDLWidget {
public:
  GUI_SkinCtrl(SDLWidget* parent, SDL_Rect& r, bool storebackground = false);
  
  void eventDraw(SDL_Surface* surface, SDL_Rect* rect);
  bool eventButtonClick(int id, SDLWidget* widget);
  
  GUI_SkinPic SkinPic;
  SDLButton bNext;
  SDLButton bPrev;
  
  SDL_Color c;
  bool drawbackground;
};

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


class GUI_CreatePlayerMenu : public GUI_BaseMenu {
public:
  GUI_CreatePlayerMenu(int id, int inum);
  int num;

  GUI_Board Board1;
  GUI_Label CreatePlayerMenu;
  
  GUI_Label lPName;
  GUI_TextEdit ePName;
  GUI_Label lSkin;
  GUI_TextEdit eSkin;
  GUI_SkinCtrl SkinCtrl;
  
  GUI_Input iUp;
  GUI_Input iDown;
  GUI_Input iLeft;
  GUI_Input iRight;
  GUI_Input iFire;
  GUI_Input iWarp;
  GUI_Input iShield;

  GUI_Input iWeapon1;
  GUI_Input iWeapon2;
  GUI_Input iWeapon3;
  GUI_Input iWeapon4;
  GUI_Input iWeapon5;
  
  GUI_Label lUp;
  GUI_Label lDown;
  GUI_Label lLeft;
  GUI_Label lRight;
  GUI_Label lFire;
  GUI_Label lWarp;
  GUI_Label lShield;
  
  GUI_Label lWeapon1;
  GUI_Label lWeapon2;
  GUI_Label lWeapon3;
  GUI_Label lWeapon4;
  GUI_Label lWeapon5;

  GUI_ButtonSmall bOK;
  GUI_ButtonSmall bCancel;
  
  void Show();
  void Hide();
  void Default();
  void Return();
  
  bool eventButtonClick(int id, SDLWidget* widget);
  bool eventNextInput(int id, SDLWidget* widget);
  bool eventMessage(MSG_MESSAGE* msg);

  void NextBox();
};

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


#define CP_INPUT_X1 (CP_PX + 5)
#define CP_INPUT_Y1 (CP_PY + 90)

#define CP_INPUT_X2 (CP_PX + 170)
#define CP_INPUT_Y2 (CP_PY + 90)

#define CCSX 60

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

  AddChild(&Board1);
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
// Sound menu widgets
/////////////////////////////////////////////////////////////////////////////
#define SM_PX 140
#define SM_PY 110
#define SM_VX 360
#define SM_VY 255


class GUI_SoundMenu : public GUI_BaseMenu {
public:
  GUI_SoundMenu();
  
  GUI_Board Board1;
  GUI_Label SoundMenu;
  GUI_Label lRange;
  
  GUI_Label lMusicVolume;
  GUI_NumEdit eMusicVolume;
  GUI_Label lSoundVolume;
  GUI_NumEdit eSoundVolume;

  GUI_CheckBox cSwap;
  GUI_ButtonSmall b3ds;
  
  GUI_Label l3dDist;
  GUI_FloatEdit e3dDist;
  GUI_Label l3dRoll;
  GUI_FloatEdit e3dRoll;

  GUI_ButtonSmall bSet;
  GUI_ButtonSmall bCancel;
  
  void Show();
  void Hide();
  void Default();
  void Return();
  
  bool eventButtonClick(int id, SDLWidget* widget);
};

void GUI_SoundMenu::Return()
{
  eMusicVolume.EditEnd();
  eSoundVolume.EditEnd();
  GUI_BaseMenu::Return();
}

GUI_SoundMenu::GUI_SoundMenu():
GUI_BaseMenu(GUI_SOUND, mkrect(SM_PX,SM_PY,SM_VX,SM_VY)),
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
{
  Default();
  
  SoundMenu.SetColor(GUI_BtnTextColor, GUI_BtnATextColor);
  SoundMenu.SetFont(MainFont);
  
  SoundMenu.bgmode = 2;
  
  lRange.bgmode = 2;
  lMusicVolume.SetAlignment(SDL_TA_RIGHT); 
  lMusicVolume.shiftx = -4;
  l3dDist.SetAlignment(SDL_TA_RIGHT); 
  l3dDist.shiftx = -4;
  l3dRoll.SetAlignment(SDL_TA_RIGHT); 
  l3dRoll.shiftx = -4;
  lSoundVolume.SetAlignment(SDL_TA_RIGHT); 
  lSoundVolume.shiftx = -4;
  
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
}

void GUI_SoundMenu::Default()
{
  char s[50];
  int i = (int)music_volume.value;
  sprintf(s, "%i", i);
  eMusicVolume.SetText(s);

  i = (int)sound_volume.value;
  sprintf(s, "%i", i);
  eSoundVolume.SetText(s);

  sprintf(s, "%.3g", snd_dist.value);
  e3dDist.SetText(s);

  
  sprintf(s, "%.3g", snd_roll.value);
  e3dRoll.SetText(s);

  if (snd_swap.value) cSwap.SetPressed(); else cSwap.SetUnpressed();
}

bool GUI_SoundMenu::eventButtonClick(int id, SDLWidget* widget)
{
  switch (id) {
  case 1:
    ConOut("");
    ConOut("> Sound menu sequence <");

    ConOut("sound_volume %s", eSoundVolume.GetText());
    CommandExecute("sound_volume %s", eSoundVolume.GetText());
    ConOut("music_volume %s", eMusicVolume.GetText());
    CommandExecute("music_volume %s", eMusicVolume.GetText());

    ConOut("snd_dist %s", e3dDist.GetText());
    CommandExecute("snd_dist %s", e3dDist.GetText());
    ConOut("snd_roll %s", e3dRoll.GetText());
    CommandExecute("snd_roll %s", e3dRoll.GetText());

    if (cSwap.GetPressed())
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

void GUI_SoundMenu::Show()
{
  Clear();
  
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
}

void GUI_SoundMenu::Hide()
{
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
}

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

  AddChild(&Board1);
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
  AddChild(&bPrev);
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
  AddChild(&bNext);
  AddChild(&bPrev);
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

  
  AddChild(&Board1);
  AddChild(&Board2);
  AddChild(&DeletePlayerMenu);
  AddChild(&bDelete);
  AddChild(&bCancel);
  AddChild(&lChoose);
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
#define VM_PX 140
#define VM_PY 110
#define VM_VX 360
#define VM_VY 270


class GUI_VideoMenu : public GUI_BaseMenu {
public:
  GUI_VideoMenu();
  
  GUI_Board Board1;
  GUI_Label VideoMenu;
  GUI_Label lEffect;

  GUI_CheckBox cFullscreen;
  GUI_CheckBox cDB;
  GUI_CheckBox cGLBlit;
  GUI_CheckBox cAlphaMenu;

  GUI_Label lGamma_r;
  GUI_FloatEdit eGamma_r;
  GUI_Label lGamma_g;
  GUI_FloatEdit eGamma_g;
  GUI_Label lGamma_b;
  GUI_FloatEdit eGamma_b;
  
  GUI_ButtonSmall bSet;
  GUI_ButtonSmall bCancel;
  
  void Show();
  void Hide();
  void Default();
  void Return();
  
  bool eventButtonClick(int id, SDLWidget* widget);
};

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
  
  AddChild(&Board1);
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
#define SMS_PX 140
#define SMS_PY 105
#define SMS_VX 360
#define SMS_VY 270


class GUI_ServerMenu : public GUI_BaseMenu {
public:
  GUI_ServerMenu();
  
  GUI_Board Board1;
  GUI_Label ServerMenu;
  
  GUI_Label lServerName;
  GUI_TextEdit eServerName;
  GUI_Label lWelcomeMsg;
  GUI_TextEdit eWelcomeMsg;
  GUI_Label lMaxClients;
  GUI_NumEdit eMaxClients;

  GUI_Label lTimelimit;
  GUI_NumEdit eTimelimit;

  GUI_CheckBox cDelTmps;
  GUI_CheckBox cRDRqs;
  GUI_CheckBox cRURqs;

  GUI_ButtonSmall bSet;
  GUI_ButtonSmall bCancel;

  GUI_Label lEffect;

  void Show();
  void Hide();
  void Default();
  void Return();
  
  bool eventButtonClick(int id, SDLWidget* widget);
};

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
Board1(NULL, SDLWidget::mkrect(SMS_PX,SMS_PY,SMS_VX, SMS_VY), false),
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
bCancel(NULL, 2, SDLWidget::mkrect(SMS_PX+25+150+10,SMS_PY+240,150,25), "BACK")
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

  AddChild(&Board1);
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
#define CMS_PX 140
#define CMS_PY 130
#define CMS_VX 360
#define CMS_VY 220


class GUI_ClientMenu : public GUI_BaseMenu {
public:
  GUI_ClientMenu();
  
  GUI_Board Board1;
  GUI_Label ClientMenu;
  
  GUI_Label lClientName;
  GUI_TextEdit eClientName;
  GUI_Label lClientDesc;
  GUI_TextEdit eClientDesc;

  GUI_CheckBox cDelTmps;
  GUI_CheckBox cDRqs;
  GUI_CheckBox cURqs;

  GUI_Label lEffect;

  GUI_ButtonSmall bSet;
  GUI_ButtonSmall bCancel;
  
  void Show();
  void Hide();
  void Default();
  void Return();
  
  bool eventButtonClick(int id, SDLWidget* widget);
};

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

  AddChild(&Board1);
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
