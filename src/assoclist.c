
/*
 * Functions for the associative array data structure
 * Copyright (c) 2008-2009, Kazuhiko Sakaguchi All rights reserved.
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

#define GLIBC_ALLOCA                                        1

#include <stdlib.h>
#include <string.h>

#if GLIBC_ALLOCA
#include <alloca.h>
#endif

#include "assoclist.h"

/*******************************************************************************
	Macros
*******************************************************************************/

#define void_pointer_addition(pointer,number) \
    ((void *)((char *)pointer+number))

#define hash(assoclist,key,number) \
    (hash_function(key,number)&((assoclist)->array_size-1))

#define get_element_info_by_offset(assoclist,offset) \
    (&(assoclist)->element_info_array[offset])

#define get_used_flag(element_info) \
    ((element_info)->used_flag)

#define get_used_flag_by_offset(assoclist,offset) \
    ((assoclist)->element_info_array[offset].used_flag)

#define get_mode_flag(element_info) \
    ((element_info)->mode_flag)

#define get_mode_flag_by_offset(assoclist,offset) \
    ((assoclist)->element_info_array[offset].mode_flag)

#define get_hash_id(element_info) \
    ((element_info)->hash_id)

#define get_hash_id_by_offset(assoclist,offset) \
    ((assoclist)->element_info_array[offset].hash_id)

#define get_long_key(element_info) \
    ((element_info)->key.long_key)

#define get_long_key_by_offset(assoclist,offset) \
    ((assoclist)->element_info_array[offset].key.long_key)

#define get_short_key(element_info) \
    ((element_info)->key.short_key)

#define get_short_key_by_offset(assoclist,offset) \
    ((assoclist)->element_info_array[offset].key.short_key)

#define get_key(element_info) \
    (get_mode_flag(element_info)?get_long_key(element_info) \
                                :get_short_key(element_info))

#define get_key_by_offset(assoclist,offset) \
    (get_mode_flag_by_offset(assoclist,offset) \
        ?get_long_key_by_offset(assoclist,offset) \
        :get_short_key_by_offset(assoclist,offset))

#define free_long_key(element_info) \
    (get_mode_flag(element_info) && (free(get_long_key(element_info)),1))

#define free_long_key_by_offset(assoclist,offset) \
    (get_mode_flag_by_offset(assoclist,offset) \
    && (free(get_long_key_by_offset(assoclist,offset)),1))

#define get_value_by_offset(assoclist,offset) \
    (void_pointer_addition((assoclist)->value_array \
    ,(assoclist)->element_size*(offset)))

#define compare_key_and_element_info(key,element_info) \
    (get_used_flag(element_info)  && compare_string(key,get_key(element_info)))

#define compare_key_and_offset(assoclist,key,offset) \
    (get_used_flag_by_offset(assoclist,offset) \
    && compare_string(key,get_key_by_offset(assoclist,offset)))

/*******************************************************************************
	Functions
*******************************************************************************/

static int compare_string(const char *str1,const char *str2)
{
	while(*str1 == *str2){
		if(!*str1){
			return 1;
		}
		str1++;
		str2++;
	}
	return 0;
}

static size_t hash_function(const char *string,size_t num)
{
	size_t result = num;
	while(*string){
		result = (result*0x00000129)^*string;
		string++;
	}
	return result;
}

static unsigned int assoclist_private_lookup(assoclist_t *assoclist
    ,const char *key,size_t *offset)
{
	size_t hash_value,hash_id = 0;
	while(hash_id != ASSOCLIST_HASH_TYPES){
		hash_value = hash(assoclist,key,hash_id);
		if(hash_id == get_hash_id_by_offset(assoclist,hash_value)
		    && compare_key_and_offset(assoclist,key,hash_value)){
			if(offset){
				*offset = hash_value;
			}
			return ASSOCLIST_SUCCESS;
		}
		hash_id++;
	}
	return ASSOCLIST_NOT_EXISTENT_KEY;
}

