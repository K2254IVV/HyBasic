#!/bin/bash

# Start HyBasic server
echo "Starting HyBasic Server..."
export DISPLAY=:0
export XDG_RUNTIME_DIR=/tmp/hybasic-runtime

mkdir -p $XDG_RUNTIME_DIR
chmod 0700 $XDG_RUNTIME_DIR

# Build if not exists
if [ ! -f hybasic ]; then
    make
fi

./hybasic
