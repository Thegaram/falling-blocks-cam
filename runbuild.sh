#!/bin/sh
mkdir build
cp -r assets build/
cmake -H. -Bbuild
make -C build