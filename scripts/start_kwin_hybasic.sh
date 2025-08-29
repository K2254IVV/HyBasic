#!/bin/bash

# Start KWin with HyBasic backend
echo "Starting KWin with HyBasic..."

export DISPLAY=:0
export HYBASIC_MODE=1
export KWIN_COMPOSE=H

# Set environment variables for HyBasic
export QT_QPA_PLATFORM=xcb
export GDK_BACKEND=x11

# Start HyBasic first
./scripts/start_hybasic.sh &

# Wait a moment for HyBasic to start
sleep 2

# Start KWin
kwin_x11 --replace --xwayland &
