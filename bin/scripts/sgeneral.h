/////////////////////////////////////////////////////////////////////////////
//###########################################################################
//## PacWars 2
//## 
//## desc.:   Scripting Map API
//## author:  Antonin Hildebrand
//## 
//## started: 12.3.2000
//## revised: ???
//## 
//##          exports general functions to scripts
//## 
//###########################################################################

#ifndef _SCRIPT_GENERAL_H_
#define _SCRIPT_GENERAL_H_

// (IMPORTS/EXPORTS)
// MUST BE COMPATIBILE WITH PACSCRIPT

//###########################################################################
//## console output functions
//###########################################################################

import void ConOut(const char *,...);
import void ConPut(int linenum, const char *str, ... );
import void ConPar(const char *str, ... );
import void SetMessenger(const char *str, ... );

import void 	Cvar_RegisterVariable(cvar_t *variable);
// registers a cvar that allready has the name, string, and optionally the
// archive elements set.

import void 	Cvar_Set(char *var_name, char *value, bool callback=true);
// equivelant to "<name> <variable>" typed at the console

import void	Cvar_SetValue(char *var_name, float value);
// expands value to a string and calls Cvar_Set

import float	Cvar_VariableValue(char *var_name);
// returns 0 if not defined or non numeric

import char	*Cvar_VariableString(char *var_name);
// returns an empty string if not defined

import char 	*Cvar_CompleteVariable(char *partial);
// attempts to match a partial variable name for command line completion
// returns NULL if nothing fits

import int	Cvar_Command(char* varname, char* value);
// called by Cmd_ExecuteString when Cmd_Argv(0) doesn't match a known
// command.  Returns true if the command was a variable reference that
// was handled. (print or change)

import void SV_BroadcastSound(Uint8 id, Uint8 volume = 100);
import void SV_ClientSound(int c,Uint8 id, Uint8 volume = 100);
import void SV_BroadcastVoice(Uint8 id, Uint8 volume = 100, Uint8 priority = 1);
import void SV_ClientVoice(int c,Uint8 id, Uint8 volume = 100, Uint8 priority = 1);

import int ChatMessage(char* m); // client side

#endif
