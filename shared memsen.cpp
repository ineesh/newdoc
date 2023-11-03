#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstdlib>

#define SHARED_MEMORY_NAME "/my_shared_memory"
#define MAX_MESSAGE_LENGTH 100

struct SharedData {
    char message[MAX_MESSAGE_LENGTH];
    bool stopFlag;
};

int main() {
    // Create and initialize shared memory
    int shm_fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }
    ftruncate(shm_fd, sizeof(SharedData));
    SharedData* sharedData = (SharedData*)mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    while (true) {
        std::cout << "Enter a message (or type 'exit' to stop): ";
        std::string message;
        std::getline(std::cin, message);

        if (message == "exit") {
            sharedData->stopFlag = true;
            break;
        }

        // Write the message to shared memory
        strncpy(sharedData->message, message.c_str(), MAX_MESSAGE_LENGTH);
        sharedData->stopFlag = false;
    }

    // Clean up shared memory
    munmap(sharedData, sizeof(SharedData));
    close(shm_fd);
    shm_unlink(SHARED_MEMORY_NAME);

    return 0;
}