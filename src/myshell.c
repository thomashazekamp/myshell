/***************************************************
Author: Thomas Hazekamp
Student ID: 20423602
Module: CA216 - Operating Systems
University: Dublin City Univeristy
Date: 10/03/2022
I have read and understood DCU's Academic Integrity Policy,
I have given detailed reference to the authors of any material used to help with this project.
****************************************************
Description: This file is part of a simple working shell called myshell, 
    this program contains the main function which reads in the input and prints a command line prompt, a prompt function, 
    an evaluate command function (to check what command has been called), 
    an external commands function (to run external commands) and a function to set new environment variables.
    This program is to be compiled with a int-commands.c and utilities.c files, which hold all functions to run internal commands and other non-command functions.
****************************************************
References:
CodeVault, A (2020) Executing commands in C source code, [source code]. https://code-vault.net/lesson/as15alvu0a:1603732432433 
    General layout of the if else statements for the fork() portions of the functions have been inspired by CodeVault
    
The following information refers to the way this program reads input (lines 219-224 and lines 243-246)
Author: Ian G Graham
Date: December 2003
Version: 1.0
School of Information Technology
Griffith University, Gold Coast
ian.graham@griffith.edu.au
copyright (c) Ian G Graham, 2003. All rights reserved.
****************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_SIZE_OF_INPUT 250 // Max size of input, used for memory allocation
#define MAX_NUM_OF_ARGS 20 // Max number of elements in some array
#define DELIMITERS " \t\r\n" // Used to tokenise the input
extern char **environ; // NULL terminated array of char

// All utility functions found in utilities.c file
void set_env(char * name); // Function to create a new environment variable
void full_prompt_out(char * initial_prompt, char * full_prompt); // Function to print a command line with the CWD
void out_redirection1(char * file_name); // Function for output file redirection type ">"
void out_redirection2(char * file_name); // Function for output file redirection type ">>"

// All internal command functions found in int-commands.c file
int cmd_cd(char * directory); // Change directory function, will check if dir exists, will move into that dir and update env variable
void cmd_clr(); // Function to clear the terminal screen using fork() and exec()
void cmd_dir(char * directory, char ** line); // Function to list all current files and dirs in the given path, using fork() and exec()
void cmd_environ(char ** line); // Function to print environment variables to terminal screen
void cmd_echo(char ** line); // Function to print any given argument after the command echo
void cmd_help(char ** line); // Function to print the manual found in the ../manual/ directory
void cmd_pause(); // Function to pause the terminal until "ENTER" has been pressed
void cmd_quit(); // Function to exit the myshell program

// Function to run any external commands either in background or foreground
void cmd_external(char ** line) {
    int pid = fork(); // Starting the forking process

    int bg = 0; // Variable to determine if we want to run the command in the background or foreground
    int count; // Number of elements in the line array

    // Get total number of elements in list, used to find position of last element
    for (count = 0; line[count] != NULL; count++) {
        continue;
    }
    // If the command is followed by "&" symbol then change bg (background) variable
    if (!strcmp(line[count - 1], "&")) {
        bg = 1;
    }

    // Variables for the input and output types and locations in command line to find the file name
    int in_type = 0; // 0 meaning no input redirection, 1 meaning there is input redirection
    int in_pos = 0;
    int out_type = 0; // 0 meaning no output redirection, 1 meaning there is type ">" output redirection, 2 meaning there is type ">>" output redirection
    int out_pos = 0;

    // Used to find input & output redirection types
    for (int i = 0; line[i] != NULL; i++) {
        // For input redirection
        if (!strcmp(line[i], "<")) {
            in_type = 1;
            in_pos = i; // Set current position, used to find name of file
        }
        // If output type 1 redirection
        if (!strcmp(line[i], ">")) {
            out_type = 1;
            out_pos = i;
        }
        // If output type 2 redirection
        else if (!strcmp(line[i], ">>")) {
            out_type = 2;
            out_pos = i;
        }
    }

    set_env("PARENT"); // Setting a new environment variable of location of execution
    
    if (pid == -1) {
        // Error occured for child process
        exit(1);
    }
    // If there is no error for child process
    if (pid == 0) {
        // If input redirection is type 1 (input redirection exists)
        if (in_type == 1) {
            // Start of input redirection
            int fd_in = open(line[in_pos + 1], O_RDONLY, 0); // Open and read file
            if (fd_in == -1) { // Error control
                printf("There was a problem with the file\n");
                exit(1);
            }
            dup2(fd_in, STDIN_FILENO); // Change file descriptor
            close(fd_in);
        }
        // If output redirection is type ">"
        if (out_type == 1) {
            out_redirection1(line[out_pos + 1]); // Function used to open a file
        }
        // If output redirection is type ">>"
        else if (out_type == 2) {
            out_redirection2(line[out_pos + 1]); // Function used to open a file
        }

        // If this command needs to be a background process
        if (bg == 1) {
            line[count - 1] = NULL; // Removing the "&" symbol from the line array
            execvp(line[0], line); // Executing the command given without the "&" (still running in the background)
            exit(0); // Exit process once completed
        }
        else {
            // If the command is not run in the background then execute as normal (foreground process)
            if (in_type == 1) { // Make "<" NULL, so execvp stops at new NULL value
                line[in_pos] = NULL;
            }
            if (out_type != 0) { // Make ">" or ">>" NULL, so execvp stops at new NULL value
                line[out_pos] = NULL;
            }
            execvp(line[0], line); // Execute command
            exit(0); // Exiting process once completed
        }
    }
    else {
        // Wait until the child process has been completed
        // Only wait for child if the process is not a background process
        if (bg != 1) {
            waitpid(pid, NULL, 0);
        }
    }
}
// Function to check if the given command is internal or external, then run the appropriate functions
void eval_command(char ** command) {

    // If command wants to change directory
    if (!strcmp(command[0], "cd")) {
        cmd_cd(command[1]);
    }
    // If command wants to clear the screen
    else if (!strcmp(command[0], "clr")) {
        cmd_clr();
    }
    // If command wants to list current or other directory
    else if (!strcmp(command[0], "dir")) {
        cmd_dir(command[1], command); // Giving a directory (or null)
    }
    // If command wants to list all environment variables
    else if (!strcmp(command[0], "environ")) {
        cmd_environ(command);
    }
    // If command wants to print any text given after the echo command
    else if (!strcmp(command[0], "echo")) {
        cmd_echo(command);
    }
    // If command wants to print the user manual
    else if (!strcmp(command[0], "help")) {
        cmd_help(command);
    }
    // If command wants to pause until ENTER is entered
    else if (!strcmp(command[0], "pause")) {
        cmd_pause();
    }
    // If command wants to quit the shell (command line interface)
    else if (!strcmp(command[0], "quit")) {
        cmd_quit();
    }
    // If command is unknown the shell will use parent system calls
    else {
        cmd_external(command);
    }
}
// Main function for program
int main (int argc, char ** argv) {
    // Variables
    char *initial_prompt = "> "; // Initial part of command line prompt
    char * all_args[MAX_NUM_OF_ARGS]; // All arguments and the total amount allowed
    char ** indiv_arg; // Char for an individual argument
    char line[MAX_SIZE_OF_INPUT]; // Size of each argument
    char full_prompt[510]; // Setting memory for final version of command line prompt
    
    set_env("SHELL"); // Setting a new environment variable of location of execution

    // If there is a file given in the command line, then read and execute commands
    if (argc > 1) {
        
        // Creating file variable
        FILE * file;

        file = fopen(argv[1], "r"); // Opening file given as argument

        // If the file has some problem
        if (file == NULL) {
            printf("There has been an error opening the file!\n");
            exit(1);
        }
        // If no error with the file
        else {
            // Loop each line until the end of file has been reachedd
            while (fgets(line, MAX_SIZE_OF_INPUT, file) != NULL) {

                indiv_arg = all_args; // Making the first arg be the start of the all_args array
                *indiv_arg++ = strtok(line, DELIMITERS); // Tokenising the given input

                while ((*indiv_arg++ = strtok(NULL, DELIMITERS))); // The last element of the array will be a NULL value

                if (all_args[0]) {
                    // Continue if an input was provided
                    eval_command(all_args);
                }
            }
            fclose(file); // Closing the given file
        }
    }
    // If no file has been given, then show command line interface and execute any commands retrieved
    else {
        // Using infinite loop, program will stop if "quit" is inputted
        while(1) {

            full_prompt_out(initial_prompt, full_prompt); // Calling function to print and create command line prompt
            
            fgets(line, MAX_SIZE_OF_INPUT, stdin); // Retrieving the input

            indiv_arg = all_args; // Making the first arg be the start of the all_args array
            *indiv_arg++ = strtok(line, DELIMITERS); // Tokenising the given input

            while ((*indiv_arg++ = strtok(NULL, DELIMITERS))); // The last element of the array will be a NULL value

            if (all_args[0]) {
                // Continue if an input was provided
                eval_command(all_args);
            }
        }
        // The following is freeing up memory taken
        free(all_args);
        free(line);
    }
    return 0; // Exiting program correctly
}