/* 0x4b1ee0, marks list i of hst_Lists used and empty */

int ai_Unk4b1ee0(unsigned short i)
{
    int j;

    if (i >= 4)
        return 0;
    hst_Lists[i].inUse = 1;
    for (j = 0; j < 4; j++) {
        hst_Lists[i].e[j].id = -1;
        hst_Lists[i].e[j].v = 0xffff;
    }
    return 1;
}
