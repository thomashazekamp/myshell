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
    this program contains functions for the internal command.
    This program is to be compiled with a myshell.c and utilities.c files
****************************************************
References:
    CodeVault, A (2020) Executing commands in C source code, [source code]. https://code-vault.net/lesson/as15alvu0a:1603732432433 
        General layout of the if else statements for the fork() portions of the functions have been inspired by CodeVault
****************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
extern char **environ; // NULL terminated array of char for environment variables

// These function are from the utilities.c file
void out_redirection1(char * file_name); // Function for output file redirection type ">"
void out_redirection2(char * file_name); // Function for output file redirection type ">>"

// Change directory function, will check if dir exists, will move into that dir and update env variable
int cmd_cd(char * directory) {
    // Variables
    char cwd[500];
    char str[500];

    DIR *dir = opendir(directory); // Opending the given directory

    if (dir) {
        // Will occur if the directory exists
        char *current_env = getenv("PWD"); // Getting PWD environment variable

        chdir(directory); // Moving to the given directory
        getcwd(cwd, sizeof(cwd)); // Getting the CWD of the new location
        strcat(str, cwd); // Attaching the CWD to the variable str (string)

        setenv("PWD", str, 1); // Setting the current PWD
        strcpy(str, "\0"); // Resetting str to a null value
        closedir(dir); // Closing the existing directory

        return 0;

    } else if (directory == NULL) {
        // If a directory was not provided
        return 0;

    } else {
        // If directory does not exits or there is some problem with opening the dir
        printf("There was a problem with this directory!\n");
        return 1;
    }
}
// Function to clear the terminal screen using fork() and exec()
void cmd_clr() {
    int pid = fork(); // Creating a child process
    
    if (pid == -1) {
        // Error occured for child process
        exit(1);
    }
    if (pid == 0) {
        // Now in the child process
        execlp("clear", "clear", NULL); // Clearing the terminal screen
    }
    else {
        // Wait until the child process has been completed
        waitpid(pid, NULL, 0);
    }
}
// Function to list all current files and dirs in the given path, using fork() and exec()
void cmd_dir(char * directory, char ** line) {
    int pid = fork(); // Creating child process

    // Output redirection variables
    int out_type = 0; // Redirection type
    int out_pos = 0; // Position of symbol (used to find file name)

    // Used to find type of output redirection
    for (int i = 0; line[i] != NULL; i++) {
        if (!strcmp(line[i], ">")) {
            out_type = 1;
            out_pos = i;
        }
        else if (!strcmp(line[i], ">>")) {
            out_type = 2;
            out_pos = i;
        }
    }

    if (pid == -1) {
        // Error occured for child process
        exit(1);
    }
    if (pid == 0) {
        // Now in the child process
        if (out_type != 0) {
            // This means there is output redirection
            if (out_type == 1) {
                out_redirection1(line[out_pos + 1]); // Function used to open a file
            }
            else if (out_type == 2) {
                out_redirection2(line[out_pos + 1]); // Function used to open a file
            }
            // If there is no given directory
            if (!strcmp(line[1], line[out_pos])) {
                execlp("ls", "ls", "-al", NULL);
            }
            else {
                execlp("ls", "ls", "-al", directory, NULL);
            }
        }
        else {
            execlp("ls", "ls", "-al", directory, NULL); // Lising all files and dirs
        }
    }
    else {
        // Wait until the child process has been completed
        waitpid(pid, NULL, 0);
    }
}
// Function to print environment variables to terminal screen
void cmd_environ(char ** line) {
    int pid = fork(); // Creating child process

    // Output redirection variables
    int out_type = 0; // Redirection type
    int out_pos = 0; // Position of symbol (used to find file name)

    // Used to find type of output redirection
    for (int i = 0; line[i] != NULL; i++) {
        if (!strcmp(line[i], ">")) {
            out_type = 1;
            out_pos = i;
        }
        else if (!strcmp(line[i], ">>")) {
            out_type = 2;
            out_pos = i;
        }
    }

    if (pid == -1) {
        // Error occured for child process
        exit(1);
    }
    if (pid == 0) {
        // Now in the child process
        if (out_type != 0) { // This means there is output redirection
            if (out_type == 1) {
                out_redirection1(line[out_pos + 1]); // Function used to open a file
            }
            else if (out_type == 2) {
                out_redirection2(line[out_pos + 1]); // Function used to open a file
            }
        }
        // Using loop to go through all environment variables
        for (int i = 0; environ[i] != NULL; i++) {
            printf("%s\n", environ[i]); // Printing to screen current environment variable
        }
        exit(0);
    }
    else {
        // Wait until the child process has been completed
        waitpid(pid, NULL, 0);
    }
}
// Function to print any given argument after the command echo
void cmd_echo(char ** line) {
    int pid = fork(); // Creating child process

    // Output redirection variables
    int out_type = 0; // Redirection type
    int out_pos = 0; // Position of symbol (used to find file name)

    // Used to find type of output redirection
    for (int i = 0; line[i] != NULL; i++) {
        if (!strcmp(line[i], ">")) {
            out_type = 1;
            out_pos = i;
        }
        else if (!strcmp(line[i], ">>")) {
            out_type = 2;
            out_pos = i;
        }
    }

    if (pid == -1) {
        // Error occured for child process
        exit(1);
    }
    if (pid == 0) {
        // Now in the child process
        if (out_type != 0) { // This means there is output redirection
            if (out_type == 1) {
                out_redirection1(line[out_pos + 1]); // Function used to open a file
            }
            else if (out_type == 2) {
                out_redirection2(line[out_pos + 1]); // Function used to open a file
            }
        }
        // Make symbol NULL so echo command only prints what is intended
        if (out_type != 0) {
            line[out_pos] = NULL;
        }
        // Using loop to print given words until NULL has been encountered(end of line)
        for (int i = 1; line[i] != NULL; i++) {
            printf("%s ", line[i]); // Printing each string
        }
        printf("\n"); // Adding a new line char after the loop
        exit(0);
    }
    else {
        // Wait until the child process has been completed
        waitpid(pid, NULL, 0);
    }
}
// Function to print the manual found in the ../manual/ directory
void cmd_help(char ** line) {
    int pid = fork(); // Creating child process

    // Output redirection variables
    int out_type = 0; // Redirection type
    int out_pos = 0; // Position of symbol (used to find file name)

    // Used to find type of output redirection
    for (int i = 0; line[i] != NULL; i++) {
        if (!strcmp(line[i], ">")) {
            out_type = 1;
            out_pos = i;
        }
        else if (!strcmp(line[i], ">>")) {
            out_type = 2;
            out_pos = i;
        }
    }
    
    if (pid == -1) {
        // Error occured for child process
        exit(1);
    }
    if (pid == 0) {
        // Now in the child process
        if (out_type != 0) { // This means there is output redirection
            if (out_type == 1) {
                out_redirection1(line[out_pos + 1]); // Function used to open a file
            }
            else if (out_type == 2) {
                out_redirection2(line[out_pos + 1]); // Function used to open a file
            }
        }
        // The following print statement will print so that if the user has moved to different directory and help command doesnt work, they have a reference on where they need to go
        printf("If error, try moving back to the \"2022-ca216-myshell/src/\" directory\n");
        execlp("more", "more", "-c", "-6", "../manual/readme.md", NULL); // Using more command to filter the manual (readme.md file in manual dir)
    }
    else {
        // Wait until the child process has been completed
        waitpid(pid, NULL, 0);
    }
}
// Function to pause the terminal until "ENTER" has been pressed
void cmd_pause() {
    printf("Press ENTER to continue!\n"); // Tell user what to press to continue
    getchar(); // Will wait until ENTER has been pressed
}
// Function to exit the myshell program
void cmd_quit() {
    exit(0); // Exiting the program without errors
}