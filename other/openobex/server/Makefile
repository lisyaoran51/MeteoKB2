VPATH=transport:openobex


INCLUDE = -Itransport -Iopenobex




TARGET  = mytest_server
#compiler 
CC = gcc
#cflags 
CFLAGS = -DHAVE_BLUETOOTH=1 -DHAVE_BLUETOOTH_LINUX=1 -lbluetooth
#object 
OBJS = test.o \
obex_test_client.o \
obex_test_server.o \
obex_io.o \
obex_transport_sock.o \
obex_transport.o \
obex_server.o \
obex_object.o \
obex_msg.o \
obex_main.o \
obex_hdr_stream.o \
obex_hdr_membuf.o \
obex_hdr_ptr.o \
obex_hdr.o \
obex_connect.o \
obex_client.o \
obex_body.o \
membuf.o \
databuffer.o \
api.o \
inobex.o \
fdobex.o \
customtrans.o \
btobex.o \





all:$(TARGET)

$(TARGET): $(OBJS) 
	$(CC) $(OBJS) -o $@ -DHAVE_BLUETOOTH=1 -DHAVE_BLUETOOTH_LINUX=1 -lbluetooth
	
.c.o:
	$(CC) $(INCLUDE) $(CFLAGS) -c -o $@ $< 	
	
.cpp.o:
	$(CC) $(INCLUDE) $(CFLAGS) -c -o $@ $< 

clean:
	rm -f *.o $(TARGET)