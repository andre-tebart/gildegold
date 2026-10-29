/* 0x55c700, the run handler "he_RunKredit" (the loan event, 4 states; the debug text "he_RunKredit(): Could not find GebaeudeId '%i'" when the building +0xac is missing; ROUGH skeleton): the player figure gets the offer news (0x5ec / "_NACHRICHTEN_HS_69", the amount 0x5b92e0, the interest from 0x509e60 through _ftol / 0x5de652 (sprintf)) and 0x4c4830 books the payment; the end frees the record */

void he_Unk55c700(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, amt;
    char text[0x200];

    if (st < 0 || st > 3 || (st != 0 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0 && *(int *)(ev + 0x84) != -1)) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b == 0) {
        sprintf(text, "he_RunKredit(): Could not find GebaeudeId '%i'", *(int *)(ev + 0xac));
        he_Unk53e1f0((int *)ev);
        return;
    }
    amt = (int)gm_GetGoodsValue(*(unsigned short *)(b + 0x27));
    txt_Format(text, 0x17f0, b, amt);
    he_ShowNews(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, 0);
    cm_SendMoveGoodsUnchecked(*(int *)(ev + 0xb0), *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(unsigned char *)(unsigned)&ai_OwnPlayer, amt);
    he_Unk53e1f0((int *)ev);
}
