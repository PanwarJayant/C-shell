#ifndef LS_H
#define LS_H

void handle_ls(char *commandParts[], int coms, char *homeDir, char *currentDir, char *finalDir);
void printDir(char *location, int aFlag, int lFlag, char *commandPart, int det);
void printFile(char *location, char *commandPart, int lFlag);

#endif