#!/bin/bash

function collect_header() {
    cd $1

    all_headers=$(find -type f -name "*.h")

    COMMON_HEADER="./gbafe-kernel.h"
    rm -rf $COMMON_HEADER

    echo "#pragma once" > $COMMON_HEADER
    echo "#include <common-chax.h>" > $COMMON_HEADER

    for file in $all_headers
    do
        echo "#include <${file#*/}>" >> $COMMON_HEADER
    done

    cd -
}

RELEASE_DIR=.release_dir

rm -rf $RELEASE_DIR
mkdir $RELEASE_DIR

# generate lyn-jump to refe
for GENERATED_LYNFILE in $(find . -type f -name "*.event"); do
    cat $GENERATED_LYNFILE | grep "PROTECT " >> *.ref.event
done

# make -j
cp fe8-kernel-* $RELEASE_DIR
cp -rf include $RELEASE_DIR
cp -rf Patches $RELEASE_DIR

# fix texts
# cat Contants/Texts/TextDefinitions.h > $RELEASE_DIR/include/constants/texts.h
cat Contants/Texts/build/msgs.h > $RELEASE_DIR/include/constants/texts.h

# fix gfx
cat Contants/Gfx/GfxDefs.h > $RELEASE_DIR/include/constants/gfx.h

collect_header $RELEASE_DIR/include
