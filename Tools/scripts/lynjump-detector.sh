#!/bin/bash

ret=0

# Round 1: detect c file
for SOURCE_CFILE in $(find . -type f -name "*.c"); do
    cat $SOURCE_CFILE | grep "LYN_REPLACE_CHECK" | awk -F'[()]' '{print $2}' | while read -r _function; do
        LYN_JUMP_FILE=$(dirname $SOURCE_CFILE)/LynJump.event
        if [ ! -e "$LYN_JUMP_FILE" ]; then
            echo "[ERROR] lynjump detection failed: $_function in file $SOURCE_CFILE"
        elif [ -z "$(grep $_function $LYN_JUMP_FILE)" ]; then
            echo "[ERROR] lynjump detection failed: $_function in file $SOURCE_CFILE"
        fi
    done
done

# Round 2: detect lyn file
for GENERATED_LYNFILE in $(find . -type f -name "*.lyn.event"); do
    SOURCE_CFILE=$(echo $GENERATED_LYNFILE | sed 's/\.lyn\.event/\.c/g')
    cat $GENERATED_LYNFILE | grep -A 3 '^ORG \$' | awk '/POIN/ {match($0, /POIN (.+)/, arr); print arr[1]}' | while read -r _function; do
        if [ -z "$(grep "LYN_REPLACE_CHECK($_function);" $SOURCE_CFILE)" ]; then
                echo "[ERROR] lynjump detection failed: $_function in file $SOURCE_CFILE"
        fi
    done
done

exit $ret
