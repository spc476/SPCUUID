
.PHONY: all clean

UNAME := $(shell uname)

ifeq ($(UNAME),Linux)
  CC     = gcc -std=c99
  CFLAGS = -g -Wall -Wextra -pedantic
  AR     = ar rcu
  RANLIB = ranlib
endif

ifeq ($(UNAME),SunOS)
  CC     = cc -xc99
  CFLAGS = -g
  AR     = ar rcu
  RANLIB = ranlib
endif

all : libuuidlib.a

libuuidlib.a : uuid_ns_dns.o	\
		uuid_ns_null.o	\
		uuid_ns_oid.o	\
		uuid_ns_url.o	\
		uuid_ns_x500.o	\
		uuidlib_cmp.o	\
		uuidlib_parse.o	\
		uuidlib_toa.o	\
		uuidlib_v1.o	\
		uuidlib_v2.o	\
		uuidlib_v3.o	\
		uuidlib_v4.o	\
		uuidlib_v5.o
	$(AR) $@ $?
	$(RANLIB) $@

uuid_ns_dns.o : uuid_ns_dns.c uuidlib.h
	$(CC) $(CFLAGS) -c -o $@ $<

uuid_ns_null.o : uuid_ns_null.c uuidlib.h
	$(CC) $(CFLAGS) -c -o $@ $<

uuid_ns_oid.o : uuid_ns_oid.c uuidlib.h
	$(CC) $(CFLAGS) -c -o $@ $<

uuid_ns_url.o : uuid_ns_url.c uuidlib.h
	$(CC) $(CFLAGS) -c -o $@ $<

uuid_ns_x500.o : uuid_ns_x500.c uuidlib.h
	$(CC) $(CFLAGS) -c -o $@ $<

uuidlib_cmp.o : uuidlib_cmp.c uuidlib.h
	$(CC) $(CFLAGS) -c -o $@ $<

uuidlib_parse.o : uuidlib_parse.c uuidlib.h
	$(CC) $(CFLAGS) -c -o $@ $<

uuidlib_toa.o : uuidlib_toa.c uuidlib.h
	$(CC) $(CFLAGS) -c -o $@ $<

uuidlib_v1.o : uuidlib_v1.c uuidlib.h
	$(CC) $(CFLAGS) -c -o $@ $<

uuidlib_v2.o : uuidlib_v2.c uuidlib.h
	$(CC) $(CFLAGS) -c -o $@ $<

uuidlib_v3.o : uuidlib_v3.c uuidlib.h
	$(CC) $(CFLAGS) -c -o $@ $<

uuidlib_v4.o : uuidlib_v4.c uuidlib.h
	$(CC) $(CFLAGS) -c -o $@ $<

uuidlib_v5.o : uuidlib_v5.c uuidlib.h
	$(CC) $(CFLAGS) -c -o $@ $<

# ===================================================

clean:
	$(RM) -rf *.o *~ libuuidlib.a

