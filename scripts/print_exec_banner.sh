#!/usr/bin/env bash
if [ $# -le 0 ]; then
    printf "Error: \nUsage: %s <prog_name>\n" "$0"
    exit
fi

APP_EXE="$1"
shift
ARGS="$@"
cols=$(tput cols)
bg=$(
    tput setab 1
    tput smul
)
fg=$(tput setaf 7)
reset=$(tput sgr0)

if [ "$(uname -s)" == "Darwin" ]; then
    date="$(gdate +'%I:%M%P') - $APP_EXE $ARGS"
else
    date="$(date +'%I:%M%P') - $APP_EXE $ARGS"
fi
len=$(printf "%s" "$date" | wc -m)
pad=$((cols - len))
printf "%s%s%s%*s%s\n" "$bg" "$fg" "$date" $pad "" "$reset"
