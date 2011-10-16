include ../Makefile.defs

.PHONY : clean all

INCLUDES = -I. -I..

STATIC = libnet.a
DEPS = Client.o Server.o ServerClient.o GameState.o \
	packet_encoder.o packet_decoder.o serialize.o \
	packet.o jail_packet.o property_packet.o register_packet.o trade_packet.o \
	dice_packet.o card_packet.o casino_packet.o action_packet.o

all: ../$(GSTATIC)

../$(GSTATIC) : $(DEPS)
	$(AR) rv $@ $^

%.o : %.C
	$(CC) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -f $(STATIC) $(DEPS)


