
/*
 * Test code for the stack data structure
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

#include <stdio.h>
#include "../../libdatastruct.h"

int main(void)
{
	stack_t *stack;
	unsigned int errcode,integer,integer_;
	size_t counter = 0;
	stack = stack_initialize(sizeof(unsigned int),NULL);
	if(!stack){
		fputs("Error!\n",stderr);
		return -1;
	}
	while(counter != 128){
		integer = counter+10;
		errcode = stack_push(stack,&integer);
		if(errcode){
			fputs("Error!\n",stderr);
			stack_release(stack);
			return -1;
		}
		if(!(counter%3)){
			errcode = stack_pop(stack,&integer);
			if(errcode){
				fputs("Error!\n",stderr);
				stack_release(stack);
				return -1;
			}
		}
		counter++;
	}
	counter = 0;
	while(counter != stack_size(stack)){
		errcode = stack_refer_from_bottom(stack,counter,&integer);
		if(errcode){
			fputs("Error!\n",stderr);
			stack_release(stack);
			return -1;
		}
		errcode = stack_refer_from_top(stack,counter,&integer_);
		if(errcode){
			fputs("Error!\n",stderr);
			stack_release(stack);
			return -1;
		}
		fprintf(stdout,"%03d : %03d %03d\n",counter,integer,integer_);
		counter++;
	}
	stack_release(stack);
	return 0;
}
