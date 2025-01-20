/**
 * @file: utils.c
 * @author: Pablo Regodon
 * 
 * This file defines useful functions to manage data operations.
 */
#include "utils.h"

/**
 * Fill a string array with the characters correspondent to the integer
 * passed as argument. The caller is responsible for passing an array with
 * space for 3 bytes, although less than 3 characters may be written. 
 * 
 * @param[n]: number to be converted to a string.
 * @param[result]: pointer to character which represents the space in memory
 * to be filled with character conversion of n.
 * @return: the number of characters written.
 */
int
uint8_to_shortest_decimal_bufn(unsigned char n, char * result)
{
    unsigned char dig; // to store single digits
    int i = 0;
    do
    {
        dig = n%10; // take least significant digit
        result[i++] = dig + '0'; // convert to its ASCII form
        n /= 10; // discard least significant digit
    } while(n != 0); // repeat until there are digits

    /* Reverse the string */
    for (int j = 0, k = i-1; j < k; j++, k--)
    {
        char temp = result[j];
        result[j] = result[k];
        result[k] = temp;
    }

    return i;
}
