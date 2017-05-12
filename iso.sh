#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/idaho.kernel isodir/boot/idaho.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "idaho" {
	multiboot /boot/idaho.kernel
}
EOF
grub-mkrescue -o idaho.iso isodir
