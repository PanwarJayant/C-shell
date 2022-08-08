#include "headers.h"
#include "util.h"
#include "cd.h"

char prevFinalDir[size];
char prevCurrentDir[size];
char targetLocation[size];
char newLocation[size];
char tempFinal[size];
char tempCurrent[size];
int cdTimes = 0;

void handle_cd(char *commandParts[], char *homeDir, char *currentDir, char *finalDir)
{
  strcpy(tempFinal, finalDir);
  strcpy(tempCurrent, currentDir);
  struct stat dirStats;
  strcpy(targetLocation, commandParts[1]);
  getAddress(newLocation, targetLocation, currentDir, homeDir);

  if (!strcmp(targetLocation, "-"))
  {
    if (cdTimes > 0)
    {
      cdTimes++;
      strcpy(finalDir, prevFinalDir);
      strcpy(currentDir, prevCurrentDir);
      printf("%s\n", currentDir);
      strcpy(prevFinalDir, tempFinal);
      strcpy(prevCurrentDir, tempCurrent);
      if (chdir(currentDir) == -1)
      {
        catch ("Error moving back to previous working directory!");
      }
    }
    else
    {
      catch ("No previous working directory found!");
    }
    return;
  }

  if (stat(newLocation, &dirStats) == 0 && (S_IFDIR & dirStats.st_mode))
  {

    if (chdir(newLocation) < 0)
    {
      catch ("Directory does not exist!");
    }
    if (!getcwd(currentDir, size))
    {
      catch ("getcwd returned error!");
    }
    strcat(currentDir, "/");
    loadFinalDir(homeDir, currentDir, finalDir);
  }
  else
  {
    catch ("Directory does not exist!");
  }
  cdTimes++;
  strcpy(prevFinalDir, tempFinal);
  strcpy(prevCurrentDir, tempCurrent);
}