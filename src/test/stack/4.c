
/*
 * Test code for the stack data structure
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

int my_abs(int n){
	return (0 < n)?n
	              :-n;
}

int main(void)
{
	stack_t *stack;
	unsigned int errcode,integer,array[32];
	size_t counter = 0,counter_,counter__;
	stack = stack_initialize(sizeof(unsigned int),NULL);
	if(!stack){
		fputs("Error!\n",stderr);
		return -1;
	}
	while(counter != 32){
		integer = counter;
		errcode = stack_push(stack,&integer);
		if(errcode){
			fputs("Error!\n",stderr);
			stack_release(stack);
			return -1;
		}
		counter++;
	}
	counter = 0;
	while(counter != 40){
		counter_ = 0;
		while(counter_ != 40){
			printf("from_top %02d %02d\n",counter,counter_);
			errcode = stack_refer_many_elements_from_top
			    (stack,counter,counter_,array);
			if(errcode){
				fputs("\tOffset is too large.\n",stdout);
			}
			else{
				counter__ = 0;
				fputc('\t',stdout);
				while(counter__ != my_abs(counter-counter_)+1){
					printf("%02d ",array[counter__]);
					counter__++;
				}
				fputc('\n',stdout);
			}
			printf("from_bottom %02d %02d\n",counter,counter_);
			errcode = stack_refer_many_elements_from_bottom
			    (stack,counter,counter_,array);
			if(errcode){
				fputs("\tOffset is too large.\n",stdout);
			}
			else{
				counter__ = 0;
				fputc('\t',stdout);
				while(counter__ != my_abs(counter-counter_)+1){
					printf("%02d ",array[counter__]);
					counter__++;
				}
				fputc('\n',stdout);
			}
			counter_ += 4;
		}
		counter += 4;
	}
	stack_release(stack);
	return 0;
}
