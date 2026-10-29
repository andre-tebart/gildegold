/* 0x548110, a Ghidra miss (disassembled): the start of the insult news event: the clock +0x18 units ahead, the news 0x1d64 (for b of kind 6 / 7, "_NACHRICHTEN_HS_20") and the event is posted (state 0); a gone figure ends it with the clock copied (ROUGH) */

void he_Unk548110(int ev)
{
    char text[0x400];
    int a, b;

    *(int *)(ev + 0x84) = -1;
    if ((*(unsigned char *)(ev + 0x78) & 4) == 0) {
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        if ((*(unsigned char *)(ev + 0x78) & 2) != 0) {
            b = gm_FindChar(*(int *)(ev + 0xb0));
            a = gm_FindChar(*(int *)(ev + 0xac));
            if (a == 0 || b == 0) {
                *(HETIME *)(ev + 0x52) = ai_Time;
                cm_SendExSetHE(-1, ev);
                return;
            }
            if (*(char *)(b + 2) == 6 || *(char *)(b + 2) == 7) {
                txt_Format(text, 0x1d64, *(unsigned short *)a, ev + 0x52, *(unsigned short *)a);
                he_ShowNews(*(int *)(b + 4), -1, text, 0, 0x5fb, "_NACHRICHTEN_HS_20");
            }
        }
        cm_SendExSetHE(0, ev);
    }
}
