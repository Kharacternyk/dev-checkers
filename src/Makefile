obj-m += checkers.o

debug: unload build load

unload:
	lsmod | grep checkers > /dev/null && sudo rmmod checkers.ko || true

clean: 
	make -C $(LINUX_SRC)/lib/modules/$(shell uname -r)/build M=$(PWD) clean

load:
	sudo insmod checkers.ko

build:
	make -C $(LINUX_SRC)/lib/modules/$(shell uname -r)/build M=$(PWD) modules
