#!/bin/bash
echo "I'm a shell script and you have ${PROCESSED_BYTES} bytes.." 1>&2
if [ "$DONE" == "true" ]; then
  echo "And we're done.." 1>&2
fi
