#!/bin/bash

prefix="/"
destdir="/bitcoin/dist/intercoin-win64"

CONFIG_SITE="$PWD/depends/x86_64-w64-mingw32/share/config.site" \
    ./configure --prefix="${prefix}" --disable-tests
make -j4

x86_64-w64-mingw32-strip "src/intercoind.exe"
x86_64-w64-mingw32-strip "src/intercoin-tx.exe"
x86_64-w64-mingw32-strip "src/qt/intercoin-qt.exe"
x86_64-w64-mingw32-strip "src/intercoin-cli.exe"
x86_64-w64-mingw32-strip "src/.libs/libintercoinconsensus-0.dll"

make install DESTDIR="${destdir}"
