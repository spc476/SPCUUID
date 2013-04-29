package = "org.conman.uuid"
version = "1.0.0-1"

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
	Blah blah blah
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
