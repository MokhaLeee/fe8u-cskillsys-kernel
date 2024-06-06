#!/bin/bash

function collect_header() {
    cd $1

    COMMON_HEADER="./gbafe-kernel.h"
    rm -rf $COMMON_HEADER

    echo "#pragma once" > $COMMON_HEADER

    for file in $(find ./kernel -type f -name "*.h")
    do
        echo "#include <${file#*/}>" >> $COMMON_HEADER
    done

    cd -
}

RELEASE_DIR=.release_dir

rm -rf $RELEASE_DIR
mkdir $RELEASE_DIR
mkdir $RELEASE_DIR/include
mkdir $RELEASE_DIR/include/kernel

# make -j
cp fe8-kernel-* $RELEASE_DIR
cp -rf include/* $RELEASE_DIR/include/kernel
cp -rf Patches $RELEASE_DIR

# fix texts
TARGET_TEXTS_H=$RELEASE_DIR/include/kernel/constants/texts.h
cp Contants/Texts/TextDefinitions.h $RELEASE_DIR/include/kernel/constants/
echo '#ifndef TEXTS_H' > $TARGET_TEXTS_H
echo '#define TEXTS_H' >> $TARGET_TEXTS_H
echo '#include "TextDefinitions.h"' >> $TARGET_TEXTS_H
echo '#endif // TEXTS_H' >> $TARGET_TEXTS_H

# fix gfx
TARGET_GFX_H=$RELEASE_DIR/include/kernel/constants/gfx.h
cp Contants/Gfx/GfxDefs.h $RELEASE_DIR/include/kernel/constants/
echo '#ifndef GFX_H' > $TARGET_GFX_H
echo '#define GFX_H' >> $TARGET_GFX_H
echo '#include "GfxDefs.h"' >> $TARGET_GFX_H
echo '#endif // GFX_H' >> $TARGET_GFX_H

collect_header $RELEASE_DIR/include
