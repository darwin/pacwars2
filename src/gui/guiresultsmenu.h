#ifndef GUIRESULTSMENU_H
#define GUIRESULTSMENU_H

#include "guibasemenu.h"
#include "pglabel.h"

#include "cvar.h"

#define RM_MAX_NAMES 20
#define RM_BASE 65

class GUI_ResultsMenu : public GUI_BaseMenu {
public:
  int index;

  GUI_ResultsMenu();
  
  PG_Label* lResultsMenu;
  PG_Label* lLabel1;

  PG_Label* lNameTitle;
  PG_Label* lFragsTitle;
  PG_Label* lPointsTitle;
  
  PG_Label* lName[RM_MAX_NAMES];
  PG_Label* lPoints[RM_MAX_NAMES];
  PG_Label* lFrags[RM_MAX_NAMES];
  
  void Default();
  void Return();

protected:  
  void eventShow();
  void eventHide();
};

#endif	// GUIRESULTSMENU_H
