#!/bin/bash

ret=0

# Round 1: detect c file
for SOURCE_CFILE in $(find . -type f -name "*.c"); do
    while read -r _function; do
        LYN_JUMP_FILE=$(dirname $SOURCE_CFILE)/LynJump.event
        if [ ! -e "$LYN_JUMP_FILE" ]; then
            echo "[ERROR] lynjump file not in the same directory as the C source file: $_function in file $SOURCE_CFILE"
            ret=1
        elif [ -z "$(grep $_function $LYN_JUMP_FILE)" ]; then
            echo "[Error] C function not referenced in lynjump file: $_function in file $SOURCE_CFILE"
            ret=1
        fi
    done < <(cat $SOURCE_CFILE | grep "LYN_REPLACE_CHECK" | awk -F'[()]' '{print $2}')
done

# Round 2: detect lyn file
for GENERATED_LYNFILE in $(find . -type f -name "*.lyn.event"); do
    SOURCE_CFILE=$(echo $GENERATED_LYNFILE | sed 's/\.lyn\.event/\.c/g')
    while read -r _function; do
        if [ -z "$(grep "LYN_REPLACE_CHECK($_function);" $SOURCE_CFILE)" ]; then
            echo "[ERROR] LYN_REPLACE_CHECK function not found for: $_function in file $SOURCE_CFILE"
            ret=1
        fi
    done < <(cat $GENERATED_LYNFILE | grep -A 3 '^ORG \$' | awk '/POIN/ {match($0, /POIN (.+)/, arr); print arr[1]}')
done

if [ $ret -eq 1 ]; then
    exit 1
fi

exit 0
