
.PHONY: all clean install install-lua remove remove-lua

UNAME := $(shell uname)

ifeq ($(UNAME),Linux)
  CC      = gcc -std=c99
  CFLAGS  = -g -Wall -Wextra -pedantic
  LDFLAGS = -shared
  SHARED  = -fPIC
  AR      = ar rcu
  RANLIB  = ranlib
endif

ifeq ($(UNAME),SunOS)
  CC     = cc -xc99
  CFLAGS = -g
  AR     = ar rcu
  RANLIB = ranlib
endif

# =============================================

INCLUDE = /usr/local/include
LIB     = /usr/local/lib
LUALIB  = /usr/local/lib/lua/5.1

# ============================================

all : lib obj lib/libspcuuid.a

lua : so lib/lua-uuid.so

lib :
	mkdir lib

obj :
	mkdir obj

so : 
	mkdir so

# ==========================================

lib/libspcuuid.a : obj/uuid_ns_dns.o	\
		obj/uuid_ns_null.o	\
		obj/uuid_ns_oid.o	\
		obj/uuid_ns_url.o	\
		obj/uuid_ns_x500.o	\
		obj/uuidlib_cmp.o	\
		obj/uuidlib_parse.o	\
		obj/uuidlib_toa.o	\
		obj/uuidlib_v1.o	\
		obj/uuidlib_v2.o	\
		obj/uuidlib_v3.o	\
		obj/uuidlib_v4.o	\
		obj/uuidlib_v5.o
	$(AR) $@ $?
	$(RANLIB) $@

lib/lua-uuid.so : so/luauuid.o		\
		so/uuid_ns_dns.o	\
		so/uuid_ns_null.o	\
		so/uuid_ns_oid.o	\
		so/uuid_ns_url.o	\
		so/uuid_ns_x500.o	\
		so/uuidlib_cmp.o	\
		so/uuidlib_parse.o	\
		so/uuidlib_toa.o	\
		so/uuidlib_v1.o		\
		so/uuidlib_v2.o		\
		so/uuidlib_v3.o		\
		so/uuidlib_v4.o		\
		so/uuidlib_v5.o
	$(CC) $(LDFLAGS) -o $@ $^

# ===========================================================

obj/uuid_ns_dns.o : src/uuid_ns_dns.c src/uuid.h
	$(CC) $(CFLAGS) -c -o $@ $<

obj/uuid_ns_null.o : src/uuid_ns_null.c src/uuid.h
	$(CC) $(CFLAGS) -c -o $@ $<

obj/uuid_ns_oid.o : src/uuid_ns_oid.c src/uuid.h
	$(CC) $(CFLAGS) -c -o $@ $<

obj/uuid_ns_url.o : src/uuid_ns_url.c src/uuid.h
	$(CC) $(CFLAGS) -c -o $@ $<

obj/uuid_ns_x500.o : src/uuid_ns_x500.c src/uuid.h
	$(CC) $(CFLAGS) -c -o $@ $<

obj/uuidlib_cmp.o : src/uuidlib_cmp.c src/uuid.h
	$(CC) $(CFLAGS) -c -o $@ $<

obj/uuidlib_parse.o : src/uuidlib_parse.c src/uuid.h
	$(CC) $(CFLAGS) -c -o $@ $<

obj/uuidlib_toa.o : src/uuidlib_toa.c src/uuid.h
	$(CC) $(CFLAGS) -c -o $@ $<

obj/uuidlib_v1.o : src/uuidlib_v1.c src/uuid.h
	$(CC) $(CFLAGS) -c -o $@ $<

obj/uuidlib_v2.o : src/uuidlib_v2.c src/uuid.h
	$(CC) $(CFLAGS) -c -o $@ $<

obj/uuidlib_v3.o : src/uuidlib_v3.c src/uuid.h
	$(CC) $(CFLAGS) -c -o $@ $<

obj/uuidlib_v4.o : src/uuidlib_v4.c src/uuid.h
	$(CC) $(CFLAGS) -c -o $@ $<

obj/uuidlib_v5.o : src/uuidlib_v5.c src/uuid.h
	$(CC) $(CFLAGS) -c -o $@ $<

# ===================================================

so/luauuid.o : src/luauuid.c src/uuid.h
	$(CC) $(CFLAGS) $(SHARED) -c -o $@ $<

so/uuid_ns_dns.o : src/uuid_ns_dns.c src/uuid.h
	$(CC) $(CFLAGS) $(SHARED) -c -o $@ $<

so/uuid_ns_null.o : src/uuid_ns_null.c src/uuid.h
	$(CC) $(CFLAGS) $(SHARED) -c -o $@ $<

so/uuid_ns_oid.o : src/uuid_ns_oid.c src/uuid.h
	$(CC) $(CFLAGS) $(SHARED) -c -o $@ $<

so/uuid_ns_url.o : src/uuid_ns_url.c src/uuid.h
	$(CC) $(CFLAGS) $(SHARED) -c -o $@ $<

so/uuid_ns_x500.o : src/uuid_ns_x500.c src/uuid.h
	$(CC) $(CFLAGS) $(SHARED) -c -o $@ $<

so/uuidlib_cmp.o : src/uuidlib_cmp.c src/uuid.h
	$(CC) $(CFLAGS) $(SHARED) -c -o $@ $<

so/uuidlib_parse.o : src/uuidlib_parse.c src/uuid.h
	$(CC) $(CFLAGS) $(SHARED) -c -o $@ $<

so/uuidlib_toa.o : src/uuidlib_toa.c src/uuid.h
	$(CC) $(CFLAGS) $(SHARED) -c -o $@ $<

so/uuidlib_v1.o : src/uuidlib_v1.c src/uuid.h
	$(CC) $(CFLAGS) $(SHARED) -c -o $@ $<

so/uuidlib_v2.o : src/uuidlib_v2.c src/uuid.h
	$(CC) $(CFLAGS) $(SHARED) -c -o $@ $<

so/uuidlib_v3.o : src/uuidlib_v3.c src/uuid.h
	$(CC) $(CFLAGS) $(SHARED) -c -o $@ $<

so/uuidlib_v4.o : src/uuidlib_v4.c src/uuid.h
	$(CC) $(CFLAGS) $(SHARED) -c -o $@ $<

so/uuidlib_v5.o : src/uuidlib_v5.c src/uuid.h
	$(CC) $(CFLAGS) $(SHARED) -c -o $@ $<


# ===================================================

install:
	install -d $(INCLUDE)/org/coman
	install src/uuid.h $(INCLUDE)/org/conman
	install lib/libspcuuid.a $(LIB)

remove:
	$(RM) -rf $(INCLUDE)/org/conman/uuid.h
	$(RM) -rf $(LIB)/libspcuuid.a
	
install-lua:
	install -d $(LUALIB)/org/conman
	install lib/lua-uuid.so $(LUALIB)/org/conman/uuid.so

remove-lua:
	$(RM) -rf $(LUALIB)/org/conman/uuid.so
	
clean:
	$(RM) -rf *~ src/*~ lib/ obj/ so/
