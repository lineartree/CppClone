#!/bin/bash

if [ "$1" == "release" ]; then
    CONFIG="release"
else
    CONFIG="debug"
fi

# meson
rm -rf build
meson setup build --buildtype $CONFIG
cd build/ || exit
meson compile
cd ..
