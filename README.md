# kernel
Basic x86_64 kernel in C. Moved from bitbucket.

## Requirements
* Cross compiled Binutils/GCC for x86_64 (Use my [script](https://github.com/optimisticninja/scripts/blob/master/cross-compiler-install.sh) )
* NASM
* xorriso
* QEMU
* My [libc](https://github.com/optimisticninja/libc)

## Getting setup
```
$ cd ~/projects
$ mkdir kernel
$ cd kernel
$ git clone https://github.com/optimisticninja/libc
$ git clone https://github.com/optimisticninja/kernel
$ cd libc
$ make
$ cd ../kernel
$ make
$ make iso
$ make run
```
