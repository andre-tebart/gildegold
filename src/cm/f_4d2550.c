/* 0x4d2550, a callback: collects the objects "WARE_" whose owner (+0x200, a record with the side in +0xa) is not the side
   of the character c into ai_Unk930380[ai_Unk6f0d60++] */

char ai_Unk4d2550(char *o, char *c)
{
    char *side = *(char **)(c + 0x16c);

    if (strncmp(o, "WARE_", 5) == 0 && *(int *)(*(char **)(o + 0x200) + 0xa) != *(unsigned short *)side)
        ai_Unk930380[ai_Unk6f0d60++] = o;
    return 1;
}
