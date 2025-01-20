#include "postgre_base_types.h"
#include <stdio.h>

#define VECTOR_DIM_128 128
#define VECTOR_DIM_256 256
#define VECTOR_DIM_512 512
#define VECTOR_DIM_768 768
#define VECTOR_DIM_1024 1024
#define VECTOR_DIM_2048 2048
#define VECTOR_MAX_DIM VECTOR_DIM_2048

typedef struct VectorHeader
{
    uint8 metric;
    uint8 model_id;
} VectorHeader;

typedef struct Vector128
{
    VectorHeader header;
    float4 x[VECTOR_DIM_128];
} Vector128;

typedef struct Vector256
{
    VectorHeader header;
    float4 x[VECTOR_DIM_256];
} Vector256;

typedef struct Vector512
{
    VectorHeader header;
    float4 x[VECTOR_DIM_512];
} Vector512;

typedef struct Vector768
{
    VectorHeader header;
    float4 x[VECTOR_DIM_768];
} Vector768;

typedef struct Vector1024
{
    VectorHeader header;
    float4 x[VECTOR_DIM_1024];
} Vector1024;

/**
 * Posible to be too large to be stored in a single PG page!!!
 */
typedef struct Vector2048
{
    VectorHeader header;
    float4 x[VECTOR_DIM_2048];
} Vector2048;

int main(void) {

    printf("Sizes of the different vectors:\n");
    printf("\tsizeof (VectorHeader): %lu\n", sizeof(VectorHeader));
    printf("\tsizeof (Vector128): %lu\n", sizeof(Vector128));
    printf("\tsizeof (Vector256): %lu\n", sizeof(Vector256));
    printf("\tsizeof (Vector512): %lu\n", sizeof(Vector512));
    printf("\tsizeof (Vector1024): %lu\n", sizeof(Vector1024));
    printf("\tsizeof (Vector2048): %lu\n", sizeof(Vector2048));


    return 0;
}
