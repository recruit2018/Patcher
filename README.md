# Patcher

## To successfully launch the project, you must complete the following steps:
```sh
$ git clone project: https://github.com/recruit2018/ProjectPatcher.git
$ git submodule init 
$ git submodule update
```
## And install libraries: libssh2, openssl

To install openssl, follow these steps:
```sh
$ sudo apt-get install libssl-dev
```

To install libssh2, follow these steps:
```sh
$ cd libssh2 directory
$ mkdir build
$ cd build
$ cmake .. -DBUILD_SHARED_LIBS=ON -DCRYPTO_BACKEND=OpenSSL
$ cmake --build .
```


