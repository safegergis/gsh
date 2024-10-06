#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gsh_builtins.h"
#include "gsh_exec.h"
#include "gsh_input.h"

int gsh_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "gsh: cd: expected argument\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("gsh: cd");
        }
    }
    return 1;
}
int gsh_mkdir(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "gsh: mkdir: expected argument\n");
    }
    else
    {
        if (mkdir(args[1], 0755) != 0)
        {
            perror("gsh: mkdir");
        }
    }
    return 1;
}
int gsh_exit()
{
    exit(0);
}
int gsh_help()
{
    printf("gsh: Gergis Shell by Safe Gergis\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");
    printf("cd\n");
    printf("help\n");
    printf("mkdir\n");
    printf("!!\n");
    printf("exit\n");
    return 1;
}