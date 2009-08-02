
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

int read_integer(FILE *input)
{
	int result = 0,is_minus_flag = 0,getf;
	if(ferror(input)){
		return 0;
	}
	getf = fgetc(input);
	if(getf == '-'){
		is_minus_flag = 1;
	}
	else if(getf != '+'){
		ungetc(getf,input);
	}
	while(1){
		if(ferror(input)){
			return 0;
		}
		getf = fgetc(input);
		if('0' <= getf && getf <= '9'){
			result = result*10+getf-'0';
		}
		else{
			ungetc(getf,input);
			break;
		}
	}
	return is_minus_flag?-result
	                    :result;
}

int ignore_space(FILE *input)
{
	int getf;
	while(1){
		if(ferror(input)){
			return -1;
		}
		getf = fgetc(input);
		if(!isspace(getf) || getf == '\n'){
			ungetc(getf,input);
			break;
		}
	}
	return 0;
}

int ignore_to_newline(FILE *input)
{
	int getf;
	while(1){
		if(ferror(input)){
			return -1;
		}
		getf = fgetc(input);
		if(getf == '\n'){
			break;
		}
	}
	return 0;
}

int stack_trace(stack_t *stack)
{
	size_t counter = 0;
	while(counter != stack->size){
		printf("%d ",((int *)stack->array)[counter]);
		counter++;
	}
	fputc('\n',stdout);
	return 0;
}

int main(void)
{
	stack_t *stack;
	int getf,buffer[128],errcode;
	size_t counter,size;
	stack = stack_initialize(sizeof(int),NULL);
	if(!stack){
		fputs("Error!\n",stderr);
		return -1;
	}
	while(1){
		ignore_space(stdin);
		getf = fgetc(stdin);
		if(getf == 's'){
			fputs("set ",stdout);
			counter = 0;
			while(counter != 128){
				ignore_space(stdin);
				getf = fgetc(stdin);
				ungetc(getf,stdin);
				if(getf == '\n'
				    || (getf != '+' && getf != '-' && !isdigit(getf))){
					break;
				}
				buffer[counter] = read_integer(stdin);
				printf("%d ",buffer[counter]);
				counter++;
			}
			ignore_to_newline(stdin);
			errcode = stack_push_many_elements(stack,counter,buffer);
			if(errcode){
				fputs("Error!\n",stderr);
				return -1;
			}
			fputc('\n',stdout);
			fputs("stack trace : ",stdout);
			stack_trace(stack);
		}
		else if(getf == 'g'){
			fputs("get ",stdout);
			ignore_space(stdin);
			size = read_integer(stdin);
			ignore_to_newline(stdin);
			errcode = stack_pop_many_elements(stack,size,buffer);
			if(errcode){
				fputs("Error!\n",stderr);
				return -1;
			}
			counter = 0;
			while(counter != size){
				printf("%d ",buffer[counter]);
				counter++;
			}
			fputc('\n',stdout);
			fputs("stack trace : ",stdout);
			stack_trace(stack);
		}
		else if(!isspace(getf)){
			break;
		}
	}
	stack_release(stack);
	return 0;
}
