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

// Function prototypes
void display_prompt();
int process_command(char **args, char ***history);

int main(int argc, char *argv[])
{
    int original_stdin, original_stdout, original_stderr;
    original_stdin = dup(STDIN_FILENO);
    original_stdout = dup(STDOUT_FILENO);
    original_stderr = dup(STDERR_FILENO);

    char **history = NULL;
    char **args = NULL;

    do
    {
        display_prompt();
        char *line = gsh_read();
        args = gsh_split(line);

        if (process_command(args, &history) == EXIT_FAILURE)
        {
            fprintf(stderr, "Command execution failed\n");
        }

        // Restore original file descriptors
        dup2(original_stdin, STDIN_FILENO);
        dup2(original_stdout, STDOUT_FILENO);
        dup2(original_stderr, STDERR_FILENO);

        // Clean up
        free(line);
        free(args);
    } while (1); // Infinite loop for shell prompt
}

void display_prompt()
{
    char cwd[1024];
    struct passwd *pw = getpwuid(getuid());
    char *username = pw->pw_name;
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s%s@%s%s%s> ", RED, username, CYN, cwd, RESET);
    }
    else
    {
        perror("getcwd() error");
    }
}

int process_command(char **args, char ***history)
{
    char **args_after_pipe = NULL;
    int pipe_found = 0;
    int built_in_found = 0;
    int redirect_found = 0;

    for (int i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "<") == 0 || strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0)
        {
            gsh_redirect(args);
            redirect_found = 1;
            break;
        }

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
            if (*history == NULL)
            {
                fprintf(stderr, "No command in history\n");
            }
            else
            {
                free(args);
                args = gsh_copy_string_array(*history);
                return gsh_execute(args);
            }
        }

        if (strcmp(args[i], "cd") == 0)
        {
            gsh_cd(args);
            built_in_found = 1;
        }
        else if (strcmp(args[i], "help") == 0)
        {
            gsh_help();
            built_in_found = 1;
        }
        else if (strcmp(args[i], "exit") == 0)
        {
            gsh_exit();
            built_in_found = 1;
        }
        else if (strcmp(args[i], "mkdir") == 0)
        {
            gsh_mkdir(args);
            built_in_found = 1;
        }
    }

    if (!pipe_found && !built_in_found && !redirect_found)
    {
        if (*history != NULL)
        {
            free(*history);
        }
        *history = gsh_copy_string_array(args);
        return gsh_execute(args);
    }

    return EXIT_SUCCESS;
}
