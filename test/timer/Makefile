obj-m += timer_list.o
all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	rm -r *.ko *.o
