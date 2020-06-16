From this post:
http://www.ks.uiuc.edu/Research/vmd/mailing_list/vmd-l/21568.html


make
sudo insmod raw1394.ko 



# Full post:


It is a rather ugly fix, but as I cannot modify "Open"Haptics it will
have to do in my case. I've looked quite a lot around the web as well,
and I can't find any other solution.

After installing "Open"Haptics and the device drivers which SensAble
provides (using dpkg) you will have to work your way around the
dependency of raw1394. I am not really sure which steps ultimately made
it work for me, but the following steps are necessary.

1. Spoof the /dev/raw1394 device
2. Create a dummy kernel node

1. To spoof the /dev/raw1394 device, simply use ln and hard link the
/dev/fw0 (or some other fwX) by typing the following commands:

sudo ln /dev/fw0 /dev/raw1394
sudo chmod 0777 /dev/raw1394

2. As I believe "Open"Haptics checks if the raw1394 module is present
during launch (using lsmod or similar), I created a kernel module that
doesn't do anything at all called raw1394. This can be done by making
two files: Makefile and raw1394.c
These should be stored in the same folder and compiled using "make".
When this is done, you will have a file called raw1394.ko which can be
loaded using insmod.

Makefile

obj-m += raw1394.o
all:
make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

raw1394.c

#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void)
{
printk(KERN_INFO "Loaded dummy raw1394 module\n");
return 0;
}

Commands to compile and load module:

make
sudo insmod raw1394.ko 

