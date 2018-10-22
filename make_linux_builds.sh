#!/bin/bash

# Exit on error
set -e

make CXX=g++ BUILD=debug
make CXX=g++ BUILD=release
make CXX=clang++ BUILD=debug
make CXX=clang++ BUILD=release
