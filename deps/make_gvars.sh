#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR/deps
(
    cd gvars
    ./configure --disable-widgets CPPFLAGS=-I$DIR/deps
    make
    git clean -fdx -e libGVars.a
)

