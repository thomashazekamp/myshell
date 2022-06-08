***********************************************************************************
This manual will explain myshell and its possible commands and functionalities.
To view this page type "help" in the myshell command line prompt
NOTE: make sure your current working directory is situated in 2022-ca216-myshell/src for this command to work properly

This shell contains multiple commands with a wide variety of functionalities
Note: myshell will use the systems Linux commands if a command is not recognised, if otherwise it will skip and continue with a new command line prompt.

Press "ENTER" to continue reading the manual
Press "q" to exit the manual and return to the command line

MYSHELL


NAME
        myshell - the myshell shell

DESCRIPTION
        myshell is a command line interpreter (shell) which is used to run internal & external commands such as:
        
        INTERNAL COMMANDS
        cd - change the current directory
        clr - clears the terminal screen
        dir - list files and directories
        environ - lists all current environment variables
        echo - displays text followed by a new line
        help - displays the user manual of myshell
        pause - requires the user to press "ENTER" to continue the shell
        quit - quits the shell

        EXTERNAL COMMANDS
        When an external command is called it used the parent's shells command process
        External commands are all non-internal commands found in the parent shell

        myshell supports Batch mode, Input & Output redirection and Background execution, a detailed explanation will be found in the following manual

Commands:


NAME
        cd  -  change directory

SYNOPSIS
        cd [given_directory or path]

DESCRIPTION
        The cd utility will remain in the current directory if no new directory is given, if a new directory or path is given it will move to that location.

EXAMPLE
        "cd test"  -  will move to an existing directory named "test"
        "cd ../bin"  -  will move into the parent directory which is the folder that holds the current locations directory and then move in the "bin" directory
        "cd"  -  will stay in the current directory

VERSION: 1.0

NAME
        clr  -  clear the current terminal screen

SYNOPSIS
        clear

DESCRIPTION
        The clear utility will remove any text and clutter from the terminal screen, scrolling back up will show any text from before the clear command was used.

EXAMPLE
        "clear"  -  will clear the current terminal screen

VERSION: 1.0

NAME
        dir - outputs the files and directories to the terminal window

SYNOPSIS
        dir [optional - given directory]

DESCRIPTION
        The dir utility will print to the terminal screen a detailed view of all current files and directories in the given directory or path, if no directory is given it will output all current files and directories in the current location. Dir will show hidden files (files starting with .). The dir command will also show the read, write permission, date & time, users and file sizes.

EXAMPLE
        "dir" - will show all files and directories in the current location
        "dir bin" - will show all files and directories in the bin directory

VERSION: 1.0

NAME
        environ - will show all environment variables

SYNOPSIS
        environ

DESCRIPTION
        The environ utility will print to the terminal screen all current environment variables, these variables can affect the way runnning processes behave on a computer. This utility only shows these variables and provides no other functionality.

EXAMPLE
        "environ" - this will print all the current environment variables, one of these variables is the PWD which means "print working directory", PWD will equal to the full path name of the current working directory (your current location)

VERSION: 1.0

NAME
        echo - will print to screen any other arguments the user has given

SYNOPSIS
        echo [1 or more strings]

DESCRIPTION
        This echo utility will print the arguments given after the command in a subsequent line ending with a new line character.

EXAMPLE
        "echo hello world" - prints the string "hello world" on a new line and ends with a new line character
        "echo "A friendly new dog"" - prints the string ""A friendly new dog"" on a new line ending with a new line character

VERSION: 1.0

NAME
        help - shows the readme.md manual in a page by page format

SYNOPSIS
        help

DESCRIPTION
        The help utility will print to screen, the name, synopsis, description, example and the current version of each command in myshell. Using "ENTER" to continue showing the manual line by line, pressing "q" to exit the manual any time.

EXAMPLE
        "help" - will show the manual page

VERSION: 1.0

NAME
        pause - will temporarily pause the terminal until the "ENTER" character has been pressed

SYNOPSIS
        pause

DESCRIPTION
        The pause utility will require the user to press "ENTER" character to continue using the terminal window, a prompt will remind the user what actions to take to resume using the command line interpreter.

EXAMPLE
        "pause" - will give a prompt "Press ENTER to continue!"

