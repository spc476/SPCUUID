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

#include "uuid.h"

	/* per RFC-4122 */

const uuid__t c_uuid_namespace_oid =
{
   .flat = {
             0x6b , 0xa7 , 0xb8 , 0x12 , 0x9d , 0xad , 0x11 , 0xd1 ,
             0x80 , 0xb4 , 0x00 , 0xc0 , 0x4f , 0xd4 , 0x30 , 0xc8
           }
};
