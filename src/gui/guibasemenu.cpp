#include "guibasemenu.h"

#include "sdlapplication.h"
#include "pac_player.h"
#include "mouse.h"
#include "gui.h"

/////////////////////////////////////////////////////////////////////////////
// Base menu widgets
/////////////////////////////////////////////////////////////////////////////

GUI_BaseMenu::GUI_BaseMenu(int iid, SDL_Rect& rect): SDLGradientWidget(NULL, rect) {
	mid = iid;
	next = NULL;
	LoadThemeStyle("GUI_BaseMenu");
}

void GUI_BaseMenu::eventDraw(SDL_Surface* surface, SDL_Rect* rect) {
	SDLGradientWidget::eventDraw(surface, rect);
	//SDL_FillRect(surface, rect, 0);
	//SDLWidget::DrawBorder(surface, rect, 1, false);
}

void GUI_BaseMenu::Clear()
{
  //SDL_FillRect(SDLApplication::GetScreen(), &my_rectDisplay, 0);
}

void GUI_BaseMenu::Return()
{
  Hide();
  blocked_inputs = 0;
  HideMouse();
  //SDL_FillRect(SDLApplication::GetScreen(), &my_rectDisplay, 0);
  if (parentmid!=-1) GUI_Show(parentmid);

  smPlaySample(SM_MENU3, menu_volume);
}