VERSION: 1.0

NAME
        quit - will exit myshell command line interpreter

SYNOPSIS
        quit

DESCRIPTION
        The quit utility is used to exit myshell and return to the parent shell

EXAMPLE
        "quit"

VERSION: 1.0

Functionalities:


INPUT AND OUTPUT REDIRECTION

        Input and output redirection can be extremely useful to manage and manipulate files.

        Input redirection ("<") will only work for external commands that support it.
        Using the character "<" followed by a file will let the program know to read some file in and send the contents to the specified external command.
        
        EXAMPLE USING INPUT REDIRECTION
                sort < someText.txt - if the string "c \n b \n a" is in someText.txt ("\n" are called new line characters, and move the remaning characters to a new line). Then the external command sort will return to the screen "a \n b \n c \n" 
        
        Output redirection works for any external commands and only works for some internal commands such as dir, environ, echo, help.
        One way to use output redirection can be is by using ">" character in the command line followed by a file name, this will create the file (if not existent), and replace any content it has with the commands output.
        
        The second method to use output redirection is by using ">>" character in the command line followed by a file name, this will create the file (if not existent), and add to the bottom of the file any new output it has retrieved from the given command 

        EXAMPLE USING OUTPUT REDIRECTION
                dir > someText.txt - will send the results of the internal command dir to the file called someText.txt, it will replace any other content in that file and create someText.txt if it has not yet been created

                dir >> someText.txt - will send the results of the internal command dir to the file called someText.txt, it will append (add to any other content) the content in that file and create someText.txt if it has not yet been created
        
        EXAMPLE USING BOTH INPUT AND OUTPUT REDIRECTION
                cat < someText.txt > someNewText.txt - the external command cat prints to screen the content of someText.txt, in this case this command will send the contents of someText.txt to someNewText.txt, the output redirection will create someNewText.txt if it has not yet been created and replace any content if the file already exists

BACKGROUND EXECUTION

        Background execution will be detected if a "&" character has been added to the end of any external command. Background execution will let the selected command work in the background until it has finished its process. Background execution will only work for external commands (will NOT work for cd, clr, dir, environ, echo, help, pause, quit). Foreground execution occurs when the symbol "&" is not detected and signifies that the command will have the top priority and will be the only process running in the shell (it is forked() - the process is essentially cloned) until it has been completed.

        EXAMPLE
                <command> &  -  This will let any command work in the background and will show only until the process has been finished

                <command> - This will be the only process running and no other process in myshell will run before this command is finished (again some commands will use fork(), creating a child process that clones the process and changes the process id, leaving two processes, the parent and child)

COMMAND LINE PROMPT
        
        The command line prompt shows the current working directory the user is currently in, the current working directory (CWD) is the full path of the current location.

PEER-REVIEWED SOURCES I USED DURING THIS ASSIGNMENT

        1. W. Richard Stevens, M. (2013).Advanced Programming in the UNIX Environment. 3rd ed. New Jersey: Pearson Education.
                This book greatly helped me with the understanding of I/O (Chapter 3) and Processes (Chapter 8)
        2. Gustavo A. Junipero Rodriguez-Rivera and Justin Ennen, (2014). Introduction to Systems Programming: a Hands-on Approach.
                This book/webpage helped me kickstart the shell and understand a general layout for the shell I was going to create
        3. Michael Kerrisk, Oct (2010). The Linux Programming Interface. 1st ed. No Starch Press.
                Since the hardest part of this program for me was I/O, I used this resource to attain a better understanding of I/O (Chapter 4)
        4. William Stallings, Feb. (2014). Operating Systems, Internals and Design Principles. 8th ed. Pearsons
                This book was referenced by the lecturer and has much detail about a good shell structure and layout
        5. waitpid() manual page from IBM.com
                Used this manual page while researching the waitpid() function.

ANY OTHER RELEVANT INFORMATION:

        Name: Thomas Hazekamp
        Student ID: 20423602
        Module: CA216 - Operating Systems
        University: Dublin City University

        I have read and understood DCU's Academic Integrity Policy, I have given detailed reference to the authors of any material used to help with this project.
