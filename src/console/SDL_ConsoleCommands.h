#ifndef SDL_ConsoleCommands_H
#define SDL_ConsoleCommands_H

// command groups

#define CMD_LOCAL  0
#define CMD_CLIENT 1
#define CMD_SERVER 2

typedef struct CommandInfo_td {
	void (*CommandCallback) (char *Parameters);
	char *CommandWord;

	char CommandGroup;
	struct CommandInfo_td *NextCommand;
} CommandInfo;


void CommandExecute(char *BackStrings, ...);
void CommandExecuteOut(char *Input, ...);
void ExecuteRemoteCommand(int who, char *BackStrings);

void AddCommand(void (*CommandCallback) (char *Parameters),
				const char *CommandWord, char CmdGroup = CMD_LOCAL);
void TabCompletion(char *CommandLine, int *location);
void ListCommands();
bool CommandExists(char *cmd_name);
void FreeCommands();

extern CommandInfo *Commands;

#endif
