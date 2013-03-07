#include "hook.h"
#include "options.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

struct hook *hooks = NULL;

struct hook* new_hook()
{
  struct hook* output = malloc(sizeof(struct hook));
  output->next = NULL;
  if (!hooks)
    hooks = output;
  else {
    struct hook* node = hooks;
    while (node->next)
      node = node->next;
    node->next = output;
  }
  return output;
}

void execute_hooks()
{
  if (hooks) {
    if (fork() == 0) {
      struct hook* node = hooks;
      while (node) {
        int output = system(node->executable);
        if (output != 0 && !quiet)
          fprintf(stderr, "Hook: %s exited with non-zero status %d", node->executable, output);
        node = node->next;
      }
      exit(0);
    }
  }
}

int has_hooks()
{
  return (hooks ? 1 : 0);
}