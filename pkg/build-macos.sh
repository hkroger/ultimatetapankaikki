#!/bin/bash
set -xeuo pipefail
BUILD_DIR=$(mktemp -d /tmp/tkbuild.XXX)
OUT_DIR=$(mktemp -d /tmp/tkout.XXX)
PROJ_DIR=$(pwd)
cd $BUILD_DIR
cmake -DCMAKE_INSTALL_PREFIX=$OUT_DIR -DCMAKE_MACOSX_BUNDLE=true -DCMAKE_BUILD_TYPE=Release $PROJ_DIR
make -j4
make install
cpack --verbose -G DragNDrop
cp -a $BUILD_DIR/*.dmg
