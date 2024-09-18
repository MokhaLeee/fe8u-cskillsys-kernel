#!/bin/bash

# update header
rm -r include
cp -r ~/fireemblem8u/include/ ./

# update refs
cp ~/fireemblem8u/fireemblem8.elf utility/
cd utility && make -j
rm fireemblem8.elf

cd ..
mv utility/fireemblem8.* reference/
make -f template.mk reference/fireemblem8.ref.o
