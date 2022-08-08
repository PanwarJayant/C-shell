#include "headers.h"
#include "history.h"
#include "util.h"

void handle_history(int num)
{
  FILE *file = fopen(historyFile, "a");
  fclose(file);
  file = fopen(historyFile, "r");
  char *history[100];
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
  int start = 0;
  if (idx - num > 0)
  {
    start = idx - num;
  }

  for (int x = start; x < idx; x++)
  {
    printf("%s\n", history[x]);
    free(history[x]);
  }

  fclose(file);
}