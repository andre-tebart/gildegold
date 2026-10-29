/* 0x51d530, scene callback: clears the flag 4 in +0x213 of the object */

char ls_ClearObjectFlag(char *o)
{
    o[0x213] &= ~4;
    return 1;
}
