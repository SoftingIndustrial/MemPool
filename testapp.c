/*==========================================================================
//
// This file is part of the MemPool library.
// Copyright (C) 2017 Bernd Edlinger
//
// MemPool is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3 or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
// As a special exception, if other files instantiate templates or use
// macros or inline functions from this file, or you compile this file
// and link it with other works to produce a work based on this file,
// this file does not by itself cause the resulting work to be covered by
// the GNU General Public License.  However the source code for this file
// must still be made available in accordance with section (3) of the GNU
// General Public License v3.
//
// This exception does not invalidate any other reasons why a work based
// on this file might be covered by the GNU General Public License.
//
//========================================================================*/
#include "mempool.h"
#include "stddef.h"
#include "stdio.h"

void *my_malloc(size_t size)
{
    return MemPool_Memory_Alloc_Func(size);
}

void *my_realloc(void* data, size_t size)
{
    return MemPool_Memory_ReAlloc_Func(data, size);
}

void my_free(void* data)
{
    MemPool_Memory_Free_Func(data);
}

/* create fragmentaion */
void memtest(unsigned k)
{
    unsigned i;
    void **p,**q;
    p=q=my_malloc(504);
    for (i=0; i<k; i++)
    {
        q=*q=my_malloc(504+4*(i%16));
    }
    *q=0;
    q=p;
    for (i=0; i<k; i++)
    {
        void **n=*q;
        q=*q=*n;
        my_free(n);
        if (!q || !*q) q=p;
    }
    my_free(p);
}

#define MAX_HEAP_SIZE 0x100000
int main()
{
   struct Cyg_Mempool_Status MallocStatus;
   void *p;
   if(!MemPool_init(MAX_HEAP_SIZE))
   {
     printf("MemPool init failure\n");
     return 2;
   }
   p = my_malloc(1);
   my_free(p);
   p = my_malloc(MAX_HEAP_SIZE); /* will fail */
   p = my_malloc(100);
   p = my_realloc(p, 1000);
   my_free(p);
   memtest(1500);
   MemPool_get_status(0, &MallocStatus);
   printf("Allocated memory: %u (max) %u (act)\n", MallocStatus.maxAllocBytes, MallocStatus.actualAllocBytes);

   MemPool_exit();
   if (MallocStatus.actualAllocBytes != 0)
   {
     return 1;
   }
   return 0;
}
