#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    //creating a child process
    pid_t child_pid;
    child_pid = fork();
    if (child_pid > 0) //it's a parent process
    {
        sleep(60); //pause for a minute
    }
    else //it's a child process
    {
        exit(0);
    }
    return 0;
}
//to find zombie process (while program is runing)
//open new terminal window and write this command:    top | grep zombie