#!/usr/bin/bash

echo "Starting client."
nc 192.168.11.11 63445 | ./build/kxm
