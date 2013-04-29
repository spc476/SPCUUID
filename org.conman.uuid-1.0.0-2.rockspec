package = "org.conman.uuid"
version = "1.0.0-2"

source =
{
  url = "git://github.com/spc476/SPCUUID.git",
  tag = "1.0.0"
}

supported_platforms = { "unix" }

description =
{
  homepage   = "http://github.com/spc476/SPCUUID.git",
  maintainer = "Sean Conner <sean@conman.org>",
  license    = "LGPL",
  summary    = "A Lua module to generate UUIDs",
  detailed   = [[
	org.conman.uuid provides functionality to generate UUIDs based on
	RFC-4122.  It supports MAC/timed based UUIDs (v1), name-based UUIDs
	(v3/MD5,v5/SHA-1) and random UUIDs (v4).

		uuidv1 = org.conman.uuid(1)
		uuidv3 = org.conman.uuid(uuid,stringdata)
		uuidv4 = org.conman.uuid()
		uuidv5 = org.conman.uuid(uuid,stringdata)

	Also defined are the following UUIDS:

		org.conman.uuid.URL
		org.conman.uuid.DNS
		org.conman.uuid.X500
		org.conman.uuid.OID
		org.conman.uuid.NIL

  ]]
}

dependencies = 
{
  "lua ~> 5.1"
}

build =
{
  type            = "make",
  build_target    = "lua",
  install_target  = "install-lua",

  build_variables =
  {
    CC     = "$(CC) -std=c99",
    CFLAGS = "$(CFLAGS)",
  },

  install_variables =
  {
    LUALIB = "$(LIBDIR)"
  },
}
