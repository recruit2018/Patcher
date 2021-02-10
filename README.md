# Patcher

### Support system: Windows/Linux
### Tested on: Linux Ubuntu 20.04 64 bit, Windows 7 64 bit
### To successfully launch the project, you must complete the following steps:
```sh
$ git clone project: https://github.com/recruit2018/ProjectPatcher.git
$ git submodule init 
$ git submodule update
```
### Install libraries: libssh2

To install libssh2, build libssh2 with one of the crypto libraries: OpenSSL, Libgcrypt, WinCNG, mbedTLS

```sh
$ cd libssh2 directory
$ mkdir build
$ cd build
$ cmake .. -DBUILD_SHARED_LIBS=ON -DCRYPTO_BACKEND=" your crypto lib "
$ cmake --build .
```


