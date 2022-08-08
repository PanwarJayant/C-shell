#include "headers.h"
#include "util.h"
#include "ls.h"

void printFile(char *location, char *commandPart, int lFlag)
{
  const char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  struct stat fileDetails;
  if (lstat(location, &fileDetails) < 0)
  {
    catch ("Error fetching the stat data struct!");
    return;
  }

  if (lFlag == 0)
  {
    printf("%s\n", commandPart);
    return;
  }

  int links = fileDetails.st_nlink;
  long long sz = fileDetails.st_size;

  struct passwd *pass = getpwuid(fileDetails.st_uid);
  struct group *grp = getgrgid(fileDetails.st_gid);
  char *usrName = pass->pw_name;
  char *grpName = grp->gr_name;
  time_t timeLastMod = fileDetails.st_mtime;
  struct tm lastMod;
  int tim[4];
  localtime_r(&timeLastMod, &lastMod);
  tim[0] = lastMod.tm_min;
  tim[1] = lastMod.tm_hour;
  tim[2] = lastMod.tm_mday;
  tim[3] = lastMod.tm_mon;

  char monthName[5];
  strcpy(monthName, months[tim[3]]);

  char initPerm[100];
  char finalPerm[100];

  if (fileDetails.st_mode & S_IFDIR)
  {
    strcpy(initPerm, "d");
  }
  else if (S_ISLNK(fileDetails.st_mode))
  {
    strcpy(initPerm, "l");
  }
  else
  {
    strcpy(initPerm, "-");
  }

  // FORMAT PERMISSIONS
  int idx = 0;
  for (int x = 8; x >= 0; x--)
  {
    int t = 1 << x;
    if (fileDetails.st_mode & t)
    {
      if (x % 3 == 2)
      {
        finalPerm[idx] = 'r';
      }
      else if (x % 3 == 1)
      {
        finalPerm[idx] = 'w';
      }
      else if (x % 3 == 0)
      {
        finalPerm[idx] = 'x';
      }
    }
    else
    {
      finalPerm[idx] = '-';
    }
    idx++;
  }
  finalPerm[idx] = '\0';

  strcat(initPerm, finalPerm);

  printf("%s%5d%10s%10s%10lld %s %02d %02d:%02d %s\n", initPerm, links, usrName, grpName, sz, monthName, tim[2], tim[1], tim[0], commandPart);
}

void printDir(char *location, int aFlag, int lFlag, char *commandPart, int det)
{
  struct dirent *dirDetails;
  DIR *dir = opendir(location);
  int total = 0, totalBlocks = 0, count = 0;

  while (readdir(dir) != NULL)
  {
    total++;
  }
  char blocks[total][size];
  closedir(dir);

  if (!strcmp(commandPart, "") && det)
  {
    printf("%s :\n", commandPart);
  }

  dir = opendir(location);

  while ((dirDetails = readdir(dir)) != NULL)
  {
    strcpy(blocks[count], dirDetails->d_name);
    count++;
    if (aFlag == 0 || (dirDetails->d_name[0] != '.'))
    {
      char elemAddress[size];
      strcpy(elemAddress, location);
      strcat(elemAddress, dirDetails->d_name);
      struct stat elemData;
      if (stat(elemAddress, &elemData) < 0)
      {
        catch ("Error fetching the stat struct data!");
        continue;
      }
      totalBlocks += elemData.st_blocks;
    }
  }

  // ALPHABETICAL SORTING
  for (int i = 0; i < total - 1; i++)
  {
    int j = i + 1;
    while (j < total)
    {
      if (strcmp(blocks[i], blocks[j]) <= 0)
      {
        j++;
        continue;
      }
      else
      {
        char temp[size];
        strcpy(temp, "");
        strcpy(temp, blocks[j]);
        strcpy(blocks[j], blocks[i]);
        strcpy(blocks[i], temp);
      }
      j++;
    }
  }

  if (lFlag == 1)
  {
    printf("TOTAL %d\n", totalBlocks / 2);
  }

  for (int x = 0; x < total; x++)
  {
    char *currentBlock = blocks[x];
    if (aFlag != 0 || currentBlock[0] != '.')
    {
      char elemAddress[size];
      strcpy(elemAddress, location);
      strcat(elemAddress, currentBlock);
      printFile(elemAddress, currentBlock, lFlag);
    }
  }
  free(dirDetails);
  closedir(dir);
}

void handle_ls(char *commandParts[], int coms, char *homeDir, char *currentDir, char *finalDir)
{
  int aFlag = 0, lFlag = 0, dirs = 0, det = 1;
  int dir[coms + 1];

  int j = 1;
  while (j < coms)
  {
    if (commandParts[j][0] != '-')
    {
      dir[dirs] = j;
      dirs++;
    }
    else
    {
      int i = 1;
      while (i < strlen(commandParts[j]))
      {
        int err = 1;
        if (commandParts[j][i] != 'l' && commandParts[j][i] != 'a')
        {
          catch ("Error: Please use valid flags 'a' or 'l' with ls!");
          err++;
          return;
        }
        else if (commandParts[j][i] == 'a')
        {
          err--;
          aFlag = 1;
        }
        else if (commandParts[j][i] == 'l')
        {
          lFlag = 1;
        }
        i++;
      }
    }
    j++;
  }
  if (dirs == 0)
  {
    det = 0;
    // strcpy(commandParts[coms], ".");
    commandParts[coms] = strdup(".");
    dir[dirs] = coms;
    dirs++;
  }

  int x;
  int fileFlag;
  char location[size];
  for (x = 0; x < dirs; x++)
  {
    struct stat dirStats;
    int i = dir[x];
    getAddress(location, commandParts[i], currentDir, homeDir);

    if (stat(location, &dirStats) == 0 && (S_IFDIR & dirStats.st_mode))
    {
      if (location[strlen(location) - 1] != '/')
      {
        strcat(location, "/");
      }
      fileFlag = 0;
      printDir(location, aFlag, lFlag, commandParts[x], det);
    }
    else if (S_IFREG & dirStats.st_mode)
    {
      fileFlag = 1;
      printFile(location, commandParts[x], lFlag);
    }
    else
    {
      catch ("Invalid Fafdile/Directory name!");
    }
    printf("\n");
  }
}