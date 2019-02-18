#!/bin/bash

make
mkdir -p isodir/boot/grub
mv *.bin -t isodir/boot
cp *.cfg -t isodir/boot/grub
grub-mkrescue -o felix.iso isodir
