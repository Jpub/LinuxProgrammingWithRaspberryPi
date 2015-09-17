KDIR = /lib/modules/3.18.7-v7/build

obj-m := gpiosignal_module.o

default:
	$(MAKE) -C $(KDIR) M=$$PWD modules

clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean

