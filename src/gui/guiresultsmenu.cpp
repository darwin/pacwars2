#include "guiresultsmenu.h"
#include "gui.h"
#include "client.h"
#include "pw2.h"

/////////////////////////////////////////////////////////////////////////////
// Results menu widgets
/////////////////////////////////////////////////////////////////////////////

GUI_ResultsMenu::GUI_ResultsMenu(): GUI_BaseMenu(GUI_RESULTS, mkrect(RM_PX,RM_PY,RM_VX,RM_VY)) 
{
  int px, py;
  lResultsMenu = new GUI_Label(this, SDLWidget::mkrect(1,4,RM_VX-2,25), "RESULTS", false);
  lLabel1 = new GUI_Label(this, SDLWidget::mkrect(1,30,RM_VX-2,20), "Press SPACE to continue", false);
  px = 0; py = 59;
  lNameTitle = new GUI_Label(this, SDLWidget::mkrect(px, py, RM_VX/2 + 20, 20), "NAME", false, "GUI_ResultTitle");
  lFragsTitle = new GUI_Label(this, SDLWidget::mkrect(px + 20 + RM_VX/2, py, 60, 20), "FRAGS", false, "GUI_ResultTitle");
  lPointsTitle = new GUI_Label(this, SDLWidget::mkrect(px + 20+60 + RM_VX/2, py, RM_VX/2-60 - 20, 20), "POINTS", false, "GUI_ResultTitle");
  
  lResultsMenu->SetFont(MainFont);
  
  px = 0;
  py = 59+19;
  for (int i=0; i<RM_MAX_NAMES; i++)
  {
    lName[i] = new GUI_Label(this, SDLWidget::mkrect(px, py + i*19, RM_VX/2 + 20, 20), "", false, "GUI_ResultLine");
    lName[i]->SetAlignment(SDL_TA_LEFT);
    lFrags[i] = new GUI_Label(this, SDLWidget::mkrect(px + 20 + RM_VX/2, py + i*19, 60, 20), "", false, "GUI_ResultLine");
    lFrags[i]->SetAlignment(SDL_TA_LEFT);
    lPoints[i] = new GUI_Label(this, SDLWidget::mkrect(px + 20 + 60 + RM_VX/2, py + i*19, RM_VX/2-60 - 20, 20), "", false, "GUI_ResultLine");
    lPoints[i]->SetAlignment(SDL_TA_LEFT);
  }

  Default();
  
  LoadThemeStyle("GUI_Results");
}

void GUI_ResultsMenu::Default()
{
  CGame *g = &client_info.game;
  index = 0;
  int i=0;
  int sort[RM_MAX_NAMES]; 
  int slots[RM_MAX_NAMES];
  for (int f=0; f<RM_MAX_NAMES; f++) { sort[f]=-100000; slots[f] = -1; }

  for (i=0; i<MAX_GAMEBAR_PLAYERS; i++)
  {
    if (g->GBSlots[i]==GAME_MAX_OBJS) break;
    if (g->objs[g->GBSlots[i]]->GetType()==ot_player)
    {
      GPlayer *p = (GPlayer*) g->objs[g->GBSlots[i]];
      if (index<RM_MAX_NAMES)
      {
        lName[index]->SetTextFormat("  %-10s", p->player_name.GetValRef()->chars);
        lFrags[index]->SetTextFormat("  %3d", (int)*p->frags.GetValRef());
        lPoints[index]->SetTextFormat("  %4d", (int)*p->points.GetValRef());
        sort[index] = p->points;
        slots[index] = g->GBSlots[i];
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
      if (sort[i]<sort[i+1])
      {
        strcpy(str, lName[i+1]->GetText());
        lName[i+1]->SetText(lName[i]->GetText());
        lName[i]->SetText(str);
        strcpy(str, lFrags[i+1]->GetText());
        lFrags[i+1]->SetText(lFrags[i]->GetText());
        lFrags[i]->SetText(str);
        strcpy(str, lPoints[i+1]->GetText());
        lPoints[i+1]->SetText(lPoints[i]->GetText());
        lPoints[i]->SetText(str);
        int x;
        x = sort[i+1];
        sort[i+1] = sort[i];
        sort[i] = x;
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
}

void GUI_ResultsMenu::Return() {
	GUI_BaseMenu::Return();
}

void GUI_ResultsMenu::eventShow() {
  Clear();
  int i=0;

	my_rectDisplay.w = RM_VX;
	my_rectDisplay.h = 59+(1+index)*19+1;
//  SizeWindow(RM_VX, 59+(1+index)*19+1);
  LoadThemeStyle("GUI_Results");

  for (i=0; i<RM_MAX_NAMES; i++)
  {
    lName[i]->Hide();
    lPoints[i]->Hide();
    lFrags[i]->Hide();
  }
  for (i=0; i<index; i++)
  {
    lName[i]->Show();
    lPoints[i]->Show();
    lFrags[i]->Show();
  }
}

void GUI_ResultsMenu::eventHide() {
  for (int i=0; i<index; i++)
  {
    lName[i]->Hide();
    lPoints[i]->Hide();
    lFrags[i]->Hide();
  }
}

