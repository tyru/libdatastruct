
/*
 * Functions for the stack data structure
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

#include <stdlib.h>
#include <string.h>
#include "libdatastruct.h"

/*******************************************************************************
	Macros
*******************************************************************************/

#define refer_by_offset(stack,offset) \
    ((stack)->array+(stack)->element_size*(offset))

#define refer_by_offset_from_front(stack,offset) \
    (refer_by_offset((stack),(offset)))

#define refer_by_offset_from_back(stack,offset) \
    (refer_by_offset((stack),stack_size(stack)-1-(offset)))

#define refer_front(stack) \
    ((stack)->array)

#define refer_back(stack) \
    refer_by_offset((stack),(stack)->size)

/*******************************************************************************
	Functions
*******************************************************************************/

stack_t *stack_initialize(const size_t element_size
    ,void (*release_function)(void *))
{
	stack_t *stack = malloc(sizeof(stack_t));
	if(!stack){
		return NULL;
	}
	stack->array = NULL;
	stack->size = 0;
	stack->element_size = element_size;
	stack->array_size = 0;
	stack->release_function = release_function?release_function
	                                          :dummy_release_function;
	return stack;
}

void stack_release(stack_t *stack)
{
	size_t counter = 0;
	if(stack){
		if(stack->release_function != dummy_release_function){
			while(counter != stack->size){
				stack->release_function(refer_by_offset(stack,counter));
				counter++;
			}
		}
		free(stack->array);
		free(stack);
	}
}

unsigned int stack_front(stack_t *stack,void *output)
{
	if(stack_empty(stack)){
		return STACK_EMPTY;
	}
	if(output){
		memcpy(output,refer_front(stack),stack->element_size);
	}
	return STACK_SUCCESS;
}

unsigned int stack_back(stack_t *stack,void *output)
{
	if(stack_empty(stack)){
		return STACK_EMPTY;
	}
	if(output){
		memcpy(output,refer_back(stack),stack->element_size);
	}
	return STACK_SUCCESS;
}

unsigned int stack_refer_from_front
    (stack_t *stack,const size_t offset,void *output)
{
	if(offset >= stack_size(stack)){
		return STACK_OFFSET_IS_TOO_LARGE;
	}
	if(output){
		memcpy(output,refer_by_offset_from_front(stack,offset)
		    ,stack->element_size);
	}
	return STACK_SUCCESS;
}

unsigned int stack_refer_from_back
    (stack_t *stack,const size_t offset,void *output)
{
	if(offset >= stack_size(stack)){
		return STACK_OFFSET_IS_TOO_LARGE;
	}
	if(output){
		memcpy(output,refer_by_offset_from_back(stack,offset)
		    ,stack->element_size);
	}
	return STACK_SUCCESS;
}

unsigned int stack_push(stack_t *stack,const void *input)
{
	void *temp;
	if(stack->size == stack->array_size){
		temp = realloc(stack->array,(stack->array_size+32)*stack->element_size);
		if(!temp){
			return STACK_MEMORY_ALLOCATION_ERROR;
		}
		stack->array = temp;
		stack->array_size = stack->array_size+32;
	}
	memcpy(refer_by_offset(stack,stack->size),input,stack->element_size);
	stack->size++;
	return STACK_SUCCESS;
}

unsigned int stack_pop(stack_t *stack,void *output)
{
	if(stack_empty(stack)){
		return STACK_EMPTY;
	}
	stack->size--;
	if(output){
		memcpy(output,refer_by_offset(stack,stack->size),stack->element_size);
	}
	return STACK_SUCCESS;
}

unsigned int stack_push_many_elements
    (stack_t *stack,const size_t push_size,const void *input)
{
	void *temp;
	if(stack->size+push_size > stack->array_size){
		temp = realloc(stack->array
		    ,((stack->size+push_size&~31)+32)*stack->element_size);
		if(!temp){
			return STACK_MEMORY_ALLOCATION_ERROR;
		}
		stack->array = temp;
		stack->array_size = (stack->size+push_size&~31)+32;
	}
	memcpy(refer_by_offset(stack,stack->size)
	    ,input,stack->element_size*push_size);
	stack->size += push_size;
	return STACK_SUCCESS;
}

unsigned int stack_pop_many_elements
    (stack_t *stack,const size_t pop_size,void *output)
{
	size_t counter = pop_size;
	if(stack->size < pop_size){
		return STACK_EMPTY;
	}
	if(output){
		while(counter){
			memcpy(output+stack->element_size*(pop_size-counter)
			    ,refer_by_offset_from_back(stack,pop_size-counter)
			    ,stack->element_size);
			counter--;
		}
	}
	stack->size -= pop_size;
	return STACK_SUCCESS;
}
