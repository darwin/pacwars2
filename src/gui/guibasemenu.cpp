#include "guibasemenu.h"

#include "pgapplication.h"
#include "pac_player.h"
#include "mouse.h"
#include "gui.h"

/////////////////////////////////////////////////////////////////////////////
// Base menu widgets
/////////////////////////////////////////////////////////////////////////////

GUI_BaseMenu::GUI_BaseMenu(int iid, const PG_Rect& rect): PG_GradientWidget(NULL, rect) {
	mid = iid;
	next = NULL;
	LoadThemeStyle("GUI_BaseMenu");
	SetStoreBackground(false);
}

void GUI_BaseMenu::eventDraw(SDL_Surface* surface, const PG_Rect& rect) {
	PG_GradientWidget::eventDraw(surface, rect);
	//SDL_FillRect(surface, rect, 0);
	//PG_Widget::DrawBorder(surface, rect, 1, false);
}

void GUI_BaseMenu::Clear()
{
  //SDL_FillRect(PG_Application::GetScreen(), &my_rectDisplay, 0);
}

void GUI_BaseMenu::Return()
{
  Hide();
  blocked_inputs = 0;
  HideMouse();
  //SDL_FillRect(PG_Application::GetScreen(), &my_rectDisplay, 0);
  if (parentmid!=-1) GUI_Show(parentmid);

  smPlaySample(SM_MENU3, menu_volume);
}
