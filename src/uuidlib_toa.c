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

#include <stdio.h>
#include <assert.h>

#include "uuid.h"

/***************************************************************************/

int uuidlib_toa(const uuid__t *const uuid,char *dest,size_t size)
{
  assert(uuid != NULL);
  assert(dest != NULL);
  assert(size >= 37);
  
  return snprintf(
  	dest,
  	size,
  	"%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
  	uuid->flat[0],
  	uuid->flat[1],
  	uuid->flat[2],
  	uuid->flat[3],
  	uuid->flat[4],
  	uuid->flat[5],
  	uuid->flat[6],
  	uuid->flat[7],
  	uuid->flat[8],
  	uuid->flat[9],
  	uuid->flat[10],
  	uuid->flat[11],
  	uuid->flat[12],
  	uuid->flat[13],
  	uuid->flat[14],
  	uuid->flat[15]
  );
}

/***********************************************************************/

