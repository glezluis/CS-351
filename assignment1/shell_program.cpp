#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;


int main()
{
    /* The command buffer */
    string cmdBuff;

    /* The ID of the child process */
    pid_t pid;

    /* Keep running until the user has typed "exit" */
    do
    {
        /* Prompt the user to enter the command */
        cerr << "cmd>";
        cin >> cmdBuff;     // store the command here

        /* If the user wants to exit */
        if(cmdBuff != "exit")
        {
            /* TODO: Create a child */
            pid = fork();   // creates a new child process with that pid

            /* TODO: Error check to make sure the child was successfully created */
            if(pid < 0){
                perror("fork: unsuccessful");   // print error that fork was unsuccessful
                exit(1);
            }

            /*** TODO: If I am child, I will do this: ****/
            /* Call execlp() to replace my program with that specified at the command line.
             * PLEASE NOTE: YOU CANNOT PASS cmdBuff DIRECTLY to execlp(). It is because
             * cmdBuff is an object of type string (i.e., a class) and execlp() expects
              * an array of characters.  However, you can pass cmdBuff.c_str(), which will
             * return an array of characters representation of the string object.
             * Also, please do not forget to error check your exelp() system calls.
             */

	    // this is where child process's instructions are written
 	    // if execlp system call returns negative, call was unsuccessful
            if((pid == 0) && (execlp(cmdBuff.c_str(), cmdBuff.c_str(), NULL) < 0)){                     
            	perror("execlp: unsuccessful");
                exit(1);
            }else{

                wait(NULL);     // if not a child, then must be a parent; parent waits here for child to terminate

            }
        }
    }
    while(cmdBuff != "exit");       // while command was not 'exit' continue the loop from above

    return 0;                       // program was successful
}
