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
        // Receive a message from the sender
        msgrcv(msg_id, &message, sizeof(message), 1, 0);
        printf("\nReceiver: %s", message.msg);

        // Check if the received message from the sender is "exit" to quit
        if (strcmp(message.msg, "exit") == 0) {
            break;
        }

        // Send the message back to the sender
        printf("\nReceiver: Enter a message to send: ");
        scanf("%s", message.msg);
        msgsnd(msg_id, &message, sizeof(message), 0);

        // Check if the sent message is "exit" to quit
        if (strcmp(message.msg, "exit") == 0) {
            break;
        }
    }

    return 0;
}