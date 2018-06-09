------------------------------------------------------------------------------------------Documnetation for NSH---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

A. Introduction: 

	NSH is a basic shell created for Unix like operating systems. It implements basic functionality of the shell like executing commands, redirecting output and also have implemented piping of commands. It can be compiled by using the Makefile provided to the user with this package. Please refer to the Makefile section for the various build/clean options. Please refer to the Functionality section to know more about how to use the shell. Thank you for using NSH.

B. Functionality:

	As discussed above, NSH provides the user with 4 basic functionalities.

	1. Execute Commands: NSH will allow you to execute any valid command that is captured by the PATH enviornment variable. This means that you can simple type 'ls' or 'ps' instead of typing the entire path to the location of the command. As NSH uses execvpe to execute the command, under normal and successful execution, it is expected to not return from the execvpe function call. In case of an error, where execvpe does return, an error message shall be printed notifying the user of the error occured. As NSH doesn't support quoting or back-quotes for that matter, any command in back-quotes will fail.

	2. Redirection Commands: NSH allows you to redirect output. There are three ways you can do that. Firstly, you can redirect the input to read the input from the file using the '<' character rather than the stdin stream. This means that after the input is redirected for the lifetime of the command, the input will switch back to stdin. If the input file doesn't exist, it will print out an error message for the user, so please make sure that the file you are trying to read from does exists in the current working directory. Secondly, you can redirect the output of a command to any file of your choice using the '>' character. If the file doesn't exist, NSH will create it for you and then populate it with the data you redirect from the command. If the file already exists, it will clear the contents of the file and then write to it. PLease make sure to backup any sensitive data before writing to a file. NSH doesn't assume the file to be stdout if you dont pass a valid argument to it, please pass a valid filename after the '>' character. Lastly, if you want to append to a file meaning that redirect to an existing file without clearing it's content, use the '>>' string with a valid file name following it. This command will also create a file if it doesn't exist. NSH will redirect ouput of the command and will write to the file designated by the argument, starting at the EOF in the file.

	3. Piped Commands: NSH allows you to pipe commands eliminating the problem of creating multiple temporary files to communicate between processes. This substantially improves user efficeincy as well as prevents the fragmentation of hard-disks. to redirect the output of one command to another, simple insert '|' character between the two commands. If the any of the commands in the pipeline fails while execution, NSH will throw a broken pipe error and will return to it's normal state again. Spaces between commands and the pipe characters are irrelevant and should work all the same.

	4. Changing Directory: NSH allows you to change the directory at you will. Since the prompt for NSH doesn't indicate which directory the user is currently in`, it is upto the user to keep track of the directory they are working in. Alternatively, the user can type the command 'pwd' or 'cd -c' to query the current working diesctory. Change the current working directory using 'cd' command followed by a valid directory. Note that NSH does not recognize '~/' directory as '/home/user/' however it does recognize '.' and '..' directories. Please use the full path name while switching between directories. Alternatively, NSH does print the current working directory and previous working directory every time you switch between directories. To go to the previous working directory simple type 'cd -p' and to go to the home directory i.e. '/home/user' directory, type 'cd -d'. Note that NSH doesn't assume that cd all by itself means 'cd /home/user'. The user needs to pass a valid directory while switching to that directory.

C. Extra functionality:

	1. Multiple pipes: NSH allows you to have a pipeline of more than two commands. Insert the '|' character between each command. Logically, the left side of command will be executed and it's output will be directed to the right side of the pipe.

	2. Exit command: NSH will exit after the user types 'exit' into the shell. NSH will exit with the most recently generated exit code of the most recently executed command. However, if exit is called with an integer argument, it will exit with the code as the integer.

	3. Handling SIGINT: NSH handles sigint in all the child processes it spawns. However it ignores sigint in the parent process. This enables the user to terminate any command that is faulty/buggy or take enormous amount of time to process.

D. Documented bugs:
	
	1. Redirection input in pipe does follow right to left order. Meaning that for example if you type "cat < input_file | wc -l < input_file2", wc -l will ignore the piped input from cat and will print "wc -l < input_file2"'s output. Similarly, output redirection follows left to right order meaning that for example if you type "ls > one.txt | wc -l > one.txt", one.txt will contain the output of ls rather than the output of ls | wc -l.

	2. Directly running scripts: Please be careful while directly running scripts on NSH i.e. (./shell script). It loops into an infinite loop and continuously executes the script over and over again. If possible, run the script after starting the shell. Please make sure that the execute bit on the script is set before executing the script.

E. Makefile:

	Makefile has two basic options:

	1. default(build): Type 'make default' or 'make' in your terminal to build the shell, it ouputs to an executable file named 'shell'.
	2. clean: Type 'make clean' to clean the solution that was built. This will clear all the temporary files created while running/compiling the shell.
