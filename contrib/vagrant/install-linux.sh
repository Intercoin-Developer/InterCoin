#!/bin/bash

version="1.0.1"
pkgname="intercoin-${version}-linux"
mirror="https://github.com/intercoin/intercoin/releases/download/v${version}"

sudo apt-get update
sudo apt-get install -y unzip

set -ex

rm -f "${pkgname}.tar.gz"
rm -rf "${pkgname}"

wget "${mirror}/${pkgname}.tar.gz"
tar xf "${pkgname}.tar.gz"

sudo rsync -var --update "${pkgname}/usr/local/" "/usr/local/"
sudo ldconfig

rm -f "${pkgname}.tar.gz"
rm -rf "${pkgname}"
