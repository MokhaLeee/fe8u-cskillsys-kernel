#!/bin/bash

root_dir="$(dirname $(readlink -f $0))"

colorzcore_src=$root_dir/.Sources/ColorzCore
eastdlib_src=$root_dir/.Sources/EAStandardLibrary
eafs_src=$root_dir/.Sources/EAFormattingSuite
lyn_src=$root_dir/.Sources/lyn
compress_src=$root_dir/.Sources/compress
eadep_src=$root_dir/.Sources/ea-dep

# ColorzCore

dotnet publish -r linux-x64 --self-contained true -c Release -o $root_dir $colorzcore_src/ColorzCore/ColorzCore.csproj

# EA Standard Library

cp "$eastdlib_src/EAstdlib.event" $root_dir/
cp -R "$eastdlib_src/Language Raws" $root_dir/
cp -R "$eastdlib_src/EA Standard Library" $root_dir/
cp -R "$eastdlib_src/Extensions" $root_dir/

# EA Tools

mkdir -p $root_dir/Tools

cp "$root_dir/.Sources/Tool Helpers.txt" $root_dir/Tools/

# - EAFormattingSuite

cd $eafs_src

cabal v1-install --bindir=$root_dir/Tools

# - compress

make -C $compress_src
cp $compress_src/compress $root_dir/Tools/

# - lyn

lyn_tmp=$(mktemp -d)
pushd $lyn_tmp
cmake $lyn_src
cmake --build .
cp lyn $root_dir/Tools/
popd
rm -rf $lyn_tmp

# ea-dep

eadep_tmp=$(mktemp -d)
pushd $eadep_tmp
cmake $eadep_src
cmake --build .
cp ea-dep $root_dir/
popd
rm -rf $eadep_tmp
