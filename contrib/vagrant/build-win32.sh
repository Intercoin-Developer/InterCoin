#!/bin/bash

prefix="/"
destdir="/bitcoin/dist/intercoin-win32"

CONFIG_SITE="$PWD/depends/i686-w64-mingw32/share/config.site" \
    ./configure --prefix="${prefix}" --disable-tests
make -j4

i686-w64-mingw32-strip "src/intercoind.exe"
i686-w64-mingw32-strip "src/intercoin-tx.exe"
i686-w64-mingw32-strip "src/qt/intercoin-qt.exe"
i686-w64-mingw32-strip "src/intercoin-cli.exe"
i686-w64-mingw32-strip "src/.libs/libintercoinconsensus-0.dll"

make install DESTDIR="${destdir}"
