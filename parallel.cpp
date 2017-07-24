#include <unistd.h>
#include <sys/types.h>
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
    pid_t pid;      // process id declared here

    /* Go through all the URLs */
    for(vector<string>::iterator urlIt = urls.begin();
        urlIt != urls.end(); ++urlIt)
    {
        /* Create a child */
        pid = fork();   // pid of new forked process


        /* Make sure the fork was a success */
        if(pid < 0)     // if process ID is negative, fork was unsuccessful
        {
            perror("fork: unsuccessful");   // output error
            exit(1);
        }
        /* The child code */
        if(pid == 0)    // here, child's code goes
        {

            fclose(stdout);
            fclose(stderr);

            /* Deploy wget */
            if(execlp("/usr/bin/wget", "child", urlIt->c_str(), NULL) < 0)      // replace current process with a new program
            {
                perror("execlp: unsuccessful");     // if execlp returns negative, call was unsuccessful
                exit(1);
            }
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
    ifstream urlFile("urls.txt");   // places file contents on stream

    /* The URL buffer */
    string urlBuffer;               // here links are temporarily stored

    /* Make sure the file was opened */
    if(!urlFile.is_open())
    {
        fprintf(stderr, "Failed to open the file");
        exit(1);
    }

    /* Read the entire file */
    while(!urlFile.eof())           // make sure we are not at the end of the file
    {
        /* Read the buffer */
        urlFile >> urlBuffer;       // capture one link

        /* Are we at the end of the file */
        if(!urlFile.eof())
            urls.push_back(urlBuffer);      // push each link into a vector of strings
    }

    /* Close the file */
    urlFile.close();
}

int main()
{


    vector<string> urls;    // declare a vector of type string

    /* Read the URLs */
    readUrls(urls);         // load the vector

    /* Create child processes */
    create_children(urls);

    /* Each child process gets exactly one wait call*/
    /* Allows each child process to simultaneously downloaded files*/
    for(int i = 0; i < urls.size(); i++){
        wait(NULL);
    }

    return 0;       // run was a success
}