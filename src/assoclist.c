
/*
 * Functions for the associative array data structure
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
	Constants
*******************************************************************************/

#define HASH_TYPES                                          4

/*******************************************************************************
	Macros
*******************************************************************************/

#define hash(assoclist,key,number) \
    (hash_function(key,number)&((assoclist)->array_size-1))

#define get_used_flag_by_offset(assoclist,offset) \
    ((assoclist)->element_info_array[offset].used_flag)

#define get_mode_flag_by_offset(assoclist,offset) \
    ((assoclist)->element_info_array[offset].mode_flag)

#define get_hash_type_by_offset(assoclist,offset) \
    ((assoclist)->element_info_array[offset].hash_type)

#define get_long_key_by_offset(assoclist,offset) \
    ((assoclist)->element_info_array[offset].key.long_key)

#define get_short_key_by_offset(assoclist,offset) \
    ((assoclist)->element_info_array[offset].key.short_key)

#define get_key_by_offset(assoclist,offset) \
    (get_mode_flag_by_offset(assoclist,offset) \
        ?get_long_key_by_offset(assoclist,offset) \
        :get_short_key_by_offset(assoclist,offset))

#define free_long_key_by_offset(assoclist,offset) \
    (get_mode_flag_by_offset(assoclist,offset) \
    && (free(get_long_key_by_offset(assoclist,offset)),1))

#define get_value_by_offset(assoclist,offset) \
    ((assoclist)->value_array+(assoclist)->element_size*(offset))

#define compare_key_and_offset(assoclist,key,offset) \
    (get_used_flag_by_offset(assoclist,offset) \
    && !strcmp(key,get_key_by_offset(assoclist,offset)))

#define is_invalid_key(key) \
    (!(key) || !(key)[0])

/*******************************************************************************
	Functions
*******************************************************************************/

static size_t hash_function(const char *string,size_t num)
{
	size_t counter = 0,result = num;
	while(string[counter]){
		result = (result*0x00000129)^string[counter];
		counter++;
	}
	return result;
}

static unsigned int assoclist_private_lookup(assoclist_t *assoclist
    ,const char *key,size_t *offset)
{
	size_t hash_value,counter = 0;
	while(counter != HASH_TYPES){
		hash_value = hash(assoclist,key,counter);
		if(counter == get_hash_type_by_offset(assoclist,hash_value)
		    && compare_key_and_offset(assoclist,key,hash_value)){
			if(offset){
				*offset = hash_value;
			}
			return ASSOCLIST_SUCCESS;
		}
		counter++;
	}
	return ASSOCLIST_NOT_EXISTENT_KEY;
}

static int assoclist_existent(assoclist_t *assoclist,const char *key)
{
	size_t hash_value,counter = 0;
	while(counter != HASH_TYPES){
		hash_value = hash(assoclist,key,counter);
		if(counter == get_hash_type_by_offset(assoclist,hash_value)
		    && compare_key_and_offset(assoclist,key,hash_value)){
			return 1;
		}
		counter++;
	}
	return 0;
}

static unsigned int assoclist_resize(assoclist_t *assoclist)
{
	size_t counter = 0;
	void *temp;
	temp = realloc(assoclist->element_info_array
	    ,sizeof(assoclist_element_info_t)*assoclist->array_size*2);
	if(!temp){
		return ASSOCLIST_MEMORY_ALLOCATION_ERROR;
	}
	assoclist->element_info_array = temp;
	temp = realloc(assoclist->value_array
	    ,assoclist->element_size*assoclist->array_size*2);
	if(!temp){
		return ASSOCLIST_MEMORY_ALLOCATION_ERROR;
	}
	assoclist->value_array = temp;
	while(counter != assoclist->array_size){
		if(get_used_flag_by_offset(assoclist,counter)
		    && (hash_function(get_key_by_offset(assoclist,counter)
		    ,get_hash_type_by_offset(assoclist,counter))
		    &assoclist->array_size)){
			assoclist->element_info_array[counter+assoclist->array_size]
			    = assoclist->element_info_array[counter];
			memcpy(get_value_by_offset(assoclist,counter+assoclist->array_size)
			    ,get_value_by_offset(assoclist,counter)
			    ,assoclist->element_size);
			get_used_flag_by_offset(assoclist,counter) = 0;
		}
		else{
			get_used_flag_by_offset(assoclist,counter+assoclist->array_size)
			    = 0;
		}
		counter++;
	}
	assoclist->array_size = assoclist->array_size*2;
	return ASSOCLIST_SUCCESS;
}

