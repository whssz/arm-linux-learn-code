obj-m := gy50_i2c.o

KDIR := /home/3.driver/ARM/kernel/linux-mini2440

all: 
	make -C $(KDIR) M=$(PWD) modules CROSS_COMPILE=arm-linux- ARCH=arm

clean:
	rm -f *.o *.ko *.order *.mod.c *.symvers 
	
