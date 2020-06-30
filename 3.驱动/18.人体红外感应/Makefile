obj-m :=humen_check.o

KDIR := /home/mini2440/linux-2.6.32.2

all:
		make -C $(KDIR) M=$(PWD) modules CROSS_COMPILE=arm-linux- ARCH=arm	
			
clean: 
		rm -f *.o  *.ko *.order *.nod.c *.symve
