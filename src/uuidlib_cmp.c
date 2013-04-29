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

#include <string.h>
#include <assert.h>
#include <arpa/inet.h>

#include "uuid.h"

/*************************************************************************/

int uuidlib_cmp(
	const uuid__t *const restrict uuid1,
	const uuid__t *const restrict uuid2
)
{
  uint32_t a32;
  uint32_t b32;
  uint16_t a16;
  uint16_t b16;
  
  assert(uuid1 != NULL);
  assert(uuid2 != NULL);
  
  /*---------------------------------------------------------------
  ; per RFC-4122, this is the "correct" way to compare UUIDs
  ;---------------------------------------------------------------*/
  
  a32 = ntohl(uuid1->uuid.time_low);
  b32 = ntohl(uuid2->uuid.time_low);
  
  if (a32 < b32)
    return -1;
  else if (a32 > b32)
    return 1;
  
  a16 = ntohs(uuid1->uuid.time_mid);
  b16 = ntohs(uuid2->uuid.time_mid);
  
  if (a16 < b16)
    return -1;
  else if (a16 > b16)
    return 1;
  
  a16 = ntohs(uuid1->uuid.time_hi_and_version);
  b16 = ntohs(uuid2->uuid.time_hi_and_version);
  
  if (a16 < b16)
    return -1;
  else if (a16 > b16)
    return 1;
      
  if (uuid1->uuid.clock_seq_hi_and_reserved < uuid2->uuid.clock_seq_hi_and_reserved)
    return -1;
  else if (uuid1->uuid.clock_seq_hi_and_reserved > uuid2->uuid.clock_seq_hi_and_reserved)
    return 1;
  
  if (uuid1->uuid.clock_seq_low < uuid2->uuid.clock_seq_low)
    return -1;
  else if (uuid1->uuid.clock_seq_low > uuid2->uuid.clock_seq_low)
    return 1;
  
  return memcmp(uuid1->uuid.node,uuid2->uuid.node,6);
}

/*************************************************************************/

