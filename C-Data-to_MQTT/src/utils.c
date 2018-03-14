#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "main.h"
#include "utils.h"

void trace(char *output, int level)
{
    // outputs text to the console when debugging is enabled and the
    // debug level is lower than the event level
    if (DEBUG && DEBUG_LEVEL <= level)
    {
        printf("%s\n", output);
    }
}

void tracedouble(double output, int level)
{
    // outputs a double number to the console when debugging is enabled and the
    // debug level is lower than the event level
    if (DEBUG && DEBUG_LEVEL <= level)
    {
        printf("%f\n", output);
    }
}

void tracebool(bool output, int level)
{
    // outputs boolean value to the console when debugging is enabled and the
    // debug level is lower than the event level
    if (DEBUG && DEBUG_LEVEL <= level)
    {
        printf("%s\n", output ? "true" : "false");
    }
}

void str_find_num(char *src, int str_size, char *dest)
{
    //
    // Find a number in a character array
    //

    char first_num = 1;
    int index = 0;
    int dest_size = sizeof(dest);
    for (int i = 0; i < str_size; i++)
    {
        if (index < dest_size)
        { // stop destination overflow
            if ((src[i] >= '0' && src[i] <= '9') | (src[i] == '.'))
            {
                char a = src[i];
                if (first_num == 1)
                {
                    if (a != '0')
                    {
                        dest[index] = a;
                        first_num = 0;
                        index++;
                    }
                }
                else
                {
                    dest[index] = a;
                    index++;
                }
            }
        }
    }
}