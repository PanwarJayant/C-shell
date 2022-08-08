#include "headers.h"
#include "process.h"
#include "util.h"

void buildProcess(char *commandParts[], int coms, int backg)
{
  char *command = strdup(commandParts[0]);
  int isBackg = backg;
  int siz = coms + 1;
  char *argv[siz];
  int p = 0;
  while (p < coms)
  {
    argv[p] = strdup(commandParts[p]);
    p++;
  }
  argv[coms] = NULL;
  int rc = fork();

  if (rc > 0)
  {
    if (backg)
    {
      printf("Child process with pid %d sent to background\n", rc);
    }
    else
    {
      waitpid(rc, NULL, 0);
    }
  }
  else if (rc < 0)
  {
    catch ("Error: Creation of Child process failed!");
  }
  else
  {
    if (backg)
    {
      setpgid(0, 0);
    }
    if (execvp(command, argv) == -1)
    {
      catch ("Error: Invalid command!");
      exit(1);
    }
  }
}