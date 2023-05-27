## This is the README file for the simple shell project.##

## Written by Antony Muga and Nicholas Oyengo##

#The program has the exact same output as sh (/bin/sh) as well as the exact same error output to normal shell.#

#The simple shell will be compiled this way:#

  gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh

#For Testing:#
  Shell should work in interactive mode.
  Shell works also in non-interactive mode.

#How our shell works:#
  Display a prompt and wait for the user to type a command. A command line always ends with a new line.
  The prompt is displayed again each time a command has been executed.
  The command lines are simple, no semicolons, no pipes, no redirections or any other advanced features.
  The command lines are made only of one word. No arguments will be passed to programs.
  If an executable cannot be found, print an error message and display the prompt again.
  Handle errors.
