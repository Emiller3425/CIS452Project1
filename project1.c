#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    //global variables
    int fd[3][2]; //3 nodes, will give the input value as the size of the array after its working for 3
    pid_t pid;
    int nodeCount;
    int whichNode;
    char message[100];

    //getting input
    printf("How Many Nodes?\n");
    scanf("%d%*c", &nodeCount);
    printf("You entered: %d\n", nodeCount);
    //create pipe
    for (int i = 0; i < 3; i++) {
        if (pipe(fd[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }
    //creating child processes
    for (int i = 0; i < 3; i++) {
        pid = fork();
        if (pid == 0) {
            break;
        }
    }
    printf("pid: %d", getpid());

    //establishing read and write connections might be wrong havnt tested throuroughly
    if (pid == 0) {
        //need to add ifs for each child process
        //probably some sort of wait for the parent to write to the pipe
    } else {
        close(fd[0][0]);
        close(fd[3-1][0]);
        close(fd[3-1][1]);

        printf("What is the message you'd like to send?\n");
        scanf("%s%*c", &message);
        printf("Which node would you like to send the message to?\n");
        scanf("%d%*c", &whichNode);
        printf("message entered: %s\n", message);
        printf("to be sent to: %d\n", whichNode);

        write(fd[whichNode][1], &message, sizeof(message));

        for (int i = 0; i < 3; i++) {
            wait(NULL);
        }
    }
    return(0);
}