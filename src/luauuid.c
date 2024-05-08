/*********************************************************************
*
* Copyright 2012 by Sean Conner.  All Rights Reserved.
*
* This library is free software; you can redistribute it and/or modify it
* under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or (at your
* option) any later version.
*
* This library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
* License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library; if not, see <http://www.gnu.org/licenses/>.
*
* Comments, questions and criticisms can be sent to: sean@conman.org
*
*********************************************************************/

#define _GNU_SOURCE

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <time.h>
#include <assert.h>

#include <arpa/inet.h>

#include <lua.h>
#include <lauxlib.h>

#include "uuid.h"

#if !defined(LUA_VERSION_NUM) || LUA_VERSION_NUM < 501
#  error You need to compile against Lua 5.1 or higher
#endif

#define TYPE_UUID       "org.conman.uuid:UUID"

/***********************************************************************/

static void     uuidluaL_pushuuid       (lua_State *const,const uuid__t *const);
static int      uuidlua_meta___tostring (lua_State *const);
static int      uuidlua_meta___eq       (lua_State *const);
static int      uuidlua_meta___le       (lua_State *const);
static int      uuidlua_meta___lt       (lua_State *const);
static int      uuidlua_meta___len      (lua_State *const);
static int      uuidlua_meta___call     (lua_State *const);
static int      uuidlua_parse           (lua_State *const);
static int      uuidlua_breakout        (lua_State *const);

/************************************************************************/

static const struct luaL_Reg muuid_reg[] =
{
  { "parse"     , uuidlua_parse         } ,
  { "breakout"  , uuidlua_breakout      } ,
  { NULL        , NULL                  }
};

static const struct luaL_Reg muuid_meta[] =
{
  { "__tostring"        , uuidlua_meta___tostring       } ,
  { "__eq"              , uuidlua_meta___eq             } ,
  { "__lt"              , uuidlua_meta___lt             } ,
  { "__le"              , uuidlua_meta___le             } ,
  { "__len"             , uuidlua_meta___len            } ,
  { NULL                , NULL                          }
};

/*************************************************************************/

int luaopen_org_conman_uuid(lua_State *const L)
{
  luaL_newmetatable(L,TYPE_UUID);
  
#if LUA_VERSION_NUM == 501
  luaL_register(L,NULL,muuid_meta);
#else
  luaL_setfuncs(L,muuid_meta,0);
#endif

#if LUA_VERSION_NUM == 501
  luaL_register(L,"org.conman.uuid",muuid_reg);
#else
  luaL_newlib(L,muuid_reg);
#endif

  lua_pushliteral(L,UUID_VERSION);
  lua_setfield(L,-2,"_VERSION");
  
  uuidluaL_pushuuid(L,&c_uuid_namespace_dns);
  lua_setfield(L,-2,"DNS");
  uuidluaL_pushuuid(L,&c_uuid_namespace_url);
  lua_setfield(L,-2,"URL");
  uuidluaL_pushuuid(L,&c_uuid_namespace_oid);
  lua_setfield(L,-2,"OID");
  uuidluaL_pushuuid(L,&c_uuid_namespace_x500);
  lua_setfield(L,-2,"X500");
  uuidluaL_pushuuid(L,&c_uuid_null);
  lua_setfield(L,-2,"NIL");
  uuidluaL_pushuuid(L,&c_uuid_max);
  lua_setfield(L,-2,"MAX");
  
  lua_createtable(L,0,1);
  lua_pushcfunction(L,uuidlua_meta___call);
  lua_setfield(L,-2,"__call");
  lua_setmetatable(L,-2);
  
  return 1;
}

/*************************************************************************/

static void uuidluaL_pushuuid(lua_State *const L,const uuid__t *const uuid)
{
  uuid__t *ns;
  
  ns = lua_newuserdata(L,sizeof(uuid__t));
  memcpy(ns->flat,uuid->flat,sizeof(ns->flat));
  luaL_getmetatable(L,TYPE_UUID);
  lua_setmetatable(L,-2);
}

/*************************************************************************/

static int uuidlua_meta___tostring(lua_State *const L)
{
  uuid__t *uuid;
  char     buffer[37];
  
  uuid  = luaL_checkudata(L,1,TYPE_UUID);
  
  uuidlib_toa(uuid,buffer,sizeof(buffer));
  lua_pushlstring(L,buffer,36);
  return 1;
}

/*************************************************************************/

