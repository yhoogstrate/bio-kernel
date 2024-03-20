The βio-Кernel project
----------

<img src="https://server.bh24.net/bio-kernel/assets/bio-kernel.png" height="250">

Introduction
----------

The field of bioinformatics is concerned with a wide range of important research applications with high societal impact, from plant breeding to corona (spread), from oncology to taxonomic species classification. An essential role within all these applications is the use of static molecular reference data, a layered summary of the molecular hereditary classification of an organism. Despite the central role of this data in almost all bioinformatics applications, its publication and implementation is outdated and there are several limitations urging for modern solutions.
For example, the release of new reference data is not standardized, must be downloaded manually from various external sources, sometimes via unsecured protocols, and there is no software rollout of updates. The publications are not provided with a digital signature(s). There is no de-facto method to store this static data in a layered and structured manner, which promotes fragmentation and incompatibility. There is great redundancy between different data releases (>95% identical), but smart data structures to model redundancy have not been implemented for the subsequent file storage. Some applications that using these data import derivative file formats, causing incompatibility and redundant storage. And while there are standardized taxonomy identifiers, they are not applied at the system level to annotate files.

The βio-Кernel project aims to resolve the issues stated above by managing and integrating bio-molecular reference data into the kernel using three interconnected software components:


```
Components:
 - bio-hub       [remote: compact webserver, defining data hierarchy and hosting data]
 - bio-kernel    [local: service with cached data and management - mounts to: /bio]
 - utils/build/  [local: userspace level tools]
   * bio
   * lsb
   * chtax
   * rmtax
```


## bio-hub:

ongoing


## bio-utils

These are the core-utils equivalent tools needed to work with bio-kernel. You can build the tools, library and data stuctures by:

```
cd utils

if test -f build ; then
  meson setup build --reconfigure  --prefix ~/.local/
else
  meson setup build  --prefix ~/.local/
fi

cd build
ninja
meson install
```

### bio

### chtax

```
$ chtax 9606 hg38_mapped.bam
$ lsb hg38_mapped.b*
.
..
hg38_mapped.bam     9606    Homo spiens
hg38_mapped.bam.bai 9606    Homo spiens

$ rmtax hg38_mapped.b*
$ lsb hg38_mapped.b*
.
..
hg38_mapped.bam
hg38_mapped.bam.bai
```



### lsb

### rmtax


## bio-kernel


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

