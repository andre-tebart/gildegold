/* 0x502e30, for a building of the type 2: runs 0x502a60 for every object of the kind 2 in its list (up to 32), with the building's owner (+0x27) */


void gm_ShowBuildingObjects(unsigned char *b)
{
    int l[32];
    int n, i, o;

    n = 0;
    if (ai_BTypes[*b * 0x24d] == 2) {
        o = (int)gm_Query(*(int *)(b + 0x5d), 1, 4, 2);
        while (o != 0) {
            l[n] = o;
            n++;
            o = (int)gm_QueryNext();
        }
        for (i = 0; i < n; i++)
            gm_Unk502a60((int)b, l[i], *(unsigned short *)(b + 0x27));
    }
}
