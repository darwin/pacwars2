#ifndef GUIYNDIALOG_H
#define GUIYNDIALOG_H

#include "guibasemenu.h"
#include "pglabel.h"

class GUI_ButtonSmall;

class GUI_YNDialog : public GUI_BaseMenu {
public:
  GUI_YNDialog();

  void Reset(void (*cb)(int res), char* title, char* line1="", char* tla="YES", char* tlb="NO");

  PG_Label* YNDialog;
  PG_Label* lPrompt1;

  GUI_ButtonSmall* bA;
  GUI_ButtonSmall* bB;

  int result;
  void (*callback)(int res);

  void Default();

  bool eventButtonClick(int id, PG_Widget* widget);
};

#endif	// GUIYNDIALOG_H