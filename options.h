#ifndef _OPTIONS_H
#define _OPTIONS_H

#include <getopt.h>

static unsigned int interval;
static char quiet;

static const struct option g_LongOpts[] = {
  { "interval", required_argument, 0, 'i' },
  { "help",     no_argument,       0, 'h' },
  { "hook",     required_argument, 0, 'H' },
  { "force",    no_argument,       0, 'f' },
  { "quiet",    no_argument,       0, 'q' },
  { 0, 0, 0, 0 }
};

#endif //_OPTIONS_H