#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    //global variables
    int fd[3];
    pid_t pid;
    int nodeCount;
    int whichNode;

    //getting input
    printf("How Many Nodes?\n");
    scanf("%d%*c", &nodeCount);
    printf("You entered: %d\n", nodeCount);
    printf("Which node would you like to send the message to?\n");
    scanf("%d%*c", &whichNode);
    printf("You entered: %d\n", whichNode);

    //create pipe
    int pipeCreationResult = pipe(fd);
    //check if it was succesful
    if (pipeCreationResult < 0) {
        perror("Pipe creation failed");
        exit(1);
    }
    //creating child processes
    for (int i = 0; i < 3-1; i++) {
        printf("%d\n", getpid());
        pid = fork();
        if (pid == 0) {
            printf("Child Process %d with pid %d\n", i+1, getpid());
            break;
        } else {
            printf("Parent Process with pid %d\n", getpid());
        }
    }




    return 0;
}