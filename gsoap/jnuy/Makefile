.SUFFIXES: .wsdl .xsd
GSOAP=soaptools/soapcpp2
WSDL2H=soaptools/wsdl2h
WSDLFILE=jnuy.wsdl

#in Linux/Unix, 以下两行不需要
SOAPC=soaptools/stdsoap2.c soaptools/dom.c
SOAPCPP=soaptools/stdsoap2.cpp soaptools/dom.cpp
SOAPLIBS=soapC.c soapClient.c
CC=gcc
CPP=g++
LIBS=
CIFLAGS=-Isoaptools
CFLAGS=-O2 -Wall -g $(CIFLAGS) -lsocket

all: 	client

client: 	client.c $(SOAPLIBS) $(SOAPC)
	$(CC) $(CFLAGS) -o client client.c soapList.c $(SOAPC) $(SOAPLIBS)

soap:	$(WSDLFILE)
#   wsdl2h在SCO下因为G++版本问题，无法编译，需要在WIN/LINUX系统下生成,命令格式
#   参照如下语句。
#	$(WSDL2H) -c -d $(WSDLFILE)
	$(GSOAP) -Isoaptools -c -C jnuy.h

.c.o: 
	$(CC) $(CFLAGS) -c $<
.cpp.o: 
	$(CC) $(CFLAGS) -c $<

.PHONY:	clean distclean
clean: 
	@echo "cleanning project" 
	-rm -f *.o *.log *.xml *.nsmap *Stub.h *Proxy.h *Object.h soapC.c soapClient.c soapH.h soapClientLib.c
	@echo "clean completed"
distclean: 
	@echo "distcleanning project" 
	-rm -f *.o *.log *.xml *.nsmap *Stub.h *Proxy.h *Object.h soapC.c soapH.h soapClient.c soapClientLib.c
	-rm -f client jnuy.h
	@echo "distclean completed"
