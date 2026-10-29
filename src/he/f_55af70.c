/* 0x55af70, the allocation handler of a news event (ROUGH): due now + random 0x42ec00(3) units, the first building (0x501190 (1, 1, ..)) of the kinds 0xb / 0xc / 0xd gives the message 0x4c4a60 (.., 0x5a, 2, 2) to its figure, the owner gets the news "_NACHRICHTEN_HS_43" (0x53dee0) and 0x509e30 / 0x4c47e0 set a flag; the record is freed */

void he_Unk55af70(int ev)
{
    int b, kind, p;
    char text[0x100];

    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, (unsigned short)util_Random(3), 0);
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
    if (b == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    kind = *(unsigned char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)b * 0x24d);
    if (kind == 0xd || kind == 0xc || kind == 0xb) {
        p = gm_FindChar(*(int *)(b + 1));
        if (p != 0)
            cm_SendSetField(*(int *)(p + 4), 0x5a, 2, 2, 0);
    }
    txt_Format(text, 0x1bba, b);
    he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, *(int *)(b + 1), -1, "_NACHRICHTEN_HS_43", 0);
    if (((int (*)(void *))gm_Unk509e30)(b) != 0)
        ((void (*)(int))cm_SendMoveGoods)(b);
    he_Unk53e1f0((int *)ev);
}