static unsigned int assoclist_resize(assoclist_t *assoclist)
{
	size_t element_counter = 0,array_size = assoclist->array_size;
	void *temp = realloc(assoclist->element_info_array
	    ,sizeof(assoclist_element_info_t)*array_size*2);
	assoclist_element_info_t *element_info;
	if(!temp){
		return ASSOCLIST_MEMORY_ALLOCATION_ERROR;
	}
	assoclist->element_info_array = temp;
	temp = realloc(assoclist->value_array,assoclist->element_size*array_size*2);
	if(!temp){
		return ASSOCLIST_MEMORY_ALLOCATION_ERROR;
	}
	assoclist->value_array = temp;
	while(element_counter != array_size){
		element_info = get_element_info_by_offset(assoclist,element_counter);
		if(get_used_flag(element_info) && (hash_function(get_key(element_info)
		    ,get_hash_id(element_info))&array_size)){
			*get_element_info_by_offset(assoclist,element_counter+array_size)
			    = *get_element_info_by_offset(assoclist,element_counter);
			memcpy(get_value_by_offset(assoclist,element_counter+array_size)
			    ,get_value_by_offset(assoclist,element_counter)
			    ,assoclist->element_size);
			get_used_flag(element_info) = 0;
		}
		else{
			get_used_flag(element_info+array_size) = 0;
		}
		element_counter++;
	}
	assoclist->array_size = array_size*2;
	return ASSOCLIST_SUCCESS;
}

static unsigned int assoclist_trymove(assoclist_t *assoclist
    ,size_t offset,unsigned int recur_limit)
{
	typedef struct
	{
		size_t offset;
		assoclist_element_info_t *element_info;
		unsigned int hash_id;
	} trymove_stack_t;
	trymove_stack_t *stack_top,*stack_bottom;
	if(!get_used_flag_by_offset(assoclist,offset)){
		return ASSOCLIST_SUCCESS;
	}
#if GLIBC_ALLOCA
	stack_top = stack_bottom = alloca(sizeof(trymove_stack_t)*(recur_limit+1));
#else
	stack_top = stack_bottom = malloc(sizeof(trymove_stack_t)*(recur_limit+1));
#endif
	if(!stack_bottom){
		return ASSOCLIST_MEMORY_ALLOCATION_ERROR;
	}
	stack_top->offset = offset;
	stack_top->element_info = get_element_info_by_offset(assoclist,offset);
	stack_top->hash_id = 0;
	while(1){
		while(stack_top-stack_bottom != recur_limit-1){
			stack_top++;
			stack_top->offset = hash(assoclist
			    ,get_key((stack_top-1)->element_info),(stack_top-1)->hash_id);
			stack_top->element_info
			    = get_element_info_by_offset(assoclist,stack_top->offset);
			stack_top->hash_id = 0;
			if(!get_used_flag(stack_top->element_info)){
				while(stack_top != stack_bottom){
					*(stack_top->element_info) = *((stack_top-1)->element_info);
					stack_top->element_info->hash_id = (stack_top-1)->hash_id;
					memcpy(get_value_by_offset(assoclist,stack_top->offset)
					    ,get_value_by_offset(assoclist,(stack_top-1)->offset)
					    ,assoclist->element_size);
					stack_top--;
				}
				get_used_flag(stack_top->element_info) = 0;
#if !GLIBC_ALLOCA
				free(stack_bottom);
#endif
				return ASSOCLIST_SUCCESS;
			}
		}
		stack_top--;
		stack_top->hash_id++;
		while(stack_top->hash_id == ASSOCLIST_HASH_TYPES){
			if(stack_top == stack_bottom){
#if !GLIBC_ALLOCA
				free(stack_bottom);
#endif
				return ASSOCLIST_HASH_COLLISION;
			}
			stack_top--;
			stack_top->hash_id++;
		}
	}
}

assoclist_t *assoclist_initialize(const size_t element_size
    ,void (*release_function)(void *))
{
	assoclist_t *assoclist = malloc(sizeof(assoclist_t));
	if(!assoclist){
		return NULL;
	}
	assoclist->size = 0;
	assoclist->element_size = element_size;
	assoclist->array_size = ASSOCLIST_DEFAULT_ARRAY_SIZE;
	assoclist->release_function = release_function;
	assoclist->element_info_array
	    = calloc(ASSOCLIST_DEFAULT_ARRAY_SIZE,sizeof(assoclist_element_info_t));
	assoclist->value_array = calloc(ASSOCLIST_DEFAULT_ARRAY_SIZE,element_size);
	if(!(assoclist->element_info_array && assoclist->value_array)){
		free(assoclist->element_info_array);
		free(assoclist->value_array);
		free(assoclist);
		return NULL;
	}
	return assoclist;
}

