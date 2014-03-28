/*
 * string.h
 *
 *  Created on: Mar 19, 2014
 *      Author: Jesus, Anthony
 */

#ifndef STRING_H_
#define STRING_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

//////////////////////////
// API Layer 0
unsigned stringGetNumberOfDigits (unsigned i);
void stringITOA(int value, char* out);
void stringConcat(char* one, char* two, char* out);
void stringCopy(char* value, char* buffer);
void stringFTOA(float value, char* out );
double stringSTOD(char *string);
int stringSTOI(char *string);
int stringLength(char *string);
int stringEquals(char* one, char* two);
void stringSplit(char* s, char delimiter, char* out1, char* out2);

#endif /* STRING_H_ */
