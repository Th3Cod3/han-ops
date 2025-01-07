#include "Queue.h"
#include <bits/pthreadtypes.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PRODUCERS 3

data_t data = {1, "Hello queue"};
queue_t queue = {NULL}; // Note: element of queue = NULL
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
FILE *file;

void *pthread_producer_interval(void *arg) {
  int interval = *((int *)arg);
  data_t _data = data;

  while (1) {
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
}

void save_queue_in_file() {
  while (!emptyQueue(&queue)) {
    pthread_mutex_lock(&mtx);
    data_t _data = *backQueue(&queue);
    popQueue(&queue);
    pthread_mutex_unlock(&mtx);

    fprintf(file, "%s\n", _data.text);
  }
  fflush(file);
}

int main(void) {
  file = fopen("queue.txt", "w+");
  if (file == NULL) {
    perror("fopen");
    exit(1);
  }

  pthread_t producers[NUM_PRODUCERS];
  int producersDelays[NUM_PRODUCERS] = {2, 3, 4};

  for (int i = 0; i < NUM_PRODUCERS; i++) {
    pthread_create(&producers[i], NULL, pthread_producer_interval, &producersDelays[i]);
  }

  while (1) {
    sleep(15);

    save_queue_in_file();
  }

  fclose(file);
  deleteQueue(&queue);

  return 0;
}
