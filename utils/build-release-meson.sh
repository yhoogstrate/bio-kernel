#!/bin/bash

mkdir -p build
meson setup build --reconfigure
cd build
ninja
# sudo meson install
