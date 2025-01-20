#include "postgres.h"
#include "c.h"

#include "fmgr.h"
#include "port.h"
#include "common/shortest_dec.h"
#include "libpq/pqformat.h"

#include "vector.h"
#include "vector_utils.h"

#include <math.h>


/* Magical wisdom from Postgres */

PG_MODULE_MAGIC;

/* Data handling functions */

/***********************************************************************************/

// 128-array

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(vector128_in);
Datum
vector128_in(PG_FUNCTION_ARGS)
{
    uint32 metric = 0;
    uint32 model_id = 0;
    char * str = PG_GETARG_CSTRING(0);
    float x[VECTOR_MAX_DIM]; // temporary array to write parsed data
    Vector128 * result;
    // char debug_buffer[1000];

    // Parse
    parse_vector(str, &metric, &model_id, x, VECTOR_DIM_128);
    
    // Allocate data in PG memory context
    result = (Vector128 * ) palloc0(sizeof(Vector128)); // This init values of struct to 0

    // Fill header
    result->header.metric = (uint8) metric;
    result->header.model_id = (uint8) model_id;

    // Fill vector data
    for (int i = 0; i < VECTOR_DIM_128; i++)
        result->x[i] = x[i]; // Copy temporary array contents into vector struct memory

    // // To debug parsing
    // int p = sprintf(debug_buffer, "Vector128 {\n\tmetric: %d,\n\tmodel_id: %d,\n\tvector: [%g", result->header.metric, result->header.model_id, result->x[5]);
    // for (int i=0; i < 128; i++){
    //     p += sprintf(&debug_buffer[p], "%g, ", result->x[i]);
    // }
    // sprintf(&debug_buffer[p], "]\n");

    // ereport(ERROR,
    //     (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
    //      errmsg("invalid input syntax for type vector: \"%s\"", str),
    //      errdetail("%s", debug_buffer)));

    // And return it
    PG_RETURN_POINTER(result);
}

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(vector128_out);
Datum
vector128_out(PG_FUNCTION_ARGS)
{
    // Get vector
    Vector128 * vector = (Vector128 * ) PG_GETARG_POINTER(0);
    char * buf;

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
    buf = (char *) palloc(16 * VECTOR_DIM_128 + 12); // Allocate worst case scenario, although string can be shorter

    // Convert to string
    vector_to_string(buf, vector->header, vector->x, VECTOR_DIM_128);

    // No idea what is this about
    PG_FREE_IF_COPY(vector, 0);

    // Return string
    PG_RETURN_CSTRING(buf);
}

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(vector128_recv);
Datum
vector128_recv(PG_FUNCTION_ARGS)
{
    // Despite the name, it can store arbitrary binary data
    StringInfo  buf = (StringInfo) PG_GETARG_POINTER(0);
    Vector128 * result;

    // Allocate vector
    result = (Vector128 *) palloc0(sizeof(Vector128));

    // Get header
    result->header.metric = (uint8) pq_getmsgbyte(buf);
    result->header.model_id = (uint8) pq_getmsgbyte(buf);

    // Get data
    for(int i = 0; i < VECTOR_DIM_128; i++)
    {
        result->x[i] = pq_getmsgfloat4(buf);
    }

    PG_RETURN_POINTER(result);
}

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(vector128_send);
Datum
vector128_send(PG_FUNCTION_ARGS)
{
    Vector128 * vector = (Vector128 *) PG_GETARG_POINTER(0);
    StringInfoData buf;

    // Start sending bytes
    pq_begintypsend(&buf);

    // Send header
    pq_sendbyte(&buf, vector->header.metric);
    pq_sendbyte(&buf, vector->header.model_id);

    // Send data
    for(int i = 0; i < VECTOR_DIM_128; i++)
    {
        pq_sendfloat4(&buf, vector->x[i]);
    }

    // Finish sending bytes and return
    PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}

/***********************************************************************************/

