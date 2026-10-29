/* 0x55b830, the run handler of a building event (ROUGH): states -2 / -1 free; the building +0xac is looked up, the player gets the news 0x17e7 and 0x1c32 for the owner (0x50a810), the flag 0x40 of the building is cleared (0x4c4a60 0x5a / 2 / 0x40, 0x4c54e0) and the record freed */

void he_Unk55b830(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, own, ch;
    char text[0x200];

    if (st < 0 || (b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac))) == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    own = gm_FindCharacterByMaster(b);
    ch = (unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c;
    txt_Format(text, 0x17e7);
    he_MessageBoxQuickjump(*(int *)(ch + 4), -1, text, 0, 0x5ec, *(int *)(b + 1), -1, 0, 0);
    if (own != 0) {
        txt_Format(text, 0x1c32, *(unsigned short *)(ev + 8) * 2 + 0x49c);
        he_MessageBoxQuickjump(*(int *)(ch + 4), -1, text, 0, 0x5ec, *(int *)(b + 1), *(int *)(own + 2), 0, 0);
    }
    ai_Unk4c54e0(b);
    cm_SendSetField(*(int *)(b + 1), 0x5a, 2, 0x40, 0);
    he_Unk53e1f0((int *)ev);
}
