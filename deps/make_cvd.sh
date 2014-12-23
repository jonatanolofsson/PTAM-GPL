#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR
(
    cd cvd
    export CXXFLAGS=-D_REENTRANT
    ./configure --without-ffmpeg CPPFLAGS=-I$DIR/deps
    make
    git clean -fdx -e libcvd.a -e cvd/config.h
)
