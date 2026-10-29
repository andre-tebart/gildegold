/* 0x468790, zlib's default free hook (zfree): frees what zcalloc returned */

void  zcfree (opaque, ptr)
    voidpf opaque;
    voidpf ptr;
{
    free(ptr);
    if (opaque) return; /* make compiler happy */
}
