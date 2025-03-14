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
REF_EVENT=$(find | grep "ref.event")
echo "#define PROTECT_EN PROCTECT_EN" >> $REF_EVENT
echo "#ifdef PROCTECT_EN" >> $REF_EVENT
for GENERATED_LYNFILE in $(find . -type f -name "*.event"); do
    cat $GENERATED_LYNFILE | grep "PROTECT " >> $REF_EVENT
done
echo "#endif /*PROCTECT_EN */" >> $REF_EVENT

# make -j
cp fe8-kernel-* $RELEASE_DIR
cp -rf include $RELEASE_DIR
cp -rf Patches $RELEASE_DIR

# fix texts
cat Contents/Texts/build/msgs.h > $RELEASE_DIR/include/constants/texts.h

# fix gfx
cat Contents/Gfx/GfxDefs.h > $RELEASE_DIR/include/constants/gfx.h

collect_header $RELEASE_DIR/include

# collect buildfile
RELEASE_EADIR=$RELEASE_DIR/buildfile

install -d $RELEASE_EADIR
cp -r Preload Wizardry Data Contents include main.event Debug Fonts $RELEASE_EADIR

zip -r $RELEASE_DIR/buildfile.zip $RELEASE_DIR/buildfile/

find $RELEASE_DIR | grep gitignore | xargs rm
echo "!*" > $RELEASE_DIR/.gitignore
