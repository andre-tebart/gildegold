/* 0x4ece60, deletes the alchemist of the record rec (+0 the character id): clears its object pointers, removes the places from the list of the object o, zeroes the record; 0 when the character is gone */

int ahm_DeleteAlchemist(int unused, int *rec, unsigned char *o)
{
    char *c;
    char *sp;
    int *q;
    int i;

    c = gm_FindChar(rec[0]);
    if (c != 0) {
        sp = gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0xff);
        *(int *)(c + 0x170) = 0;
        *(int *)(c + 0x16c) = 0;
        q = rec + 3;
        for (i = 6; i != 0; i--) {
            if (*q != 0) {
                if (sp != 0)
                    gm_RemoveObject(sp + 0x14, *q);
                *q = 0;
                q[1] = 0;
            }
            q += 2;
        }
        memset(rec, 0, 0x3c);
        unk_Unk503c60(*(unsigned short *)c, 1);
        return 1;
    }
    return 0;
}
