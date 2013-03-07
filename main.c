#include "hook.h"

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <getopt.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>

#define ENV_BYTES "PROCESSED_BYTES"

static unsigned int interval = 5;

unsigned long long counter = 0;
char running = 1;

static const struct option g_LongOpts[] = {
  { "interval", required_argument, 0, 'i' },
  { "help",     no_argument,       0, 'h' },
  { "hook",     required_argument, 0, 'H' },
  { "force",    no_argument,       0, 'f' },
  { 0, 0, 0, 0 }
};

void publishProgress(unsigned long long count) {
  char buffer[sizeof(count)];
  if (sprintf(buffer, "%lld", count)) {
    setenv(ENV_BYTES, buffer, 1);
#ifdef DEV
    fprintf(stderr, "So far I got %s bytes.\n", buffer);
#endif
    execute_hooks();
  }
}

void* thread(void* args) {
  unsigned long long tmpCount = counter;
  while (running) {
    sleep(interval);
    if (tmpCount != counter) {
      tmpCount = counter;
      publishProgress(tmpCount);
    }
  }
  return NULL;
}

static void usage() {
  printf("USAGE: redigo [options]\n");
  printf("Options are:\n");
  printf(" -i, --interval [seconds]\tAmount of seconds between the hooks. Defaults to 5.\n");
  printf(" -H, --hook [executable]\tProgram to execute to report our progress.\n");
  printf(" -f, --force\t\t\tForce execution without hooks.\n");
  printf(" -h, --help\t\t\tShow this help page.\n");
}

int main(int argc, char** argv) {
  int iArg, iOptIndex, tmp = -1;
  int force = 0;
  struct hook* hook;
  while ((iArg = getopt_long(argc, argv, "fhi:H:", g_LongOpts, &iOptIndex)) != -1) {
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
      case 'f':
        force = 1;
        break;
      default:
      case 'h':
        usage();
        return 0;
    }
  }
  if (!has_hooks()) {
    fprintf(stderr, "You didn't specifiy any hooks :/\n");
    if (!force)
      return 1;
  }
  unsetenv("DONE");
  int c;
  pthread_t thread_t;
  pthread_create(&thread_t, NULL, thread, NULL);
  while ((c = getchar()) != EOF) {
    putchar(c);
    counter++;
  }
  setenv("DONE","true",1);
  running = 0;
  publishProgress(counter);
  unsetenv("DONE");
  return 0;
}