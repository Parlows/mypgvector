#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Type definition
typedef struct VectorHeader
{
    unsigned char metric;
    unsigned char model_id;
} VectorHeader;

typedef struct Vector128
{
    VectorHeader header;
    float x[128];
} Vector128;

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

#define ERROR_OPENING_PARENTHESIS 1
#define ERROR_READING_FIRST_INT 2
#define ERROR_COMMA_AFTER_FIRST_INT 3
#define ERROR_READING_SECOND_INT 4
#define ERROR_COMMA_AFTER_SECOND_INT 5
#define ERROR_OPENING_BRACKETS 6
#define ERROR_OVERFLOW 7
#define ERROR_BRACKET_CLOSED_EARLY 8
#define ERROR_PARENTHESIS_CLOSED_EARLY 9
#define ERROR_STRING_ENDED_EARLY 10
#define ERROR_READING_FLOAT 11
#define ERROR_WRONG_CHARACTER_IN_VECTOR 12
#define ERROR_CLOSING_PARENTHESIS 13
#define ERROR_ENDING_STRING 14

int
parse_vector(char * str, unsigned int * metric, unsigned int * model_id, float x[], unsigned short vector_dim)
{
    unsigned short values_read;
    char * stringEnd; // Needed to parse string into numbers

    // Discard spaces
    while(vector_isspace(*str))
        str++;

    // Check for opening parenthesis
    if (*str != '(') // Parsing error
        return 1;

    str++;

    // Discard spaces
    while(vector_isspace(*str))
        str++;

    // Read first integer
    printf("%c\n\n", *str);
    *metric = strtoul(str, &stringEnd, 0);

    // Check if parsing failed
    if(stringEnd == str)
        return 2;

    // Resume
    str = stringEnd;

    // Now, go for the comma
    while(vector_isspace(*str))
        str++;
    
    if (*str == ',')
        str++;
    else
        return 3;
    
    // Go for second integer
    while(vector_isspace(*str))
        str++;

    // Read first integer
    *model_id = strtoul(str, &stringEnd, 0);

    // Check if parsing failed
    if(stringEnd == str)
        return 4;

    // Resume
    str = stringEnd;

    // Now, go for the comma
    while(vector_isspace(*str))
        str++;
    
    if (*str == ',')
        str++;
    else
        return 5;

    while(vector_isspace(*str))
        str++;

    // Check for opening vector sign
    if (*str != '[') // Parsing error
        return 6;

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
        float val;

        // Check for vector overflow
        if(values_read >= vector_dim)
            return 7;


        // Check if brackets are closed early
        if (*str == ']')
            return 8;
        else if (*str == ')')
            return 9;
            
        // We can have a space, a comma or whatever, so lets see

        // Eat up spaces
        while(vector_isspace(*str))
            str++;
        
        // Check if string ended already
        if (*str == '\0')
            return 10;
        
        // No clue why this is done
        // errno = 0;

        // Parse string to float using stdlib function:
        val = strtof(str, &stringEnd);

        // Check: stringEnd points at the character after the number. Thats because we
        // do not know in advance the number of characters the floating value has, so
        // we need a pointer to resume our string parsign

        // Check if parsing failed
        if(stringEnd == str)
            return 11;

        // Check element
        // CheckElement(val);

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
            return 12;
    }

    // Check for white space
    while(vector_isspace(*str))
        str++;

    if (*str == ')')
        str++;
    else
        return 13;
    
    // Check for remaining white space
    while(vector_isspace(*str))
        str++;

    // If we find something apart from string ending, error
    if (*str != '\0')
        return 14;
    
    return 0;
}

int main() {
    
    char * str = "( 1, 4, [28.9,70.6,90.1,44.4,75.7,36.8,63,67.9,71.7,39.3,64.8,13.2,49,73.5,35.3,67.1,57.1,36.4,28,9.6,23.1,38.8,93.1,47.7,63.8,36.8,54.3,42.7,96.5,73.1,21.9,68.3,41.5,88.8,87.5,20.1,13.3,92.4,14.9,90.7,17,25.4,85,83,84.6,82,75.1,2,14.7,2.3,44.4,61.6,24.2,77.3,69.4,34.7,62.7,38.3,19.9,50.3,3.3,71.8,76.3,51.8,62.4,62,45.7,89.7,42.7,83,3.1,74.4,83.1,71.6,42.9,84.9,95.6,78.1,77.3,65.9,29,20.3,78.4,15.1,63.5,88.3,32.7,66.1,67.3,9.4,47.9,84.2,62.7,74.9,17.4,56.6,12.8,88.6,86.3,96.8,92.8,35.7,84.5,49.2,72.1,7.9,22.2,23.3,30.5,27.9,17.6,79.5,62.9,13.5,81.6,64.7,91.2,3.3,31.8,21.8,66.6,39.7,13.4,54.7,46.4,83.9,62.4,6.2] )";

    Vector128 * result;
    unsigned int metric = 0;
    unsigned int model_id = 0;
    float x[2048];

    int ret = parse_vector(str, &metric, &model_id, x, 128);

    if(ret != 0)
    {
        printf("Error %d has occurred\n", ret);
        return -1;
    }

    result = (Vector128 *) malloc(sizeof(Vector128));

    result->header.metric = metric;
    result->header.model_id = model_id;

    for (int i = 0; i < 128; i++)
        result->x[i] = x[i];

    printf("Vector128 {\n\tmetric: %d,\n\tmodel_id: %d,\n\tvector: [", result->header.metric, result->header.model_id);
    for (int i=0; i < 128; i++){
        printf("%g, ", result->x[i]);
    }
    printf("]\n");

    free(result);

    return 0;
}





