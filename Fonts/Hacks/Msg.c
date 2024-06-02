#include "common-chax.h"
#include "utf8.h"

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

void SetMsgTerminator(signed char * str)
{
    short off = 0;
    u8 ch;

    while (str[off] != 0)
    {
        ch = str[off];
        if (ch == CHFE_L_LoadFace)   /* [LoadFace] */
            off += 2;

        if (ch == '\x80')   /* [HalfCloseEyes] */
            off += 1;

        off++;
    }

    off--;
    while (off >= 0)
    {
        ch = str[off];
        if (ch != '\x1F')   /* [.] */
            return;

        /* <!> [.] --> \x0 */
        ch = str[off - 1];
        if (ch != '\x80')   /* [HalfCloseEyes] */
            str[off] = '\0';

        off--;
    }
}