// 256-array

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(vector256_in);
Datum
vector256_in(PG_FUNCTION_ARGS)
{
    uint32 metric = 0;
    uint32 model_id = 0;
    char * str = PG_GETARG_CSTRING(0);
    float x[VECTOR_MAX_DIM]; // temporary array to write parsed data
    Vector256 * result;

    // Parse
    parse_vector(str, &metric, &model_id, x, VECTOR_DIM_256);
    
    // Allocate data in PG memory context
    result = (Vector256 * ) palloc0(sizeof(Vector256)); // This init values of struct to 0

    // Fill header
    result->header.metric = (uint8) metric;
    result->header.model_id = (uint8) model_id;

    // Fill vector data
    for (int i = 0; i < VECTOR_DIM_256; i++)
        result->x[i] = x[i]; // Copy temporary array contents into vector struct memory

    // And return it
    PG_RETURN_POINTER(result);
}

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(vector256_out);
Datum
vector256_out(PG_FUNCTION_ARGS)
{
    // Get vector
    Vector256 * vector = (Vector256 * ) PG_GETARG_POINTER(0);
    char * buf;

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
    buf = (char *) palloc(16 * VECTOR_DIM_256 + 12); // Allocate worst case scenario, although string can be shorter

    // Convert to string
    vector_to_string(buf, vector->header, vector->x, VECTOR_DIM_256);

    // No idea what is this about
    PG_FREE_IF_COPY(vector, 0);

    // Return string
    PG_RETURN_CSTRING(buf);
}

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(vector256_recv);
Datum
vector256_recv(PG_FUNCTION_ARGS)
{
    // Despite the name, it can store arbitrary binary data
    StringInfo  buf = (StringInfo) PG_GETARG_POINTER(0);
    Vector256 * result;

    // Allocate vector
    result = (Vector256 *) palloc0(sizeof(Vector256));

    // Get header
    result->header.metric = (uint8) pq_getmsgbyte(buf);
    result->header.model_id = (uint8) pq_getmsgbyte(buf);

    // Get data
    for(int i = 0; i < VECTOR_DIM_256; i++)
    {
        result->x[i] = pq_getmsgfloat4(buf);
    }

    PG_RETURN_POINTER(result);
}

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(vector256_send);
Datum
vector256_send(PG_FUNCTION_ARGS)
{
    Vector256 * vector = (Vector256 *) PG_GETARG_POINTER(0);
    StringInfoData buf;

    // Start sending bytes
    pq_begintypsend(&buf);

    // Send header
    pq_sendbyte(&buf, vector->header.metric);
    pq_sendbyte(&buf, vector->header.model_id);

    // Send data
    for(int i = 0; i < VECTOR_DIM_256; i++)
    {
        pq_sendfloat4(&buf, vector->x[i]);
    }

    // Finish sending bytes and return
    PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}

/***********************************************************************************/

// 512-array

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(vector512_in);
Datum
vector512_in(PG_FUNCTION_ARGS)
{
    uint32 metric = 0;
    uint32 model_id = 0;
    char * str = PG_GETARG_CSTRING(0);
    float x[VECTOR_MAX_DIM]; // temporary array to write parsed data
    Vector512 * result;

    // Parse
    parse_vector(str, &metric, &model_id, x, VECTOR_DIM_512);
    
    // Allocate data in PG memory context
    result = (Vector512 * ) palloc0(sizeof(Vector512)); // This init values of struct to 0

    // Fill header
    result->header.metric = (uint8) metric;
    result->header.model_id = (uint8) model_id;

    // Fill vector data
    for (int i = 0; i < VECTOR_DIM_512; i++)
        result->x[i] = x[i]; // Copy temporary array contents into vector struct memory

    // And return it
    PG_RETURN_POINTER(result);
}

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(vector512_out);
Datum
vector512_out(PG_FUNCTION_ARGS)
{
    // Get vector
    Vector512 * vector = (Vector512 * ) PG_GETARG_POINTER(0);
    char * buf;

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
    buf = (char *) palloc(16 * VECTOR_DIM_512 + 12); // Allocate worst case scenario, although string can be shorter

    // Convert to string
    vector_to_string(buf, vector->header, vector->x, VECTOR_DIM_512);

    // No idea what is this about
    PG_FREE_IF_COPY(vector, 0);

    // Return string
    PG_RETURN_CSTRING(buf);
}

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(vector512_recv);
Datum
vector512_recv(PG_FUNCTION_ARGS)
{
    // Despite the name, it can store arbitrary binary data
    StringInfo  buf = (StringInfo) PG_GETARG_POINTER(0);
    Vector512 * result;

    // Allocate vector
    result = (Vector512 *) palloc0(sizeof(Vector512));

    // Get header
    result->header.metric = (uint8) pq_getmsgbyte(buf);
    result->header.model_id = (uint8) pq_getmsgbyte(buf);

    // Get data
    for(int i = 0; i < VECTOR_DIM_512; i++)
    {
        result->x[i] = pq_getmsgfloat4(buf);
    }

    PG_RETURN_POINTER(result);
}

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(vector512_send);
Datum
vector512_send(PG_FUNCTION_ARGS)
{
    Vector512 * vector = (Vector512 *) PG_GETARG_POINTER(0);
    StringInfoData buf;

    // Start sending bytes
    pq_begintypsend(&buf);

    // Send header
    pq_sendbyte(&buf, vector->header.metric);
    pq_sendbyte(&buf, vector->header.model_id);

    // Send data
    for(int i = 0; i < VECTOR_DIM_512; i++)
    {
        pq_sendfloat4(&buf, vector->x[i]);
    }

    // Finish sending bytes and return
    PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}

