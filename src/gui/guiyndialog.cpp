
#include "guiyndialog.h"
#include "gui.h"
#include "SDL_Console.h"
#include "client.h"

extern void App_Quit();
extern void CEndServer(char *string);


/////////////////////////////////////////////////////////////////////////////
// YN dialog widgets
/////////////////////////////////////////////////////////////////////////////

void GUI_YNDialog::Reset(void (*cb)(int res), char* title, char* line1, char* tla, char* tlb)
{
  YNDialog->SetText(title);
  lPrompt1->SetText(line1);
  bA->SetText(tla);
  bB->SetText(tlb);
  callback = cb;
}

GUI_YNDialog::GUI_YNDialog() : GUI_BaseMenu(GUI_YNDIALOG, PG_Rect(YN_PX,YN_PY,YN_VX,YN_VY)) {

	Default();

	YNDialog = new GUI_Label(this, PG_Rect(1,4,YN_VX-2,25), "", false);
	lPrompt1 = new GUI_Label(this, PG_Rect(1,35, YN_VX-2,20), "", false);
	bA = new GUI_ButtonSmall(this, 1, PG_Rect(25,65,150,25), "YES");
	bB = new GUI_ButtonSmall(this, 2, PG_Rect(25+150+10,65,150,25), "NO");

	YNDialog->SetFont(MainFont);

	lPrompt1->SetAlignment(PG_TA_CENTER);

	LoadThemeStyle("GUI_Board");
}

void GUI_YNDialog::Default()
{
}

bool GUI_YNDialog::eventButtonClick(int id, PG_Widget* widget)
{
  switch (id) {
  case 1:
    callback(1);
    break;
  case 2:
    result = 0;
    callback(0);
    break;
  }
  return true;
}

/////////////////////////////////////////////////////////////////////////////
// YN Callbacks
/////////////////////////////////////////////////////////////////////////////

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
    ConOut("");
    ConOutEx(MISC_FONT, "> Shutdown sequence <");
    CommandExecuteOut("es");
    ConOutEx(MISC_FONT, "> end of sequence <");
    ConOut("");
  }
  while (GUI_id) GUI_Return();
  GUI_OpenMenu(GUI_MAINMENU);
}

void DisconnectCB(int res)
{
  if (res && net_client_status)
  {
    ConOut("");
    ConOutEx(MISC_FONT, "> Disconnect sequence <");
    CommandExecuteOut("disconnect");
    ConOutEx(MISC_FONT, "> end of sequence <");
    ConOut("");
  }
  while (GUI_id) GUI_Return();
  GUI_OpenMenu(GUI_MAINMENU);
}


void EndGameCB(int res)
{
  if (res) CEndServer("");
  GUI_Return();
}

