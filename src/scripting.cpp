/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   PacWars2 top scripting layer
//## author:  Antonin Hildebrand
//## 
//## started: 2.3.2000
//## revised: 25.6.2000
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

//#define import ;
//#define NULL 0

//#include "../pw2bin/scripts/scStdio.h"
//#include "../pw2bin/scripts/scStdlib.h"
//#include "../pw2bin/scripts/scString.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL.h"

#include "const.h"
#include "scripting.h"
#include "console/SDL_Console.h"
#include "console/SDL_DrawText.h"
#include "mapi.h"
#include "oapi.h"
#include "vapi.h"
#include "messenger.h"
#include "server.h"
#include "client.h"


//###########################################################################
//## Globals
//###########################################################################

ScriptManager *ScriptMan = NULL;

//###########################################################################
//## Helpers
//###########################################################################

// function to be called during compilation to display progress bar
int progresscallback(int percent)
{
  static int per=-1;
  percent/=5;
  
  //if (per==percent) return 1;//don't repeat yourself
  per=percent;
  ConPut(1, "Scripting: compilation in progress (%d%% done).", percent);
  return 1;
}

//###########################################################################
//## Script desc
//## 
//## Wrapper class for script 
//## 
//###########################################################################

ScriptDesc::ScriptDesc(char *filename, char* ext)
{
  CompileScript(filename, ext);
  verbose = false;
}

ScriptDesc::~ScriptDesc()
{
  DoneScript();
}

int ScriptDesc::CompileScript(char *filename, char* ext = script_ext.string)
{
  char exp[_MAX_PATH];
  int err;
  
  if (state>SCS_UNUSED) 
  {  
    err = DoneScript();
    if (err) return err;
  }
  
  // expand filename 
  FileNameConversion(script_dir.string, filename, ext, exp);
  
  // compile script
  strcpy(name, filename);
  scrpt = scCompile_File(exp);
  if (scErrorNo)
  {
    if (verbose) ConPar("Scripting: %s> %s",scErrorMsg, scErrorLine);
    return scErrorNo;
  }
  
  if (!scrpt)
  {
    if (verbose) ConOut("Scripting: script %s not found !",exp);
    return 1;
  }
  
  if (verbose) ConOut("Scripting: %s loaded and compiled succesfully.", exp);
  state = SCS_COMPILED;
  return 0;   
}

int ScriptDesc::InstanceScript()
{
  int err;
  
  if (state>SCS_COMPILED) 
  {  
    err = DoneInstance();
    if (err) return err;
  }
  
  if (state == SCS_UNUSED)
  {
    if (verbose) ConOut("Scripting: script %s isn't compiled !", name);
    return 1;
  }
  
  // create instance of the script - you can run only instance of script,
  prog = scCreate_Instance(scrpt,"");
  if (!prog)
  {
    if (verbose) ConOut("Scripting: Error creating instance of %s - %s", name, scErrorMsg);
    return scErrorNo;
  }
  
  if (verbose) ConOut("Scripting: instance of %s created", name);
  state = SCS_INSTANCED;
  return 0;
}

int ScriptDesc::RunScript(char *funcname)
{
  int err;
  
  if (state < SCS_INSTANCED) 
  {  
    if (verbose) ConOut("Scripting: script %s isn't instanced !", name);
    return 1;
  }
  
  //  now, let's execute the function "funcname"
  err = scCall_Instance(prog,scGet_Symbol(prog, funcname));
  if (scErrorNo)
  {
    if (verbose) ConOut("Scripting: %s\n",scErrorMsg);
    return scErrorNo;
  }
  
  if (verbose) ConOut("Scripting: %s executed.", name);
  return 0;
}

int ScriptDesc::VRunScript(char *funcname, ...)
{
  int err;
  
  if (state < SCS_INSTANCED) 
  {  
    if (verbose) ConOut("Scripting: script %s isn't instanced !", name);
    return 1;
  }
  
  //  now, let's execute the function "funcname"
  va_list va;
  va_start(va,funcname);
  err = scVCall_Instance(prog,scGet_Symbol(prog, funcname),-1,&(va_arg(va,int)));
  
  if (scErrorNo)
  {
    if (verbose) ConOut("Scripting: %s\n",scErrorMsg);
    return scErrorNo;
  }
  
  if (verbose) ConOut("Scripting: %s executed.", name);
  return 0;
}

