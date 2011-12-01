CPPFLAGS = -g
LIBDIR=../lib
LDFLAGS = -L$(LIBDIR)
LDLIBS = -lalpc
LIBRARY = $(LIBDIR)/libalpc.a

OBJS = \
        ConfigurationFile.o \
        NamedSections.o \
        Scanner.o \
        State.o \
        Token.o

#all: main.o $(LIBRARY)
#	g++ $(LDFLAGS) -o alpc $(LDLIBS)

$(LIBRARY): $(OBJS)
	ar r $(LIBRARY) $(OBJS)

clean:
	rm -f *.o alpc $(LIBRARY)
