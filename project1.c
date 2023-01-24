#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    //global variables
    int fd[4]; //3 nodes, will give the input value as the size of the array after its working for 3
    pid_t pid;
    int nodeCount;
    int whichNode;
    char message[] = "Apple";

    //getting input
    printf("How Many Nodes?\n");
    scanf("%d%*c", &nodeCount);
    printf("You entered: %d\n", nodeCount);
    //create pipe
    int pipeCreationResult = pipe(fd);
    //check if it was succesful
    if (pipeCreationResult < 0) {
        perror("Pipe creation failed");
        exit(1);
    }
    //creating child processes
    for (int i = 0; i < 3; i++) {
        pid = fork();
        if (pid == 0) {
            break;
        }
    }

    if (pid > 0) {
        printf("Which node would you like to send the message to?\n");
        scanf("%d%*c", &whichNode);
        printf("You entered: %d\n", whichNode);
        //wait of somesort
    } else {
        
    }
    //establishing read and write connections might be wrong havnt tested throuroughly
    if (pid > 0) {
        write(fd[1], message, sizeof(message));
        read(fd[3], &message, sizeof(message));
    } else {
        if (whichNode == 1) {
            read(fd[0], &message, sizeof(message));
            write(fd[2], message, sizeof(message));
            printf("Child wrote: [%s]\n", message);
            exit(1);
        } else {

        }
        if (whichNode == 2) {
            read(fd[1], &message, sizeof(message));
            write(fd[3], message, sizeof(message));
            printf("Child wrote: [%s]\n", message);
            exit(1);
        } else {

        }
        if (whichNode == 3) {
            read(fd[2], &message, sizeof(message));
            write(fd[0], message, sizeof(message));
            printf("Child wrote: [%s]\n", message);
            exit(1);
        } else {
            printf("Invalid Node Number\n");
        }
    }
}