int ScriptDesc::DoneInstance()
{
  if (state == SCS_INSTANCED)
  {
    //  let's delete instance
    scFree_Instance(prog);
    state = SCS_COMPILED;
    if (verbose) ConOut("Scripting: instance of script %s was deleted.", name);
    return 0;    
  }
  else
  {
    if (verbose) ConOut("Scripting: script %s isn't instanced !", name);
    return 1;
  }
}

int ScriptDesc::DoneScript()
{
  if (state == SCS_INSTANCED) DoneInstance();
  if (state == SCS_COMPILED)
  {
    free(scrpt);
    state = SCS_UNUSED;
    if (verbose) ConOut("Scripting: script %s was deleted.", name);
    return 0;    
  }
  return 0;
}

//###########################################################################
//## Script manager
//## 
//## script manager controlled threw the console
//## 
//###########################################################################

ScriptManager::~ScriptManager()
{
  // for (int i=0; i<MAX_SCRIPTS; i++)
  //   if (scripts[i].state != SCS_UNUSED) scripts[i].DoneScript();
}

int ScriptManager::FindScript(char *name)
{
  for (int i=0; i<MAX_SCRIPTS; i++)
    if (scripts[i].state != SCS_UNUSED)
      if (strcmp(scripts[i].name, name) == 0)
        return i;
      
      return -1;
}

int ScriptManager::NewScript(char *name, char* ext)
{
  // check for name collisions
  if (FindScript(name)!=-1) DeleteScript(name);
  
  // find unused slot and create&compile script
  for (int i=0; i<MAX_SCRIPTS; i++)
    if (scripts[i].state == SCS_UNUSED)
    {
      if (scripts[i].CompileScript(name, ext)==0)
        return i;
      else
        return -1;
    }
    
    if (verbose) ConOut("Scripting: no more free script slots !");
    return -1;
}

int ScriptManager::DeleteScript(char *name)
{
  return DeleteScript(FindScript(name));
}

int ScriptManager::DeleteScript(int num)
{
  if ((num>=0) && (num<MAX_SCRIPTS))
    return scripts[num].DoneScript();
  else
    return 1;
}

int ScriptManager::InstanceScript(char *name)
{
  return InstanceScript(FindScript(name));
}

int ScriptManager::InstanceScript(int num)
{
  if ((num>=0) && (num<MAX_SCRIPTS))
    return scripts[num].InstanceScript();
  else
    if (verbose) ConOut("Scripting: script not installed !");
    return 1;
}

int ScriptManager::RunScript(char *name, char *funcname)
{
  return RunScript(FindScript(name), funcname);
}

int ScriptManager::RunScript(int num, char *funcname)
{
  if ((num>=0) && (num<MAX_SCRIPTS))
    return scripts[num].RunScript(funcname);
  else
    if (verbose) ConOut("Scripting: script not installed !");
    return 1;
}

int ScriptManager::DoneInstanceScript(char *name)
{
  return DoneInstanceScript(FindScript(name));
}

int ScriptManager::DoneInstanceScript(int num)
{
  if ((num>=0) && (num<MAX_SCRIPTS))
    return scripts[num].DoneInstance();
  else
    if (verbose) ConOut("Scripting: script not installed !");
    return 1;
}

int ScriptManager::ListScripts()
{
  const char *SCS_NAMES[3] = {"[UNUSED]   ", "[COMPILED] ", "[INSTANCED]"};
  char sel[3] = {' ', ' ', 0};
  
  ConOut("");
  ConOut("ScriptManager:");
  ConOut("--------------");
  
  for (int i=0; i<MAX_SCRIPTS; i++)
  {
    // make arrow for selected script
    if (selected == i) 
    { sel[0]='-'; sel[1]='>'; }
    else
    { sel[0]=' '; sel[1]=' '; }
    
    if (scripts[i].state == SCS_UNUSED)
      ConOut("%s%d. %s", sel,  i, SCS_NAMES[scripts[i].state]);
    else
      ConOut("%s%d. %s %s", sel, i, SCS_NAMES[scripts[i].state], scripts[i].name);
  }
  
  ConOut("");
  return 0;
}