static int uuidlua_meta___eq(lua_State *const L)
{
  lua_pushboolean(
    L,
    uuidlib_cmp(
      luaL_checkudata(L,1,TYPE_UUID),
      luaL_checkudata(L,2,TYPE_UUID)
    ) == 0
  );
  return 1;
}

/*************************************************************************/

static int uuidlua_meta___le(lua_State *const L)
{
  lua_pushboolean(
   L,
    uuidlib_cmp(
      luaL_checkudata(L,1,TYPE_UUID),
      luaL_checkudata(L,2,TYPE_UUID)
    ) <= 0
  );
  return 1;
}

/*************************************************************************/

static int uuidlua_meta___lt(lua_State *const L)
{
  lua_pushboolean(
    L,
    uuidlib_cmp(
      luaL_checkudata(L,1,TYPE_UUID),
      luaL_checkudata(L,2,TYPE_UUID)
    ) < 0
  );
  return 1;
}

/*************************************************************************/

static int uuidlua_meta___len(lua_State *const L)
{
  luaL_checkudata(L,1,TYPE_UUID);
  lua_pushinteger(L,sizeof(uuid__t));
  return 1;
}

/*************************************************************************/

static int uuidlua_meta___call(lua_State *const L)
{
  uuid__t    *uuid;
  uuid__t    *pns;
  uuid__t     ns;
  char const *name;
  size_t      len;
  char const *data;
  size_t      dlen;
  int         rc;
  bool        sha1;
  
  lua_settop(L,4);
  
  if (lua_isnoneornil(L,2))
  {
    uuid = lua_newuserdata(L,sizeof(uuid__t));
    uuidlib_v4(uuid);
  }
  else if (lua_isboolean(L,2))
  {
    uuid = lua_newuserdata(L,sizeof(uuid__t));
    uuidlib_v7(uuid);
  }
  else if (lua_isnumber(L,2))
  {
    uuid = lua_newuserdata(L,sizeof(uuid__t));
    if (lua_toboolean(L,3))
      uuidlib_v6(uuid,lua_tointeger(L,2));
    else
      uuidlib_v1(uuid,lua_tointeger(L,2));
  }
  else if (lua_isstring(L,2))
  {
    data = luaL_checklstring(L,2,&dlen);
    if (dlen == 16)
      memcpy(ns.flat,data,sizeof(uuid__t));
    else
    {
      rc = uuidlib_parse(&ns,data);
      if (rc !=  0)
      {
        lua_pushnil(L);
        lua_pushinteger(L,rc);
        return 2;
      }
    }
    
    uuid = lua_newuserdata(L,sizeof(uuid__t));
    
    if (lua_isnoneornil(L,3))
    {
      *uuid = ns;
      uuidlib_v8(uuid);
    }
    else
    {
      name = luaL_checklstring(L,3,&len);
      sha1 = lua_isnoneornil(L,4);
      
      if (sha1)
        uuidlib_v5(uuid,&ns,name,len);
      else
        uuidlib_v3(uuid,&ns,name,len);
    }
  }
  else if (lua_isuserdata(L,2))
  {
    pns = luaL_checkudata(L,2,TYPE_UUID);
    name = luaL_checklstring(L,3,&len);
    sha1 = lua_isnoneornil(L,4);
    uuid = lua_newuserdata(L,sizeof(uuid__t));
    
    if (sha1)
      uuidlib_v5(uuid,pns,name,len);
    else
      uuidlib_v3(uuid,pns,name,len);
  }
  
  /*-------------------------------------------------------
  ; This is a mistake, but oh well, too late now to change.
  ;--------------------------------------------------------*/
  
  else
  {
    uuid = lua_newuserdata(L,sizeof(uuid__t));
    uuidlib_v1(uuid,-1);
  }
  
  luaL_getmetatable(L,TYPE_UUID);
  lua_setmetatable(L,-2);
  return 1;
}

/************************************************************************/

static int uuidlua_parse(lua_State *const L)
{
  uuid__t    *puuid;
  uuid__t     uuid;
  const char *data;
  size_t      size;
  int         rc;
  
  data = luaL_checklstring(L,1,&size);
  if (size == 16)
    memcpy(uuid.flat,data,sizeof(struct uuid));
  else
  {
    rc = uuidlib_parse(&uuid,data);
    if (rc != 0)
    {
      lua_pushnil(L);
      lua_pushinteger(L,rc);
      return 2;
    }
  }
  
  puuid = lua_newuserdata(L,sizeof(uuid__t));
  *puuid = uuid;
  luaL_getmetatable(L,TYPE_UUID);
  lua_setmetatable(L,-2);
  lua_pushinteger(L,0);
  return 2;
}

