Here's a README for the project in markdown format:

# GSH - Gergis Shell

GSH (Gergis Shell) is a custom Unix-like shell implementation written in C. It provides basic shell functionality including command execution, I/O redirection, piping, and built-in commands.

## Features

- Command execution
- I/O redirection (`<`, `>`, `>>`)
- Command piping (`|`)
- Built-in commands:
  - `cd`: Change directory
  - `exit`: Exit the shell
  - `help`: Display help information
  - `mkdir`: Create a new directory
- Command history (using `!!`)
- Colorful prompt displaying username and current working directory

## Building

To compile the shell, use the following command:

```bash
gcc -o gsh main.c gsh_exec.c gsh_builtins.c gsh_input.c -Wall -Wextra
```

## Usage

After compiling, run the shell:

```bash
./gsh
```

## Commands

- Execute any system command by typing it at the prompt
- Use `<`, `>`, or `>>` for input/output redirection
- Use `|` for command piping
- Built-in commands:
  - `cd [directory]`: Change current directory
  - `exit`: Exit the shell
  - `help`: Display help information
  - `mkdir [directory]`: Create a new directory
  - `!!`: Execute the last command

## Example Usage

```
gsh> ls -l
gsh> echo "Hello, World!" > output.txt
gsh> cat < input.txt | grep "pattern" > filtered.txt
gsh> cd /path/to/directory
gsh> mkdir new_folder
gsh> !!
gsh> exit
```

## Known Limitations

- Limited error handling for complex scenarios
- No support for advanced features like job control or command substitution

## Contributing

Contributions to improve GSH are welcome. Please submit pull requests or open issues to discuss proposed changes.

## License

[Specify your license here, e.g., MIT, GPL, etc.]

## Author

Safe Gergis

## Acknowledgments

This project was created as a learning exercise in systems programming and Unix concepts.
