
/*
 * Test code for the double-ended queue
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

#include <stdio.h>
#include <stdlib.h>
#include "../../libdatastruct.h"

void free_object(void *pointer)
{
	free(*(void **)pointer);
}

int main(void)
{
	deque_t *deque;
	int errcode;
	size_t counter = 0;
	void *pointer;
	deque = deque_initialize(sizeof(void *),free_object);
	if(!deque){
		fputs("Error!\n",stderr);
		return -1;
	}
	while(counter != 128){
		pointer = malloc(1);
		errcode = deque_push_front(deque,&pointer);
		if(errcode){
			deque_release(deque);
			fputs("Error!\n",stderr);
			return -1;
		}
		fprintf(stdout,"push_front : %d\n",pointer);
		if(counter%3){
			errcode = deque_pop_back(deque,&pointer);
			if(errcode){
				deque_release(deque);
				fputs("Error!\n",stderr);
				return -1;
			}
			fprintf(stdout,"pop_back   : %d\n",pointer);
			free_object(&pointer);
		}
		else{
			pointer = malloc(1);
			errcode = deque_push_back(deque,&pointer);
			if(errcode){
				deque_release(deque);
				fputs("Error!\n",stderr);
				return -1;
			}
			fprintf(stdout,"push_back  : %d\n",pointer);
		}
		counter++;
	}
	while(deque_size(deque) != 10){
		errcode = deque_pop_front(deque,&pointer);
		if(errcode){
			deque_release(deque);
			fputs("Error\n",stderr);
			return -1;
		}
		fprintf(stdout,"pop_front  : %d\n",pointer);
		free_object(&pointer);
	}
	deque_release(deque);
	return 0;
}
