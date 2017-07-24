#include <unistd.h>
#include <sys/types.h> // ?? //
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fstream>
#include <vector>
using namespace std;

/**
 * Forks children and lets them perform their tasks
 * @param urls - the urls to download
 */
void create_children(vector<string>& urls)
{
    /* The process id */
    pid_t pid;      // process ID is declared here

    /* Go through all the URLs */
    for(vector<string>::iterator urlIt = urls.begin();
        urlIt != urls.end(); ++urlIt)
    {
        /* Create a child */
        pid = fork();       // create a new child process


        /* Make sure the fork was a success */
        if(pid < 0)     // negative pid indicate fork was unsuccessful
        {
            perror("fork:");    // print error
            exit(1);
        }
        /* The child code */
        if(pid == 0)    // child process's code goes below
        {

            fclose(stdout);
            fclose(stderr);

            /* Deploy wget */
            if(execlp("/usr/bin/wget", "child", urlIt->c_str(), NULL) < 0)      // replace current process's space with a new program
            {                                                                   // also display error if execlp call returns negative
                perror("execlp: unsuccessful");     // print error
                exit(1);
            }

        }else{

            wait(NULL);     // parent wait until child terminate

        }
    }
}

/**
 * Read the URLs from the file
 * @param urls - the URLs to download
 */

void readUrls(vector<string>& urls)
{
    /* Open the file */
    ifstream urlFile("urls.txt");   // open the file on stream

    /* The URL buffer */
    string urlBuffer;               // stores a link temporarily

    /* Make sure the file was opened */
    if(!urlFile.is_open())
    {
        fprintf(stderr, "Failed to open the file");
        exit(1);
    }

    /* Read the entire file */
    while(!urlFile.eof())           // if not at end of file
    {
        /* Read the buffer */
        urlFile >> urlBuffer;       // store a link in buffer

        /* Are we at the end of the file */
        if(!urlFile.eof())
            urls.push_back(urlBuffer);      // push the links to the vector
    }

    /* Close the file */
    urlFile.close();
}

int main()
{
    vector<string> urls;    // declare vector of strings

    /* Read the URLs */
    readUrls(urls);         // load vector

    /* Create child processes */
    create_children(urls);  // fork a child for each link

    return 0;   // run was a success
}