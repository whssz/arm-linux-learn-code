objs := button.o interrupt.o led.o mmu.o uart.o nand.o dma.o lcd.o bmp.o ts.o dm9000.o spi.o dht11.o

all : $(objs)
	arm-linux-ld -r -o dev.o $^
	
%.o : %.c
	arm-linux-gcc ${CFLAGS} -c $^
	
%.o : %.S
	arm-linux-gcc ${CFLAGS} -c $^

clean:
	rm -f *.o *.bak		
	
