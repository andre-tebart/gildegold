/* 0x4fea50, finds what has the id: a character (0x300 records, int +4; stored in *chr, returns 3), a building (*bld, returns 1) or an object instance (*obj, returns 2); each result pointer is cleared first; 0 when nothing (or the prototype table is not loaded) */

int gm_FindById(int *bld, int *obj, int *chr, int id)
{
    short *p;
    char *b;
    int i;

    if (ai_Goods != 0) {
        if (bld != 0)
            *bld = 0;
        if (obj != 0)
            *obj = 0;
        if (chr != 0) {
            *chr = 0;
            p = (short *)ai_Chars[0];
            i = 0;
            do {
                if (*p != -1 && *(int *)(p + 2) == id) {
                    *chr = (int)p;
                    return 3;
                }
                p += 0x10e;
                i++;
            } while (i < 0x300);
        }
        if (bld != 0) {
            i = 0;
            b = ai_Blds;
            do {
                if (*b != 0 && *(int *)(b + 1) == id) {
                    *bld = (int)b;
                    return 1;
                }
                i++;
                b += 0xa9;
            } while (i < 0x100);
        }
        if (obj != 0 && (i = 0, p = (short *)ai_ObjectsV, 0 < f3gm_ObjectCount)) {
            do {
                if (*p != 0) {
                    if (*(int *)(p + 1) == id) {
                        *obj = (int)p;
                        return 2;
                    }
                    i++;
                }
                p = (short *)((char *)p + 0x43);
            } while (i < f3gm_ObjectCount);
        }
    }
    return 0;
}
