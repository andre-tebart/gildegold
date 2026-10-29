/* 0x4f36e0, the index of the record with the id (+0) in 0x91f700 or -1 */

int unk_Unk4f36e0(int id)
{
    int i;
    int *p;

    i = 0;
    p = (int *)gv_Graveyard;
    do {
        if (*p == id)
            return i;
        p = (int *)((char *)p + 0x2d);
        i++;
    } while ((int)p < (int)((unsigned)&ai_Unk925100));
    return -1;
}
