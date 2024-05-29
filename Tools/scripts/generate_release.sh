#!/bin/bash

RELEASE_DIR=.release_dir

rm -rf $RELEASE_DIR
mkdir $RELEASE_DIR

# make -j
cp fe8-kernel-* $RELEASE_DIR
cp -rf include $RELEASE_DIR
cp -rf Patches $RELEASE_DIR

TARGET_INCLUDE=$RELEASE_DIR/include

# fix texts
TARGET_TEXTS_H=$TARGET_INCLUDE/constants/texts.h
cp Contants/Texts/TextDefinitions.h $TARGET_INCLUDE/constants/
echo '#ifndef TEXTS_H' > $TARGET_TEXTS_H
echo '#define TEXTS_H' >> $TARGET_TEXTS_H
echo '#include "TextDefinitions.h"' >> $TARGET_TEXTS_H
echo '#endif // TEXTS_H' >> $TARGET_TEXTS_H

# fix gfx
TARGET_GFX_H=$TARGET_INCLUDE/constants/gfx.h
cp Contants/Gfx/GfxDefs.h $TARGET_INCLUDE/constants/
echo '#ifndef GFX_H' > $TARGET_GFX_H
echo '#define GFX_H' >> $TARGET_GFX_H
echo '#include "GfxDefs.h"' >> $TARGET_GFX_H
echo '#endif // GFX_H' >> $TARGET_GFX_H
