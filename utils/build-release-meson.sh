#!/bin/bash

#mkdir -p build

if test -f build ; then
  meson setup build --reconfigure
else
  meson setup build
fi

cd build
ninja
# sudo meson install
