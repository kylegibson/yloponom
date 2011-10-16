include ../Makefile.defs

.PHONY : clean all

INCLUDES = -I. -I..

STATIC = libcore.a
DEPS = Random_Generator.o GameLogicUnit.o PlayerState.o PropertyState.o Game_Base.o GameRules.o

all: ../$(GSTATIC)

../$(GSTATIC) : $(DEPS)
	$(AR) rv $@ $^

%.o : %.C
	$(CC) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -f $(STATIC) $(DEPS)


