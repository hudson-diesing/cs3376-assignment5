// *----------------------------------------------------------------------------*/
// *
// *      Programmer : Hudson Diesing
// * assignment numb : 5
// *    Program Desc : Change code to execute the following double pipe command: “ls -ltr | grep 3376 | wc –l”
// *					1- Use one parent and two children to do the work. Call the file TwoPipesTwoChildren.cpp.
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
	int childpid;

	char *ls_args[] = {(char *)"ls", (char *)"-ltr", NULL};
	char *grep_args[] = {(char *)"grep", (char *)"3376", NULL};
	char *wc_args[] = {(char *)"wc", (char *)"-l", NULL};
	
	// create one pipe to send the output of "ls" process to "grep" process
	int pipes_lstogrep[2];
	pipe(pipes_lstogrep);
	
	
	// fork the first child (to execute cat)
	if((childpid = fork()) == -1){
		perror("Error creating first child process");
		exit(1);
	}
	if (childpid == 0) {
		// replace cat's stdout with write part of 1st pipe
		dup2(pipes_lstogrep[1], 1);
		// close all pipes_lstogrep (very important!); end we're using was safely copied
		
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

			//int pipes_lstogrep[2];
			dup2(pipes_lstogrep[0],0);
			
			close(pipes_lstogrep[0]);
			close(pipes_lstogrep[1]);

			//int pipes_greptowc[2];			
			dup2(pipes_greptowc[1], 1);
			
			close(pipes_greptowc[0]);
			close(pipes_greptowc[1]);
			
			execvp(*grep_args, grep_args);

			exit(0);
		}
		else 
		// replace ls' stdout with write part of 1st pipe
		dup2(pipes_greptowc[0], 0);
		// close all pipes_greptowc (very important!); end we're using was safely copied
		
		close(pipes_greptowc[0]);
		close(pipes_greptowc[1]);
		close(pipes_lstogrep[0]);
		close(pipes_lstogrep[1]);
		
		execvp(*wc_args, wc_args);
					
		
	}
	return(0);

}