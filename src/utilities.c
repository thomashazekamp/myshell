/***************************************************
Author: Thomas Hazekamp
Student ID: 20423602
Module: CA216 - Operating Systems
University: Dublin City Univeristy
Date: 11/03/2022
I have read and understood DCU's Academic Integrity Policy,
I have given detailed reference to the authors of any material used to help with this project.
****************************************************
Description: This file is part of a simple working shell called myshell, 
    this program contains functions for that are called by myshell.c and int-commands.c.
    This program is to be compiled with a myshell.c and int-commands.c files
****************************************************
References:
    CodeVault, M (2020) Redirecting standard output in C, [source code]. https://code-vault.net [video link]. https://www.youtube.com/watch?v=5fnVr-zH-SE 
        General layout of output redirection commands are inspired by CodeVault
****************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

// Function to set a new evironment variable (SHELL or PARENT)
void set_env(char * name) {
    // If name is SHELL then we create a new environment varible called SHELL
    if (!strcmp(name, "SHELL")) {
        // Variables
        char cwd[500];
        char shell_line[510];

        getcwd(cwd, sizeof(cwd)); // Getting the current working directory

        // Concatinating the cwd and /myshell to variable
        strcat(shell_line, cwd);
        strcat(shell_line, "/myshell");
        
        // Creating a new environment variable SHELL
        putenv("SHELL=");

        // Setting the full path of the SHELL environment variable
        setenv("SHELL", shell_line, 1);
    }
    // If name is PARENT then we create a new environment variable called PARENT
    if (!strcmp(name, "PARENT")) {
        // Variables
        char cwd[500];
        char parent_line[510];

        getcwd(cwd, sizeof(cwd)); // Getting the current working directory
        
        // Concatinating the cwd to parent variable
        strcat(parent_line, cwd);
        
        // Creating a new environment variable PARENT
        putenv("PARENT=");

        // Setting the full path of the PARENT environment variable
        setenv("PARENT", parent_line, 1);
    }
}
// Function to create and print a command line prompt showing the current path
void full_prompt_out(char * initial_prompt, char * full_prompt) {
    // Variable
    char cwd[500];

    strcpy(full_prompt, "\0"); // Setting the full prompt string to null

    getcwd(cwd, sizeof(cwd)); // Getting the cwd

    strcat(full_prompt, initial_prompt); // Adding the start of prompt to final version
    strcat(full_prompt, cwd); // Adding the cwd to the final version
    strcat(full_prompt, " $ "); // Adding sign at end of final version

    fputs(full_prompt, stdout); // Outputting the full command line prompt
}
// Function to open and change file descriptors, for output redirection type 1 (">")
void out_redirection1(char * file_name) {
    int fd_out = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (fd_out == -1) {
        printf("There was a problem with the file\n");
        exit(1);
    }
    dup2(fd_out, STDOUT_FILENO);
    close(fd_out);
}
// Function to open and change file descriptors, for output redirection type 2 (">>")
void out_redirection2(char * file_name) {
    int fd_out = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0777);
    if (fd_out == -1) {
        printf("There was a problem with the file\n");
        exit(1);
    }
    dup2(fd_out, STDOUT_FILENO);
    close(fd_out);

}