/* 0x56e460, the alloc of the event 0x99 (not while paused, flags +0x78 & 4): when another event of the type 0x99 of the same player (+8) already runs, this one only announces the text 0x3ff7 (0x5176d0 / 0x53dda0), schedules with the handle -1 (0x4c4c60 ( -1, ev) into +0x84) and frees; else the start clock +0x44 and the due clock +0x52 = now, due 1, +0xcc = the notification 0x4c59f0 (0xd, ev's +0xb0, -1, 0x2d, 0, 0, the text 0x628dc8) and the handler scheduled (0x4c4c60 (0, ev)) into +0x84 */


void ai_Unk4c59f0(unsigned char a, int b, int c, unsigned char d, unsigned char e, unsigned char f, const char *s);

void he_Unk56e460(int ev)
{
    char text[0x400];
    int r;

    if (*(unsigned char *)(ev + 0x78) & 4)
        return;
    for (r = he_FindEvent(1, 0, 0x99); r != 0; r = he_NextEvent()) {
        if (r != ev && *(unsigned short *)(r + 8) == *(unsigned short *)(ev + 8))
            goto taken;
    }
    *(HETIME *)(ev + 0x44) = ai_Time;
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
    *(int *)(ev + 0xcc) = ((int (*)(int, int, int, int, int, int, const char *))ai_Unk4c59f0)(0xd, *(int *)(ev + 0xb0), -1, 0x2d, 0, 0, (char *)(unsigned)&full_data_628dc8);
    *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    return;
taken:
    txt_Format(text, 0x3ff7, *(unsigned short *)(ev + 8));
    he_ShowNews(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5fa, 0);
    *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
    he_Unk53e1f0((int *)ev);
}