/**************************************************************************/

static int uuidlua_breakout(lua_State *const L)
{
  uuid__t  *uuid;
  int64_t   timestamp;
  uint16_t  sequence;
  char      node[13];
  size_t    bytes;
  
  uuid = luaL_checkudata(L,1,TYPE_UUID);
  lua_createtable(L,0,0);
  
  lua_pushlstring(L,(char *)uuid->flat,sizeof(struct uuid));
  lua_setfield(L,-2,"bits");
  
  if ((uuid->flat[8] & 0x80) == 0x00)
  {
    lua_pushboolean(L,true);
    lua_setfield(L,-2,"NCS_reserved");
    return 1;
  }
  else if ((uuid->flat[8] & 0xE0) == 0xC0)
  {
    lua_pushboolean(L,true);
    lua_setfield(L,-2,"Microsoft");
    return 1;
  }
  else if ((uuid->flat[8] & 0xE0) == 0xE0)
  {
    lua_pushboolean(L,true);
    lua_setfield(L,-2,"reserved");
    return 1;
  }
  
  assert((uuid->flat[8] & 0x80) == 0x80);
  
  lua_pushinteger(L,(uuid->flat[6] & 0xF0) >> 4);
  lua_setfield(L,-2,"version");
  
  switch(uuid->flat[6] & 0xF0)
  {
    case 0x10:
         timestamp = ((int64_t)ntohs(uuid->uuid.time_hi_and_version) << 48)
                   | ((int64_t)ntohs(uuid->uuid.time_mid)            << 32)
                   | ((int64_t)ntohl(uuid->uuid.time_low));
         timestamp -= UUID_EPOCH;
         timestamp &= 0x0FFFFFFFFFFFFFFFLL;
         
         lua_pushnumber(L,(lua_Number)timestamp / 10000000.0);
         lua_setfield(L,-2,"timestamp");
         sequence = (uuid->uuid.clock_seq_hi_and_reserved << 8)
                  | (uuid->uuid.clock_seq_low);
         sequence &= 0x3FFF;
         lua_pushinteger(L,sequence);
         lua_setfield(L,-2,"clock_sequence");
         bytes = snprintf(
                node,
                sizeof(node),
                "%02X%02X%02X%02X%02X%02X",
                uuid->uuid.node[0],
                uuid->uuid.node[1],
                uuid->uuid.node[2],
                uuid->uuid.node[3],
                uuid->uuid.node[4],
                uuid->uuid.node[5]
         );
         lua_pushlstring(L,node,bytes);
         lua_setfield(L,-2,"node");
         break;
         
    case 0x60:
         timestamp = ((int64_t)ntohl(uuid->uuid6.time_high) << 28)
                   | ((int64_t)ntohs(uuid->uuid6.time_mid)  << 12)
                   | ((int64_t)ntohs(uuid->uuid6.time_low_and_version) & 0xFFFLL);
         timestamp -= UUID_EPOCH;
         lua_pushnumber(L,(lua_Number)timestamp / 1000000.0);
         lua_setfield(L,-2,"timestamp");
         sequence = (uuid->uuid6.clock_seq_hi_and_reserved << 8)
                  | (uuid->uuid6.clock_seq_low);
         sequence &= 0x3FFF;
         lua_pushinteger(L,sequence);
         lua_setfield(L,-2,"clock_sequence");
         bytes = snprintf(
                node,
                sizeof(node),
                "%02X%02X%02X%02X%02X%02X",
                uuid->uuid.node[0],
                uuid->uuid.node[1],
                uuid->uuid.node[2],
                uuid->uuid.node[3],
                uuid->uuid.node[4],
                uuid->uuid.node[5]
         );
         lua_pushlstring(L,node,bytes);
         lua_setfield(L,-2,"node");
         break;
         
    case 0x70:
         timestamp = ((int64_t)ntohl(uuid->uuid7.utime_msh) << 16)
                   | ((int64_t)ntohs(uuid->uuid7.utime_msl));
         lua_pushnumber(L,(lua_Number)timestamp / 1000.0);
         lua_setfield(L,-2,"timestamp");
         lua_pushlstring(L,(char const *)&uuid->flat[6],10);
         lua_setfield(L,-2,"random");
         break;
         
    default:
         break;
  }
  
  return 1;
}

/***********************************************************************/
