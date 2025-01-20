/**
 * This file includes code useful to manage vectors.
 * 
 * Includes parsing functions
 * 
 */
#include "vector_utils.h"

#include <math.h>


/*
 * Check for whitespace, since array_isspace() is static
 */
bool
vector_isspace(char ch)
{
	if (ch == ' ' ||
		ch == '\t' ||
		ch == '\n' ||
		ch == '\r' ||
		ch == '\v' ||
		ch == '\f')
		return true;
	return false;
}

/*
 * Ensure finite element
 */
void
CheckElement(float value)
{
	if (isnan(value))
		ereport(ERROR,
				(errcode(ERRCODE_DATA_EXCEPTION),
				 errmsg("NaN not allowed in vector")));

	if (isinf(value))
		ereport(ERROR,
				(errcode(ERRCODE_DATA_EXCEPTION),
				 errmsg("infinite value not allowed in vector")));
}

void
CheckMetric(uint32 metric)
{
    if(metric > 255)
        ereport(ERROR,
                (errcode(ERRCODE_DATA_EXCEPTION),
                 errmsg("Metric value (%d) not valid", metric)));
}

void
CheckModelId(uint32 model_id)
{
    if(model_id > 255)
        ereport(ERROR,
                (errcode(ERRCODE_DATA_EXCEPTION),
                 errmsg("Metric value (%d) not valid", model_id)));
}

void
CheckModel(VectorHeader v1, VectorHeader v2)
{
    if (v1.model_id != v2.model_id)
        ereport(ERROR,
            (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
            errmsg("one vector was generated with model %d and the other with model %d", v1.model_id, v2.model_id),
            errdetail("Vectors must be generated with same model")));
}

void
parse_vector(char * str, uint32 * metric, uint32 * model_id, float4 x[], uint16 vector_dim)
{
    uint16 values_read;
    char * stringEnd; // Needed to parse string into numbers

    // Discard spaces
    while(vector_isspace(*str))
        str++;

    // Check for opening parenthesis
    if (*str != '(') // Parsing error
        ereport(ERROR,
        (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
         errmsg("invalid input syntax for type vector: \"%s\"", str),
         errdetail("Contents must start with \"(\".")));

    str++;

    // Discard spaces
    while(vector_isspace(*str))
        str++;

    // Read first integer
    *metric = strtoul(str, &stringEnd, 0);

    // Check if parsing failed
    if(stringEnd == str)
        ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                errmsg("invalid input syntax for type vector: \"%s\"", str)));

    // Resume
    str = stringEnd;

    // Now, go for the comma
    while(vector_isspace(*str))
        str++;
    
    if (*str == ',')
        str++;
    else
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                    errmsg("invalid input syntax for type vector: \"%s\"", str)));
    
    // Go for second integer
    while(vector_isspace(*str))
        str++;

    // Read first integer
    *model_id = strtoul(str, &stringEnd, 0);

    // Check if parsing failed
    if(stringEnd == str)
        ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                errmsg("invalid input syntax for type vector: \"%s\"", str)));

    // Resume
    str = stringEnd;

    // Now, go for the comma
    while(vector_isspace(*str))
        str++;
    
    if (*str == ',')
        str++;
    else
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                    errmsg("invalid input syntax for type vector: \"%s\"", str)));

    while(vector_isspace(*str))
        str++;

    // Check for opening vector sign
    if (*str != '[') // Parsing error
        ereport(ERROR,
        (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
         errmsg("invalid input syntax for type vector: \"%s\"", str),
         errdetail("Vector contents must start with \"[\".")));

    // If opening allright
    str++;

    // Check again for spaces
    while(vector_isspace(*str))
        str++;

    // Start reading data
    values_read = 0;
    for(;;)
    {
        // Loop variables
        float4 val;

        // Check for vector overflow
        if(values_read >= vector_dim)
            ereport(ERROR,
                    (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
                     errmsg("Vector has too much values")));


        // Check if brackets are closed early
        if (*str == ']')
            ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
             errmsg("not enough elements in vector: \"%s\"", str),
             errdetail("Vector must include %d elements.", vector_dim)));
        else if (*str == ')')
            ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
             errmsg("vector not closed correctly", str),
             errdetail("Vector must end with \"]\" character.", vector_dim)));
            
        // We can have a space, a comma or whatever, so lets see

        // Eat up spaces
        while(vector_isspace(*str))
            str++;
        
        // Check if string ended already
        if (*str == '\0')
            ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
            errmsg("invalid input syntax for type vector: \"%s\"", str)));
        
        // No clue why this is done
        errno = 0;

        // Parse string to float using stdlib function:
        val = strtof(str, &stringEnd);

        // Check: stringEnd points at the character after the number. Thats because we
        // do not know in advance the number of characters the floating value has, so
        // we need a pointer to resume our string parsign

        // Check if parsing failed
        if(stringEnd == str)
            ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                    errmsg("invalid input syntax for type vector: \"%s\"", str)));
        
        // Check for range error
        if (errno == ERANGE && isinf(val))
            ereport(ERROR,
                    (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
                     errmsg("\"%s\" is out of range for type vector", pnstrdup(str, stringEnd - str))));

        // Check element
        CheckElement(val);

        // Insert value in array
        x[values_read] = val;

        // Increment read values counter
        values_read++;

        // Resume parsing
        str = stringEnd;

        // Now, go for the comma
        while(vector_isspace(*str))
            str++;
        
        if (*str == ',')
            str++;
        else if (*str == ']')
        {
            str++;
            break;
        }
        else
            ereport(ERROR,
                    (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                     errmsg("invalid input syntax for type vector: \"%s\"", str)));
    }

    // Check for white space
    while(vector_isspace(*str))
        str++;

    if (*str == ')')
        str++;
    else
        ereport(ERROR,
                (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
                    errmsg("invalid input syntax for type vector: \"%s\"", str)));
    
    // Check for remaining white space
    while(vector_isspace(*str))
        str++;

    // If we find something apart from string ending, error
    if (*str != '\0')
        ereport(ERROR,
            (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
             errmsg("invalid input syntax for type vector: \"%s\"", str),
             errdetail("Junk after closing right brace.")));
}

/**
 * TODO: (Check)
 * 
 * In this function, we increment buf variable (the memaddress).
 * Does this affect the original buf variable in vectorXXX_in() function?
 * That original variable MUST NOT be modified, since it points to the beggining of the string.
 * Since I pass that memaddress as value, modifying buf variable inside this function should not affect
 * outside buf variable, right??
 */
void
vector_to_string(char * buf, VectorHeader header, float4 * vector, uint16 dim)
{

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
