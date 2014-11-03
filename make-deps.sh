#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR/deps
(
    cd cvd
    export CXXFLAGS=-D_REENTRANT
    ./configure --without-ffmpeg
    make
)
(
    cd gvars
    ./configure --disable-widgets
    make
)

