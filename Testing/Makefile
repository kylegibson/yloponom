VPATH = .:../core/:../network/
CC = $(CXX)
AR = ar
LDFLAGS = 
CXXFLAGS = -Wall -I. -I..
STATIC = libtest.a

BIN_PROGS = rand server base rules
#BIN_PROGS = rand server packet base 
ALL_DEPS = 

all: $(BIN_PROGS)

packet_DEPS = Packet_Test.o $(STATIC)
rand_DEPS = Random_Generator_Test.o Random_Generator.o
server_DEPS = Server_Test.o $(STATIC) 
base_DEPS = Game_Base_Test.o Game_Base.o Random_Generator.o
rules_DEPS = GameRules_Test.o GameRules.o Random_Generator.o Game_Base.o $(STATIC)

define P_TEMPLATE
 $(1): $$($(1)_DEPS)
 ALL_DEPS+=$$($(1)_DEPS)
endef

$(foreach BIN,$(BIN_PROGS),$(eval $(call P_TEMPLATE,$(BIN))))

$(BIN_PROGS) : 
	$(CC) $(LDFLAGS) -o $@ $^

%.o : %.C
	$(CC) $(CXXFLAGS) -c -o $@ $<

$(STATIC): net core
	$(AR) rv $@ ../network/*.o ../core/*.o

net: 
	$(MAKE) -C ../network

core:
	$(MAKE) -C ../core

clean:
	-rm -f -- $(BIN_PROGS) $(ALL_DEPS) $(STATIC)

.PHONY : clean all net core

