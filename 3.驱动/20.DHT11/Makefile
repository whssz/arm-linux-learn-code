obj-m := dht11_device.o dht11_driver.o

KDIR := /home/3.driver/ARM/kernel/linux-mini2440

all: 
	make -C $(KDIR) M=$(PWD) modules CROSS_COMPILE=arm-linux- ARCH=arm

clean:
	rm -f *.o *.ko *.order *.mod.c *.symvers 
	
