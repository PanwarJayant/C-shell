#include "headers.h"
#include "handle.h"
#include "util.h"
#include "cd.h"
#include "ls.h"
#include "pinfo.h"
#include "process.h"
#include "background.h"
#include "history.h"

void handleCommand(char *command, int backg, char *homeDir, char *currentDir, char *finalDir)
{
  // PARTITION GIVEN COMMAND INTO ITS PARTS
  char *commandParts[size];
  int coms = 0;
  commandParts[0] = strtok(command, " \n\t");
  while (commandParts[coms] != NULL)
  {
    coms++;
    commandParts[coms] = strtok(NULL, " \t\n");
  }

  if (coms == 0)
  {
    return;
  }

  // HANLDE THE GIVEN COMMAND ACCORDINGLY

  if (!strcmp(commandParts[0], "cd"))
  {
    if (coms == 1)
    {
      commandParts[1] = malloc(10);
      strcpy(commandParts[1], "~");
    }
    handle_cd(commandParts, homeDir, currentDir, finalDir);
  }
  else if (!strcmp(commandParts[0], "repeat"))
  {
    int times = atoi(commandParts[1]);
    for (int i = 0; i < times; i++)
    {
      char newCommandParts[size];
      strcpy(newCommandParts, "");
      for (int x = 2; x < coms; x++)
      {
        strcat(newCommandParts, commandParts[x]);
        strcat(newCommandParts, " ");
      }

      handleCommand(newCommandParts, backg, homeDir, currentDir, finalDir);
    }
  }
  else if (!strcmp(commandParts[0], "echo"))
  {
    for (int x = 1; x < coms; x++)
    {
      printf("%s ", commandParts[x]);
    }
    printf("\n");
  }
  else if (!strcmp(commandParts[0], "pwd"))
  {
    currentDir[strlen(currentDir) - 1] = '\0';
    printf("%s\n", currentDir);
    strcat(currentDir, "/");
  }
  else if (!strcmp(commandParts[0], "ls"))
  {
    handle_ls(commandParts, coms, homeDir, currentDir, finalDir);
  }
  else if (!strcmp(commandParts[0], "pinfo"))
  {
    if (coms == 1)
    {
      commandParts[1] = malloc(10);
      int pid = getpid();
      sprintf(commandParts[1], "%d", pid);
    }
    handle_pinfo(commandParts);
  }
  else if (!strcmp(commandParts[0], "history"))
  {
    if (coms == 1)
    {
      handle_history(10);
    }
    else
    {
      int num = atoi(commandParts[1]);
      if (num <= 0 || num > 20)
      {
        catch ("Error: Need to specify between 1 and 20 numeric values for history command");
        return;
      }
      else
      {
        handle_history(num);
      }
    }
  }
  else if (!strcmp(commandParts[0], "clear"))
  {
    printf("\e[1;1H\e[2J");
  }
  else if (!strcmp(commandParts[0], "exit"))
  {
    killBackground();
    printf("SHELL TERMINATED! see you again soon :)");
    _exit(0);
  }
  else
  {
    buildProcess(commandParts, coms, backg);
  }
}

void handleInput(char *inp, char *homeDir, char *currentDir, char *finalDir)
{
  char inpcpy[size];
  char inpcpy2[size];
  char *command;

  strcpy(inpcpy, inp);
  strcpy(inpcpy2, inpcpy);

  // GETTING THE FIRST COMMAND
  command = strtok(inp, ";&");

  // GETTING NO OF OTHER COMMANDS
  int coms = 0;
  while (command)
  {
    coms++;
    command = strtok(NULL, ";&");
  }

  if (coms == 0)
  {
    catch ("Invalid command entered!");
    return;
  }

  // EXTRACTING ALL THE COMMANDS
  int i = 0;
  char *cpy = inpcpy;
  char *allCommands[coms + 1];
  allCommands[0] = strtok(inpcpy, ";&");

  while (allCommands[i] != NULL && strcmp(allCommands[i], ""))
  {
    i++;
    allCommands[i] = strtok(NULL, ";&");
  }

  // PROCESSING ALL THE COMMANDS ACCORDINGLY: IF BACKGROUND OR FOREGROUND
  for (int x = 0; x < coms; x++)
  {
    int bg = 0;
    if (inpcpy2[strlen(allCommands[x]) + (allCommands[x] - cpy)] == '&')
    {
      bg = 1;
    }

    handleCommand(allCommands[x], bg, homeDir, currentDir, finalDir);
  }
}