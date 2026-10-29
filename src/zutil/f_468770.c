/* 0x468770, zlib's default allocation hook (zalloc): items*size bytes, uninitialised - this build uses
 * malloc where upstream uses calloc */








#ifndef STDC
extern voidp  calloc OF((uInt items, uInt size));
extern void   free   OF((voidpf ptr));
#endif

voidpf zcalloc (opaque, items, size)
    voidpf opaque;
    unsigned items;
    unsigned size;
{
    if (opaque) items += size - size; /* make compiler happy */
    /* GildeGold: malloc instead of upstream calloc (0x468770) */
    return (voidpf)malloc(items * size);
}
