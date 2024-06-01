#include "global.h"
#include <string.h>

void InsertPrefix(char * str, const char * insert_str, s8 c)
{
    int len = strlen(str);

    if(insert_str != NULL)
    {
        int len_sert = strlen(insert_str);

        for( int i = len; i >= 0; i--)
            str[i + len_sert] = str[i];

        for( int i = 0; i < len_sert; i++)
            str[i] = insert_str[i];

        return;
    }

    for( int i = len; i >= 0; i--)
        str[i + 1] = str[i];

    str[0] = ' ';
}
