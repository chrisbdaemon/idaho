#!/usr/bin/env sh

PREFIX="$PWD/build"
TARGET=i686-elf
PATH="$PREFIX/bin:$PATH"

rm -rf build-binutils
mkdir build-binutils &&
cd build-binutils &&
../binutils-2.28.0/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror --disable-gdb --disable-libdecnumber --disable-readline --disable-sim &&
make &&
make install

cd ..

rm -rf build-binutils
mkdir build-gcc &&
cd build-gcc
../gcc-6.3.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers &&
make all-gcc &&
make all-target-libgcc &&
make install-gcc &&
make install-target-libgcc
