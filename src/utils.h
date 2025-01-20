/**
 * @file: utils.h
 * @author: Pablo Regodon
 * 
 * This file declares useful functions to manage data operations.
 */
#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

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
uint8_to_shortest_decimal_bufn(unsigned char n, char * result);

#endif