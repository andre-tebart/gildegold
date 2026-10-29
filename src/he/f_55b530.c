/* 0x55b530, the run handler of a building event (ROUGH): states -2 / -1 free; the building +0xac is looked up (none: free), the minutes since the clock +0x60 (0x4fe280) scale a value (+0xb0 .. 0x50cad0), the news 0x17cb ("_NACHRICHTEN_HS_68") goes to the owner (0x53dee0) and the event is posted 0xa later */

void he_Unk55b530(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, m;
    char text[0x100];

    if (st < 0 || (b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac))) == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    m = gm_MinutesBetween((unsigned)&ai_Time, ev + 0x60);
    *(int *)(ev + 0xb0) = (int)(m * 0.5 + 0.25) + *(int *)(ev + 0xb0);
    gm_Unk50cad0(b, *(int *)(ev + 0xb0));
    if (*(int *)(ev + 0xb0) != 0) {
        txt_Format(text, 0x17cb, b);
        he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5ec, *(int *)(b + 1), -1, "_NACHRICHTEN_HS_68", 0);
    }
    *(HETIME *)(ev + 0x60) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0xa, 0);
}
