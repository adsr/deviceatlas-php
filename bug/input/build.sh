#!/bin/bash
set -euxo pipefail

main() {
  mkdir /work
  pushd /work
  compile_library
  compile_bug_program
  compile_json_to_bin
  find .
  popd
}

compile_library() {
  dir=$(mktemp -d)
  pushd "$dir"
  unzip /input/deviceatlas-enterprise-c-3.3.0.zip
  pushd deviceatlas-enterprise-c-3.3.0
  cmake -S . -B build \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_INSTALL_PREFIX:PATH=/work/da
  make -C build
  make -C build install
  popd
  popd
}

compile_bug_program() {
  gcc -g -O0 -Wall -Wextra \
    -I /work/da/include \
    -L /work/da/lib \
    /input/bug.c \
    -lda \
    -o bug
}

compile_json_to_bin() {
  /work/da/bin/dajsonconv \
    -j /input/data.json \
    -b data.bin
}

main
