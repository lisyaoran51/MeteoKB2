
obj-m	+= headphone_switch.o

all drivers:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C  /lib/modules/$(shell uname -r)/build M=$(PWD) clean