static unsigned int assoclist_trymove(assoclist_t *assoclist
    ,size_t offset,unsigned int nest_counter)
{
	size_t move_to,counter = 0;
	unsigned int errcode;
	assoclist_element_info_t *current_info
	    = &assoclist->element_info_array[offset];
	if(!get_used_flag_by_offset(assoclist,offset)){
		return ASSOCLIST_SUCCESS;
	}
	if(!nest_counter){
		return ASSOCLIST_HASH_COLLISION;
	}
	while(counter != HASH_TYPES){
		if(counter != get_hash_type_by_offset(assoclist,offset)){
			move_to
			    = hash(assoclist,get_key_by_offset(assoclist,offset),counter);
			errcode = assoclist_trymove(assoclist,move_to,nest_counter-1);
			if(!errcode){
				assoclist->element_info_array[move_to]
				    = assoclist->element_info_array[offset];
				get_hash_type_by_offset(assoclist,move_to) = counter;
				memcpy(get_value_by_offset(assoclist,move_to)
					,get_value_by_offset(assoclist,offset)
				    ,assoclist->element_size);
				get_used_flag_by_offset(assoclist,offset) = 0;
				return ASSOCLIST_SUCCESS;
			}
		}
		counter++;
	}
	return ASSOCLIST_HASH_COLLISION;
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
	assoclist->release_function = release_function?release_function
	                                              :dummy_release_function;
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
	if(assoclist){
		while(counter != assoclist->array_size){
			if(get_used_flag_by_offset(assoclist,counter)){
				free_long_key_by_offset(assoclist,counter);
				(*assoclist->release_function)
				    (get_value_by_offset(assoclist,counter));
			}
			counter++;
		}
		free(assoclist->element_info_array);
		free(assoclist->value_array);
		free(assoclist);
	}
}

unsigned int assoclist_add(assoclist_t *assoclist
    ,const char *key,const void *input)
{
	size_t hash_value,counter = 0,key_length;
	unsigned int errcode;
	if(is_invalid_key(key)){
		return ASSOCLIST_INVALID_KEY;
	}
	if(assoclist_existent(assoclist,key)){
		return ASSOCLIST_KEY_COLLISION;
	}
	if(assoclist->size*4 >= assoclist->array_size*3){
		errcode = assoclist_resize(assoclist);
		if(errcode){
			return errcode;
		}
	}
	while(counter != HASH_TYPES){
		hash_value = hash(assoclist,key,counter);
		errcode = assoclist_trymove(assoclist,hash_value,2);
		if(!errcode){
			break;
		}
		counter++;
	}
	if(errcode){
		errcode = assoclist_resize(assoclist);
		if(errcode){
			return errcode;
		}
		counter = 0;
		while(1){
			hash_value = hash(assoclist,key,counter);
			errcode = assoclist_trymove(assoclist,hash_value,4);
			if(!errcode){
				break;
			}
			counter++;
			if(counter == HASH_TYPES){
				return ASSOCLIST_HASH_COLLISION;
			}
		}
	}
	key_length = strlen(key);
	if(get_mode_flag_by_offset(assoclist,hash_value)
	    = key_length >= ASSOCLIST_MAX_OF_SHORT_KEY_SIZE){
		get_long_key_by_offset(assoclist,hash_value) = malloc(key_length+1);
		if(!get_long_key_by_offset(assoclist,hash_value)){
			return ASSOCLIST_MEMORY_ALLOCATION_ERROR;
		}
	}
	memcpy(get_key_by_offset(assoclist,hash_value),key,key_length+1);
	get_used_flag_by_offset(assoclist,hash_value) = 1;
	get_hash_type_by_offset(assoclist,hash_value) = counter;
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
	if(is_invalid_key(key)){
		return ASSOCLIST_INVALID_KEY;
	}
	errcode = assoclist_private_lookup(assoclist,key,&offset);
	if(errcode){
		return errcode;
	}
	(*assoclist->release_function)(get_value_by_offset(assoclist,offset));
	memcpy(get_value_by_offset(assoclist,offset),input,assoclist->element_size);
	return ASSOCLIST_SUCCESS;
}

unsigned int assoclist_lookup(assoclist_t *assoclist
    ,const char *key,void *output)
{
	size_t offset;
	unsigned int errcode;
	if(is_invalid_key(key)){
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
	if(is_invalid_key(key)){
		return ASSOCLIST_INVALID_KEY;
	}
	errcode = assoclist_private_lookup(assoclist,key,&offset);
	if(errcode){
		return errcode;
	}
	free_long_key_by_offset(assoclist,offset);
	if(output){
		memcpy(output
		    ,get_value_by_offset(assoclist,offset),assoclist->element_size);
	}
	else{
		(*assoclist->release_function)(get_value_by_offset(assoclist,offset));
	}
	get_used_flag_by_offset(assoclist,offset) = 0;
	assoclist->size--;
	return ASSOCLIST_SUCCESS;
}
