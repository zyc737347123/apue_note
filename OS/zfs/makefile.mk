CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
AR = $(PREFIX)ar


INCDIR = $(TOPDIR)/include
LIBFCVDIR = $(TOPDIR)/lib

FCVLIB = $(LIBFCVDIR)/libzfs.a

linksyslibs = -lm -lpthread
linkfcvlibs = -L$(LIBFCVDIR) -lzfs

CFLAGS += -Wall -O2
CFLAGS += -I$(INCDIR)

CXXFLAGS += -Wall -O2
CXXFLAGS += -I$(INCDIR)

#CFLAGS +=-DDEBUG
CXXFLAGS +=-DDEBUG
CFLAGS += -DPERF_STATISTICS
#CFLAGS += -DPERF_STATISTICS

CFLAGS += -D__linux__
CXXFLAGS += -D_linux__

ARFLAGS = -rcv

LDFLAGS += $(linksyslibs)

%.i : %.c
	$(CC) $(CFLAGS) -E $< -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.d : %.c
	@$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed -e 's/\($(@F)\.o\)[ :]*/$(<D)\/\1 $(@D)\/$(@F) : /g' $@.$$$$ > $@ ; \
	$(RM) $@.$$$$;

%.i : %.cpp
	$(CXX) $(CXXFLAGS) -E $< -o $@

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.d : %.cpp
	@$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
	sed -e 's/\($(@F)\.o\)[ :]*/$(<D)\/\1 $(@D)\/$(@F) : /g' $@.$$$$ > $@ ; \
	$(RM) $@.$$$$;

sinclude $(.depens)
