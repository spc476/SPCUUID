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

#include <stdlib.h>
#include <assert.h>

#include <sys/time.h>
#include <netinet/in.h>

#include "uuid.h"

/*************************************************************************/

int uuidlib_v7(uuid__t *uuid)
{
  struct timeval now;
  int64_t        timestamp;
  
  assert(uuid != NULL);
  
  for (size_t i = 0 ; i < (sizeof(struct uuid) / sizeof(rand__t)) ; i++)
    uuid->rnd[i] = (unsigned)rand() + (unsigned)rand();
    
  uuid->flat[6] = (uuid->flat[6] & 0x0F) | 0x70;
  uuid->flat[8] = (uuid->flat[8] & 0x3F) | 0x80;
  
  gettimeofday(&now,NULL);
  timestamp = (now.tv_sec * 1000LL)
            + (now.tv_usec);
  uuid->uuid7.utime_msh = htonl((timestamp >> 16) & 0xFFFFFFFFLL);
  uuid->uuid7.utime_msl = htons(timestamp & 0xFFFFLL);
  return 0;
}

/*************************************************************************/
