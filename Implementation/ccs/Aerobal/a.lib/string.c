/*
 * string.c
 *
 *  Created on: Mar 19, 2014
 *      Author: Administrator
 */


#include "string.h"

//////////////////////////////
// API Layer 0

/*
 * Returns the number of digits for
 * an unsigned integer.
 */
unsigned stringGetNumberOfDigits (unsigned i)
{
	return i > 0 ? (int) log10 ((double) i) + 1 : 1;
}

/*
 * Converts an integer value to a string.
 */
void stringITOA(int value, char* out)
{
	int size = stringGetNumberOfDigits(value);
	int neg = (value < 0);
	if(neg) {
		value *= -1;
		size = stringGetNumberOfDigits(value) + 1;
	}
	out[size] = '\0';
	int i = size - 1;
	for(;i >= 0; i--) {
		out[i] = value%10 + '0';
		value /= 10;
	}
	if(neg)
		out[0] = '-';
}

/*
 * Concatenates two string together.
 */
void stringConcat(char* one, char* two, char* out) {
	int i;
	for(i = 0; one[i] != '\0'; i++) {
		out[i] = one[i];
	}
	int j;
	for(j = 0; two[j] != '\0'; j++) {
		out[j + i] = two[j];
	}
	out[i + j] = '\0';
}

/*
 * Copies a string.
 */
void stringCopy(char* value, char* buffer) {
	int i = 0;
	for(i = 0; value[i] != '\0'; i++) {
		buffer[i] = value[i];
	}
	buffer[i] = '\0';
}

/*
 * Converts a float to a string.
 */
void stringFTOA(float value, char* out ) {

	int neg = (value < 0);
	if(neg)
		value *= -1;
	int integer = (int) value;
	stringITOA(integer,out);
	if(neg) {
		char buffer[20];
		stringCopy(out, buffer);
		stringConcat("-",buffer,out);
	}
    char* pt = ".";
	stringConcat(out,pt,out);
	char curr[6];
	stringITOA((int)((value - (int)value)*10000),curr);
	stringConcat(out,curr,out);

}
/*
 * Converts a string to a double value.
 * Does not verify characters.
 */
double stringSTOD(char *string){

    int length = stringLength(string);
    int i=0;
    int j=0;
    if (string[i]=='-') {
        i++;
        j++;
    }
    for (; string[i] != '\0'; i++) {
        if(string[i]=='.'){
            break;
        }
    }

    double value = 0;

    for(; j<i; j++){
        value *=10;
        value += string[j] - '0';
    }

    if(length != i){
        //There's a rational part.
        int k = 1;
        float svalue = 0;

        for(j = k+i; string[j] != '\0'; j++){
            svalue *=10;
            svalue += string[j] - '0';
            k++;
        }

        float div = pow(10, k-1);
        svalue = (double) svalue / (double)div;
        value = (double) value +  (double) svalue;

    }
    if (string[0]=='-') {
        value*=-1;
    }
    return value;
}

/*
 * Converts a string to an integer.
 * Does not verify other characters.
 */
int stringSTOI(char *string){
    int value = 0;
    int i=0;
    if (string[i]=='-') {
        i++;
    }
    for(; string[i] != '\0'; i++){
        value *=10;
        value += string[i] - '0';
    }
    if (string[i]=='-') {
        value*=-1;
    }
    return value;
}

/*
 * Calculates the length of
 * a string.
 */
int stringLength(char *string){
    int i ;
    for (i = 0; string[i] != '\0'; i++);
    return i;
}

/*
 * Determines whether the string one is
 * equal to the string two by evaluating
 * character by character.
 */
int stringEquals(char* one, char* two) {
	int eq = 1;
	int i = 0;
	for(i = 0; one[i] != '\0' || two[i] != '\0'; i++) {
		if(one[i] != two[i]) {
			eq = 0;
			break;
		}
	}
	eq = one[i] == two[i];
	return eq;
}

/*
 * Splits the string into two using the given delimeter and outputs
 * result to both the variable out1 and out2.
 */
void stringSplit(char* s, char delimiter, char* out1, char* out2){
    int i;
    int o = 0;
    for (i = 0; i < stringLength(s); i++) {
        if (s[i] == delimiter) {
            o = i+1;
            if (i != stringLength(s)) {
                out1[i]='\0';
            }
        }
        if (!o){
            out1[i] = s[i];

        }
        else{
            out2[i-o] = s[i];
        }
    }
    if(!o){
        out1[i] = '\0';
        out2[0] = '\0';
    }
    else{
        out2[i-o] = '\0';
    }
    return;

}
/////////////////////////////////
// API Layer 1

