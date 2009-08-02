
/*
 * Test code for the queue data structure
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
#include "../../libdatastruct.h"

int main(void)
{
	queue_t *queue;
	int errcode;
	size_t counter = 0,integer,integer_;
	queue = queue_initialize(sizeof(int),NULL);
	if(!queue){
		fputs("Error!\n",stderr);
		return -1;
	}
	while(counter != 128){
		integer = counter+10;
		errcode = queue_enqueue(queue,&integer);
		if(errcode){
			fputs("Error!\n",stderr);
			queue_release(queue);
			return -1;
		}
		counter++;
	}
	counter = 0;
	while(counter != 32){
		errcode = queue_dequeue(queue,&integer);
		if(errcode){
			fputs("Error!\n",stderr);
			queue_release(queue);
			return -1;
		}
		counter++;
	}
	counter = 0;
	while(counter != queue_size(queue)){
		errcode = queue_refer_from_front(queue,counter,&integer);
		if(errcode){
			fputs("Error!\n",stderr);
			queue_release(queue);
			return -1;
		}
		errcode = queue_refer_from_back(queue,counter,&integer_);
		if(errcode){
			fputs("Error!\n",stderr);
			queue_release(queue);
			return -1;
		}
		printf("%03d : %03d %03d\n",counter,integer,integer_);
		counter++;
	}
	queue_release(queue);
	return 0;
}
