#!/bin/bash

ret=0

for SOURCE_CFILE in $(find . -type f -name "*.c"); do
    cat $SOURCE_CFILE | grep "LYN_REPLACE_CHECK" | awk -F'[()]' '{print $2}' | while read -r _function; do
        LYN_JUMP_FILE=$(dirname $SOURCE_CFILE)/LynJump.event
        if [ -n "$(grep -A 1 "LynJump" $SOURCE_CFILE | grep $_function)" ]; then
            if [ ! -e "$LYN_JUMP_FILE" ]; then
                echo "[ERROR] lynjump detection failed: $_function in file $SOURCE_CFILE"
            elif [ -z "$(grep $_function $LYN_JUMP_FILE)" ]; then
                echo "[ERROR] lynjump detection failed: $_function in file $SOURCE_CFILE"
            fi
        fi
    done
done

exit $ret
