/* 0x4ed270, reads the avatar table of 0x4ed1e0 from the file f: the used bytes are first all 0xff, then each saved (id, used) pair sets the record of that id; an unknown id and a record without a pair
   are reported (0x5de652, "Avatar >%s< not found in savegame, initializing to >unused<") and the count must be 0x20 ("Inconsisten Avatar count"); 1 when all was read. ROUGH */

int gv_ReadAvatars(void *f)
{
    unsigned n, i, k;
    int id;
    char used;
    char name[5];
    char buf[256];

    if (f == 0)
        return 0;
    if (UNK_FR(&n, 4, 1, f) == 0)
        return 0;
    if (n != 0x20)
        sprintf(buf, "Inconsisten Avatar count: <Expected: %i> <Found: %i>", 0x20, n);
    for (i = 0; i < 0x1e80; i += 0xf4)
        unk_Avatars[0][i + 0xf2] = (char)0xff;
    i = 0;
    if (n != 0) {
        do {
            if (UNK_FR(&id, 4, 1, f) == 0)
                return 0;
            if (UNK_FR(&used, 1, 1, f) == 0)
                return 0;
            k = 0;
            while (k < 0x1e80) {
                if (*(int *)&unk_Avatars[0][k] == id) {
                    unk_Avatars[0][k + 0xf2] = used;
                    goto next;
                }
                k += 0xf4;
            }
            name[0] = (char)(id >> 24);
            name[1] = (char)(id >> 16);
            name[2] = (char)(id >> 8);
            name[3] = (char)id;
            name[4] = 0;
            sprintf(buf, "Avatar >%s< not found in savegame, initializing to >unused<", name);
        next:
            i++;
        } while (i < n);
    }
    for (i = 0; i < 0x1e80; i += 0xf4) {
        if (unk_Avatars[0][i + 0xf2] == (char)0xff) {
            sprintf(buf, "Avatar >%s< not found in savegame, initializing to >unused<", &unk_Avatars[0][i + 4]);
            unk_Avatars[0][i + 0xf2] = 0;
        }
    }
    return 1;
}
