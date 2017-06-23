CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
AR = $(PREFIX)ar

INCDIR = $(TOPDIR)/include
LIBDIR = $(TOPDIR)/lib

CFLAGS += -Wall -O2
CFLAGS += -I$(INCDIR)

CXXFLAGS += -Wall -O2
CXXFLAGS += -I$(INCDIR)

CFLAGS += -DDEBUG
CXXFLAGS += -DDEBUG

ARFLAGS = -rcv

LDFLAGS += -L$(LIBDIR) -lshell

LDFLAGS += -lm -lpthread

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
