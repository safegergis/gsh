#ifndef GSH_EXEC_H
#define GSH_EXEC_H

// Function prototypes for command execution
int gsh_pipe(char **args, char **args2);
int gsh_execute(char **args);
int gsh_redirect(char **args);
#endif // GSH_EXEC_H
