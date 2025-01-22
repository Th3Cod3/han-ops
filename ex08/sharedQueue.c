#include "Queue.h"
#include <bits/pthreadtypes.h>
#include <stdbool.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_PRODUCERS 3

enum sq_status {
  // Before the threads are created
  SQ_STATUS_STARTING,
  // After the threads are created
  SQ_STATUS_RUNNING,
  // When the user sends SIGINT
  SQ_STATUS_EXITING,
  // After all the producers have finished
  SQ_STATUS_STOPPED
};

// status is modified by the main process and read by the consumer
volatile sig_atomic_t status = SQ_STATUS_STARTING;
data_t data = {1, ""};
queue_t queue = {NULL}; // Note: element of queue = NULL
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
FILE *file;

static inline int min(unsigned long a, unsigned long b) {
  return (a < b) ? a : b;
}

void *pthread_producer_interval(void *arg) {
  int interval = *((int *)arg);
  data_t _data = data;

  while (status != SQ_STATUS_EXITING) {
    sleep(interval);
    pthread_mutex_lock(&mtx);
    data.intVal++;
    pthread_mutex_unlock(&mtx);

    snprintf(_data.text, TEXT_SIZE, "v: %d, i: %d", data.intVal, interval);

    pthread_mutex_lock(&mtx);
    emptyQueue(&queue)
      ? createQueue(&queue, _data)
      : pushQueue(&queue, _data);
    pthread_mutex_unlock(&mtx);
  }

  return NULL;
}

void save_queue_in_file() {
  while (!emptyQueue(&queue)) {
    pthread_mutex_lock(&mtx);
    data_t _data = *frontQueue(&queue);
    popQueue(&queue);
    pthread_mutex_unlock(&mtx);

    fprintf(file, "%s\n", _data.text);
    write(STDOUT_FILENO, strcat(_data.text, "\n"), min(strlen(_data.text) + 1, TEXT_SIZE));
  }

  fflush(file);
}

void *ptread_consumer() {
  while (status != SQ_STATUS_STOPPED) {
    sleep(15);

    save_queue_in_file();
  }

  return NULL;
}

static void handler(int signum) {
  if (signum == SIGINT) {
    status = SQ_STATUS_EXITING;
  }
}

int main(void) {
  struct sigaction sa;

  sa.sa_handler = handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  if (sigaction(SIGINT, &sa, NULL) == -1) {
    printf("Error setting signal handler\n");

    return 1;
  }

  file = fopen("queue.txt", "w+");
  if (file == NULL) {
    perror("fopen");
    exit(1);
  }

  pthread_t producers[NUM_PRODUCERS];
  int producersDelays[NUM_PRODUCERS] = {2, 3, 4};

  for (int i = 0; i < NUM_PRODUCERS; i++) {
    int retval = pthread_create(&producers[i], NULL, pthread_producer_interval, &producersDelays[i]);

    if (retval != 0) {
      perror("pthread_create");
      exit(1);
    }
  }

  pthread_t consumer;
  pthread_create(&consumer, NULL, ptread_consumer, NULL);

  for (int i = 0; i < NUM_PRODUCERS; i++) {
    int retval = pthread_join(producers[i], NULL);

    if (retval != 0) {
      perror("pthread_join");
    }
  }

  status = SQ_STATUS_STOPPED;

  pthread_join(consumer, NULL);
  pthread_mutex_destroy(&mtx);

  fclose(file);
  deleteQueue(&queue);

  return 0;
}