int ScriptManager::ParamTest(char *name, char* var, int* adr)
// necessary tests for all Get_() and Set_() functions
{
  int num = FindScript(name);
  
  if ((num<0) || (num>=MAX_SCRIPTS))
  {
    if (verbose) ConOut("Scripting: script not installed !");
    return -1;
  }
  
  if (scripts[num].state != SCS_INSTANCED)
  {
    if (verbose) ConOut("Scripting: script not instanced !");
    return -1;
  }
  
  (*adr) = scGet_Symbol(scripts[num].prog, var);
  if ((*adr)==-1)
  {
    if (verbose) ConOut("Scripting: symbol %s not found in script %s !", var, name);
    return -1;
  }
  
  return num;
}

int ScriptManager::SetActiveScript(char *name)
{
  return SetActiveScript(FindScript(name));
}

int ScriptManager::SetActiveScript(int num)
{
  if ((num<0) || (num>=MAX_SCRIPTS))
  {
    if (verbose) ConOut("Scripting: script not installed !");
    return -1;
  }
  else 
  {
    selected = num;
    if (verbose) ConOut("Scripting: %s selected.", scripts[selected].name);
    return 0;
  }
}

int ScriptManager::GetActiveScript(char **name)
{
  if (scripts[selected].state == SCS_UNUSED) return 1;
  *name = scripts[selected].name;
  return 0;
}

int ScriptManager::Get_data(char *name, char* var, void** value)
{
  int adr;
  int num = ParamTest(name, var, &adr);
  if (num == -1) return 1;
  
  (*value) = scripts[num].prog->data_stack+256*4+adr;
  return 0;
}

int ScriptManager::Set_data(char *name, char* var, void* value, int length)
{
  int adr;
  int num = ParamTest(name, var, &adr);
  if (num == -1) return 1;
  
  memcpy(scripts[num].prog->data_stack+256*4+adr, value, length);
  return 0;
}

int xrand()
{
  return rand();
}

//###########################################################################
//## Global functions
//## 
//## initialization / deinitialization of scripting layer
//## define external symbols for scripting
//## 
//###########################################################################

