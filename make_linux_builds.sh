#!/bin/bash

# Exit on error
set -e

make CXX=g++     BUILD=debug   ARCH=x64 $*
make CXX=g++     BUILD=release ARCH=x64 $*
make CXX=clang++ BUILD=debug   ARCH=x64 $*
make CXX=clang++ BUILD=release ARCH=x64 $*

## Can't build 32bit on cygwin
# make CXX=g++     BUILD=debug   ARCH=x86 $*
# make CXX=g++     BUILD=release ARCH=x86 $*
# make CXX=clang++ BUILD=debug   ARCH=x86 $*
# make CXX=clang++ BUILD=release ARCH=x86 $*
