ifneq ($(KERNELRELEASE),)

obj-m :=my_blk_drv.o

else

KDIR := /lib/modules/2.6.32-279.el6.i686/build
all:
	make -C $(KDIR) M=$(PWD) modules 
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.order *.ko.unsigned

endif
