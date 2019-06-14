#!/usr/bin/bash

# Change these to point to machine with input devices
KXM_SERVER_ADDR=192.168.11.11
KXM_SERVER_PORT=63445

echo "Connecting to $KXM_SERVER_ADDR:$KXM_SERVER_PORT"
# Sub-shell to cd to build to read config.txt
$(cd build; nc $KXM_SERVER_ADDR $KXM_SERVER_PORT | ./kxm)


