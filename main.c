#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void destruct();

void signal_handler(int signal) {
    printf("\nCaught signal %d\n", signal);
    destruct();
    exit(1);
}

void wait_ms(int milliseconds) {
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;

    while (nanosleep(&ts, &ts) == -1 && errno == EINTR);
}

#define SHMSZ sizeof(double)
key_t key;
int shmid;
double *shm, *s;


void init_connector() {
    // Generate a key for the shared memory segment
    key = ftok("/temp/", 'a');

    // Create the shared memory segment
    shmid = shmget(key, SHMSZ, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    // Attach the shared memory segment to our data space
    shm = (double *) shmat(shmid, NULL, 0);
    if (shm == (double *) -1) {
        perror("shmat");
        exit(1);
    }

    s = shm;
    printf("Connection on\n");
}

void destruct() {
    shm[0] = 0;
    // Detach the shared memory segment from our data space
    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(1);
    }

    printf("Connection off\n");
}

int main() {
    int connection = 1;
    init_connector();
    atexit(destruct);
    signal(SIGINT, signal_handler); // register handler for Ctrl+C
    signal(SIGTERM, signal_handler); // register handler for termination signal

    s = shm;
    s[0] = 1;

    double p = 0, y = 0, r = 0;

    double i = 0.01;

    while(connection) {
        i += 0.0001;
        // Write some values to the shared memory segment
        r = i;
        y = i/2;
        s[1] = p;
        s[2] = y;
        s[3] = r;
        wait_ms(0.001);
    }

    return 0;
}
