/* 0x4ed040, reads the record of 0x4ecf10 from the file f into the object o (+0x65, allocated when missing); 1 when all was read @deep B3 draft, 136 differing instructions: the body was read against the original and is complete (the id, float, 4 share bytes, 6 place pairs and the record copy into the allocated +0x65 block; ls_Read is ls.c's 0x51c920). The hunks are the prologue (the original keeps the file in edi, ours in esi), the frame slots, and the return-0 block placement: the original inlines pop edi/pop esi/pop ebx/add esp,0x3c/ret after every failed read and skips it with jne, ours routes every return 0 to one shared out-of-line tail with je. Lead tried this session: the !ls_Read(...) form (compiles to the same 136 differing instructions - the source comparison is not the cause); the same family lesson from ahm_WriteRecord (0x4ecf10, escalated): the shared-tail / prologue difference is compiler block placement. */

int ahm_ReadRecord(void *f, unsigned char *o)
{
    AHMOBJ *obj = (AHMOBJ *)o;
    int rec[15];
    int i;
    int *s, *d;

    if (ls_Read(&rec[0], 4, 1, f) == 0)
        return 0;
    if (ls_Read(&rec[1], 4, 1, f) == 0)
        return 0;
    if (ls_Read((char *)rec + 8, 1, 1, f) == 0)
        return 0;
    if (ls_Read((char *)rec + 9, 1, 1, f) != 0) {
        if (ls_Read((char *)rec + 10, 1, 1, f) == 0)
            return 0;
        if (ls_Read((char *)rec + 11, 1, 1, f) != 0) {
            i = 0;
            d = &rec[3];
            while (ls_Read(d - 1 + 0, 4, 1, f) != 0) {
                if (ls_Read(d, 4, 1, f) == 0)
                    return 0;
                i++;
                d += 2;
                if (i > 5) {
                    if (obj->rec == 0)
                        obj->rec = mem_Alloc(0x3c, "ahm_AllocAhmInfo");
                    s = rec;
                    d = (int *)obj->rec;
                    for (i = 15; i != 0; i--)
                        *d++ = *s++;
                    return 1;
                }
            }
        }
        return 0;
    }
    return 0;
}
