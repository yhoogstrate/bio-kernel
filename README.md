The βio-Кernel project
----------

```
Components:
 - bio-kerneld   [service with cached data and management]
 - biokernel.mod [kernel level interface / API]
 - utils/build/  [userspace level tools]
   * bio
   * lsb
   * chtax
   * rmtax
```


## biokernel.mod

Building:

```
make
```


Loading:
```
	sudo dmesg -C
	sudo insmod biokernel.ko
	sudo rmmod biokernel.ko
	/sbin/modinfo biokernel.ko
	dmesg
```


help:

https://kukuruku.co/post/writing-a-file-system-in-linux-kernel/
http://books.gigatux.nl/mirror/kerneldevelopment/0672327201/ch12lev1sec9.html
https://unix.stackexchange.com/a/684444

https://github.com/krinkinmu/aufs
https://www.thegeekstuff.com/2013/07/write-linux-kernel-module/
https://blog.sourcerer.io/writing-a-simple-linux-kernel-module-d9dc3762c234
https://github.com/Papierkorb/tarfs
https://github.com/sysprog21/simplefs/blob/master/fs.c

1. register_filesystem -> registers a new type of fs?
2. mount_bdev
 - would biokernel.mod emulate a 'device'?
