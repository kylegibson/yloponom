include ../Makefile.defs

.PHONY: all clean 

DEPS = Makefile.deps
INCLUDES = $(QTINC) -I. -I..

all: $(DEPS) ../$(GSTATIC)

include $(DEPS)

$(DEPS): project.dat
	../depgen project.dat 
	$(MAKE) clean
	$(MAKE) 

../$(GSTATIC) : $(OBJECTS)
	$(AR) rv $@ $^

%-moc.o : %-moc.C
	$(CC) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

%-moc.C : %.h
	$(MOC) -o $@ $<

%.o : %.C
	$(CC) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	-rm -f -- $(OBJECTS) $(STATIC) *-moc.C


