Redigo
======

[![Build Status](https://travis-ci.org/schoentoon/redigo.png)](https://travis-ci.org/schoentoon/redigo)

Simple application that allows you to execute hooks based upon data piped to it. The options to this program are fairly simple.
-i, --interval allows you to adjust the time between the hooks.
-f, --force allows you to run without any hooks, I don't see why you would want this though.
-H, --hook allows you to specify a hook to execute, you can use multiple hooks.

This program can have quite some uses, you could use it to report the progress of some long running progress to a database/web application by just putting it in the middle of it. Think about something like the following.

ssh user@some.host pg_dump -F t somedatabase | bzip -9 -z -c | redigo -i 10 -H report_progress.sh > backup.tar.bzip2

Or for receiving a file using netcat for exampe.

nc 192.168.0.1 4444 | redigo -i 1 -H report_progress.sh > copy.of.file

In your hooks you can get the amount of bytes that are processed so far by reading the PROCESSED_BYTES enviroment variable. Once it's done it'll do one last call to the hooks which will also have the enviroment variable DONE set to true.
