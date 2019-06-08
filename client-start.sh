#!/usr/bin/bash

# Change these to point to machine with input devices
KXM_SERVER_ADDR=192.168.11.11
KXM_SERVER_PORT=63445

echo "Connecting to $KXM_SERVER_ADDR:$KXM_SERVER_PORT"

nc $KXM_SERVER_ADDR $KXM_SERVER_PORT | ./build/kxm build/config-client.txt

echo "Client exited."
