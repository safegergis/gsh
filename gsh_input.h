#ifndef GSH_INPUT_H
#define GSH_INPUT_H

// Function prototypes for input handling
char *gsh_read();
char **gsh_split(char *line);
char **gsh_copy_string_array(char **src);

#endif // GSH_INPUT_H
