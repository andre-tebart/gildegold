/* 0x589570, the tooltip line of the slot idx (256 B): a = U_I (0x6f2308) (else U_I (0x6f230c) -> its
   0x4ffe90 (a + 0x5d, 2, 6, 0, 0xfd) when U_I (0x6f2310) is 0, else U_I (0x75ec60) or U_I (0x6f1bc8)),
   b = U_I (0x6f2310). The slot 0x6f5ec4 + idx * 0xc = a + 1, 0x6f5ec8 = b + 2, or -1 without b or with b's word
   0xfd; the line "%s = %G, %s" (0x62aa14: idx + 0x205e, a, b's word * 2 + 0x9a8) or "%s = %G" (0x62aa20) in
   0x5176d0, then 0x52e330 (the line) */

void ai_Unk589570(int idx)
{
    char buf[0x100];
    char *a;
    char *b;

    a = *(char **)(unsigned)&ai_PlayerBuilding;
    b = 0;
    if (a != 0) {
        b = *(char **)(unsigned)&sv_PanelShown;
    } else {
        a = *(char **)(unsigned)&ai_FallbackBuilding;
        if (a != 0) {
            b = *(char **)(unsigned)&sv_PanelShown;
            if (b == 0)
                gm_Query(a + 0x5d, 2, 6, 0, 0xfd);
        } else {
            a = *(char **)(unsigned)&ai_Unk75ec60;
            if (a == 0)
                a = *(char **)(unsigned)&ai_MarketBuilding;
        }
    }
    *(int *)((unsigned)&full_data_6f5ec4 + idx * 0xc) = *(int *)(a + 1);
    if (b == 0 || *(unsigned short *)b == 0xfd) {
        *(int *)((unsigned)&full_data_6f5ec8 + idx * 0xc) = -1;
        txt_Format(buf, (char *)(unsigned)&full_data_62aa20, (char *)(idx + 0x205e), a);
    } else {
        *(int *)((unsigned)&full_data_6f5ec8 + idx * 0xc) = *(int *)(b + 2);
        txt_Format(buf, (char *)(unsigned)&full_data_62aa14, (char *)(idx + 0x205e), a, *(unsigned short *)b * 2 + 0x9a8);
    }
    sv_SetStatusText(buf);
}
