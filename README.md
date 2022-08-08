# C Shell

It's a basic shell written in C by Jayant Panwar: 2019114013.

# File description

1. **main.c** : Implements the main loop that runs all the programs. Also handles trimming of user input, signal interrupts for background processes, and printing the shell.
2. **handle.c** : Responsible for processing the trimmed user input and executing commands accordingly. Also implements functions like `repeat`, `echo`, `pwd`, `clear`, `exit` due to their trivial implementation.
3. **cd.c** : Implements the `cd` function
4. **ls.c** : Implements the `ls` function
5. **history.c** : Implements the printing for `history` function
6. **process.c** : Implements the creation of child processes whether in the foreground or background
7. **background.c** : Implementation for checking the status of background processes and also killing them
8. **pinfo.c** : Implements the `pinfo` function
9. **util.c** : Contains resuable code and other utilities like getting the directory path, error printing, storing command history, fetching the hostname and system name, etc.

# Additional features

1. Supports Ctrl + C functionality to terminate the shell
2. Bonus feature of `history` command implemented

# Steps to run the shell:

1. Type `make`.
2. Then type `./vroom` to run the shell.
3. Type `exit` to exit the shell. You can also press Ctrl + C to exit the shell.

# Assumptions

- Max length of string commands is 2000 characters. If more are needed, please upsize **size** variable in **headers.h** file.
- Memory for `pinfo` is shown in KB
- **/proc** will be supported
- **cd -** command mimics the functionality of the same linux terminal command so it will also print to the terminal the address of the previous working directory where it is cding into.
- **historyFile** will need access to the **tmp** folder. If it fails, please provide the necessary permissions. Also, the pathname can be changed in the **headers.h** file.
- <signal.h> will be supported. It is used to handle signal interrupts while handling background process.
