/* 0x4e48b0, sends a message of the type 0x11 (0x5ec) for the party m (a +4 the owner) from the object b (+0x14 the building, +0
   its id, +0x26 bit 4: a text follows): the text `text` (and the second one, joined, flag 0x10) after the header at +0xf8 of the buffer;
   +0x60 gets the kind of the building's thing (0x501ce0 / 0x4ffe90 +2), -1 without. ROUGH */

void ai_Unk4e48b0(int *a, char *b, char *text, char *extra)
{
    char buf[0x8f8];
    char *o, *p;
    int x, len;

    memset(buf, 0, 0xf8);
    buf[4] = 0x11;
    *(int *)(buf + 0xc) = -1;
    buf[0x36] = 9;
    *(int *)(buf + 8) = a[1];
    *(int *)(buf + 0x58) = 0x5ec;
    if (*(int *)(b + 0x14) != -1 && (b[0x26] & 0x10))
        buf[0x9c] = 0xe;
    else
        buf[0x9c] = 0xc;
    *(int *)(buf + 0x5c) = *(int *)(b + 0x14);
    *(int *)(buf + 0x64) = *(int *)b;
    x = -1;
    *(int *)(buf + 0x60) = -1;
    if (*(int *)(b + 0x14) != -1) {
        o = (char *)gm_FindBuildingById(*(int *)(b + 0x14));
        if (o != 0) {
            if (gm_IsHomeType(o) == 0) {
                p = (char *)((void *(*)(unsigned char *))gm_GetGebaeudeHauptraum)(o);
                if (p != 0)
                    *(int *)(buf + 0x60) = *(int *)(p + 2);
            } else {
                p = (char *)gm_Query(*(int *)(o + 0x5d), 1, 0, 0xfd);
                if (p != 0)
                    *(int *)(buf + 0x60) = *(int *)(p + 2);
            }
        }
    }
    strcpy(buf + 0xf8, text);
    len = strlen(buf + 0xf8) + 1;
    if (extra != 0 && *extra != 0) {
        strcpy(buf + 0xf8 + len, extra);
        buf[0x9c] |= 0x10;
        ai_Unk4c4bc0(buf, len + strlen(extra) + 1, buf + 0xf8);
    } else
        ai_Unk4c4bc0(buf, strlen(text) + 1, text);
}
