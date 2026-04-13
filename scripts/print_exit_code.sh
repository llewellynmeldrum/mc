#!/usr/bin/env bash
if [ $# -ne 2 ]; then
    printf "Error: \nUsage: %s <prog_name> <exit_code>\n" "$0"
    exit
fi

APP_EXE="$1"
exit_code="$2"
shift 2
fmt_clear=$(tput sgr0)
fmt_bold=$(tput bold)

c_RED=1
c_GREEN=2
c_WHITE=7
c_BLACK=0
c_UNDERLINE=0
if [ $exit_code -eq 0 ]; then
    fmt_exitcode=$(
        tput smul
        tput setaf $c_BLACK
        tput setab $c_GREEN
    )
else
    fmt_exitcode=$(
        tput smul
        tput setaf $c_WHITE
        tput setab $c_RED
    )
fi

printf "%s%s exited with exit code %s%s%s\n" "$fmt_exitcode" "$APP_EXE" "$fmt_bold" "$exit_code" "$fmt_clear"
