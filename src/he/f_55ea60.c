/* 0x55ea60, the id of the contact (the person to talk to at the place) of the figure `p`, or -1 (ROUGH): -1 for no figure, the kinds 0xd / 0xe, no current building (+0x16c) or object (+0x184) and for a busy job code (+0x180: 0x3f, 0x40, 0x48, 0x3c, 0x49, 0x44, 0x62, 0x61, 0x65, 0x64, 0x14, 0x75, 0x1a, 0x18, 0xa1); a building that 0x502520 knows takes the owner contact (0x4ffe90 (building object +0x5d, 1, 0, 0xfd) or the type 0x37 for the job codes 3 / 8 / 9) and gives its +2 id when the target of the figure (+0x184 +0x2c / +0x30) is not it (0x501b50 as the fallback) */

int he_Unk55ea60(int p)
{
    int code, b, o;

    if (p == 0 || *(unsigned char *)(p + 2) == 0xd || *(unsigned char *)(p + 2) == 0xe)
        return -1;
    b = *(int *)(p + 0x16c);
    if (b == 0 || *(int *)(p + 0x184) == 0)
        return -1;
    code = *(unsigned char *)(p + 0x180);
    if (code == 0x3f || code == 0x40 || code == 0x48 || code == 0x3c || code == 0x49 || code == 0x44 || code == 0x62 || code == 0x61 || code == 0x65 || code == 0x64 || code == 0x14 || code == 0x75 || code == 0x1a || code == 0x18 || code == 0xa1)
        return -1;
    if (*(unsigned char *)(p + 0x1b1) != 0)
        return -1;
    if (gm_IsHomeType(b) != 0) {
        if (code == 3 || code == 8 || code == 9)
            o = gm_Query(*(int *)(b + 0x5d), 1, 0, 0xfd);
        else
            o = gm_Query(*(int *)(b + 0x5d), 1, 0, 0x37);
        if (o != 0 && *(unsigned short *)o != 0xfd && *(int *)(*(int *)(p + 0x184) + 0x2c) != *(int *)(*(int *)(p + 0x184) + 0x30))
            return *(int *)(o + 2);
        return -1;
    }
    o = gm_BuildingMainObject(b);
    return o != 0 ? *(int *)(o + 2) : -1;
}
