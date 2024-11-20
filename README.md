# deviceatlas-php

A PHP wrapper for the DeviceAtlas C library.

See `deviceatlas.stub.php` for the API.

### Notes

* C Library is here: https://deviceatlas.com/resources/download-enterprise-api#c
* Requires login to download (@mjec shared via 1Password)
* To build library:
  ```
  $ cmake -S . -B build -DCMAKE_C_FLAGS_DEBUG='-g -O0' -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_INSTALL_PREFIX:PATH=$(pwd)/root
  $ make -C build
  $ make -C build install
  ```
* To re-generate `deviceatlas_arginfo.h`, edit `deviceatlas.stub.php` then run `~/php-src/build/gen_stub.php -f`
* To compile extension:
  ```
  $ phpize
  $ CFLAGS='-g -O0' ./configure --with-deviceatlas=/home/adam/deviceatlas-enterprise-c-3.3.0/root
  $ make
  ```
* To run test, put a DeviceAtlas `data.json` in `tests/`, then run `make test`
* Using the binary format segfaults for some reason. See commented out code.
* To generate the binary format, run `~/deviceatlas-enterprise-c-3.3.0/root/bin/dajsonconv -j data.json -b data.bin`
