/* 0x5471e0, the duel intro: lets both duellists (p2 / p3, 0x4c4a60 flag 0x400) hear the news 0x1d83 / 0x1d82 (messages 0x5fa / 0x5fb, "_DUELL_INTRO_EVENT_DUELLANTEN_HS_00" for the first) and posts the event (state 2) */

void he_Unk5471e0(int ev, unsigned short *p2, unsigned short *p3)
{
    char buf[1024];

    cm_SendSetField(*(int *)(p2 + 2), 0x1c8, 4, 0x400, 0);
    cm_SendSetField(*(int *)(p3 + 2), 0x1c8, 4, 0x400, 0);
    if (*(char *)(p2 + 1) == 6 || *(char *)(p2 + 1) == 7) {
        txt_Format(buf, 0x1d83, *p3, ev + 0x52, *p3);
        he_ShowNews(*(int *)(p2 + 2), *(int *)(p2 + 2), buf, 0, 0x5fa, "_DUELL_INTRO_EVENT_DUELLANTEN_HS_00");
    }
    if (*(char *)(p3 + 1) == 6 || *(char *)(p3 + 1) == 7) {
        txt_Format(buf, 0x1d82, *p2, ev + 0x52, *p2);
        he_ShowNews(*(int *)(p3 + 2), -1, buf, 0, 0x5fb, "_DUELL_INTRO_EVENT_DUELLANTEN_HS_00");
    }
    cm_SendExSetHE(2, ev);
}
