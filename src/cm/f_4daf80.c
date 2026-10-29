/* 0x4daf80, a trial / session request p (+0x34 .. +0x44 five character ids, +0x48 a sixth one when the byte +0x30 is >= 6): every character must
   exist, not be of kind 0xf, be alive (+8) and, when +0x1b1 is set, not be among the characters of the list 0x53e400 / 0x53e300 with the same
   +0xac as its +4 (code 6; 7 while the list is scanned). A bad slot i gives the error code i + 1 (slots 3 and 4 both 4, the sixth 5), the
   error texts 0x11d8 .. 0x11df are shown through 0x53dda0 (-1, text, 0x5f5); with no error the messages 0x11d0 .. 0x11d7 go to the
   participants of kind 6 / 7 (0x5176d0 formats, 0x4e48b0 sends). ROUGH, the notification part is only sketched */

void ai_Unk4daf80(char *p)
{
    char text[0x200];
    char *ch[6];
    int ids[6], code, i, k;
    char *c, *n;

    code = 0;
    ch[0] = 0;
    ids[0] = *(int *)(p + 0x34);
    ids[1] = *(int *)(p + 0x38);
    ids[2] = *(int *)(p + 0x3c);
    ids[3] = *(int *)(p + 0x40);
    ids[4] = *(int *)(p + 0x44);
    ids[5] = *(int *)(p + 0x48);
    for (i = 0; i < 6; i++) {
        c = (char *)gm_FindChar(ids[i]);
        ch[i] = c;
        if (code != 0)
            continue;
        if (i == 5 && *(unsigned char *)(p + 0x30) < 6)
            continue;
        if (c == 0 || c[2] == 0xf || c[8] == 0) {
            code = (i == 5) ? 5 : (i == 4) ? 4 : i + 1;
            continue;
        }
        if (*(unsigned char *)(c + 0x1b1) != 0) {
            code = 7;
            for (n = (char *)he_FindEvent(1, 0, 0x3d); n != 0; n = (char *)he_NextEvent()) {
                if (*(int *)(n + 0xac) == *(int *)(c + 4)) {
                    code = 6;
                    break;
                }
            }
        }
    }
    if (code != 0) {
        switch (code) {
        case 1: txt_Format(text, 0x11d8); break;
        case 2: txt_Format(text, 0x11d9); break;
        case 3: txt_Format(text, 0x11da); break;
        case 4: txt_Format(text, 0x11db); break;
        case 5: txt_Format(text, 0x11dc); break;
        case 6: txt_Format(text, 0x11de); break;
        case 7: txt_Format(text, 0x11df); break;
        }
        ((int (*)(int, int, int))he_ShowNews)(-1, text, 0x5f5);
        return;
    }
    for (i = 0; i < 6; i++) {
        k = (unsigned char)ch[i][2];
        if (k == 6 || k == 7) {
            txt_Format(text, 0x11d0 + i, *(unsigned short *)ch[i]);
            strcat(text, &ch_EmptyName);
            ai_Unk4e48b0(ch[i], 0, text);
        }
    }
    ((int (*)(int, int, int))he_ShowNews)(-1, text, 0x5f5);
    G22_4C5580();
}
