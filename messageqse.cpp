#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msg_buffer {
    long msg_type;
    char msg[100];
} message;

int main() {
    key_t my_key;
    int msg_id;
 my_key = ftok("progfile", 65); // Used to create a unique key
    msg_id = msgget(my_key, 0666 | IPC_CREAT); // Create or access the message queue
    message.msg_type = 1;

    while (1) {
        // message to be sent to the receiver
        printf("\nSender: Enter a message to send (or 'exit' to quit): ");
        fgets(message.msg, sizeof(message.msg), stdin);

        // Send the message to the receiver
        msgsnd(msg_id, &message, sizeof(message.msg), 0);

        // Check if the message is "exit" to quit
        if (strcmp(message.msg, "exit\n") == 0) {
            break;
        }
    }

    return 0;
}