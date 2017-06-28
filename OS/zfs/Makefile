TOPDIR = $(shell pwd)

export TOPDIR

include $(TOPDIR)/makefile.mk

LIBDIRS = $(LIBDIR)
LIBDIRS += $(LIBSHELLDIR)

APPDIRS = $(TOPDIR)/zfs

LDFLAGS += $(linkfcvlibs)
LDFLAGS += $(linkysqfdlibs)

all : libs apps

libs : force
	@for dir in $(LIBDIRS); do \
		$(MAKE) -C $$dir;      \
	done

apps : force
	@for dir in $(APPDIRS); do \
		$(MAKE) -C $$dir;      \
	done

force:

clean : libclean appclean

libclean :
	@for dir in $(LIBDIRS); do \
		$(MAKE) -C $$dir clean ;  \
	done
	$(RM) $(EXES)
	@find . -name "*.[od]" | xargs rm -f

appclean :
	@for dir in $(APPDIRS); do \
		$(MAKE) -C $$dir clean ;  \
	done
	$(RM) $(EXES)
	@find . -name "*.[od]" | xargs rm -f

.PHONY : all clean force libs
