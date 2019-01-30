#!/bin/bash
set -xeuo pipefail
INFILE=icon.png
ICONSET=./icon.iconset
mkdir -p $ICONSET
sips -z 16 16 "$INFILE" --out "$ICONSET/icon_16x16.png"
sips -z 32 32 "$INFILE" --out "$ICONSET/icon_16x16@2x.png"
sips -z 32 32 "$INFILE" --out "$ICONSET/icon_32x32.png"
sips -z 64 64 "$INFILE" --out "$ICONSET/icon_32x32@2x.png"
sips -z 128 128 "$INFILE" --out "$ICONSET/icon_128x128.png"
sips -z 256 256 "$INFILE" --out "$ICONSET/icon_128x128@2x.png"
sips -z 256 256 "$INFILE" --out "$ICONSET/icon_256x256.png"
sips -z 512 512 "$INFILE" --out "$ICONSET/icon_256x256@2x.png"
sips -z 512 512 "$INFILE" --out "$ICONSET/icon_512x512.png"
cp $INFILE $ICONSET/icon_512x512@2x.png
iconutil -c icns $ICONSET
rm -R $ICONSET