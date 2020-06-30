obj-m=kernel_list.o

KDIR:=/home/3.driver/ARM/kernel/linux-mini2440/

all:
	make -C $(KDIR) M=$(PWD) modules CROSS_COMPILE=arm-linux- ARCH=arm
	
clean:
	rm -f *.o *.order *.ko *.symvers *.mod.c
	