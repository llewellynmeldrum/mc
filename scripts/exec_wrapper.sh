#!/usr/bin/env bash
if [ $# -ne 1 ]; then
    printf "Error: \nUsage: %s <prog_name>\n" "$0"
    exit
fi

APP_EXE="$1"
shift
ARGS="$@"

./scripts/print_exec_banner.sh "$APP_EXE" "$ARGS"
./"$APP_EXE" "$ARGS"
./scripts/print_exit_code.sh "$APP_EXE" "$?"
