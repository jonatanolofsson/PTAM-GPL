#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR/deps
(
    cd cvd
    export CXXFLAGS=-D_REENTRANT
    ./configure --without-ffmpeg CPPFLAGS=-I$DIR/deps
    make
)
(
    cd gvars
    ./configure --disable-widgets CPPFLAGS=-I$DIR/deps
    make
)

