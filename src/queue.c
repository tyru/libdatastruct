
/*
 * Functions for the queue data structure
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

#include <stdlib.h>
#include <string.h>
#include "libdatastruct.h"

/*******************************************************************************
	Macros
*******************************************************************************/

#define void_pointer_addition(pointer,number) \
    ((void *)((char *)pointer+number))

#define refer_by_offset(queue,offset) \
    (void_pointer_addition((queue)->array \
    ,(queue)->element_size*((offset)%(queue)->array_size)))

#define refer_by_offset_from_front(queue,offset) \
    refer_by_offset((queue),(queue)->head+(offset))

#define refer_by_offset_from_back(queue,offset) \
    refer_by_offset((queue),(queue)->head+queue_size(queue)-1-(offset))

#define refer_front(queue) \
    refer_by_offset((queue),(queue)->head)

#define refer_back(queue) \
    refer_by_offset_from_front((queue),(queue)->size-1)

/*******************************************************************************
	Functions
*******************************************************************************/

queue_t *queue_initialize(const size_t element_size
    ,void (*release_function)(void *))
{
	queue_t *queue = malloc(sizeof(queue_t));
	if(!queue){
		return NULL;
	}
	queue->size = 0;
	queue->element_size = element_size;
	queue->array_size = QUEUE_DEFAULT_ARRAY_SIZE;
	queue->head = 0;
	queue->release_function = release_function?release_function
	                                          :dummy_release_function;
	queue->array = malloc(element_size*QUEUE_DEFAULT_ARRAY_SIZE);
	if(!queue->array){
		free(queue);
		return NULL;
	}
	return queue;
}

void queue_release(queue_t *queue)
{
	if(queue){
		size_t counter = 0;
		while(counter != queue->size){
			queue->release_function(refer_by_offset_from_front(queue,counter));
			counter++;
		}
		free(queue->array);
		free(queue);
	}
}

unsigned int queue_front(queue_t *queue,void *output)
{
	if(queue_empty(queue)){
		return QUEUE_EMPTY;
	}
	if(output){
		memcpy(output,refer_front(queue),queue->element_size);
	}
	return QUEUE_SUCCESS;
}

unsigned int queue_back(queue_t *queue,void *output)
{
	if(queue_empty(queue)){
		return QUEUE_EMPTY;
	}
	if(output){
		memcpy(output,refer_back(queue),queue->element_size);
	}
	return QUEUE_SUCCESS;
}

unsigned int queue_refer_from_front
    (queue_t *queue,const size_t offset,void *output)
{
	if(offset >= queue->size){
		return QUEUE_OFFSET_IS_TOO_LARGE;
	}
	if(output){
		memcpy(output,refer_by_offset_from_front(queue,offset)
		    ,queue->element_size);
	}
	return QUEUE_SUCCESS;
}

unsigned int queue_refer_from_back
    (queue_t *queue,const size_t offset,void *output)
{
	if(offset >= queue->size){
		return QUEUE_OFFSET_IS_TOO_LARGE;
	}
	if(output){
		memcpy(output,refer_by_offset_from_back(queue,offset)
		    ,queue->element_size);
	}
	return QUEUE_SUCCESS;
}

unsigned int queue_enqueue(queue_t *queue,const void *input)
{
	if(queue->head+queue->size <= queue->array_size
	    && queue->array_size*3 <= queue->size*4){
		void *temp
		    = realloc(queue->array,queue->element_size*queue->array_size*2);
		if(!temp && queue->size == queue->array_size){
			return QUEUE_MEMORY_ALLOCATION_ERROR;
		}
		else if(temp){
			queue->array = temp;
			queue->array_size = queue->array_size*2;
		}
	}
	else if(queue->size == queue->array_size){
		void *temp
		    = realloc(queue->array,queue->element_size*queue->array_size*2);
		if(!temp){
			return QUEUE_MEMORY_ALLOCATION_ERROR;
		}
		queue->array = temp;
		memcpy(void_pointer_addition(queue->array
		    ,queue->element_size*queue->array_size)
		    ,queue->array,queue->element_size*queue->head);
		queue->array_size = queue->array_size*2;
	}
	memcpy(refer_by_offset_from_front(queue,queue->size)
	    ,input,queue->element_size);
	queue->size++;
	return QUEUE_SUCCESS;
}

unsigned int queue_dequeue(queue_t *queue,void *output)
{
	if(queue_empty(queue)){
		return QUEUE_EMPTY;
	}
	if(output){
		memcpy(output,refer_front(queue),queue->element_size);
	}
	queue->head = (queue->head+1)%queue->array_size;
	queue->size--;
	/*
	if(queue->size <= queue->array_size/4 && queue->head <= queue->array_size/2
	    && queue->head+queue->size <= queue->array_size/2
	    && QUEUE_DEFAULT_ARRAY_SIZE < queue->array_size){
		void *temp = realloc(queue->array,queue->array_size/2);
		if(temp){
			queue->array = temp;
			queue->array_size = queue->array_size/2;
		}
	}
	*/
	return QUEUE_SUCCESS;
}
