TOPDIR = $(shell pwd)

export TOPDIR

LIBDIRS = $(TOPDIR)/lib

include $(TOPDIR)/makefile.mk

EXES = example

all : libs $(EXES)

example : example.c force
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)


libs : force
	@for dir in $(LIBDIRS); do \
		$(MAKE) -C $$dir;      \
	done

force:

clean :
	@for dir in $(LIBDIRS); do \
		$(MAKE) -C $$dir $@ ;  \
	done
	$(RM) $(EXES)
	@find . -name "*.[od]" | xargs rm -f

.PHONY : all clean force libs
