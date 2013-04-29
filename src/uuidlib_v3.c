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
#include <openssl/evp.h>

#include "uuid.h"

/*************************************************************************/

int uuidlib_v3(
	uuid__t       *const uuid,
	const uuid__t *const namespace,
	const void    *const name,
	const size_t         len
)
{
  const EVP_MD *m = EVP_md5();
  EVP_MD_CTX    ctx;
  unsigned char hash[EVP_MAX_MD_SIZE];
  unsigned int  hashsize;
  
  assert(uuid      != NULL);
  assert(namespace != NULL);
  assert(name      != NULL);
  assert(len       >  0);
  
  EVP_DigestInit(&ctx,m);
  EVP_DigestUpdate(&ctx,namespace->flat,sizeof(struct uuid));
  EVP_DigestUpdate(&ctx,name,len);
  EVP_DigestFinal(&ctx,hash,&hashsize);
  
  memcpy(uuid->flat,hash,sizeof(struct uuid));
  uuid->flat[6] = (uuid->flat[6] & 0x0F) | 0x30;
  uuid->flat[8] = (uuid->flat[8] & 0x3F) | 0x80;
  return 0;
}

/**************************************************************************/