/***********************************************************************************/

// 1024 array

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(vector1024_in);
Datum
vector1024_in(PG_FUNCTION_ARGS)
{
    uint32 metric = 0;
    uint32 model_id = 0;
    char * str = PG_GETARG_CSTRING(0);
    float x[VECTOR_MAX_DIM]; // temporary array to write parsed data
    Vector1024 * result;

    // Parse
    parse_vector(str, &metric, &model_id, x, VECTOR_DIM_1024);
    
    // Allocate data in PG memory context
    result = (Vector1024 * ) palloc0(sizeof(Vector1024)); // This init values of struct to 0

    // Fill header
    result->header.metric = (uint8) metric;
    result->header.model_id = (uint8) model_id;

    // Fill vector data
    for (int i = 0; i < VECTOR_DIM_1024; i++)
        result->x[i] = x[i]; // Copy temporary array contents into vector struct memory

    // And return it
    PG_RETURN_POINTER(result);
}

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(vector1024_out);
Datum
vector1024_out(PG_FUNCTION_ARGS)
{
    // Get vector
    Vector1024 * vector = (Vector1024 *) PG_GETARG_POINTER(0);
    char * buf;

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
    buf = (char *) palloc(16 * VECTOR_DIM_1024 + 12); // Allocate worst case scenario, although string can be shorter

    vector_to_string(buf, vector->header, vector->x, VECTOR_DIM_1024);

    // No idea what is this about
    PG_FREE_IF_COPY(vector, 0);

    // Return string
    PG_RETURN_CSTRING(buf);
}

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(vector1024_recv);
Datum
vector1024_recv(PG_FUNCTION_ARGS)
{
    // Despite the name, it can store arbitrary binary data
    StringInfo  buf = (StringInfo) PG_GETARG_POINTER(0);
    Vector1024 * result;

    // Allocate vector
    result = (Vector1024 *) palloc0(sizeof(Vector1024));

    // Get header
    result->header.metric = (uint8) pq_getmsgbyte(buf);
    result->header.model_id = (uint8) pq_getmsgbyte(buf);

    // Get data
    for(int i = 0; i < VECTOR_DIM_1024; i++)
    {
        result->x[i] = pq_getmsgfloat4(buf);
    }

    PG_RETURN_POINTER(result);
}

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(vector1024_send);
Datum
vector1024_send(PG_FUNCTION_ARGS)
{
    Vector1024 * vector = (Vector1024 *) PG_GETARG_POINTER(0);
    StringInfoData buf;

    // Start sending bytes
    pq_begintypsend(&buf);

    // Send header
    pq_sendbyte(&buf, vector->header.metric);
    pq_sendbyte(&buf, vector->header.model_id);

    // Send data
    for(int i = 0; i < VECTOR_DIM_1024; i++)
    {
        pq_sendfloat4(&buf, vector->x[i]);
    }

    // Finish sending bytes and return
    PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}

/***********************************************************************************/

