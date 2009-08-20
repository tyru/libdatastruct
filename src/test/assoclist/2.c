
/*
 * Test code for the associative array data structure
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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../libdatastruct.h"

void free_object(void *pointer)
{
	free(*(void **)pointer);
}

char *integer_to_string(unsigned int integer)
{
	static char buf[32];
	size_t counter = 31;
	buf[31] = '\0';
	if(!integer){
		counter--;
		buf[30] = '0';
	}
	while(integer){
		counter--;
		buf[counter] = (integer&15)<10?'0'+(integer&15)
		                              :'A'-10+(integer&15);
		integer = integer/16;
	}
	return &buf[counter];
}

int main(void)
{
	assoclist_t *assoclist;
	unsigned int errcode;
	size_t counter = 0;
	void *pointer;
	void *checkbuf[65536];
	assoclist = assoclist_initialize(sizeof(void *),free_object);
	if(!assoclist){
		return -1;
	}
	while(counter != 65536){
		pointer = malloc(1);
		checkbuf[counter] = pointer;
		errcode = assoclist_add(assoclist,integer_to_string(counter),&pointer);
		if(errcode){
			fputs("Error!\n",stderr);
			return -1;
		}
		fprintf(stdout,"add : %s,%d\n",integer_to_string(counter),pointer);
		counter++;
	}
	counter = 0;
	while(counter != 65536){
		errcode = assoclist_lookup(assoclist
		    ,integer_to_string(counter),&pointer);
		if(errcode){
			fputs("Error!\n",stderr);
		}
		fprintf(stdout,"find : %s,%d\n",integer_to_string(counter),pointer);
		assert(checkbuf[counter] == pointer);
		counter++;
	}
	assoclist_release(assoclist);
	return 0;
}
