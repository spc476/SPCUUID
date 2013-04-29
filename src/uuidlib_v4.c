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

#include <stdlib.h>
#include <assert.h>

#include "uuid.h"

/*************************************************************************/

int uuidlib_v4(uuid__t *const uuid)
{
  assert(uuid != NULL);
  
  for (size_t i = 0 ; i < (sizeof(struct uuid) / sizeof(rand__t)) ; i++)
    uuid->rnd[i] = (unsigned)rand() + (unsigned)rand();
  
  uuid->flat[6] = (uuid->flat[6] & 0x0F) | 0x40;
  uuid->flat[8] = (uuid->flat[8] & 0x3F) | 0x80;
  return 0;
}

/*************************************************************************/
