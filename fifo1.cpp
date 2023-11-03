#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#define FIFO_PATH "myfifo"
#define BUFFER_SIZE 100
int main() {
char message[BUFFER_SIZE];
int fifo_fd;
// Create or open the FIFO
mkfifo(FIFO_PATH, 0666);
fifo_fd = open(FIFO_PATH, O_RDWR);
if (fifo_fd == -1) {
perror("FIFO open failed");
return 1;
}
signal(SIGINT, SIG_IGN); // Ignore Ctrl+C to keep the communication running
while (1) {
printf("Process 1: Enter a message (or 'STOP' to quit): ");
fgets(message, sizeof(message), stdin);
// Send the message to Process 2
write(fifo_fd, message, strlen(message));
if (strcmp(message, "STOP\n") == 0) {
break;
}
// Receive and display the response from Process 2

read(fifo_fd, message, sizeof(message));
printf("Process 1 received: %s", message);
}
close(fifo_fd);
unlink(FIFO_PATH); // Remove the FIFO when done
return 0;
}