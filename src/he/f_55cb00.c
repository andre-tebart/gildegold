/* 0x55cb00, the run handler of a message event (ROUGH): states -2 / -1 and the missing figure +0xb0 free the record; a figure of kind 6 / 7 gets a message record (0x4c4b70) and the news 0x4c59b0; then the record is freed */

void he_Unk55cb00(int ev)
{
    int st = *(int *)(ev + 0x70);
    int p;
    unsigned char m[0xf8];

    if (st <= -2 || (p = gm_FindChar(*(int *)(ev + 0xb0))) == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (*(unsigned char *)(p + 2) == 6 || *(unsigned char *)(p + 2) == 7) {
        memset(m, 0, sizeof(m));
        cm_SendLongMessage(m);
        ((void (*)(int))ai_Unk4c59b0)(*(int *)(ev + 0x10));
    }
    he_Unk53e1f0((int *)ev);
}
