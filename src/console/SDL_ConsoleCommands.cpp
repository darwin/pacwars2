/*  SDL_Console.c
 *  Written By: Garrett Banuk <mongoose@wpi.edu>
 *  This is free, just be sure to give me credit when using it
 *  in any of your programs.
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SDL.h"
#include "SDL_Console.h"
#include "SDL_ConsoleCommands.h"
#include "../client.h"
#include "../server.h"
#include "../cvar.h"


/* Linked list of all the commands. */
CommandInfo *Commands = NULL;


/* executes the command passed in from the string */
void CommandExecute(char *Input, ...)
{
	va_list marker;
	char BackStrings[300];

	va_start(marker, Input);
	vsprintf(BackStrings, Input, marker);
	va_end(marker);

	char Command[CHARS_PER_LINE];
	char *RestOfLine;
	CommandInfo *CurrentCommand = Commands;
	Uint8 buffer[CHARS_PER_LINE];

	/* Get the command out of the string */
	if (EOF == sscanf(BackStrings, "%s", Command))
		return;

	RestOfLine = BackStrings + strlen(Command);
	while (*RestOfLine == ' ')
		RestOfLine++;

	// do we have remote or local command ?
	if (Command[0] != REMOTE_CMD_CHAR) {
		/* Find the command nd execute the function associated with it */
		while (CurrentCommand) {
			if (0 == strcmp(Command, CurrentCommand->CommandWord)) {
				CurrentCommand->CommandCallback(BackStrings +
												strlen(Command) + 1);
				return;
			}
			CurrentCommand = CurrentCommand->NextCommand;
		}
		// Command wasn't found, test if it is cvar
		if (Cvar_Command(Command, RestOfLine)) {	// wasn't cvar
			// assume this is message and send it to others
			if (net_client_status != NS_UNINITED)
				ChatMessage(BackStrings);
		}
	} else						// remote command
	{
		buffer[POS_MSG_ID] = MSGID_REMOTECMD;
		int len =
			snprintf((char *) &buffer[POS_MSG_CMD], LEN_MSG_CMD, "%s",
					 &BackStrings[1]);
		len += POS_MSG_CMD + 1;
		SendMsg(&server.msg_pool, msg_csock, 0, buffer, len, true,
				RESEND_SYSTEM);
	}
}

void ExecuteRemoteCommand(int who, char *BackStrings)
// only aplicable to CMD_SERVER commands !!!
{
	char Command[CHARS_PER_LINE];
	CommandInfo *CurrentCommand = Commands;

	/* Get the command out of the string */
	if (EOF == sscanf(BackStrings, "%s", Command))
		return;

	if (client[who].status == CS_ACTIVE) {
		//! TODO: SECURITY TESTS

		ConOut("[%s] %s", client[who].name, BackStrings);
		/* Find the command nd execute the function associated with it */
		while (CurrentCommand) {
			if ((CurrentCommand->CommandGroup == CMD_SERVER) &&
				(0 == strcmp(Command, CurrentCommand->CommandWord))) {
				CurrentCommand->CommandCallback(BackStrings +
												strlen(Command) + 1);
				return;
			}
			CurrentCommand = CurrentCommand->NextCommand;
		}
		/* Command wasn't found */
		ConOut("[%s] Bad command", client[who].name);
	}
	ConErr("Remote command execution by unconnected client !");
	return;
}


/* Use this to add commands.
 * Pass in a pointer to a funtion that will take a string which contains the 
 * arguments passed to the command. Second parameter is the command to execute
 * on.
 */
void AddCommand(void (*CommandCallback) (char *Parameters),
				const char *CommandWord, char CmdGroup)
{
	CommandInfo **CurrentCommand = &Commands;


	while (*CurrentCommand)
		CurrentCommand = &((*CurrentCommand)->NextCommand);


	/* Add a command to the list */
	*CurrentCommand = (CommandInfo *) malloc(sizeof(CommandInfo));
	(*CurrentCommand)->CommandCallback = CommandCallback;
	(*CurrentCommand)->CommandWord = (char *) malloc(strlen(CommandWord) + 1);
	(*CurrentCommand)->CommandGroup = CmdGroup;
	strcpy((*CurrentCommand)->CommandWord, CommandWord);
	(*CurrentCommand)->NextCommand = NULL;


}

/* tab completes commands, not perfect finds the first matching command */
void TabCompletion(char *CommandLine, int *location)
{
	CommandInfo *CurrentCommand = Commands;


	if (CommandLine[0] != REMOTE_CMD_CHAR)
		while (CurrentCommand) {
			if (0 ==
				strncmp(CommandLine, CurrentCommand->CommandWord,
						strlen(CommandLine))) {
				strcpy(CommandLine, CurrentCommand->CommandWord);
				CommandLine[strlen(CurrentCommand->CommandWord)] = ' ';
				*location = strlen(CurrentCommand->CommandWord) + 1;
				UpdateConsole();
				return;
			}
			CurrentCommand = CurrentCommand->NextCommand;
	} else
		while (CurrentCommand) {
			if ((CurrentCommand->CommandGroup == CMD_SERVER) &&
				(0 ==
				 strncmp(&CommandLine[1], CurrentCommand->CommandWord,
						 strlen(CommandLine) - 1))) {
				strcpy(&CommandLine[1], CurrentCommand->CommandWord);
				CommandLine[strlen(CurrentCommand->CommandWord) + 1] = ' ';
				*location = strlen(CurrentCommand->CommandWord) + 2;
				UpdateConsole();
				return;
			}
			CurrentCommand = CurrentCommand->NextCommand;
		}

	// Command completion wasn't found, 
	// try to complete cvar
	char *cvar;
	cvar = Cvar_CompleteVariable(CommandLine);
	if (cvar) {
		strcpy(CommandLine, cvar);
		CommandLine[strlen(cvar)] = ' ';
		*location = strlen(cvar) + 1;
		UpdateConsole();
		return;
	}
}


/* Lists all the commands to be used in the console */
void ListCommands()
{
	CommandInfo *CurrentCommand = Commands;

	ConOut("Local commands:");
	ConOut("---------------");
	while (CurrentCommand) {
		if (CurrentCommand->CommandGroup == CMD_LOCAL)
			ConOut("%s", CurrentCommand->CommandWord);
		CurrentCommand = CurrentCommand->NextCommand;
	}
	ConOut("Client commands:");
	ConOut("-----------------");
	CurrentCommand = Commands;
	while (CurrentCommand) {
		if (CurrentCommand->CommandGroup == CMD_CLIENT)
			ConOut("%s", CurrentCommand->CommandWord);
		CurrentCommand = CurrentCommand->NextCommand;
	}
	ConOut("Server commands:");
	ConOut("----------------");
	CurrentCommand = Commands;
	while (CurrentCommand) {
		if (CurrentCommand->CommandGroup == CMD_SERVER)
			ConOut("%s", CurrentCommand->CommandWord);
		CurrentCommand = CurrentCommand->NextCommand;
	}
}

bool CommandExists(char *cmd_name)
{
	CommandInfo *CurrentCommand = Commands;

	while (CurrentCommand) {
		if (!strcmp(cmd_name, CurrentCommand->CommandWord))
			return true;
		CurrentCommand = CurrentCommand->NextCommand;
	}
	return false;
}

void FreeCommands()
{
	CommandInfo *CurrentCommand = Commands;
	CommandInfo *del;

	while (CurrentCommand) {
		del = CurrentCommand->NextCommand;
		free(CurrentCommand->CommandWord);

		free(CurrentCommand);
		CurrentCommand = del;
	}
}
