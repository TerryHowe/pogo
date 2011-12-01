CPPFLAGS = -I..
LIBDIR =../../lib
LDFLAGS = -L$(LIBDIR) --static
LDLIBS = -lcppunit -lalpc
LIBRARY = $(LIBDIR)/libalpc.a

OBJS = \
       main.o \
       TestFixedPoint.o

test: $(OBJS) $(LIBRARY)
	g++ $(LDFLAGS) -o test $(OBJS) $(LDLIBS)

clean:
	rm -f *.o test
