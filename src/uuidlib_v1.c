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

#ifdef __GNUC__
#  define _GNU_SOURCE
#endif

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#if defined(__SunOS) || defined(__APPLE__)
#  include <net/if_arp.h>
#  include <sys/socket.h>
#  include <sys/sockio.h>
#endif

#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <netinet/in.h>

#include "uuid.h"

/**************************************************************************/

static bool    m_init;
static uint8_t m_mac[6] = { 0xDE , 0xCA , 0xFB , 0xAD , 0x02 , 0x01 };

/*************************************************************************/

static int uuidlib_init(void)
{
  struct ifreq  *it;
  struct ifreq  *end;
  struct ifconf  ifc;
  char           buffer[BUFSIZ];
  int            err;
  int            sock;
  
  m_init = true;
  
  sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_IP);
  if (sock == -1) return errno;
  
  ifc.ifc_len = sizeof(buffer);
  ifc.ifc_buf = buffer;
  
  if (ioctl(sock,SIOCGIFCONF,&ifc) < 0)
    goto uuidlib_init_error;
  
  it  = ifc.ifc_req;
  end = it + (ifc.ifc_len / sizeof(struct ifreq));
  
  for ( ; it != end ; it++)
  {
    if (ioctl(sock,SIOCGIFFLAGS,it) < 0)
      goto uuidlib_init_error;
      
    if ((it->ifr_flags & IFF_LOOPBACK) == IFF_LOOPBACK)
      continue;

#if defined(__SunOS)
    struct arpreq arpreq;

    memcpy(&arpreq.arp_pa,&it->ifr_addr,sizeof(it->ifr_addr));
    if (ioctl(sock,SIOCGARP,&arpreq) < 0)
      goto uuidlib_init_error;
    memcpy(m_mac,arpreq.arp_ha.sa_data,sizeof(m_mac));
#elif defined(__APPLE__)
    if (ioctl(sock,SIOCGIFMAC,it) < 0)
      goto uuidlib_init_error;
    memcpy(m_mac,it->ifr_addr.sa_data,sizeof(m_mac));
#elif defined(__linux__)
    if (ioctl(sock,SIOCGIFHWADDR,it) < 0)
      goto uuidlib_init_error;
    memcpy(m_mac,it->ifr_hwaddr.sa_data,sizeof(m_mac));
#endif

    close(sock);
    return 0;
  }
  
  /*-----------------------------------------------------------------------
  ; we found no Ethernet interface to use.  So let's make up a MAC address
  ;
  ; The size calculation of rndbuf[] is done such that it will be the 
  ; smallest integral size of rand__t that is larger than a MAC address.
  ;-----------------------------------------------------------------------*/
  
  uint8_t rndbuf[(sizeof(m_mac) / sizeof(rand__t)) + sizeof(rand__t)];
  
  assert(sizeof(rndbuf) >= sizeof(m_mac));
  
  for (size_t i = 0 ; i < (sizeof(rndbuf) / sizeof(rand__t)) ; i++)
    rndbuf[i] = (unsigned)rand() + (unsigned)rand();
  
  memcpy(m_mac,rndbuf,sizeof(m_mac));
  m_mac[0] &= 0xFE;	/* this is an individual address */
  m_mac[0] |= 0x02;	/* this is an assigned (local) address */
  
  errno = 0;
  
uuidlib_init_error:
  err = errno;
  close(sock);
  return err;
}

/***********************************************************************/

int uuidlib_v1(uuid__t *const uuid,const int clock)
{
  int64_t timestamp;
  
  assert(uuid != NULL);
  
  if (!m_init)
  {
    int rc = uuidlib_init();
    if (rc != 0)
      return rc;
  }

#ifdef __APPLE__
  struct timeval now;
  
  gettimeofday(&now,NULL);
  timestamp = (now.tv_sec  * 1000000LL)
            + (now.tv_usec *      10LL)
            + UUID_EPOCH;
#else
  struct timespec now;
  
  clock_gettime(CLOCK_REALTIME,&now);
  timestamp = (now.tv_sec  * 10000000LL)
            + (now.tv_nsec /      100LL)
            + UUID_EPOCH;
#endif

  uuid->uuid.time_hi_and_version       = htons(timestamp >> 48);
  uuid->uuid.time_mid                  = htons((timestamp >> 32) & 0xFFFFLL);
  uuid->uuid.time_low                  = htonl(timestamp & 0xFFFFFFFFLL);
  if (clock < 0)
  {
    uuid->uuid.clock_seq_hi_and_reserved = rand() & 0xFF;
    uuid->uuid.clock_seq_low             = rand() & 0xFF;
  }
  else
  {
    uuid->uuid.clock_seq_hi_and_reserved = (clock >> 8) & 0xFF;
    uuid->uuid.clock_seq_low             = clock & 0xFF;
  }
  memcpy(uuid->uuid.node,m_mac,6);
  
  uuid->flat[6] = (uuid->flat[6] & 0x0F) | 0x10;
  uuid->flat[8] = (uuid->flat[8] & 0x3F) | 0x80;
  return 0;
}

/*************************************************************************/
