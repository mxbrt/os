#!/bin/bash

SCRIPT_DIR="$(realpath "${BASH_SOURCE[0]}")"

export LD_LIBRARY_PATH=/usr/local/lib:/import/sage-7.4/local/lib/
export PATH="$PATH:/home/mi/linnert/arm/bin/"

ln -s /home/mi/linnert/arm/bin/qemu-system-arm "${SCRIPT_DIR}/qemu"
