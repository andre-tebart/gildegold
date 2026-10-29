/* 0x5a53e0, the kidnapping start (0x90 frame): 0x520fe0 (param_1); the first record of the type 0x3c (0x53e400
   (1, 0, 0x3c) / 0x53e300) whose +0xb4 is the owner (param_1 + 1) shows the text 0x14408ac and out; else the
   type 0x3d records: an own record +0xb0 with a live character 0x505df0 (+0xac) whose +0x1b1 is set shows the
   text and out, otherwise it is marked 0x4c4c60 (-1, rec) at +0x84; with no record the request (0, 0x400,
   0x5eed68, 0x5eed6c, 6, the eight -1 at +0x1c) and the text 0x19b8 go to 0x525220 (param_1, 0) / 0x5c1130
   (the request, the text, 0, the callback 0x5a5120), then 0x525440 / 0x52d7a0 */

void ai_Unk5a53e0(int param_1)
{
    int req[20];
    char buf[0x40];
    int rec;
    int ch;
    int i;

    if (sv_CheckCharacterFlag(param_1) != 0)
        return;
    rec = he_FindEvent(1, 0, 0x3c);
    while (rec != 0) {
        if (U_I(rec + 0xb4) == U_I(param_1 + 1))
            goto found;
        rec = he_NextEvent();
    }
    rec = he_FindEvent(1, 0, 0x3d);
    for (;;) {
        if (rec == 0) {
            for (i = 0; i < 0x14; i++)
                req[i] = 0;
            for (i = 0; i < 8; i++)
                req[7 + i] = -1;
            req[0] = 0;
            req[1] = 0x400;
            req[4] = 6;
            req[2] = U_I(((unsigned)&ai_Unk5eeab8 + 0x2b0));
            req[3] = U_I(((unsigned)&ai_Unk5eeab8 + 0x2b4));
            txt_Format(buf, 0x19b8);
            sv_Unk525220(param_1, 0);
            ai_AmountMenu(req, buf, 0, (void *)(unsigned)&ai_Unk5a5120);
            sv_ClosePlayerBar();
            sv_CameraFollow();
            return;
        }
        if (U_I(rec + 0xb0) == U_I(param_1 + 1)) {
            ch = gm_FindChar(U_I(rec + 0xac));
            if (ch != 0 && *(unsigned char *)(ch + 0x1b1) > 0)
                goto found;
            U_I(rec + 0x84) = cm_SendExSetHE(-1, rec);
        }
        rec = he_NextEvent();
    }
found:
    sv_MessageBox((char *)U_I((unsigned)&full_data_14408ac), 0);
}
