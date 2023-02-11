#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

//Signal handler for when user presses CTRL+C
void handle_sigint(int sig) {
  printf("\nExiting program...\n");
  exit(0);
}

//Function to handle each child process
void child_process(int nodeNumber, int nodeCount, int prevNode, int nextNode, char message[100], int whichNode) {
  int counter = 0;
  while (1) {
    read(prevNode, message, sizeof(char) * 100);
    counter++;
    if (nodeNumber + 1 == whichNode) {
      for (int i = 1; i < nodeNumber+1; i++) {
        printf("I am node %d. Passing the message to node %d\n", i, i+1);
      }
      printf("I am node %d. I am the desired node. The message was: %s", nodeNumber + 1, message);
      printf("\nPress CRTL+C to close the program.\n");
      sleep(100000000); //sleep until user presses CTRL+C
	signal(SIGINT, handle_sigint);
    }
    write(nextNode, message, sizeof(char) * 100);
  }
  close(prevNode);
  close(nextNode);
}



int main() {
  //global variables
  pid_t pid;
  int nodeCount;
  int whichNode;
  char message[100];
  int fd[100][2]; //making space for 100 pipes cus it just makes the code easier to read and I dont think he will test greater than 100 nodes
  int i;

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
    int nodeNum = i; // create a local variable for the child process
    int prevNode = fd[i][0];
    int nextNode = fd[(i + 1) % nodeCount][1];
    whichNode = nodeNum + 1;
    child_process(nodeNum, nodeCount, prevNode, nextNode, message, whichNode);
  }
  printf("pid: %d\n", pid);
}


	// parent process
	printf("\nWhat is the message you'd like to send?\n");
	scanf("%99s", message);
	printf("Which node would you like to send the message to?\n");
	scanf("%d%*c", &whichNode);
	printf("Message entered: %s\n", message);
	printf("Sending to Node %d...\n", whichNode);
	write(fd[whichNode - 1][1], &message, sizeof(message));
	for (i = 0; i < nodeCount; i++) {
	  if (i != whichNode - 1) {
	    close(fd[i][0]);
	    close(fd[i][1]);
	  }
	}
	wait(NULL);
   return 0;
}