#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

unsigned long long counter = 0;
char running = 1;
pthread_mutex_t thread_mutex;

void* thread(void* args) {
  while (running) {
    fprintf(stderr, "So far I got %lld bytes.\n", counter);
    sleep(5);
  }
  return NULL;
}

int main(int argc, char** argv) {
  int c;
  pthread_mutex_init(&thread_mutex, NULL);
  pthread_t thread_t;
  pthread_create(&thread_t, NULL, thread, NULL);
  while ((c = getchar()) != EOF) {
    putchar(c);
    counter++;
  }
  running = 0;
  printf("I got a total of %lld bytes.\n", counter);
  return 0;
}