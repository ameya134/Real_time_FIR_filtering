#ifndef _STRING_H
#define _STRING_H


#include "stdint.h"

/* *******************************************************************************
 * This function converts an integer into its base 10 string representation
 *
 * param: num		number that is to be converted into string format
 *
 * param: restStr	pointer to string where results are written to.
 *
 * param: numZeros	number of leading zeros
 *
 * return: void
 * 
 * brief: The function converts the integer value into its base 10 character
 * represenation. The result is written to the memory where retStr pointer points
 *
 * *******************************************************************************/
void convertIntToString(uint32_t num, char *retStr, int numZeros);


/* ***************************************************************************
 * This function converts a base 10 representation of string to an integer
 *
 * param: str	pointer to the string to be converted
 *
 * return: uint32_t returns the integer value represented by the string 
 * 
 * brief: The function converts the string values into integer equivalent and 
 * returns the integer value.
 *
 * **************************************************************************/
uint32_t convertStringToInt(char *str);


/* ***************************************************************************
 * This function reverses the index order of the characters in the string
 *
 * param: str	pointer to the string to be reversed
 *
 * param: len	length of the string to be reversed
 *
 * return: void
 * 
 * brief: The function reverses the order of characters in a string for a given
 * length. It is possible to reverse only a part of the string.
 *
 * **************************************************************************/
void reverseString(char * str, uint8_t len);


/* ***************************************************************************
 * This function compares two strings
 *
 * param: str1	pointer to the first string to be compared
 * param: str2	pointer to the second string to be compared
 *
 * return: uint32_t returns the result of comparison
 * 
 * brief: The function compares two strings by subtracting their values. both
 * strings need to be null terminated. if both strings are identical then 0x00
 * is returned. If the strings don't match then -1 is returned.
 *
 * **************************************************************************/
int32_t strCmp(char *str1, char *str2,int len);


/* ***************************************************************************
 * This sets the value of the memory location
 *
 * param: __ptr	pointer to the start of memory location
 * param: value	value to be set at memory location
 * param: len	length of memory location
 *
 * return: pointer to memory location
 * 
 * brief: This function sets the value from mem location __ptr to __ptr + len
 * to value arg given by caller
 * **************************************************************************/
void *memset(void *__ptr, uint8_t value, uint32_t len);



/* ***************************************************************************
 * This function copies data from __src to __dst
 *
 * param: __dst	pointer to the copy destination
 * param: str2	pointer to the source
 * param: length of memory to be copied
 *
 * return: pointer to destination
 * 
 * brief: The function compares two strings by subtracting their values. both
 * strings need to be null terminated. if both strings are identical then 0x00
 * is returned. If the strings don't match then -1 is returned.
 *
 * **************************************************************************/
void *memcpy(void *__dst, const void *__src, uint32_t len);


#endif
