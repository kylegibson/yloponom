include Makefile.defs

RDIR = yloponom-release
QADIR = /local/group_projects/cs3141/project/group7/qa

INCLUDES = -I. -I./GUI -I./core -I./network $(QTINC)

.PHONY: all gui core net

all: yloponom server

yloponom: client_main.o gui core net 
	g++ $(QTLIB) -o yloponom client_main.o $(GSTATIC)

server: server_main.o core net
	g++ -o server server_main.o $(GSTATIC)

%.o : %.C
	$(CC) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

gui: 
	$(MAKE) -C GUI

core:
	$(MAKE) -C core

net:
	$(MAKE) -C network

release: yloponom server
	mkdir -p $(RDIR)
	cp -aR yloponom server data.dat bin $(RDIR)/
	chmod g+rwx,a+rx $(RDIR)/{yloponom,server}
	chmod a+r $(RDIR)/data.dat
	chmod -R a+r $(RDIR)/bin
	tar -cjf "$(QADIR)/$(RDIR)-`date +%s`.bz2" $(RDIR)/
	rm -f $(QADIR)/$(RDIR).bz2
	tar -cjf "$(QADIR)/$(RDIR).bz2" $(RDIR)/
	-chmod -f a+r $(QADIR)/*.bz2
	-rm -fR $(RDIR)/

clean:
	-rm -f -- yloponom server_main.o client_main.o libylopy.a
	$(MAKE) -C GUI clean
	$(MAKE) -C core clean
	$(MAKE) -C network clean

