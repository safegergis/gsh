#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "gsh_input.h"

// Function to read a line of input from the user
char *gsh_read()
{
    char *line = NULL;
    size_t bufsize = 0;
    if (getline(&line, &bufsize, stdin) == -1)
    {
        if (feof(stdin))
        {
            printf("\n");
            exit(EXIT_SUCCESS); // Exit gracefully on EOF (Ctrl+D)
        }
        else
        {
            perror("gsh_read");
            exit(EXIT_FAILURE);
        }
    }
    return line;
}

// Function to split the input line into arguments
char **gsh_split(char *line)
{
    char **args = malloc(sizeof(char *) * 64);
    if (!args)
    {
        perror("gsh_split: malloc");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    char *token = strtok(line, " \t\r\n\a");
    while (token != NULL)
    {
        args[i] = token;
        i++;
        if (i >= 64)
        {
            // Reallocate if we need more space
            args = realloc(args, sizeof(char *) * 128);
            if (!args)
            {
                perror("gsh_split: realloc");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, " \t\r\n\a");
    }
    args[i] = NULL; // Null-terminate the argument list
    return args;
}

// Function to copy a string array
char **gsh_copy_string_array(char **src)
{
    if (src == NULL)
        return NULL;

    // Count the number of strings in the source array
    int count = 0;
    while (src[count] != NULL)
        count++;

    // Allocate memory for the new array of pointers
    char **dest = malloc((count + 1) * sizeof(char *));
    if (!dest)
    {
        perror("gsh_copy_string_array: malloc");
        exit(EXIT_FAILURE);
    }

    // Copy each string
    for (int i = 0; i < count; i++)
    {
        dest[i] = strdup(src[i]);
        if (!dest[i])
        {
            perror("gsh_copy_string_array: strdup");
            exit(EXIT_FAILURE);
        }
    }

    // Null-terminate the array
    dest[count] = NULL;

    return dest;
}