// This function tells SeeR addresses of functions and variables it can
// use (import). If script imports symbol you don't tell the address SeeR
// will complain : unresolved reference. In this way you also add variables.
void InitExternals()
{

  // general externals
  scAdd_External_Symbol("ConOut",ConOut);
  scAdd_External_Symbol("ConPut",ConPut);
  scAdd_External_Symbol("ConPar",ConPar);
  scAdd_External_Symbol("SetMessenger",SetMessenger);

  scAdd_External_Symbol("Cvar_RegisterVariable", Cvar_RegisterVariable);
  scAdd_External_Symbol("Cvar_Set", Cvar_Set);
  scAdd_External_Symbol("Cvar_SetValue", Cvar_SetValue);
  scAdd_External_Symbol("Cvar_VariableValue", Cvar_VariableValue);

  scAdd_External_Symbol("Cvar_VariableString", Cvar_VariableString);
  scAdd_External_Symbol("Cvar_CompleteVariable", Cvar_CompleteVariable);
  scAdd_External_Symbol("Cvar_Command", Cvar_Command);

  scAdd_External_Symbol("malloc",malloc);
  scAdd_External_Symbol("free",free);
  scAdd_External_Symbol("rand",rand);
  
  scAdd_External_Symbol("SV_BroadcastSound", SV_BroadcastSound);
  scAdd_External_Symbol("SV_ClientSound", SV_ClientSound);
  scAdd_External_Symbol("SV_BroadcastVoice", SV_BroadcastVoice);
  scAdd_External_Symbol("SV_ClientVoice", SV_ClientVoice);
  
  scAdd_External_Symbol("ChatMessage", ChatMessage);


  // mapi externals
  AddMapiExternals();
  // oapi externals
  AddOapiExternals();
  // vapi externals
  AddVapiExternals();

#ifdef _WIN32
#ifdef SCRIPTING_STD_LIBS
  // Stdio.h externals
  scAdd_External_Symbol("_iob", &_iob);
  scAddExtSym(_filbuf);
  scAddExtSym(_flsbuf);
  scAddExtSym(_fsopen);
  
  scAddExtSym(clearerr);
  scAddExtSym(fclose);
  scAddExtSym(_fcloseall);
  
  scAddExtSym(_fdopen);
  
  scAddExtSym(feof);
  scAddExtSym(ferror);
  scAddExtSym(fflush);
  scAddExtSym(fgetc);
  scAddExtSym(_fgetchar);
  scAddExtSym(fgetpos);
  scAddExtSym(fgets);
  
  scAddExtSym(_fileno);
  
  scAddExtSym(_flushall);
  scAddExtSym(fopen);
  scAddExtSym(fprintf);
  scAddExtSym(fputc);
  scAddExtSym(_fputchar);
  scAddExtSym(fputs);
  scAddExtSym(fread);
  scAddExtSym(freopen);
  scAddExtSym(fscanf);
  scAddExtSym(fsetpos);
  scAddExtSym(fseek);
  scAddExtSym(ftell);
  scAddExtSym(fwrite);
  scAddExtSym(getc);
  scAddExtSym(getchar);
  scAddExtSym(_getmaxstdio);
  scAddExtSym(gets);
  scAddExtSym(_getw);
  scAddExtSym(perror);
  scAddExtSym(_pclose);
  scAddExtSym(_popen);
  scAddExtSym(printf);
  scAddExtSym(putc);
  scAddExtSym(putchar);
  scAddExtSym(puts);
  scAddExtSym(_putw);
  scAddExtSym(remove);
  scAddExtSym(rename);
  scAddExtSym(rewind);
  scAddExtSym(_rmtmp);
  scAddExtSym(scanf);
  scAddExtSym(setbuf);
  scAddExtSym(_setmaxstdio);
  scAddExtSym(setvbuf);
  scAddExtSym(_snprintf);
  scAddExtSym(sprintf);
  scAddExtSym(sscanf);
  scAddExtSym(_tempnam);
  scAddExtSym(tmpfile);
  scAddExtSym(tmpnam);
  scAddExtSym(ungetc);
  scAddExtSym(_unlink);
  scAddExtSym(vfprintf);
  scAddExtSym(vprintf);
  scAddExtSym(_vsnprintf);
  scAddExtSym(vsprintf);
  
  scAddExtSym(_wfsopen);
  
  scAddExtSym(fgetwc);
  scAddExtSym(_fgetwchar);
  scAddExtSym(fputwc);
  scAddExtSym(_fputwchar);
  scAddExtSym(getwc);
  scAddExtSym(getwchar);
  scAddExtSym(putwc);
  scAddExtSym(putwchar);
  scAddExtSym(ungetwc);
  
  scAddExtSym(fgetws);
  scAddExtSym(fputws);
  scAddExtSym(_getws);
  scAddExtSym(_putws);
  
  scAddExtSym(fwprintf);
  scAddExtSym(wprintf);
  scAddExtSym(_snwprintf);
  scAddExtSym(swprintf);
  scAddExtSym(vfwprintf);
  scAddExtSym(vwprintf);
  scAddExtSym(_vsnwprintf);
  scAddExtSym(vswprintf);
  scAddExtSym(fwscanf);
  scAddExtSym(swscanf);
  scAddExtSym(wscanf);
  
  scAddExtSym(_wfdopen);
  scAddExtSym(_wfopen);
  scAddExtSym(_wfreopen);
  scAddExtSym(_wperror);
  scAddExtSym(_wpopen);
  scAddExtSym(_wremove);
  scAddExtSym(_wtempnam);
  scAddExtSym(_wtmpnam);
  
  scAddExtSym(fcloseall);
  scAddExtSym(fdopen);
  scAddExtSym(fgetchar);
  scAddExtSym(fileno);
  scAddExtSym(flushall);
  scAddExtSym(fputchar);
  scAddExtSym(getw);
  scAddExtSym(putw);
  scAddExtSym(rmtmp);
  scAddExtSym(tempnam);
  scAddExtSym(unlink);
  
  // Stdlib.h externals
  scAdd_External_Symbol("__mb_cur_max", &__mb_cur_max);
  scAdd_External_Symbol("_sys_errlist", &_sys_errlist);   /* perror error message table */
  scAdd_External_Symbol("_sys_nerr", &_sys_nerr);           /* # of entries in sys_errlist table */
  scAdd_External_Symbol("environ", &environ);                 /* pointer to environment table */
//  scAdd_External_Symbol("_fmode", &_fmode);          /* default file translation mode */
  scAdd_External_Symbol("_fileinfo", &_fileinfo);       /* open file info mode (for spawn) */
  
  scAdd_External_Symbol("_osver", &_osver); 
  scAdd_External_Symbol("_winver", &_winver);
  scAdd_External_Symbol("_winmajor", &_winmajor);
  scAdd_External_Symbol("_winminor", &_winminor);
  
//  scAddExtSym(_errno);
  
/*  scAddExtSym(__p___argc);
  scAddExtSym(__p___argv);
  scAddExtSym(__p___wargv);
  scAddExtSym(__p__environ);
  scAddExtSym(__p__wenviron);
  scAddExtSym(__p__pgmptr);
  scAddExtSym(__p__wpgmptr);
 */
  
  scAddExtSym(abort);
  scAddExtSym(exit);
  
  scAddExtSym(abs);
  scAddExtSym(atof);
  scAddExtSym(atoi);
  
  scAddExtSym(calloc);
  scAddExtSym(div);
  scAddExtSym(free);
  scAddExtSym(getenv);
  scAddExtSym(_itoa);
  // scAddExtSym(_i64toa);
  // scAddExtSym(_ui64toa);
  // scAddExtSym(_atoi64);
  scAddExtSym(malloc);
  scAddExtSym(mblen);
  scAddExtSym(_mbstrlen);
  scAddExtSym(mbtowc);
  scAddExtSym(mbstowcs);
//  scAddExtSym(rand);
  scAddExtSym(realloc);
  scAddExtSym(_set_error_mode);
  scAddExtSym(srand);
  scAddExtSym(strtod);
  scAddExtSym(system);
  scAddExtSym(wctomb);
  scAddExtSym(wcstombs);
  
  scAddExtSym(_itow );
  scAddExtSym(wcstod);
  scAddExtSym(_wgetenv);
  scAddExtSym(_wsystem);
  scAddExtSym(_wtoi);
  // scAddExtSym(_i64tow);
  // scAddExtSym(_ui64tow);
  // scAddExtSym(_wtoi64);
  
  scAddExtSym(_ecvt);
  scAddExtSym(_exit);
  scAddExtSym(_fcvt);
  scAddExtSym(_fullpath);
  scAddExtSym(_gcvt);
  scAddExtSym(_makepath);
  
  scAddExtSym(_putenv);
  scAddExtSym(_rotl);
  scAddExtSym(_rotr);
  scAddExtSym(_searchenv);
  scAddExtSym(_splitpath);
  scAddExtSym(_swab);
  
  scAddExtSym(_wfullpath);
  scAddExtSym(_wmakepath);
  scAddExtSym(_wputenv);
  scAddExtSym(_wsearchenv);
  scAddExtSym(_wsplitpath);
  
  scAddExtSym(_seterrormode);
  scAddExtSym(_beep);
  scAddExtSym(tolower);
  scAddExtSym(toupper);
  
  scAddExtSym(ecvt);
  scAddExtSym(fcvt);
  scAddExtSym(gcvt);
  scAddExtSym(itoa);
  scAddExtSym(onexit);
  scAddExtSym(putenv);
  scAddExtSym(swab);
  
  // String.h externals
  scAddExtSym(memcpy);
  scAddExtSym(memcmp);
  scAddExtSym(memset);
  scAddExtSym(_strset);
  scAddExtSym(strcpy);
  scAddExtSym(strcat);
  scAddExtSym(strcmp);
  scAddExtSym(strlen);
  scAddExtSym(_memccpy);
  scAddExtSym(memchr);
  scAddExtSym(_memicmp);
  
  scAddExtSym(memmove);
  
  scAddExtSym(strchr);
  scAddExtSym(_strcmpi);
  scAddExtSym(_stricmp);
  scAddExtSym(strcoll);
  scAddExtSym(_stricoll);
  scAddExtSym(_strncoll);
  scAddExtSym(_strnicoll);
  scAddExtSym(strcspn);
  scAddExtSym(_strdup);
  scAddExtSym(_strerror);
  scAddExtSym(strerror);
  scAddExtSym(_strlwr);
  scAddExtSym(strncat);
  scAddExtSym(strncmp);
  scAddExtSym(_strnicmp);
  scAddExtSym(strncpy);
  scAddExtSym(_strnset);
  scAddExtSym(strpbrk);
  scAddExtSym(strrchr);
  scAddExtSym(_strrev);
  scAddExtSym(strspn);
  scAddExtSym(strstr);
  scAddExtSym(strtok);
  scAddExtSym(_strupr);
  scAddExtSym(strxfrm );
  
  scAddExtSym(memccpy);
  scAddExtSym(memicmp);
  scAddExtSym(strcmpi);
  scAddExtSym(stricmp);
  scAddExtSym(strdup);
  scAddExtSym(strlwr);
  scAddExtSym(strnicmp);
  scAddExtSym(strnset);
  scAddExtSym(strrev);
  scAddExtSym(strset);
  scAddExtSym(strupr);
  
  scAddExtSym(wcscat);
  scAddExtSym(wcschr);
  scAddExtSym(wcscmp);
  scAddExtSym(wcscpy);
  scAddExtSym(wcscspn);
  scAddExtSym(wcslen);
  scAddExtSym(wcsncat);
  scAddExtSym(wcsncmp);
  scAddExtSym(wcsncpy);
  scAddExtSym(wcspbrk);
  scAddExtSym(wcsrchr);
  scAddExtSym(wcsspn);
  scAddExtSym(wcsstr);
  scAddExtSym(wcstok);
  
  scAddExtSym(_wcsdup);
  scAddExtSym(_wcsicmp);
  scAddExtSym(_wcsnicmp);
  scAddExtSym(_wcsnset);
  scAddExtSym(_wcsrev);
  scAddExtSym(_wcsset);
  
  scAddExtSym(_wcslwr);
  scAddExtSym(_wcsupr);
  scAddExtSym(wcsxfrm);
  scAddExtSym(wcscoll);
  scAddExtSym(_wcsicoll);
  scAddExtSym(_wcsncoll);
  scAddExtSym(_wcsnicoll);
  
  scAddExtSym(wcsdup);
  scAddExtSym(wcsicmp);
  scAddExtSym(wcsnicmp);
  scAddExtSym(wcsnset);
  scAddExtSym(wcsrev);
  scAddExtSym(wcsset);
  scAddExtSym(wcslwr);
  scAddExtSym(wcsupr);
  scAddExtSym(wcsicoll);
#endif
#endif

}


int InitScripting()
{
  if (!ScriptMan)
  {
    scInit_SeeR();

    InitExternals();
    
    // set progreess callback
    scProgressCallback = progresscallback;
    
#ifdef SCRIPT_DEBUG
    // when SeeR is in debug mode, here it will store all debugging data
    scOpen_Debug("scripting.dbg");//start debugging
#endif
    ScriptMan = new ScriptManager;
    
    ConOut("SeeR scripting engine was initialized.");
    //    if (verbose) ConOut("Script manager was born.");
    return 0;
  }
  else 
    return 1;
}

int DoneScripting()
{
  if (ScriptMan)
  {
    delete ScriptMan;
    
    scFreeDictionaries();

#ifdef SCRIPT_DEBUG
    // close debugging - close the debug file also
    scClose_Debug();
#endif
    
    ConOut("SeeR scripting done.");
    return 0;
  }
  else
    return 1;
}