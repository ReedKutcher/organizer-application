#! /bin/bash

cd "$(dirname "$0")"
gcc $(pkg-config --cflags gtk4) -o kiosk kiosk.c $(pkg-config --libs gtk4)
