#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include "gsh_exec.h"
#include "gsh_input.h"
#include "gsh_builtins.h"

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

int main(int argc, char *argv[])
{

    char **history = NULL;
    char **args = NULL;
    do
    {
        char cwd[1024];
        struct passwd *pw = getpwuid(getuid());
        char *username = pw->pw_name;
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("%s%s@%s%s%s> ", RED, username, CYN, cwd, RESET); // Display prompt with current directory
        }
        else
        {
            perror("getcwd() error");
        }
        char *line = gsh_read();
        char **args_after_pipe = NULL;
        int pipe_found = 0;

        args = gsh_split(line);

        // Check for pipe symbol and handle accordingly
        for (int i = 0; args[i] != NULL; i++)
        {
            if (strcmp(args[i], "|") == 0)
            {
                args[i] = NULL;
                args_after_pipe = &args[i + 1];
                if (gsh_pipe(args, args_after_pipe) == 0)
                {
                    fprintf(stderr, "Pipe command failed\n");
                }
                pipe_found = 1;
                break;
            }
            if (strcmp(args[i], "!!") == 0)
            {
                if (history == NULL)
                {
                    fprintf(stderr, "No command in history\n");
                }
                else
                {
                    free(args);
                    args = gsh_copy_string_array(history);
                    gsh_execute(args);
                }
            }
        }

        // If no pipe found, execute as a single command
        if (!pipe_found)
        {
            if (history != NULL)
            {
                free(history);
            }
            history = gsh_copy_string_array(args);
            if (gsh_execute(args) == EXIT_FAILURE)
            {
                fprintf(stderr, "Command execution failed\n");
            }
        }

        // Clean up
        free(line);
        free(args);
    } while (1); // Infinite loop for shell prompt
}
