#include "headers.h"
#include "background.h"

void checkBackground()
{
  int child = 0, stat;
  struct dirent *dirDetails;
  DIR *dir = opendir("/proc");
  if (dir == NULL)
  {
    return;
  }

  int rrc;
  int pids[size];
  char *pnames[size];

  while ((dirDetails = readdir(dir)) != NULL)
  {
    if (isdigit(dirDetails->d_name[0]))
    {
      char location[size];
      sprintf(location, "/proc/%s/stat", dirDetails->d_name);
      FILE *file = fopen(location, "r");
      if (file != NULL)
      {
        int pid, ppid;
        char state;
        char pname[size];
        fscanf(file, "%d %s", &pid, pname);
        fscanf(file, " %c ", &state);
        fscanf(file, " %d", &ppid);
        int gpid = (int)getpid();
        if (ppid == gpid)
        {
          pids[child] = pid;
          pnames[child++] = strdup(pname);
        }
        fclose(file);
      }
    }
  }
  closedir(dir);

  for (int z = 0; z != -1; z++)
  {
    if ((rrc = waitpid(-1, &stat, WNOHANG)) > 0)
    {
      char state[200];
      int t;
      if (WIFSIGNALED(stat))
      {
        t = WTERMSIG(stat);
        sprintf(state, "because of signal %d", t);
      }
      else if (WIFEXITED(stat))
      {
        t = WIFEXITED(stat);
        sprintf(state, "normally with status %d", t);
      }
      else
      {
        sprintf(state, "abnormally");
      }

      char txt[size * 2];
      char pname[size];
      strcpy(pname, " ");
      for (int x = 0; x < child; x++)
      {
        if (pids[x] == rrc)
        {
          strcpy(pname, pnames[x]);
          break;
        }
      }
      int len = sprintf(txt, "\nChild process %s %d exited %s\n", pname, rrc, state);
      write(2, txt, len);
    }
    else
    {
      break;
    }
  }
}

void killBackground()
{
  int child = 0, stat;
  struct dirent *dirDetails;
  DIR *dir = opendir("/proc");
  if (dir == NULL)
  {
    return;
  }

  while ((dirDetails = readdir(dir)) != NULL)
  {
    if (isdigit(dirDetails->d_name[0]))
    {
      char location[size];
      sprintf(location, "/proc/%s/stat", dirDetails->d_name);
      FILE *file = fopen(location, "r");
      if (file != NULL)
      {
        int pid, ppid;
        char state;
        char pname[size];
        fscanf(file, "%d %s", &pid, pname);
        fscanf(file, " %c ", &state);
        fscanf(file, " %d", &ppid);
        int gpid = (int)getpid();
        if (ppid == gpid)
        {
          child++;
          kill(pid, SIGKILL);
        }
        fclose(file);
      }
    }
  }
  closedir(dir);
  for (int x = 0;; x++)
  {
    if (waitpid(-1, &stat, WNOHANG) > 0)
    {
      continue;
    }
    else
    {
      break;
    }
  }
}