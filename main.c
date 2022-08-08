#include "headers.h"
#include "handle.h"
#include "util.h"
#include "background.h"

char homeDir[size];
char currentDir[size];
char finalDir[size];

char *trim(char *inp)
{
  int inpSize = strlen(inp) - 1;

  // MAKE LAST CHAR NULL IF UNWANTED SYMBOL
  for (int x = inpSize; x >= 0; x--)
  {
    if (inp[x] != ' ' && inp[x] != '\t' && inp[x] != '\n')
    {
      break;
    }
    else
    {
      inp[x] = '\0';
    }
  }

  // REMOVE LEADING CHAR IF UNWANTED SYMBOL
  int rem = 0;
  for (int x = 0; x <= inpSize; x++)
  {
    if (inp[x] == ' ' || inp[x] == '\t' || inp[x] == '\n')
    {
      rem++;
    }
    else
    {
      break;
    }
  }

  while (rem)
  {
    inp++;
    rem--;
  }

  return inp;
}

void handleInterrupts(int signum)
{
  if (signum == SIGCHLD)
  {
    checkBackground();
  }
  else if (signum == SIGINT)
  {
    killBackground();
    _exit(0);
  }
}

void printPrompt(char *shellName, char *finalDir)
{
  char displayDir[size];
  strcpy(displayDir, finalDir);
  strcat(displayDir, ">");
  printf("%s", shellName);
  printf("%s ", displayDir);
}

int main()
{
  signal(SIGINT, handleInterrupts);
  signal(SIGCHLD, handleInterrupts);
  char *shellName = getShellName();
  loadDirs(homeDir, currentDir, finalDir);
  while (1)
  {
    printPrompt(shellName, finalDir);
    char *inp = malloc(size);
    fgets(inp, size, stdin);
    inp = trim(inp);
    storeHistory(inp);

    handleInput(inp, homeDir, currentDir, finalDir);
  }

  return 0;
}