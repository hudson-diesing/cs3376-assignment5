// *----------------------------------------------------------------------------*/
// *
// *      Programmer : Hudson Diesing
// * assignment numb : 5
// *    Program Desc : The following requirements should be implemented:
// *    					1- Source file will be called DynPipe.cpp, executable called dynpipe.
// *    					2- The piped commands to execute need to be passed as arguments to dynpipe,and not hardcoded.
// *    					3- The max number of arguments should not exceed 5, and not less than 2 (otherwise print an error
// *    					   message)
// *    					4- Each argument should be a unix/linux command with its parameters. The first argument will be the
// *    					   first to execute, followed by the second one, etc.. We will assume that only valid commands can be
// *    					   used, for simplicity
// *          Course : CS 3376.502 - C/C++ Programming in a UNIX Environment - S17
// *            Date : April 7, 2017
// *
// *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


//Tokerizer function that takes the command line input and returns a two-dimenstional character array of the command with its paramaters.
void getToken (char *command_args[], char *command) {
	char *temp_command = command;
	int i = 0;
	temp_command = strtok(temp_command, " ");
	while (temp_command != NULL) {
		command_args[i] = temp_command;
		temp_command = strtok(NULL, " ");
		if ( temp_command == NULL)
			command_args[i+1] = NULL;
		i++;
	}
	return;
}

int main(int argc, char **argv){



	//Initialize space for holding all of the arguments. Using a 3d array.
	int maxArgs = argc; int maxArgSections = 100; int maxCharacters = 100;
	char*** commands;
	
    // check for command line arguments, exit if less than 3 or more than 5
    if (argc < 3 || argc > 6) { 
       printf("Usage: %s “cmd1 -args” “cmd2 -args” ... “cmd5 -args”  with 2-5 commands\n", argv[0]);
        return(1);
    }	
	
	// begin memory allocation
	commands = new char**[maxArgs];
	for(int x = 0; x < maxArgs; ++x) {
		commands[x] = new char*[maxArgSections];
			for(int y = 0; y < maxArgSections; ++y) {
				commands[x][y] = new char[maxCharacters];
		}
	}	

	
	//Set commands equal to the argv commands from the command line.
	int i;
	int argCount = argc -1;
	for ( i = 0; argCount > i; i++){
		getToken(*(commands+i), argv[i+1]);
	}
	
	 //First child proccess id. Has to be executed no matter what.
	int childpid1;
	
	//Create first pipe from the first child.
	int pipes1[2];
	
	if (pipe(pipes1) == -1) {
		perror("bad pipes1");
    exit(1);
	}	

	// fork the first child (to execute first command)
	if((childpid1 = fork()) == -1){
		perror("Error creating first child process");
		exit(1);
	}
	if (childpid1 == 0) {
		
		//Replace stdout
		dup2(pipes1[1], 1);
		
		//closing uneeded pipe file descriptors.
		close(pipes1[0]);
		close(pipes1[1]);
	
		//Call first command
		if (argCount >= 1) {
			execvp(*commands[0], commands[0]);	
			
		}
		//Exit from the child
		exit(0);
	}
		
	else {
		
		
		//create pipes to second child
		int pipes2[2];
		if (pipe(pipes2) == -1) {
			perror("bad pipes2");
		exit(1);
	}
		//create second child proccess id
		int childpid2;	
			
		if((childpid2 = fork()) == -1){
			perror("Error creating second child process");
			exit(1);
		}
		
		if (childpid2 == 0) {
			
			if (argCount == 2) {
				
				dup2(pipes1[0], 0);
				
				close(pipes1[0]); close(pipes1[1]); close(pipes2[0]); close(pipes2[1]);
				
				execvp(*commands[1], commands[1]);
			}	
			else {
				//duplicating pipe ends to allow it to send input from one side to the other.
				dup2(pipes1[0], 0);
				dup2(pipes2[1], 1);
				
				//closing uneeded pipe file descriptors.
				close(pipes1[0]); close(pipes1[1]); close(pipes2[0]); close(pipes2[1]);
				
				//if there are only 2 arguments then only one child is needed
				if (argCount >= 2) {
					execvp(*commands[1], commands[1]);
				}
				exit(0);
			}
		}
		else {		
			//create pipes to third child
			int pipes3[2];
			pipe(pipes3);	
			
			//create third child proccess id
			int childpid3;	
				
			if((childpid3 = fork()) == -1){
				perror("Error creating third child process");
				exit(1);
			}
			
			if (childpid3 == 0) {
				if (argCount == 3) {
					
					dup2(pipes2[0], 0);
					
					close(pipes1[0]); close(pipes1[1]); close(pipes2[0]); close(pipes2[1]); close(pipes3[0]); close(pipes3[1]);
					execvp(*commands[2], commands[2]);
				}
				else {				
					//duplicating pipe ends to allow it to send input from one side to the other.
					dup2(pipes2[0], 0);
					dup2(pipes3[1], 1);
					
					//closing uneeded pipe file descriptors.
					close(pipes1[0]); close(pipes1[1]); close(pipes2[0]); close(pipes2[1]); close(pipes3[0]); close(pipes3[1]);
					
					//if there are only 3 arguments then only two children are needed
					if (argCount >= 3) {
						execvp(*commands[2], commands[2]);
					}
				}
				exit(0);
			}
			else {
				//create pipes to third child
				int pipes4[2];
				pipe(pipes4);	
				
				//create third child proccess id
				int childpid4;	
					
				if((childpid4 = fork()) == -1){
					perror("Error creating forth child process");
					exit(1);
				}
				
				if (childpid4 == 0) {
					if (argCount == 4){
						
						dup2(pipes3[0], 0);
						
						//closing all pipes
						close(pipes1[0]); close(pipes1[1]);	 close(pipes2[0]); close(pipes2[1]); close(pipes3[0]); close(pipes3[1]); close(pipes4[0]); close(pipes4[1]);	
						execvp(*commands[3], commands[3]);
					}	
				else {
					//duplicating pipe ends to allow it to send input from one side to the other.
					dup2(pipes3[0], 0);
					dup2(pipes4[1], 1);
					
					//closing uneeded pipe file descriptors.
					close(pipes1[0]); close(pipes1[1]);	 close(pipes2[0]); close(pipes2[1]); close(pipes3[0]); close(pipes3[1]); close(pipes4[0]); close(pipes4[1]);	
					
					//if there are only 4 arguments then only three children are needed.
					if (argCount >= 4){
						execvp(*commands[3], commands[3]);
					}
				}
				exit(0);
				}
				else {
					int childpid5;
					if((childpid5 = fork()) == -1){
						perror("Error creating fifth child process");
						exit(1);
					}
					if (childpid5 == 0) {
						
						//Replace stdout to display any output.
						dup2(pipes4[0], 0);
						
						//closing uneeded pipe file descriptors.
						close(pipes1[0]); close(pipes1[1]);	 close(pipes2[0]); close(pipes2[1]); close(pipes3[0]); close(pipes3[1]); close(pipes4[0]); close(pipes4[1]);	

						if (argCount == 5) {
							execvp(*commands[4], commands[4]);
						}
						exit(0);
					}				
				}	
			}		 
		}
		//Used to ensure the parrent doesnt finish first.
		sleep(1);	
	}
	return(0);
}