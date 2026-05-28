#!/bin/sh

if xdpyinfo -display :2 >/dev/null 2>&1; then
  DISPLAY=:2 ./dwm &
else
  DISPLAY=:1 Xephyr :2 -screen 1920x1080 -ac -br &
  sleep 1
  DISPLAY=:2 dwm &
fi

sleep 2
WAYLAND_DISPLAY="" DISPLAY=:2 QT_QPA_PLATFORM=xcb cargo run
