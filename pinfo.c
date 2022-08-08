#include "headers.h"
#include "pinfo.h"
#include "util.h"

void print_pinfo(int pid, char stat, char *mem, char *execPath)
{
  printf("pid -- %d\n", pid);
  printf("Process Status -- %c\n", stat);
  printf("memory -- %s\n", mem);
  printf("Executable Path -- %s\n", execPath);
  printf("\n");
}

void handle_pinfo(char *commandParts[])
{
  char add[size];
  char execPath[size];
  int pid, cpid, execSz;
  char stat;
  char exec[size];
  char mem[size];

  sprintf(add, "/proc/%s/stat", commandParts[1]);
  FILE *pointer = fopen(add, "r");
  if (!pointer)
  {
    catch ("Error: Process cannot be accessed!");
    return;
  }

  strcpy(execPath, "defunct :");
  fscanf(pointer, "%d ", &pid);
  cpid = pid;
  fscanf(pointer, "%s ", exec);
  execSz = strlen(exec) - 1;
  fscanf(pointer, "%c ", &stat);
  int charStat = (int)stat;
  sprintf(add, "/proc/%s/exe", commandParts[1]);

  int linkDesc = readlink(add, execPath, size);
  if (linkDesc == -1)
  {
    strcat(execPath, exec);
  }
  else
  {
    execPath[linkDesc] = '\0';
  }

  sprintf(add, "/proc/%s/status", commandParts[1]);
  fclose(pointer);
  pointer = fopen(add, "r");

  if (!pointer)
  {
    catch ("Error: Process cannot be accessed!");
    return;
  }

  while (fscanf(pointer, " %499s", mem) == 1)
  {
    if (!strcmp(mem, "VmSize:"))
    {
      fscanf(pointer, " %499s", mem);
      break;
    }
  }
  print_pinfo(pid, stat, mem, execPath);
  fclose(pointer);
}