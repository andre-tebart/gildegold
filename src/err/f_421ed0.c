/* 0x421ed0, the i-th string record of the symbol map (walking the length-prefixed records), NULL past
   the end */

ERRSTR *err_GetString(ERRMAP *map, unsigned int i)
{
    ERRSTR *s;

    if (i < map->nstrings) {
        s = (ERRSTR *)((char *)map + 0x10);
        for (; i > 0; i--)
            s = (ERRSTR *)((char *)s + s->len + 4);
        return s;
    }
    return NULL;
}
