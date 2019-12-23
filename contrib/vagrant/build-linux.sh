#!/bin/bash

prefix="/usr/local"
version="1.0.1"
destdir="/bitcoin/dist/intercoin-${version}-linux"

CONFIG_SITE="$PWD/depends/x86_64-pc-linux-gnu/share/config.site" \
    ./configure --prefix="${prefix}" --disable-tests
make clean && make -j4

strip "src/intercoind"
strip "src/intercoin-tx"
strip "src/qt/intercoin-qt"
strip "src/intercoin-cli"
strip "src/.libs/libintercoinconsensus.so"

make install DESTDIR="${destdir}"
