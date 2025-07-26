#!/usr/bin/env bash
qemu-system-x86_64 image.iso -cpu host -serial stdio -vga std -accel kvm -d int -no-reboot -no-shutdown -machine q35 -drive if=pflash,format=raw,readonly=on,file=/usr/share/edk2/x64/OVMF_CODE.4m.fd -s
