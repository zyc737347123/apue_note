LIBOAL = libzfs.a

lib-objs += zfs.o

.depens := $(patsubst %.o, %.d, $(lib-objs))

.PHONY : all clean
all : $(LIBOAL)

$(LIBOAL) : $(lib-objs)
	$(AR) $(ARFLAGS) $@ $^

clean :
	$(RM) $(LIBOAL) *.o *.d *.i

include $(TOPDIR)/makefile.mk
