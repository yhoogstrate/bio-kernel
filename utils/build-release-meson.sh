#!/bin/bash

#mkdir -p build

if test -f build ; then
  meson setup build --reconfigure  --prefix ~/.local/
else
  meson setup build  --prefix ~/.local/
fi

cd build
ninja
#meson configure bindir 
meson install
# sudo meson install
