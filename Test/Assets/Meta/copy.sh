#!/bin/sh

ASSETS_PATH="$(dirname $0)/../../Assets"
DST_PATH="$1"
SELF_COPY_PATH="$DST_PATH/Assets/Meta/$(basename $0)"

echo "Copy assets to \"$DST_PATH\""
cp -Rf $ASSETS_PATH $DST_PATH
rm -Rf $SELF_COPY_PATH

exit 0
