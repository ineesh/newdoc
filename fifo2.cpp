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
// Open the existing FIFO
fifo_fd = open(FIFO_PATH, O_RDWR);
if (fifo_fd == -1) {
perror("FIFO open failed");
return 1;
}
signal(SIGINT, SIG_IGN); // Ignore Ctrl+C to keep the communication running
while (1) {
// Receive and display the message from Process 1
read(fifo_fd, message, sizeof(message));
printf("Process 2 received: %s", message);
if (strcmp(message, "STOP\n") == 0) {
break;
}
printf("Process 2: Enter a response (or 'STOP' to quit): ");
fgets(message, sizeof(message), stdin);
// Send the response back to Process 1
write(fifo_fd, message, strlen(message));
}
close(fifo_fd);

return 0;
}