#ifndef GUIYNDIALOG_H
#define GUIYNDIALOG_H

#include "guibasemenu.h"

class GUI_Label;
class GUI_ButtonSmall;

class GUI_YNDialog : public GUI_BaseMenu {
public:
  GUI_YNDialog();

  void Reset(void (*cb)(int res), char* title, char* line1="", char* tla="YES", char* tlb="NO");

  GUI_Label* YNDialog;
  GUI_Label* lPrompt1;

  GUI_ButtonSmall* bA;
  GUI_ButtonSmall* bB;

  int result;
  void (*callback)(int res);

  void Default();

  bool eventButtonClick(int id, SDLWidget* widget);
};

#endif	// GUIYNDIALOG_H