// *----------------------------------------------------------------------------*/
// *
// *      Programmer : Hudson Diesing
// * assignment numb : 5
// *    Program Desc : Change code to execute the following double pipe command: “ls -ltr | grep 3376 | wc –l”
// *					2- Write another version where the parent create 3 children, and the children will execute the
// *					   commands (parent will do nothing, just lay in the sofa ! ). Call the file
// *					   TwoPipesThreeChildren.cpp
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


int main(int argc, char **argv){
	
	//int status; //unused variable
	int childpid1;

	//I cast the args to remove the annoying warning.
	char *ls_args[] = {(char *)"ls", (char *)"-ltr", NULL};
	char *grep_args[] = {(char *)"grep",(char *) "3376", NULL};
	char *wc_args[] = {(char *)"wc", (char *)"-l", NULL};
	
	// create one pipe to send the output of "ls" process to "grep" process
	int pipes_lstogrep[2];
	pipe(pipes_lstogrep);
	
	
	// fork the first child (to execute cat)
	if((childpid1 = fork()) == -1){
		perror("Error creating first child process");
		exit(1);
	}
	if (childpid1 == 0) {
		//replacing ls' stdout with pipe
		dup2(pipes_lstogrep[1], 1);
		
		//closing uneeded pipe file descriptors.
		close(pipes_lstogrep[0]);
		close(pipes_lstogrep[1]);
		
		execvp(*ls_args, ls_args);	
		
		exit(0);
	}
		
	else {
		int pipes_greptowc[2];
		pipe(pipes_greptowc);	
		int childpid2;	
			
		if((childpid2 = fork()) == -1){
			perror("Error creating second child process");
			exit(1);
		}
		
		if (childpid2 == 0) {

			//duplicating pipe ends to allow it to send input from one side to the other.
			dup2(pipes_lstogrep[0],0);
			dup2(pipes_greptowc[1], 1);
			
			//closing uneeded pipe file descriptors.
			close(pipes_lstogrep[0]);
			close(pipes_lstogrep[1]);			
			close(pipes_greptowc[0]);
			close(pipes_greptowc[1]);
			
			execvp(*grep_args, grep_args);

			exit(0);
		}
		else{		
			
			int childpid3;
			if((childpid3 = fork()) == -1){
				perror("Error creating third child process");
				exit(1);
			}
			
			if (childpid3 == 0) {

				// replace ls' stdout with write part of 1st pipe
				dup2(pipes_greptowc[0], 0);
				
				//closing uneeded pipe file descriptors.
				close(pipes_greptowc[0]);
				close(pipes_greptowc[1]);
				close(pipes_lstogrep[0]);
				close(pipes_lstogrep[1]);
				
				execvp(*wc_args, wc_args);
				
				exit(0);
			}
			//Used to insure the parrent doesnt finish first.
			sleep(1);
		}		 
	}
	return(0);
}