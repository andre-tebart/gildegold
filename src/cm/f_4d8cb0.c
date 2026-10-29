/* 0x4d8cb0, the fight result list (a) or one more picture of its three stacks: b set: the three headings (texts 0x12d3 / 0x12d4 / 0x12d5)
   in the window 0x6ed938 and the counters 0x6f0d6c / 0x6f0d68 / 0x6f0d70 cleared; else the next picture 0x4df of stack c (0, 1, 2) */

void ai_Unk4d8cb0(int a, int b, int c)
{
    char buf[0x40];
    int o;

    if (b != 0) {
        d2_RemoveWindowObjects(d2_LastWindow, 1);
        txt_Format(buf, 0x12d3);
        o = d2_CreateWinText(0, 0x20, buf, d2_LastWindow);
        d2_SetObjFont(o, 0x54);
        txt_Format(buf, 0x12d4);
        o = d2_CreateWinText(0, 0x3e, buf, d2_LastWindow);
        d2_SetObjFont(o, 0x54);
        txt_Format(buf, 0x12d5);
        o = d2_CreateWinText(0, 0x2f, buf, d2_LastWindow);
        d2_SetObjFont(o, 0x54);
        ai_Unk6f0d6c = 0;
        ai_Unk6f0d68 = 0;
        ai_Unk6f0d70 = 0;
        return;
    }
    if (c == 0) {
        d2_WinOpenPic((ai_Unk6f0d6c * 5 + 0x28) * 2, 0x20, 0x4df, d2_LastWindow);
        ai_Unk6f0d6c++;
    } else if (c == 1) {
        d2_WinOpenPic((ai_Unk6f0d68 * 5 + 0x28) * 2, 0x3e, 0x4df, d2_LastWindow);
        ai_Unk6f0d68++;
    } else if (c == 2) {
        d2_WinOpenPic((ai_Unk6f0d70 * 5 + 0x28) * 2, 0x2f, 0x4df, d2_LastWindow);
        ai_Unk6f0d70++;
    }
}
