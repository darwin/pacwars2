#ifndef GUIRESULTSMENU_H
#define GUIRESULTSMENU_H

#include "guibasemenu.h"
#include "cvar.h"

#define RM_MAX_NAMES 20
#define RM_BASE 65

class GUI_Label;

class GUI_ResultsMenu : public GUI_BaseMenu {
public:
  int index;

  GUI_ResultsMenu();
  
  GUI_Label* lResultsMenu;
  GUI_Label* lLabel1;

  GUI_Label* lNameTitle;
  GUI_Label* lFragsTitle;
  GUI_Label* lPointsTitle;
  
  GUI_Label* lName[RM_MAX_NAMES];
  GUI_Label* lPoints[RM_MAX_NAMES];
  GUI_Label* lFrags[RM_MAX_NAMES];
  
  void Default();
  void Return();

protected:  
  void eventShow();
  void eventHide();
};

#endif	// GUIRESULTSMENU_H
