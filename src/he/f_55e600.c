/* 0x55e600, the run handler of a relation event (ROUGH): states -2 / -1 free; the building +0x10 (none: free) and the figure +0xac; every character of the town in the same building gets the relation change 0x4c4b00 (id, +0xac, 0x3e8-scaled by 0x5085f0 (1000)) and the player figures (kind 6 / 7) the news 0x19ab (0x53dee0, 0x5ec); then the record is freed */

void he_Unk55e600(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, c;
    char text[0x100];

    if (st < 0 || (b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10))) == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    ((void (*)(int))gm_PlayerFactor)(0x3e8);
    for (c = (unsigned)&ai_CharInts; c < ((unsigned)((unsigned)ai_CharInts + 0x653fe) + 0x16a); c += 0x21c) {
        if (*(unsigned short *)(c - 0x4) != 0xffff && *(int *)(c + 0x168) == b) {
            cm_SendRelations(*(int *)c, *(int *)(ev + 0xac), 0, 0);
            if (*(unsigned char *)(c - 2) == 6 || *(unsigned char *)(c - 2) == 7) {
                txt_Format(text, 0x19ab, b);
                he_MessageBoxQuickjump(*(int *)c, -1, text, 0, 0x5ec, -1, 0, 0, 0);
            }
        }
    }
    he_Unk53e1f0((int *)ev);
}
