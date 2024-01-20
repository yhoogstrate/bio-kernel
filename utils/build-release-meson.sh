#!/bin/bash

mkdir -p build
meson setup build
cd build
ninja
# sudo meson install
