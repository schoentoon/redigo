#include "hook.h"
#include "options.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

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
    struct hook* node = hooks;
    while (node) {
      int output = system(node->executable);
      if (output != 0 && !quiet)
        fprintf(stderr, "Hook: %s exited with non-zero status %d", node->executable, output);
      node = node->next;
    }
  }
}

int has_hooks()
{
  return (hooks ? 1 : 0);
}