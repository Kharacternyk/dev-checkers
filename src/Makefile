obj-m += checkers.o

debug: unload format build load

unload:
	lsmod | grep checkers > /dev/null && sudo rmmod checkers.ko || true

clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load:
	sudo insmod checkers.ko

build:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

format:
	astyle --formatted --project *.c
