#!/usr/bin/bash

# Change these to point to machine with input devices
KXM_SERVER_PORT=63445

echo "Server starting on port $KXM_SERVER_PORT"
./build/kxm build/config.txt | nc -lvp $KXM_SERVER_PORT

echo "Exited server."
