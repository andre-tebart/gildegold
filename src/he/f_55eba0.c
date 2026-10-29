/* 0x55eba0, selects the scene of the figure `p` when it should be seen (ROUGH): nothing for no figure, the kinds 0xd / 0xe or a busy job code (as 0x55ea60: 4, 0x10, 0x13, 0x1d, 0x1e and the same list), the owner contact of its building (+0x16c; 0x4ffe90 types 0x2c / 0x109) or its target (+0x170) is looked up (0x4fdcb0, 0x4fe980) and the scene shown (0x511e60, or 0x511dc0 for the free-standing case) */

void he_Unk55eba0(int p)
{
    int b, code, o;

    if (p == 0 || *(unsigned char *)(p + 2) == 0xd || *(unsigned char *)(p + 2) == 0xe)
        return;
    b = *(int *)(p + 0x16c);
    code = *(unsigned char *)(p + 0x180);
    if (code == 4 || code == 0x10 || code == 0x13 || code == 0x1d || code == 0x1e)
        return;
    if (code == 0x3f || code == 0x40 || code == 0x48 || code == 0x3c || code == 0x49 || code == 0x44 || code == 0x62 || code == 0x61 || code == 0x65 || code == 0x64 || code == 0x14 || code == 0x75 || code == 0x1a || code == 0x18 || code == 0xa1)
        return;
    if (*(unsigned char *)(p + 0x1b1) != 0)
        return;
    if (b != 0 && gm_IsHomeType(b) != 0) {
        o = gm_Query(*(int *)(b + 0x5d), 1, 0, 0x2c);
        if (o == 0)
            o = gm_Query(*(int *)(b + 0x5d), 1, 0, 0x109);
        if (o != 0)
            init_ObjectSceneUniverse(o);
        return;
    }
    if (*(int *)(p + 0x170) != 0) {
        init_BuildingSceneUniverse(*(int *)(p + 0x170));
        return;
    }
    o = gm_FindObjectInst(*(int *)(p + 4));
    if (o != 0)
        init_BuildingSceneUniverse(*(int *)(o + 0x5d));
}
