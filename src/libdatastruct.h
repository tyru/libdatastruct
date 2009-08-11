
/*
 * Definition for the LibDataStruct
 * Copyright (c) 2009, Kazuhiko Sakaguchi All rights reserved.
 * This file is part of the LibDataStruct.
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

#ifndef HEADER_LIBDATASTRUCT_H

#define HEADER_LIBDATASTRUCT_H

#include <stddef.h>

/*******************************************************************************
	Constants
*******************************************************************************/

/*associative array*/

#define ASSOCLIST_SUCCESS                                   0x00000000
#define ASSOCLIST_MEMORY_ALLOCATION_ERROR                   0x00000001
#define ASSOCLIST_INVALID_KEY                               0x00000002
#define ASSOCLIST_KEY_COLLISION                             0x00000004
#define ASSOCLIST_NOT_EXISTENT_KEY                          0x00000008
#define ASSOCLIST_HASH_COLLISION                            0x00000010

#define ASSOCLIST_MAX_OF_SHORT_KEY_SIZE                     16
#define ASSOCLIST_DEFAULT_ARRAY_SIZE                        (1<<6)

/*double-ended queue*/

#define DEQUE_SUCCESS                                       0x00000000
#define DEQUE_MEMORY_ALLOCATION_ERROR                       0x00000001
#define DEQUE_EMPTY                                         0x00000002
#define DEQUE_OFFSET_IS_TOO_LARGE                           0x00000004

#define DEQUE_DEFAULT_ARRAY_SIZE                            (1<<6)

/*stack*/

#define STACK_SUCCESS                                       0x00000000
#define STACK_MEMORY_ALLOCATION_ERROR                       0x00000001
#define STACK_EMPTY                                         0x00000002
#define STACK_OFFSET_IS_TOO_LARGE                           0x00000004

#define STACK_MEMORY_ALLOCATION_UNIT_SIZE                   32

/*queue*/

#define QUEUE_SUCCESS                                       0x00000000
#define QUEUE_MEMORY_ALLOCATION_ERROR                       0x00000001
#define QUEUE_EMPTY                                         0x00000002
#define QUEUE_OFFSET_IS_TOO_LARGE                           0x00000004

#define QUEUE_DEFAULT_ARRAY_SIZE                            (1<<6)

/*******************************************************************************
	Macros
*******************************************************************************/

/*associative array*/

#define assoclist_size(assoclist) \
    ((assoclist)->size)

#define assoclist_empty(assoclist) \
    (!(assoclist)->size)

/*double-ended queue*/

#define deque_size(deque) \
    ((deque)->size)

#define deque_empty(deque) \
    (!(deque)->size)

/*stack*/

#define stack_size(stack) \
    ((stack)->size)

#define stack_empty(stack) \
    (!(stack)->size)

/*queue*/

#define queue_size(queue) \
    ((queue)->size)

#define queue_empty(queue) \
    (!(queue)->size)

/*******************************************************************************
	Structures
*******************************************************************************/

/*associative array*/

typedef struct assoclist_element_info
{
	unsigned used_flag:1;
	unsigned mode_flag:1;
	size_t hash_type;
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

/*double-ended queue*/

typedef struct deque
{
	void *array;
	size_t size;
	size_t element_size;
	size_t array_size;
	size_t head;
	void (*release_function)(void *);
} deque_t;

/*stack*/

typedef struct stack
{
	void *array;
	size_t size;
	size_t element_size;
	size_t array_size;
	size_t head;
	void (*release_function)(void *);
} stack_t;

/*queue*/

typedef struct queue
{
	void *array;
	size_t size;
	size_t element_size;
	size_t array_size;
	size_t head;
	void (*release_function)(void *);
} queue_t;

/*******************************************************************************
	Functions
*******************************************************************************/

/*common*/

extern void dummy_release_function
    (void *);
extern unsigned int power_of_two_alignment
    (unsigned int);

/*associative array*/

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

/*double-ended queue*/

extern deque_t *deque_initialize
    (const size_t,void (*)(void *));
extern void deque_release
    (deque_t *);
extern unsigned int deque_front
    (deque_t *,void *);
extern unsigned int deque_back
    (deque_t *,void *);
extern unsigned int deque_refer_from_front
    (deque_t *,const size_t,void *);
extern unsigned int deque_refer_from_back
    (deque_t *,const size_t,void *);
extern unsigned int deque_push_front
    (deque_t *,const void *);
extern unsigned int deque_push_back
    (deque_t *,const void *);
extern unsigned int deque_pop_front
    (deque_t *,void *);
extern unsigned int deque_pop_back
    (deque_t *,void *);

/*stack*/

extern stack_t *stack_initialize
    (const size_t,void (*)(void *));
extern void stack_release
    (stack_t *);
extern unsigned int stack_bottom
    (stack_t *,void *);
extern unsigned int stack_top
    (stack_t *,void *);
extern unsigned int stack_refer_from_bottom
    (stack_t *,const size_t,void *);
extern unsigned int stack_refer_from_top
    (stack_t *,const size_t,void *);
extern unsigned int stack_refer_many_elements_from_bottom
    (stack_t *,const size_t,const size_t,void *);
extern unsigned int stack_refer_many_elements_from_top
    (stack_t *,const size_t,const size_t,void *);
extern unsigned int stack_push
    (stack_t *,const void *);
extern unsigned int stack_pop
    (stack_t *,void *);
extern unsigned int stack_push_many_elements
    (stack_t *,const size_t,const void *);
extern unsigned int stack_pop_many_elements
    (stack_t *,const size_t,void *);

/*queue*/

extern queue_t *queue_initialize
    (const size_t,void (*)(void *));
extern void queue_release
    (queue_t *);
extern unsigned int queue_front
    (queue_t *,void *);
extern unsigned int queue_back
    (queue_t *,void *);
extern unsigned int queue_refer_from_front
    (queue_t *,const size_t,void *);
extern unsigned int queue_refer_from_back
    (queue_t *,const size_t,void *);
extern unsigned int queue_enqueue
    (queue_t *,const void *);
extern unsigned int queue_dequeue
    (queue_t *,void *);

#endif
