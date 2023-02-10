#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    //global variables
    pid_t pid;
    int nodeCount;
    int whichNode;
    char message[100];
    int fd[100][2]; //making space for 100 pipes cus it just makes the code easier to read and I dont think he will test greater than 100 nodes
    int i;
    int count = 0;

    //getting input
    printf("How Many Nodes?\n");
    scanf("%d%*c", &nodeCount);
    printf("You entered: %d\n", nodeCount);
    //create pipes
    for (i = 0; i < nodeCount; i++) {
        if (pipe(fd[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }
    printf("\nChild Processes Started:\n");
    //creating child processes
    for (i = 0; i < nodeCount; i++) {
        pid = fork();
        if (pid == 0) {
            break;
        }
       printf("pid: %d\n", pid);
    }
    
    i = 0;

    //establishing read and write connections
    if (pid == 0) {
    // child process
 
    int nextNode = (i + 1) % nodeCount;
    int prevNode = (i + nodeCount - 1) % nodeCount;
    close(fd[prevNode][1]);
    close(fd[nextNode][0]);

    while (1) {
        read(fd[prevNode][0], &message, sizeof(message));
        printf("Node %d received message: %s\n", i + 1, message);
        count++;
        if (count == whichNode) {
            printf("I am node %d. I am the desired node, exiting program.\n", i + 1);
            exit(0);
        }
        sleep(1);
        printf("Node %d passing the message to node %d...\n", i + 1, nextNode + 1);
        write(fd[nextNode][1], &message, sizeof(message));
    	}
    } else {
    // parent process
        printf("\nWhat is the message you'd like to send?\n");
        scanf("%99s", message);
        printf("Which node would you like to send the message to?\n");
        scanf("%d%*c", &whichNode);
        printf("Message entered: %s\n", message);
        printf("Sending to Node %d...\n", whichNode);
        write(fd[whichNode][1], &message, sizeof(message));
        for (i = 0; i < whichNode; i++) {
            close(fd[i][0]);
            close(fd[i][1]);
        }
        for (i = 0; i < whichNode; i++) {
            wait(NULL);
        }
    }
    return(0);
}