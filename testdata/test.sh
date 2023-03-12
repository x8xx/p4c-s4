#!/bin/bash

set -e

SCRIPT_DIR=$(cd $(dirname $0); pwd)
make -j6
./p4c-s4 -I ./p4include/ -o $SCRIPT_DIR/test1/test1  -w $SCRIPT_DIR/test1/parser.wasm $SCRIPT_DIR/test1/test1.p4
