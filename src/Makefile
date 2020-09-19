obj-m += checkers.o

debug: format build load

unload:
	sudo rmmod checkers.ko

clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load:
	sudo insmod checkers.ko

build:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

format:
	astyle --formatted --project *.c
