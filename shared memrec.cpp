#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <cstdlib>

#define SHARED_MEMORY_NAME "/my_shared_memory"
#define MAX_MESSAGE_LENGTH 100

struct SharedData {
    char message[MAX_MESSAGE_LENGTH];
    bool stopFlag;
};

int main() {
    // Open the existing shared memory
    int shm_fd = shm_open(SHARED_MEMORY_NAME, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }
    SharedData* sharedData = (SharedData*)mmap(NULL, sizeof(SharedData), PROT_READ, MAP_SHARED, shm_fd, 0);

    while (true) {
        // Check if the stopFlag is set
        if (sharedData->stopFlag) {
            break;
        }

        // Read and display the message from shared memory
        std::cout << "Received: " << sharedData->message << std::endl;

        // Sleep for a while to avoid busy-waiting
        usleep(500000);  // Sleep for 0.5 seconds
    }

    // Clean up shared memory
    munmap(sharedData, sizeof(SharedData));
    close(shm_fd);

    return 0;
}