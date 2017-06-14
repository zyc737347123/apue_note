include $(TOPDIR)/makefile.mk

EXES = example

LDFLAGS += $(linkfcvlibs)
LDFLAGS += $(linksyslibs)

all : $(EXES)

example : example.c 
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean :
	$(RM) $(EXES)

.PHONY : all clean
