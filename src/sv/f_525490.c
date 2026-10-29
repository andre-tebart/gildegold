/* 0x525490, a callback (the object o in the argument): copies the 6 colour records of the table by the day phase (0x760610 and the flag 0x10 of o +0x211 pick 0x623be0 / 0x623c50 / 0x623d30 / 0x623da0) into the
   record +0x1e8 of o (+0x18 .. +0x24 of 6 rows of 0x38); returns 1 */

char sv_Unk525490(char *o)
{
    int *t;
    int i;
    unsigned char f;

    f = (unsigned char)o[0x211];
    if (ai_Unk760610 == 0)
        t = (int *)((f & 0x10) ? ((unsigned)&ai_Unk623baa + 0xa6) : ((unsigned)&ai_Unk623baa + 0x36));
    else
        t = (int *)((f & 0x10) ? ((unsigned)&ai_Unk623cc0 + 0xe0) : ((unsigned)&ai_Unk623cc0 + 0x70));
    for (i = 0; i < 0x150; i += 0x38) {
        *(int *)(*(int *)(o + 0x1e8) + i + 0x18) = t[0 + i / 0x38 * 4];
        *(int *)(*(int *)(o + 0x1e8) + i + 0x1c) = t[1 + i / 0x38 * 4];
        *(int *)(*(int *)(o + 0x1e8) + i + 0x20) = t[2 + i / 0x38 * 4];
        *(int *)(*(int *)(o + 0x1e8) + i + 0x24) = t[3 + i / 0x38 * 4];
    }
    return 1;
}
