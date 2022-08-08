#ifndef UTIL_H
#define UTIL_H
void catch ();
void setNull();
void loadFinalDir();
void loadDirs();
char *getShellName();
void getAddress(char *newLocation, char *targetLocation, char *currentDir, char *homeDir);
void storeHistory(char commands[]);
#endif