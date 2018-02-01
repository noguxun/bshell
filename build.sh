#!/bin/sh

if [ "$#" -ne 1 ]; then
  echo "Usage: build.sh BOARD_NAME" >&2
  exit 1
fi


make BOARD=$1 -f Makefile


