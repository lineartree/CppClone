#!/bin/bash

if [ "$1" == "release" ]; then
    CONFIG="release"
else
    CONFIG="debug"
fi

rm -rf build
premake5 gmake

cd build/makefiles || exit
make config=$CONFIG -j4
cd ../../
./build/bin/App/${CONFIG^}/App
