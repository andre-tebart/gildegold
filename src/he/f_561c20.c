/* 0x561c20, the run handler of the burglary ("Einbruch" / "Entdeckt", the news "_NACHRICHTEN_HS_76" / "_77", the formats "$Z%T$A" and "%i %s$A"; ROUGH skeleton, 3264 bytes): the detection check 0x553ae0 / 0x4f2e30, the guards (0x53e400 over the type records, 0x4c4ed0), the loot by a random roll (util_Random, 0x5085f0) taken from the owner (0x4c4830), the news to the owner and the thieves (0x53dda0) and the end releases the group and frees the record */

void he_Unk561c20(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b;
    char text[0x200];

    if (st < 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    txt_Format(text, 0x1bba, b);
    he_ShowNews(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5fb, 0);
    ((void (*)(int))gm_PlayerFactor)((unsigned short)util_Random(100));
    cm_SendMoveGoodsUnchecked(*(int *)(ev + 0xac), -1, 0, 0);
    he_Unk53e1f0((int *)ev);
}
