CPPFLAGS =
LIBDIR=../lib
LDFLAGS = -L$(LIBDIR)
LDLIBS = -lalpc
LIBRARY = $(LIBDIR)/libalpc.a

OBJS = \
        FixedPoint.o

$(LIBRARY): $(OBJS)
	ar r $(LIBRARY) $(OBJS)

clean:
	rm -f *.o $(LIBRARY)
