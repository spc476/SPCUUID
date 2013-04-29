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
#include <errno.h>
#include <assert.h>

#include "uuid.h"

/************************************************************************/

static int uuidlib_parse_seg(
	uuid__t     *const uuid,
	size_t             idx,
	const char       **ptext,
	const size_t       bytes
)
{
  char        buf[3];
  const char *p;
  
  assert(uuid   != NULL);
  assert(idx    <  sizeof(struct uuid));
  assert(ptext  != NULL);
  assert(*ptext != NULL);
  assert(bytes  >  0);
  
  errno = 0;
  p     = *ptext;
  
  for (size_t i = 0 ; i < bytes ; i++)
  {
    buf[0] = *p++;
    buf[1] = *p++;
    buf[2] = '\0';
    
    errno = 0;
    uuid->flat[idx++] = strtoul(buf,NULL,16);
    if (errno != 0) return errno;
  }
  
  if ((*p != '-') && (*p != '\0')) 
    return EINVAL;
  
  *ptext = p + 1;
  return 0;
}

/*************************************************************************/

int uuidlib_parse(uuid__t *const uuid,const char *text)
{
  int rc;
  
  assert(uuid != NULL);
  assert(text != NULL);
  
  if ((rc = uuidlib_parse_seg(uuid, 0,&text,4)) != 0) return rc;
  if ((rc = uuidlib_parse_seg(uuid, 4,&text,2)) != 0) return rc;
  if ((rc = uuidlib_parse_seg(uuid, 6,&text,2)) != 0) return rc;
  if ((rc = uuidlib_parse_seg(uuid, 8,&text,2)) != 0) return rc;
  if ((rc = uuidlib_parse_seg(uuid,10,&text,6)) != 0) return rc;
  
  return 0;
}

/***************************************************************************/

