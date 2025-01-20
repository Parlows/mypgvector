#ifndef VECTOR_H
#define VECTOR_H

#include "c.h"
#include "common/shortest_dec.h"
#include "libpq/pqformat.h"

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

/**
 * Including more info may be useful. Internal node dat used in indexes may be included here.
 * Also, what type of distance must be used to compare these vectors. Check how PASE use these
 * inner params: declares a int32 header variable whaich uses a bitmap.
 */
typedef struct Vector
{
    int32  len;
    uint16 dim;
    uint8  metric;
    uint8  model;
    float4  x[FLEXIBLE_ARRAY_MEMBER];
} Vector;



#if PG_VERSION_NUM >= 160000
#define FUNCTION_PREFIX
#else
#define FUNCTION_PREFIX PGDLLEXPORT
#endif

#endif