LIBSHELL = libshell.a

lib-objs += shell.o
lib-objs += cmd.o

.depens := $(patsubst %.o, %.d, $(lib-objs))

.PHONY : all clean
all : $(LIBSHELL)

$(LIBSHELL) : $(lib-objs)
	$(AR) $(ARFLAGS) $@ $^

clean :
	$(RM) $(LIBSHELL) *.o *.d *.i

include $(TOPDIR)/makefile.mk
