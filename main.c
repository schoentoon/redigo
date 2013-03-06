#include "hook.h"

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <getopt.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>

#define ENV_BYTES "PROCESSED_BYTES"

static int interval = 5;

unsigned long long counter = 0;
char running = 1;
pthread_mutex_t thread_mutex;

static const struct option g_LongOpts[] = {
  { "interval", required_argument, 0, 'i' },
  { "help",     no_argument,       0, 'h' },
  { "hook",     required_argument, 0, 'H' },
  { 0, 0, 0, 0 }
};

void publishProgress(unsigned long long count) {
  char buffer[50];
  if (sprintf (buffer, "%lld", count)) {
    setenv(ENV_BYTES, buffer, 1);
#ifdef DEV
    fprintf(stderr, "So far I got %s bytes.\n", buffer);
#endif
    execute_hooks();
  }
}

void* thread(void* args) {
  while (running) {
    sleep(interval);
    publishProgress(counter);
  }
  return NULL;
}

static void usage() {
  printf("USAGE: redigo [options]\n");
  printf("Options are:\n");
  printf(" -i, --interval [seconds]\tAmount of seconds between the hooks.\n");
  printf(" -H, --hook [executable]\tProgram to execute to report our progress.\n");
  printf(" -h, --help\t\t\tShow this help page.\n");
}

int main(int argc, char** argv) {
  int iArg, iOptIndex, tmp = -1;
  struct hook* hook;
  while ((iArg = getopt_long(argc, argv, "i:hH:", g_LongOpts, &iOptIndex)) != -1) {
    switch (iArg) {
      case 'H':
        hook = new_hook();
        hook->executable = optarg;
        break;
      case 'i':
        tmp = strtol(optarg, NULL, 10);
        if ((errno == ERANGE || (tmp == LONG_MAX || tmp == LONG_MIN)) || (errno != 0 && tmp == 0) || tmp < 0) {
          fprintf(stderr, "--interval requires a positive amount of seconds.\n");
          return 1;
        }
        interval = tmp;
        break;
      default:
      case 'h':
        usage();
        return 0;
    }
  }
  unsetenv("DONE");
  int c;
  pthread_mutex_init(&thread_mutex, NULL);
  pthread_t thread_t;
  pthread_create(&thread_t, NULL, thread, NULL);
  while ((c = getchar()) != EOF) {
    putchar(c);
    counter++;
  }
  setenv("DONE","true",1);
  publishProgress(counter);
  running = 0;
  return 0;
}