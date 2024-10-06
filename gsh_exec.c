#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gsh_exec.h"
#include <fcntl.h>

// Function to handle piped commands
int gsh_pipe(char **args, char **args2)
{
    int fd[2];
    if (pipe(fd) == -1)
    {
        perror("gsh_pipe: pipe");
        return 0;
    }
    int pid = fork();
    if (pid == 0)
    {
        // Child process for the first command
        if (dup2(fd[1], STDOUT_FILENO) == -1)
        {
            perror("gsh_pipe: dup2");
            exit(EXIT_FAILURE);
        }
        close(fd[0]);
        close(fd[1]);
        if (execvp(args[0], args) == -1)
        {
            perror("gsh_pipe: execvp");
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0)
    {
        perror("gsh_pipe: fork");
        return 0;
    }
    if (fork() == 0)
    {
        // Child process for the second command
        if (dup2(fd[0], STDIN_FILENO) == -1)
        {
            perror("gsh_pipe: dup2");
            exit(EXIT_FAILURE);
        }
        close(fd[0]);
        close(fd[1]);
        if (execvp(args2[0], args2) == -1)
        {
            perror("gsh_pipe: execvp");
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0)
    {
        perror("gsh_pipe: fork");
        return 0;
    }
    // Parent process
    close(fd[0]);
    close(fd[1]);
    // Wait for both child processes to finish
    if (wait(NULL) == -1)
    {
        perror("gsh_pipe: wait");
    }
    if (wait(NULL) == -1)
    {
        perror("gsh_pipe: wait");
    }
    return 1;
}
// Function to implement redirection
int gsh_redirect(char **args)
{
    int i;
    int in_fd = -1, out_fd = -1;
    char *input_file = NULL, *output_file = NULL;
    int append = 0;

    // Parse arguments for redirection symbols
    for (i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "<") == 0)
        {
            if (args[i + 1] == NULL)
            {
                fprintf(stderr, "gsh: syntax error near unexpected token `<'\n");
                return 0;
            }
            input_file = args[i + 1];
            args[i] = NULL;
            i++;
        }
        else if (strcmp(args[i], ">") == 0)
        {
            if (args[i + 1] == NULL)
            {
                fprintf(stderr, "gsh: syntax error near unexpected token `>'\n");
                return 0;
            }
            output_file = args[i + 1];
            args[i] = NULL;
            i++;
        }
        else if (strcmp(args[i], ">>") == 0)
        {
            if (args[i + 1] == NULL)
            {
                fprintf(stderr, "gsh: syntax error near unexpected token `>>'\n");
                return 0;
            }
            output_file = args[i + 1];
            args[i] = NULL;
            append = 1;
            i++;
        }
    }

    // Handle input redirection
    if (input_file != NULL)
    {
        in_fd = open(input_file, O_RDONLY);
        if (in_fd == -1)
        {
            perror("gsh_redirect: open input file");
            return 0;
        }
        if (dup2(in_fd, STDIN_FILENO) == -1)
        {
            perror("gsh_redirect: dup2 input");
            close(in_fd);
            return 0;
        }
        close(in_fd);
    }

    // Handle output redirection
    if (output_file != NULL)
    {
        int flags = O_WRONLY | O_CREAT;
        flags |= (append) ? O_APPEND : O_TRUNC;
        out_fd = open(output_file, flags, 0644);
        if (out_fd == -1)
        {
            perror("gsh_redirect: open output file");
            return 0;
        }
        if (dup2(out_fd, STDOUT_FILENO) == -1)
        {
            perror("gsh_redirect: dup2 output");
            close(out_fd);
            return 0;
        }
        close(out_fd);
    }

    // Execute the command
    return gsh_execute(args);
}

// Function to execute a single command
int gsh_execute(char **args)
{
    int status;
    int wpid;
    int pid = fork();
    if (pid == 0)
    {
        // Child process
        if (execvp(args[0], args) == -1)
        {
            perror("gsh_execute: execvp");
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0)
    {
        perror("gsh_execute: fork");
        return EXIT_FAILURE;
    }
    else
    {
        // Parent process
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
            if (wpid == -1)
            {
                perror("gsh_execute: waitpid");
                return EXIT_FAILURE;
            }
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return EXIT_SUCCESS;
}
