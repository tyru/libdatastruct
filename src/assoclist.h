
/*
 * Definition for the associative array data structure
 * Copyright (c) 2009, Kazuhiko Sakaguchi All rights reserved.
 * This file is part of the libdatastruct.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Stricter.org nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY KAZUHIKO SAKAGUCHI ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL KAZUHIKO SAKAGUCHI BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef HEADER_ASSOCLIST_H

#define HEADER_ASSOCLIST_H

#include <stddef.h>

/*******************************************************************************
	Constants
*******************************************************************************/

#define ASSOCLIST_SUCCESS                                   0x00000000
#define ASSOCLIST_MEMORY_ALLOCATION_ERROR                   0x00000001
#define ASSOCLIST_INVALID_KEY                               0x00000002
#define ASSOCLIST_KEY_COLLISION                             0x00000004
#define ASSOCLIST_NOT_EXISTENT_KEY                          0x00000008
#define ASSOCLIST_HASH_COLLISION                            0x00000010

#define ASSOCLIST_MAX_OF_SHORT_KEY_SIZE                     16
#define ASSOCLIST_DEFAULT_ARRAY_SIZE                        64

#define ASSOCLIST_HASH_TYPES                                4

/*******************************************************************************
	Structures
*******************************************************************************/

typedef struct assoclist_element_info
{
	size_t hash_id;
	unsigned used_flag:1,mode_flag:1;
	union
	{
		char *long_key;
		char short_key[ASSOCLIST_MAX_OF_SHORT_KEY_SIZE];
	} key;
} assoclist_element_info_t;

typedef struct assoclist
{
	assoclist_element_info_t *element_info_array;
	void *value_array;
	size_t size;
	size_t element_size;
	size_t array_size;
	void (*release_function)(void *);
} assoclist_t;

/*******************************************************************************
	Macros
*******************************************************************************/

#define assoclist_size(assoclist) \
    ((assoclist)->size)

#define assoclist_empty(assoclist) \
    (!(assoclist)->size)

/*******************************************************************************
	Functions
*******************************************************************************/

extern assoclist_t *assoclist_initialize
    (const size_t,void (*)(void *));
extern void assoclist_release
    (assoclist_t *);
extern unsigned int assoclist_add
    (assoclist_t *,const char *,const void *);
extern unsigned int assoclist_reassign
    (assoclist_t *,const char *,const void *);
extern unsigned int assoclist_lookup
    (assoclist_t *,const char *,void *);
extern unsigned int assoclist_remove
    (assoclist_t *,const char *,void *);

#endif