// 2048 array

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(vector2048_in);
Datum
vector2048_in(PG_FUNCTION_ARGS)
{
    uint32 metric = 0;
    uint32 model_id = 0;
    char * str = PG_GETARG_CSTRING(0);
    float x[VECTOR_MAX_DIM]; // temporary array to write parsed data
    Vector2048 * result;

    // Parse
    parse_vector(str, &metric, &model_id, x, VECTOR_DIM_2048);
    
    // Allocate data in PG memory context
    result = (Vector2048 * ) palloc0(sizeof(Vector2048)); // This init values of struct to 0

    // Fill header
    result->header.metric = (uint8) metric;
    result->header.model_id = (uint8) model_id;

    // Fill vector data
    for (int i = 0; i < VECTOR_DIM_2048; i++)
        result->x[i] = x[i]; // Copy temporary array contents into vector struct memory

    // And return it
    PG_RETURN_POINTER(result);
}

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(vector2048_out);
Datum
vector2048_out(PG_FUNCTION_ARGS)
{
    // Get vector
    Vector2048 * vector = (Vector2048 *) PG_GETARG_POINTER(0);
    char * buf;

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
    buf = (char *) palloc(16 * VECTOR_DIM_2048 + 12); // Allocate worst case scenario, although string can be shorter

    vector_to_string(buf, vector->header, vector->x, VECTOR_DIM_2048);

    // No idea what is this about
    PG_FREE_IF_COPY(vector, 0);

    // Return string
    PG_RETURN_CSTRING(buf);
}

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(vector2048_recv);
Datum
vector2048_recv(PG_FUNCTION_ARGS)
{
    // Despite the name, it can store arbitrary binary data
    StringInfo  buf = (StringInfo) PG_GETARG_POINTER(0);
    Vector2048 * result;

    // Allocate vector
    result = (Vector2048 *) palloc0(sizeof(Vector2048));

    // Get header
    result->header.metric = (uint8) pq_getmsgbyte(buf);
    result->header.model_id = (uint8) pq_getmsgbyte(buf);

    // Get data
    for(int i = 0; i < VECTOR_DIM_2048; i++)
    {
        result->x[i] = pq_getmsgfloat4(buf);
    }

    PG_RETURN_POINTER(result);
}

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(vector2048_send);
Datum
vector2048_send(PG_FUNCTION_ARGS)
{
    Vector2048 * vector = (Vector2048 *) PG_GETARG_POINTER(0);
    StringInfoData buf;

    // Start sending bytes
    pq_begintypsend(&buf);

    // Send header
    pq_sendbyte(&buf, vector->header.metric);
    pq_sendbyte(&buf, vector->header.model_id);

    // Send data
    for(int i = 0; i < VECTOR_DIM_2048; i++)
    {
        pq_sendfloat4(&buf, vector->x[i]);
    }

    // Finish sending bytes and return
    PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}

/***********************************************************************************/

/* Operators */

// Cosine distance

// This is exactly from pgvector
static float8
cosine_distance(float4 * a, float4 * b, uint32 dim)
{
    float similarity = 0.0;
    float norma = 0.0;
    float normb = 0.0;

    /* Auto-vectorized */
    for (int i = 0; i < dim; i++)
    {
        similarity += a[i] * b[i];
        norma += a[i] * a[i];
        normb += b[i] * b[i];
    }

    /* Use sqrt(a * b) over sqrt(a) * sqrt(b) */
    return (float8) 1 - (similarity / sqrt((double) norma * (double) normb));
}

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(cosine_distance128);
Datum
cosine_distance128(PG_FUNCTION_ARGS)
{

    Vector128 * v1 = (Vector128 *) PG_GETARG_POINTER(0);
    Vector128 * v2 = (Vector128 *) PG_GETARG_POINTER(1);
    float8 sim;

    CheckModel(v1->header, v2->header);
    sim = cosine_distance(v1->x, v2->x, VECTOR_DIM_128);

    PG_RETURN_FLOAT8(sim);
}

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(cosine_distance256);
Datum
cosine_distance256(PG_FUNCTION_ARGS)
{

    Vector256 * v1 = (Vector256 *) PG_GETARG_POINTER(0);
    Vector256 * v2 = (Vector256 *) PG_GETARG_POINTER(1);
    float8 sim;

    CheckModel(v1->header, v2->header);
    sim = cosine_distance(v1->x, v2->x, VECTOR_DIM_256);

    PG_RETURN_FLOAT8(sim);
}

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(cosine_distance512);
Datum
cosine_distance512(PG_FUNCTION_ARGS)
{

    Vector512 * v1 = (Vector512 *) PG_GETARG_POINTER(0);
    Vector512 * v2 = (Vector512 *) PG_GETARG_POINTER(1);
    float8 sim;

    CheckModel(v1->header, v2->header);
    sim = cosine_distance(v1->x, v2->x, VECTOR_DIM_512);

    PG_RETURN_FLOAT8(sim);
}

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(cosine_distance1024);
Datum
cosine_distance1024(PG_FUNCTION_ARGS)
{

    Vector1024 * v1 = (Vector1024 *) PG_GETARG_POINTER(0);
    Vector1024 * v2 = (Vector1024 *) PG_GETARG_POINTER(1);
    float8 sim;

    CheckModel(v1->header, v2->header);
    sim = cosine_distance(v1->x, v2->x, VECTOR_DIM_1024);

    PG_RETURN_FLOAT8(sim);
}

FUNCTION_PREFIX PG_FUNCTION_INFO_V1(cosine_distance2048);
Datum
cosine_distance2048(PG_FUNCTION_ARGS)
{

    Vector2048 * v1 = (Vector2048 *) PG_GETARG_POINTER(0);
    Vector2048 * v2 = (Vector2048 *) PG_GETARG_POINTER(1);
    float8 sim;

    CheckModel(v1->header, v2->header);
    sim = cosine_distance(v1->x, v2->x, VECTOR_DIM_2048);

    PG_RETURN_FLOAT8(sim);
}


