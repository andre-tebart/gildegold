/* 0x4e53e0, the progress of the dialog dlg: with c set the clock 0x6333cc and the duration a are stored (and the window cleared, 0x475e40);
   else the left percent of the duration is the value; the slider (0x475cb0, 0 .. 100) is set to it, returned */

int ai_Unk4e53e0(int dlg, int a, int c)
{
    int v;

    v = 100;
    if (c != 0) {
        ai_Unk6f0dc0 = ai_Unk6333cc;
        ai_Unk60d374 = a;
        d2_SetObjEnabled(dlg, 0);
    } else {
        v = (int)((1.0f - (float)(unsigned int)(ai_Unk6333cc - ai_Unk6f0dc0) / ai_Unk60d374) * 100.0f);
        v = v > 0 ? v : 0;
    }
    d2_SetObjValue(dlg, 0, 100, v);
    return v;
}
