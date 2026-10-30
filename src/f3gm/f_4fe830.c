/* 0x4fe830, runs through the object instances (0x5008 records of 0x43 bytes: word +0 the prototype, int +6 the owner id): the owner (found by 0x4fea50 as a building / object / character) gets the instance back (0x4ffff0 on its list at +0x5d / +0x14 / +0x178), an instance without a live owner is freed (0x500080); then the table is cleared */

void gm_ClearObjects(void)
{
    int i, t;
    int b, o, c;

    i = 0;
    do {
        if (*(short *)((char *)ai_Objects + i) != 0) {
            gm_FindById(&b, &o, &c, *(int *)((char *)ai_Objects + i + 6));
            if (b == 0) {
                if (o != 0) {
                    t = o + 0x14;
                    goto have;
                }
                if (c != 0) {
                    t = c + 0x178;
                    goto have;
                }
            } else {
                t = b + 0x5d;
            have:
                if (t != 0) {
                    gm_RemoveObject(t, *(short *)((char *)ai_Objects + i));
                    goto next;
                }
            }
            gm_FreeObjectExtra((char *)ai_Objects + i);
            *(short *)((char *)ai_Objects + i) = 0;
            f3gm_ObjectCount--;
        }
    next:
        i += 0x43;
        if (i > 0x14f217) {
            memset(ai_Objects, 0, 0x14f218);
            return;
        }
    } while (1);
}
