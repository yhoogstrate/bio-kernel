bio-kernel
----------

```
Components:
 - bio-kerneld   [service with cached data and management]
 - biokernel.mod [kernel level interfacet / API]
 - bin/          [userspace level tools]
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

