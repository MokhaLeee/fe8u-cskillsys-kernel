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
echo "// #define PROTECT_EN PROCTECT_EN" >> *.ref.event
echo "#ifdef PROCTECT_EN" >> *.ref.event
for GENERATED_LYNFILE in $(find . -type f -name "*.event"); do
    cat $GENERATED_LYNFILE | grep "PROTECT " >> *.ref.event
done
echo "#endif /*procTECT_EN */" >> *.ref.event

# make -j
cp fe8-kernel-* $RELEASE_DIR
cp -rf include $RELEASE_DIR
cp -rf Patches $RELEASE_DIR

# fix texts
cat Texts/build/msgs.h > $RELEASE_DIR/include/constants/texts.h

# fix gfx
cat Contents/Gfx/GfxDefs.h > $RELEASE_DIR/include/constants/gfx.h

collect_header $RELEASE_DIR/include

# collect buildfile
RELEASE_EADIR=$RELEASE_DIR/buildfile

install -d $RELEASE_EADIR
cp -r Wizardry Data Texts Contents include main.event $RELEASE_EADIR
