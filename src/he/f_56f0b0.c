/* 0x56f0b0, the run of the event 0x9b: any state but 0 frees; the state 0 sends the text 0x4035 (0x628e40) when the record of +0xac exists, the text 0x4036 when the record of +0xb0 exists, then frees */

void he_Unk56f0b0(int ev)
{
    char text[0x100];
    int r;

    if (HM_I(ev, 0x70) != 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    r = gm_FindChar(HM_I(ev, 0xac));
    if (r != 0) {
        txt_Format(text, 0x4035);
        he_ShowNews(HM_I(ev, 0xac), -1, text, 1, 0x5fb, (char *)(unsigned)&full_data_628e40);
    }
    if (gm_FindChar(HM_I(ev, 0xb0)) != 0) {
        txt_Format(text, 0x4036, *(unsigned short *)r);
        he_ShowNews(HM_I(ev, 0xb0), -1, text, 1, 0x5fa, 0);
    }
    he_Unk53e1f0((int *)ev);
}