void assoclist_release(assoclist_t *assoclist)
{
	size_t counter = 0;
	if(!assoclist){
		return;
	}
	while(counter != assoclist->array_size){
		assoclist_element_info_t *element_info
		    = get_element_info_by_offset(assoclist,counter);
		if(get_used_flag(element_info)){
			free_long_key(element_info);
			if(assoclist->release_function){
				assoclist->release_function
				    (get_value_by_offset(assoclist,counter));
			}
		}
		counter++;
	}
	free(assoclist->element_info_array);
	free(assoclist->value_array);
	free(assoclist);
}

unsigned int assoclist_add(assoclist_t *assoclist
    ,const char *key,const void *input)
{
	size_t hash_value,hash_id,key_length;
	unsigned int errcode,counter = 1;
	assoclist_element_info_t *element_info;
	if(!key){
		return ASSOCLIST_INVALID_KEY;
	}
	if(!assoclist_private_lookup(assoclist,key,NULL)){
		return ASSOCLIST_KEY_COLLISION;
	}
	if(assoclist->size*4 >= assoclist->array_size*3){
		/* 
			もしハッシュテーブルの3/4がデータで埋まっていれば、リサイズを試みる。
			失敗した場合でもエラー処理の必要は無い。
		*/
		assoclist_resize(assoclist);
	}
	while(1){
		hash_id = 0;
		while(hash_id != ASSOCLIST_HASH_TYPES){
			hash_value = hash(assoclist,key,hash_id);
			if(!assoclist_trymove(assoclist,hash_value,counter*2)){
				goto SUCCEED_IN_TRYMOVE;
			}
			hash_id++;
		}
		if(counter == 1 && (errcode = assoclist_resize(assoclist))){
			return errcode;
		}
		else if(counter == 2){
			return ASSOCLIST_HASH_COLLISION;
		}
		counter++;
	}
	SUCCEED_IN_TRYMOVE:;
	key_length = strlen(key);
	element_info = get_element_info_by_offset(assoclist,hash_value);
	get_mode_flag(element_info)
	    = (key_length >= ASSOCLIST_MAX_OF_SHORT_KEY_SIZE);
	if(get_mode_flag(element_info)){
		get_long_key(element_info) = malloc(key_length+1);
		if(!get_long_key(element_info)){
			return ASSOCLIST_MEMORY_ALLOCATION_ERROR;
		}
		memcpy(get_long_key(element_info),key,key_length+1);
	}
	else{
		memcpy(get_short_key(element_info),key,key_length+1);
	}
	get_used_flag(element_info) = 1;
	get_hash_id(element_info) = hash_id;
	memcpy(get_value_by_offset(assoclist,hash_value)
	    ,input,assoclist->element_size);
	assoclist->size++;
	return ASSOCLIST_SUCCESS;
}

unsigned int assoclist_reassign(assoclist_t *assoclist
    ,const char *key,const void *input)
{
	size_t offset;
	unsigned int errcode;
	if(!key){
		return ASSOCLIST_INVALID_KEY;
	}
	errcode = assoclist_private_lookup(assoclist,key,&offset);
	if(errcode){
		return errcode;
	}
	if(assoclist->release_function){
		assoclist->release_function(get_value_by_offset(assoclist,offset));
	}
	memcpy(get_value_by_offset(assoclist,offset),input,assoclist->element_size);
	return ASSOCLIST_SUCCESS;
}

unsigned int assoclist_lookup(assoclist_t *assoclist
    ,const char *key,void *output)
{
	size_t offset;
	unsigned int errcode;
	if(!key){
		return ASSOCLIST_INVALID_KEY;
	}
	errcode = assoclist_private_lookup(assoclist,key,&offset);
	if(errcode){
		return errcode;
	}
	if(output){
		memcpy(output
		    ,get_value_by_offset(assoclist,offset),assoclist->element_size);
	}
	return ASSOCLIST_SUCCESS;
}

unsigned int assoclist_remove(assoclist_t *assoclist
    ,const char *key,void *output)
{
	size_t offset;
	unsigned int errcode;
	assoclist_element_info_t *element_info;
	if(!key){
		return ASSOCLIST_INVALID_KEY;
	}
	errcode = assoclist_private_lookup(assoclist,key,&offset);
	if(errcode){
		return errcode;
	}
	element_info = get_element_info_by_offset(assoclist,offset);
	free_long_key(element_info);
	if(output){
		memcpy(output
		    ,get_value_by_offset(assoclist,offset),assoclist->element_size);
	}
	else if(assoclist->release_function){
		assoclist->release_function(get_value_by_offset(assoclist,offset));
	}
	get_used_flag(element_info) = 0;
	assoclist->size--;
	return ASSOCLIST_SUCCESS;
}
