#include "headers.h"
#include "util.h"

void catch (char *msg)
{
  perror(msg);
}

void getAddress(char *newLocation, char *targetLocation, char *currentDir, char *homeDir)
{
  if (targetLocation[0] == '~')
  {
    strcpy(newLocation, homeDir);
    strcat(newLocation, targetLocation + 1);
  }
  else if (targetLocation[0] == '/')
  {
    strcpy(newLocation, targetLocation);
  }
  else
  {
    strcpy(newLocation, currentDir);
    if (newLocation[strlen(newLocation) - 1] != '/')
    {
      strcat(newLocation, "/");
    }
    strcat(newLocation, targetLocation);
  }
}

void setNull(char *finalDir)
{
  if (finalDir[strlen(finalDir) - 1] == '/')
  {
    finalDir[strlen(finalDir) - 1] = '\0';
  }
}

void loadFinalDir(char *homeDir, char *currentDir, char *finalDir)
{
  if (strlen(currentDir) < strlen(homeDir))
  {
    strcpy(finalDir, currentDir);
    setNull(finalDir);
    return;
  }
  for (int x = 0; x < strlen(homeDir); x++)
  {
    if (homeDir[x] != currentDir[x])
    {
      strcpy(finalDir, currentDir);
      return;
    }
  }
  strcpy(finalDir, "~");
  strcat(finalDir, "/");
  strcat(finalDir, currentDir + strlen(homeDir));
  setNull(finalDir);
}

void loadDirs(char *homeDir, char *currentDir, char *finalDir)
{
  if (getcwd(homeDir, size) == NULL)
  {
    catch ("Command getcwd failed!");
  }
  char last = homeDir[strlen(homeDir) - 1];
  if (last != '/')
  {
    strcat(homeDir, "/");
  }
  strcpy(currentDir, homeDir);
  loadFinalDir(homeDir, currentDir, finalDir);
}

char *getShellName()
{
  // FETCHING THE USERNAME, HOSTNAME
  int len = 200;
  char userName[len];
  char hostName[len];
  char *shellName = (char *)malloc(len);
  int login = getlogin_r(userName, len);
  int host = gethostname(hostName, len);

  // ERROR HANDLING FOR FAILED FETCHING
  if (login != 0)
  {
    catch ("Login name couldn't be fetched!");
  }

  if (host != 0)
  {
    catch ("Host name couldn't be fetched!");
  }

  // SETTING THE FINAL SHELL NAME
  strcpy(shellName, "<");
  strcat(shellName, userName);
  strcat(shellName, "@");
  strcat(shellName, hostName);
  strcat(shellName, ":");

  return shellName;
}

void storeHistory(char commands[])
{
  if (!strcmp(commands, ""))
  {
    return;
  }
  FILE *file = fopen(historyFile, "a");
  fclose(file);
  file = fopen(historyFile, "r");
  char *history[100];
  char newCommands[size];
  int idx;
  if (file)
  {
    size_t st = size;
    idx = 0;
    history[idx] = malloc(size);
    while (getline(&history[idx], &st, file) > -1)
    {
      if (strcmp(history[idx], "\n"))
      {
        idx++;
        history[idx] = malloc(size);
      }
    }
  }
  fclose(file);

  file = fopen(historyFile, "w");
  int x = 0;
  if (idx - 20 > 0)
  {
    x = idx - 20;
  }
  for (x; x < idx; x++)
  {
    fprintf(file, "%s", history[x]);
  }

  sprintf(newCommands, "%s\n", commands);
  if (strcmp(newCommands, history[x - 1]))
  {
    fprintf(file, "%s", newCommands);
  }
  else if (!x)
  {
    fprintf(file, "%s\n", newCommands);
  }

  fclose(file);
  for (int j = 0; j <= idx; j++)
  {
    free(history[j]);
  }
}