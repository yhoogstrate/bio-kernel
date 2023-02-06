#!/bin/bash

mkdir -p build
meson build
cd build
ninja

