/* 0x5d0530, a message callback (423 B), the twin of 0x5d0320: param_1 the message record (+2 the
   type), param_2 the request record (+4 the target id). Type 6: the same config / window 0x5c1130
   (cfg, text 0x9c, 0, 0) and then the type 0x82 message (+8 the record + 4, +0xc = -1, +0x36 = 2,
   +0x58 = 0, +0x59 = 4, +0x5c = 0xbe2aaaab (-1/6), +0x60 = 0xbeaaaaab (-1/3), +0x64 = 0x18) through
   0x4c4b70; other types: only the target 0x505df0 (param_2 + 4), no message. When the target's type is
   6 / 7 the text 0x5176d0 (0xff6) goes through 0x53dda0 (the target + 4, -1, 0, 0x5fb). Returns 1 (0
   without a record / target). */

int ai_Unk5d0530(int param_1, int param_2)
{
    int cfg[20];
    char cfgtext[0x100];
    char text[0x200];
    int rec[0x3e];
    int req;
    int i;

    if (*(char *)(param_1 + 2) != 6) {
        req = gm_FindChar(*(int *)(param_2 + 4));
        if (req == 0)
            return 0;
    } else {
        for (i = 0; i < 20; i++)
            cfg[i] = 0;
        for (i = 7; i < 15; i++)
            cfg[i] = -1;
        txt_Format(cfgtext, (char *)0x9c);
        cfg[0] = 0;
        cfg[1] = 0x400;
        cfg[2] = U_I(((unsigned)&ai_Unk5eeab8 + 0x2b0));
        cfg[3] = U_I(((unsigned)&ai_Unk5eeab8 + 0x2b4));
        cfg[4] = 6;
        req = ai_AmountMenu(cfg, cfgtext, 0, 0);
        if (req == 0)
            return 0;
        *(char *)((char *)rec + 4) = 0x82;
        rec[2] = *(int *)(req + 4);
        rec[3] = -1;
        *(char *)((char *)rec + 0x36) = 2;
        *(char *)((char *)rec + 0x58) = 0;
        *(char *)((char *)rec + 0x59) = 4;
        *(int *)((char *)rec + 0x5c) = 0xbe2aaaab;
        *(int *)((char *)rec + 0x60) = 0xbeaaaaab;
        *(int *)((char *)rec + 0x64) = 0x18;
        cm_SendLongMessage(rec);
    }
    if (*(char *)(req + 2) == 6 || *(char *)(req + 2) == 7) {
        txt_Format(text, (char *)0xff6);
        he_ShowNews(*(int *)(req + 4), -1, text, 0, 0x5fb, 0);
    }
    return 1;
}
