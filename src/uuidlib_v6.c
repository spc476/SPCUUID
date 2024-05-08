/*********************************************************************
*
* Copyright 2024 by Sean Conner.  All Rights Reserved.
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

#define _POSIX_C_SOURCE 199309L

#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include <netinet/in.h>

#include "uuid.h"

extern bool    m_init;
extern uint8_t m_mac[6];
extern int     uuidlib_init(void);

/***********************************************************************/

int uuidlib_v6(uuid__t *uuid,int clock)
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

  uuid->uuid6.time_high            = htonl((timestamp >> 28) & 0xFFFFFFFFLL);
  uuid->uuid6.time_mid             = htons((timestamp >> 12) & 0xFFFFLL);
  uuid->uuid6.time_low_and_version = htons(timestamp & 0x0FFFLL);
  
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
  
  uuid->flat[6] = (uuid->flat[6] & 0x0F) | 0x60;
  uuid->flat[8] = (uuid->flat[8] & 0x3F) | 0x80;
  return 0;
}

/*************************************************************************/
