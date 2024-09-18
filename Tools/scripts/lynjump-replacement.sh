#!/bin/bash

TMP_FILE=/tmp/1
SCR_DIR=$(dirname "$(realpath "$0")")

function main() {
    src_file=$1
    cat $src_file | awk -f $SCR_DIR/lynjump-replacement.awk > $TMP_FILE
    cp $TMP_FILE $src_file
}

for SOURCE_CFILE in $(find . -type f -name "*.c"); do
    main $SOURCE_CFILE
done
