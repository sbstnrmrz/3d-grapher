#!/bin/bash

CC="clang"
CCFLAGS="-Wall -Wextra"
LIBDIR="-L/usr/local/lib"
INCLUDEDIR="-I/usr/local/include"
LIBFLAGS="-lm -lSDL3 -lSDL3_image -lSDL3_ttf"
RPATH='-rpath $LIBDIR'
SRC="src/*.c"

$CC $CCFLAGS $SRC -o main $INCLUDEDIR $LIBDIR $LIBFLAGS

