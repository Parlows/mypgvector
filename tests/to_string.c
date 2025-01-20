#include <stdlib.h>
#include <stdio.h>
#include "lib/ryu/ryu/ryu.h"

typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long int64;
typedef unsigned long uint64;
typedef float float4;
typedef double float8;

typedef struct VectorHeader
{
    uint8 metric;
    uint8 model_id;
} VectorHeader;

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

#define AppendChar(ptr, c) (*(ptr)++ = (c))
#define AppendFloat(ptr, f) ((ptr) += f2s_buffered_n((f), (ptr)))
#define AppendUInt8(ptr, f) ((ptr) += uint8_to_shortest_decimal_bufn((f), (ptr)))

static inline void
vector_to_string(char * buf, VectorHeader header, float4 * vector, uint16 dim)
{
    /**
     * To allocate the string in the buffer, we need the suitable space. We have:
     *     - Space occupied by floats: dim * FLOAT_STRING_MAX_SIZE
     *     - Space occupied by vector commas: dim - 1
     *     - Space occupied by square brackets: 2
     *     - Space occupied by brackets: 2
     *     - Space occupied by header bytes: 2 * BYTE_STRING_MAX_SIZE
     *     - Space occupied by header commas: 2
     *     - Space occupied by '\0' byte: 1
     * We know:
     *     - FLOAT_STRING_MAX_SIZE = 15
     *     - BYTE_STRING_MAX_SIZE = 3
     * So, total: 16 * dim + 12
     */

    // Append starting bracket
    AppendChar(buf, '(');
    
    // Append header
    AppendUInt8(buf, header.metric);
    AppendChar(buf, ',');
    AppendUInt8(buf, header.model_id);
    AppendChar(buf, ',');

    // Append starting array bracket
    AppendChar(buf, '[');
    
    // Append data
    for (int i = 0; i < dim; i++)
    {
        // Append in-between commas
        if (i > 0)
            AppendChar(buf, ',');
        
        // Append numbers
        AppendFloat(buf, vector[i]);
    }

    // Append ending array bracket
    AppendChar(buf, ']');

    // Append ending bracker
    AppendChar(buf, ')');

    // Append string ending character
    *buf = '\0';
}

#define DIM 10

int main(void)
{
    char * buf;
    VectorHeader header = {7,5};
    float4 vector[DIM] = {2.76, 7.76, 3.54, 4.05, 8.62, 3.02, 3.45, 9.72, 1.99, 7.06};
    uint16 dim=DIM;

    buf = (char *) malloc(sizeof(char)*(16 * dim + 12)); // Allocate worst case scenario, although string can be shorter
    if (buf == NULL){
        printf("Memory not allocated");
        exit(1);
    } else {
        printf("Memory successfully allocated\n\n");
    }

    vector_to_string(buf, header, vector, dim);

    char out_string[16 * dim + 12];
    for(int k = 0; k < 16 * dim + 12; k++)
    {
        out_string[k] = buf[k];
    }

    free(buf);
    printf("%s\n", out_string);
    return 0;
}
