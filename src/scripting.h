/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   PacWars2 top scripting layer
//## author:  Antonin Hildebrand
//## 
//## started: 2.3.2000
//## revised: 7.3.2000
//## 
//## PacScript is based on SeeR:
//##
//##        -= SeeR - C-Scripting engine v 0.94a =-
//## State      : under implementation
//## Author     : Przemyslaw Podsiadly
//##              email: ppodsiad@elka.pw.edu.pl
//##              WWW:   http://home.elka.pw.edu.pl/~ppodsiad/seer/
//## 
//###########################################################################

#ifndef _SCRIPTING_H_
#define _SCRIPTING_H_

#include "const.h"
#include "seer/seer.h"

#define SCS_UNUSED      0
#define SCS_COMPILED    1
#define SCS_INSTANCED   2

#define SCM_OK          0

// This is simple script wrapper class
class DECLSPEC ScriptDesc {
public:
  int state;
  char name[50];
  bool verbose;
  
  scScript scrpt; //scrpt will contain the compiled script
  scInstance *prog; //it contains all the data from script.

  ScriptDesc() { verbose=true; state = SCS_UNUSED; };
  ScriptDesc(char *filename, char* desc);
  ~ScriptDesc();
  
  int CompileScript(char *filename, char* ext);
  int InstanceScript();
  int RunScript(char *funcname);
  int VRunScript(char *funcname, ...);
  int DoneInstance();
  int DoneScript();
};

// This is script manager class DECLSPEC 
//  + selecting script
//  + scripts debug support
//
class DECLSPEC ScriptManager {
public:  
  int state;
  ScriptDesc scripts[MAX_SCRIPTS];
  int selected; // index of active/selected script
  bool verbose;
  
  ScriptManager() { verbose=true; state = SCM_OK; selected =0;};
  ~ScriptManager();

  int FindScript(char *name);
  int NewScript(char *name, char* ext);
  int DeleteScript(char *name);
  int DeleteScript(int num);
  int InstanceScript(char *name);
  int InstanceScript(int num);
  int RunScript(char *name, char *funcname);
  int RunScript(int num, char *funcname);
  int DoneInstanceScript(char *name);
  int DoneInstanceScript(int num);

  int SetActiveScript(char *name);
  int SetActiveScript(int num);
  int GetActiveScript(char **name);

  // diagnostic
  int ListScripts();
  
  // debug
  int ParamTest(char *name, char* var, int* adr);
  
  int Get_data(char *name, char* var, void** value);
  int Set_data(char *name, char* var, void* value, int length);
  
};

// main global functions
extern DECLSPEC int InitScripting();
extern DECLSPEC int DoneScripting();

extern DECLSPEC ScriptManager *ScriptMan;

#endif
