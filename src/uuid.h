/*********************************************************************
*
* Copyright 2013 by Sean Conner.  All Rights Reserved.
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

#ifndef UUIDLIB_H
#define UUIDLIB_H

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#ifndef __GNUC__
#  define __attribute__(x)
#endif

#if RAND_MAX == SHRT_MAX
  typedef unsigned short rand__t;
#else
  typedef unsigned int   rand__t;
#endif

#define UUID_VERS(m,n,p)	(((m) << 16) | ((n) << 8) | (p))
#define UUID_API		UUID_VERS(1,3,0)
#define UUID_VERSION		"1.3.0"
#define UUID_EPOCH		0x01B21DD213814000LL

/*******************************************************************/

#ifdef __SunOS
#pragma pack(1)
#endif

struct uuid
{
  uint32_t time_low;
  uint16_t time_mid;
  uint16_t time_hi_and_version;
  uint8_t  clock_seq_hi_and_reserved;
  uint8_t  clock_seq_low;
  uint8_t  node[6];
} __attribute__((packed));

struct uuid6
{
  uint32_t time_high;
  uint16_t time_mid;
  uint16_t time_low_and_version;
  uint8_t  clock_seq_hi_and_reserved;
  uint8_t  clock_seq_low;
  uint8_t  node[6];
} __attribute__((packed));

struct uuid7
{
  uint32_t utime_msh;
  uint16_t utime_msl;
  uint16_t ver_rnd;
  uint32_t var_rnd;
  uint32_t rnd;
} __attribute__((packed));

typedef union 
{
  struct uuid  uuid;
  struct uuid6 uuid6;
  struct uuid7 uuid7;
  uint8_t      flat[sizeof(struct uuid)];
  rand__t      rnd [sizeof(struct uuid) / sizeof(rand__t)];
} __attribute__((packed)) uuid__t;

#ifdef __SunOS
#pragma pack()
#endif

/*******************************************************************/

extern uuid__t const c_uuid_namespace_dns;
extern uuid__t const c_uuid_namespace_url;
extern uuid__t const c_uuid_namespace_oid;
extern uuid__t const c_uuid_namespace_x500;
extern uuid__t const c_uuid_null;
extern uuid__t const c_uuid_max;

extern int uuidlib_v1    (uuid__t *,const int)                             __attribute__((nonnull,nothrow));
extern int uuidlib_v2    (uuid__t *)                                       __attribute__((nonnull,nothrow));
extern int uuidlib_v3    (uuid__t *,uuid__t const *,void const *,size_t)   __attribute__((nonnull,nothrow));
extern int uuidlib_v4    (uuid__t *)                                       __attribute__((nonnull,nothrow));
extern int uuidlib_v5    (uuid__t *,uuid__t const *,void const *,size_t)   __attribute__((nonnull,nothrow));
extern int uuidlib_v6    (uuid__t *,int)                                   __attribute__((nonnull,nothrow));
extern int uuidlib_v7    (uuid__t *)                                       __attribute__((nonnull,nothrow));
extern int uuidlib_v8    (uuid__t *)                                       __attribute__((nonnull,nothrow));
extern int uuidlib_cmp   (uuid__t const *restrict,uuid__t const *restrict) __attribute__((nonnull,nothrow));
extern int uuidlib_parse (uuid__t *,char const *)                          __attribute__((nonnull,nothrow));
extern int uuidlib_toa   (uuid__t const *,char *dest,size_t)               __attribute__((nonnull,nothrow));

#endif
