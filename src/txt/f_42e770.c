/* 0x42e770, returns the index of a text key, or -1. */

int txt_FindKey(char *key)
{
    int i;

    for (i = 0; i < 0x4800; i++)
        if (!stricmp(key, txt_Keys[i]))
            return i;
    return -1;
}
