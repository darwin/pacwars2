#include "guideleteplayermenu.h"
#include "gui.h"
#include "client.h"
#include "pw2.h"


/////////////////////////////////////////////////////////////////////////////
// DeletePlayer menu widgets
/////////////////////////////////////////////////////////////////////////////

GUI_DeletePlayerMenu::GUI_DeletePlayerMenu() : GUI_BaseMenu(GUI_DELETEPLAYER, PG_Rect(DPM_PX,DPM_PY,DPM_VX,DPM_VY)) {

  DeletePlayerMenu = new GUI_Label(this, PG_Rect(1,4,DPM_VX-2,25), "DELETE PLAYER", false);
  lChoose = new GUI_Label(this, PG_Rect(25,40, 159,20), "Choose player name to delete:", false);
  WidgetList = new PG_WidgetList(this, PG_Rect(26+150+10, 40, 148, 64));
  bDelete = new GUI_ButtonSmall(this, 1, PG_Rect(25,120,150,25), "DELETE");
  bCancel = new GUI_ButtonSmall(this, 2, PG_Rect(25+150+10,120,150,25), "BACK");

  Default();

  DeletePlayerMenu->SetFont(MainFont);

  LoadThemeStyle("GUI_Board");
}

void GUI_DeletePlayerMenu::Default()
{
  GPlayer* p;
  CGame& g = client_info.game;
  GAME_MAXOBJS_TYPE i;

  WidgetList->DeleteAll();
  int id = 0;
  for (i=0; i<GAME_MAX_OBJS; i++)
  {
    if ((g.objs[i]->state&OSTATE_ACTIVE) && (g.objs[i]->GetType()==ot_player))
    {
      p = (GPlayer*)g.objs[i];
      if (p->brain_owner==client_info.client_num)
      {
        id++;
		    WidgetList->AddWidget(new GUI_LabelL(NULL, PG_Rect(0,0,150-14,16), p->player_name.GetValRef()->chars, &selected1, 0, GUI_UnselectedItem, GUI_SelectedItem));
      }
    }
  }
  if (WidgetList->GetWidgetCount())
    selected1 = (GUI_LabelL*)WidgetList->FindWidget(0);
  else
    selected1 = NULL;
}

bool GUI_DeletePlayerMenu::eventButtonClick(int id, PG_Widget* widget) {
  switch (id) {
  case 1:
    if (selected1)
    {
      ConOut("");
      ConOutEx(MISC_FONT, "> DeletePlayer menu sequence <");
      CommandExecuteOut("dp %s", selected1->GetText());
      ConOutEx(MISC_FONT, "> end of sequence <");
      ConOut("");
    }
    Return();
    break;
  case 2:
    Return();
    break;
  }
  return true;
}

void GUI_DeletePlayerMenu::Return() {
	GUI_BaseMenu::Return();
}

void GUI_DeletePlayerMenu::eventShow() {
  Clear();
}

void GUI_DeletePlayerMenu::eventHide() {
}

