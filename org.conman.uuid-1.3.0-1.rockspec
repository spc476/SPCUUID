package = "org.conman.uuid"
version = "1.3.0-1"
source = {
   url = "git+https://github.com/spc476/SPCUUID.git",
   tag = "1.3.0"
}
description = {
   summary = "A Lua module to generate UUIDs",
   detailed = [[
	org.conman.uuid provides functionality to generate UUIDs based on
	RFC-4122.  It supports MAC/timed based UUIDs (v1), name-based UUIDs
	(v3/MD5,v5/SHA-1), random UUIDs (v4), timed based (v6, v7) and
	custom (v8).
  ]],
   homepage = "http://github.com/spc476/SPCUUID.git",
   license = "LGPL",
   maintainer = "Sean Conner <sean@conman.org>"
}
supported_platforms = {
   "unix"
}
dependencies = {
   "lua >= 5.1, <= 5.4"
}
build = {
   type = "make",
   platforms = {
      linux = {
         build_variables = {
            CC = "gcc -std=c99"
         }
      },
      solaris = {
         build_varaibles = {
            CC = "c99"
         }
      }
   },
   build_target = "lua",
   build_variables = {
      CC = "$(CC)",
      CFLAGS = "$(CFLAGS) -DNDEBUG",
      LUA = "$(LUA)",
      LUA_INCDIR = "$(LUA_INCDIR)"
   },
   install_target = "install-lua",
   install_variables = {
      LIBDIR = "$(LIBDIR)"
   }
}
