#ifndef VECTOR_UTILS_H
#define VECTOR_UTILS_H

#include "postgres.h"
#include "utils.h"
#include "vector.h"

#define AppendChar(ptr, c) (*(ptr)++ = (c))
#define AppendFloat(ptr, f) ((ptr) += float_to_shortest_decimal_bufn((f), (ptr)))
#define AppendUInt8(ptr, f) ((ptr) += uint8_to_shortest_decimal_bufn((f), (ptr)))

bool
vector_isspace(char ch);

void
CheckElement(float value);

void
CheckModel(VectorHeader v1, VectorHeader v2);

void
parse_vector(char * str, uint32 * metric, uint32 * model_id, float x[], uint16 vector_dim);

void
vector_to_string(char * buf, VectorHeader header, float * vector, uint16 dim);

#endif