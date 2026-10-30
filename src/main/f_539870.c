/* 0x539870, the end of the round report of the player (0x6f25b8): the bookkeeping of the character record (0x506430: income / expense fields +0x14 .. +0x54, texts 0x2005 .. 0x204e of 0x515960)
   shown on a page (BLACK fade, the window 0x52fa10, the loop 0x5309a0 until a key), then the fields are rolled over (+0x14 .. +0x24 added to +0x2c .. +0x38 and cleared); when the balance goes
   negative (0x50a6c0) a message of kind 0x60 / 9 is sent and a replacement figure is created (0x539620) if 0x6f25b0 is set; ROUGH (many float sums, the __ftol arguments, are guesses) */

void main_Unk539870(void)
{
    char msg[0x114];
    char lines[0x40];
    int a[3];
    int sum[6], cnt[6];
    unsigned char kind[8];
    unsigned char *fade;
    unsigned char *b;
    unsigned char *o;
    int bk, id, iv, v, t, tot, win, nrow, i, j, k;
    int w200, w1fc, w1f8, w1ec, w1e8;

    iv = 0;
    w200 = 0;
    w1ec = 0;
    w1e8 = 0;
    fade = 0;
    U_I((unsigned)&sv_PanelFlag) = 0;
    if (U_I((unsigned)&full_data_624560) != 0) {
        fade = (unsigned char *)d2_StartFade(0, 0, (int)*(short *)(unsigned)&d2_ScreenW, (int)*(short *)(unsigned)&d2_ScreenH, "BLACK", 0x1e, 1);
        while ((*fade & 4) == 0)
            sv_RunFrame(0, U_I((unsigned)&sv_FrameMask));
    }
    id = U_I((unsigned)&ai_PlayerId) & 0xffff;
    bk = gm_PlayerRecord((unsigned)&ai_CharsT + id * 0x21c);
    *(int *)(bk + 0x3c) = *(int *)(bk + 0x3c) + U_I(((unsigned)&ai_Selected + 0x1c) + id * 0x21c) + U_I(((unsigned)&ai_Selected + 0x18) + id * 0x21c);
    d3_SetObjectPos(U_I((unsigned)&ai_Unkfc2dec), bk + 0x8c);
    d3_SetObjectRot(U_I((unsigned)&ai_Unkfc2dec), bk + 0x9c);
    if (U_I((unsigned)&full_data_624560) != 0) {
        w1f8 = 0;
        d2_SetPointerSprite(0, 0);
        sv_Unk522280();
        sv_SetScreenCenter(0x24087);
        win = sv_Unk52fa10();
        d2_CentreViewWindows(win);
        if (fade != 0) {
            d2_ReleaseFade(fade);
            fade = 0;
        }
        d2_StartFade(0, 0, (int)*(short *)(unsigned)&d2_ScreenW, (int)*(short *)(unsigned)&d2_ScreenH, "BLACK", 0x1e, 10);
        d2_SetForm(win, 2);
        io_PrintText(0x7d);
        gm_ClockToDate((unsigned)&ai_TimeT, lines);
        d2_SetForm(win, 1);
        io_PrintText(0x2007);
        iv = U_I(((unsigned)&ai_Selected + 0x14) + id * 0x21c);
        w1fc = iv - U_I(((unsigned)&ai_Selected + 0x10) + id * 0x21c);
        if (iv != 0)
            io_PrintText(0x2008, iv);
        if (U_I(((unsigned)&ai_Selected + 0x10) + id * 0x21c) != 0)
            io_PrintText(0x2009, U_I(((unsigned)&ai_Selected + 0x10) + id * 0x21c));
        iv = w1fc;
        if (*(int *)(bk + 0x40) != 0) {
            io_PrintText(0x2014, *(int *)(bk + 0x40));
            w200 = *(int *)(bk + 0x40);
            iv = w1fc - w200;
        }
        if (*(int *)(bk + 0x44) != 0) {
            io_PrintText(0x2016, *(int *)(bk + 0x44));
            iv -= *(int *)(bk + 0x44);
            w200 += *(int *)(bk + 0x44);
        }
        if (*(int *)(bk + 0x48) != 0) {
            io_PrintText(0x201b, *(int *)(bk + 0x48));
            iv -= *(int *)(bk + 0x48);
            w200 += *(int *)(bk + 0x48);
        }
        if (*(int *)(bk + 0x4c) != 0) {
            io_PrintText(0x201a, *(int *)(bk + 0x4c));
            iv -= *(int *)(bk + 0x4c);
            w200 += *(int *)(bk + 0x4c);
        }
        if (*(int *)(bk + 0x54) != 0) {
            io_PrintText(0x2015, *(int *)(bk + 0x54));
            iv -= *(int *)(bk + 0x54);
            w200 += *(int *)(bk + 0x54);
        }
        if (*(int *)(bk + 0x28) != 0) {
            io_PrintText(0x201c, *(int *)(bk + 0x28));
            iv -= *(int *)(bk + 0x28);
            w200 += *(int *)(bk + 0x28);
        }
        if (*(int *)(bk + 0x14) != 0) {
            io_PrintText(0x200d, *(int *)(bk + 0x14));
            iv += *(int *)(bk + 0x14);
            w1fc += *(int *)(bk + 0x14);
        }
        if (*(int *)(bk + 0x18) != 0) {
            io_PrintText(0x200c, *(int *)(bk + 0x18));
            iv += *(int *)(bk + 0x18);
            w1fc += *(int *)(bk + 0x18);
        }
        if (*(int *)(bk + 0x1c) != 0) {
            io_PrintText(0x200b, *(int *)(bk + 0x1c));
            iv += *(int *)(bk + 0x1c);
            w1fc += *(int *)(bk + 0x1c);
        }
        if (*(int *)(bk + 0x20) != 0) {
            io_PrintText(0x2010, *(int *)(bk + 0x20));
            iv += *(int *)(bk + 0x20);
            w1fc += *(int *)(bk + 0x20);
        }
        if (*(int *)(bk + 0x50) != 0) {
            io_PrintText(0x201d, *(int *)(bk + 0x50));
            iv -= *(int *)(bk + 0x50);
            w200 += *(int *)(bk + 0x50);
        }
        if (iv == 0)
            io_PrintText(0x201e);
        else
            io_PrintText(0x204c, iv);
        d2_SetForm(win, 3);
        *(short *)(U_I((unsigned)&d2_LastWindowPtr) + 0x27c) = 0x55;
        io_PrintText(0x2005);
        d2_SetForm(win, 1);
        io_PrintText((unsigned)"$N");
        b = (unsigned char *)gm_BuildQuery(1, 6);
        while (b != 0) {
            if (*(short *)(b + 0x27) == (short)U_I((unsigned)&ai_PlayerId) || *(short *)(b + 0x25) == (short)U_I((unsigned)&ai_PlayerId)) {
                t = gm_BuildingClass(*b);
                v = gm_Unk508d80(b, U_I((unsigned)&ai_PlayerId) & 0xffff, a);
                if (v != 0 || (t != 5 && t != 6)) {
                    w1fc += a[0];
                    w1ec += a[1];
                    w200 += a[2];
                    w1f8 += a[2];
                }
            }
            b = (unsigned char *)gm_BuildNext();
        }
        nrow = gm_Unk50a1c0(U_I((unsigned)&ai_PlayerId) & 0xffff, 0, 0xffff, lines);
        j = 0;
        v = 0;
        if (nrow > 0) {
            do {
                w1fc = 0;
                v = 0;
                w1f8 = 0;
                j++;
            } while (j < nrow);
            if (v != 0)
                io_PrintText(0x2048, v);
        }
        k = id * 0x21c;
        if (*(char *)(((unsigned)&ai_CharInts + 0x162) + k) != 0 || *(char *)(((unsigned)&ai_CharInts + 0x165) + k) != 0) {
            gm_CharacterWages(U_I((unsigned)&ai_PlayerId), 0, (unsigned char *)a);
            k = (U_I((unsigned)&ai_PlayerId) & 0xffff) * 0x21c;
            t = (-(*(char *)((unsigned)&ai_Unk770b89 + k) != 0) & 0x28) + 0x253;
            if (*(char *)(((unsigned)&ai_CharInts + 0x162) + k) != 0)
                io_PrintText(0x2049, *(unsigned char *)(((unsigned)&ai_CharInts + 0x162) + k) + t, a[1]);
            if (*(char *)(((unsigned)&ai_CharInts + 0x165) + (U_I((unsigned)&ai_PlayerId) & 0xffff) * 0x21c) != 0)
                io_PrintText(0x2049, *(unsigned char *)(((unsigned)&ai_CharInts + 0x165) + (U_I((unsigned)&ai_PlayerId) & 0xffff) * 0x21c) + t, a[2]);
            w1f8 = w1f8 + a[1] + a[2];
            w1fc = w1fc + a[1] + a[2];
        }
        if (unk_Unk4fa4b0(U_I((unsigned)&ai_PlayerId), kind, 0) != 0) {
            i = 0;
            do {
                if (kind[i] == 0)
                    break;
                io_PrintText("%L%s %i %R%T%A", kind[i] + 0x1f64, cnt[i], sum[i]);
                t = sum[i];
                w1f8 -= t;
                w200 += t;
                w1e8 += t;
                i++;
            } while (i < 6);
        }
        if (w1ec != 0)
            io_PrintText(0x204a, w1ec);
        j = gm_Unk50a150(U_I((unsigned)&ai_PlayerId) & 0xffff, 0, lines);
        iv = 0;
        k = 0;
        if (0 < j) {
            do {
                w200 = 0;
                iv = 0;
                w1f8 = 0;
                k++;
            } while (k < j);
        }
        if (w200 != 0 && iv != 0)
            io_PrintText(0x204b, iv);
        io_PrintText(0x204c, w1f8);
        w1fc -= w200;
        if (w1fc > 0)
            io_PrintText(0x204d, w1fc);
        else if (w1fc < 0)
            io_PrintText(0x204e, w1fc);
        t = gm_GetGoodsValue((unsigned)&ai_CharsT + (U_I((unsigned)&ai_PlayerId) & 0xffff) * 0x21c);
        io_PrintText(0x2029, ((t - w1e8) - w1ec) - iv);
        he_Unk53d9f0(0xffff);
        d2_ClearSurface(U_I((unsigned)&d2_ScreenSurf));
        d2_ShowPicture(0x79d);
        U_I((unsigned)&ai_Unkfb9060) = (int)*(short *)(unsigned)&d2_ScreenW;
        U_I((unsigned)&ai_Unkfb906c) = (int)*(short *)(unsigned)&d2_ScreenH;
        U_I((unsigned)&full_data_6f25c0) = 1;
        U_I((unsigned)&ai_Unkfb9068) = 0;
        U_I((unsigned)&ai_Unkfb9064) = 0;
        d2_SetForm(win, 1);
        t = U_I((unsigned)&d2_LastWindow);
        d2_SetForm(win, 2);
        io_PrintText(0x7d);
        d2_CreateScrollButtons(-1, -1, U_I((unsigned)&d2_LastWindow), t, 0x767);
        U_I(((unsigned)&ai_Unkf8f13c + 0x248) + t * 0x3c4) = 0x18;
        sb_PlaySample(0, "RollenKurz_s_01", 1, 0x3f, 0x7f, 0, 0);
        t = sv_RunFrame((int)main_Unk539870, 0x24087);
        while (t != 0) {
            sv_Unk52fad0();
            if (U_I((unsigned)&d2_ClickedId) == 0 || *(char *)(unsigned)&kb_Last == 0x1c)
                U_I((unsigned)&sv_PopRequest) = 1;
            t = sv_RunFrame((int)main_Unk539870, 0x24087);
        }
        sv_Unk52fa90();
    }
    *(int *)(bk + 0x2c) = *(int *)(bk + 0x2c) + *(int *)(bk + 0x18) + *(int *)(bk + 0x14);
    *(int *)(bk + 0x14) = 0;
    *(int *)(bk + 0x30) = *(int *)(bk + 0x30) + *(int *)(bk + 0x1c);
    *(int *)(bk + 0x18) = 0;
    *(int *)(bk + 0x34) = *(int *)(bk + 0x34) + *(int *)(bk + 0x20);
    *(int *)(bk + 0x1c) = 0;
    *(int *)(bk + 0x38) = *(int *)(bk + 0x38) + *(int *)(bk + 0x24);
    *(int *)(bk + 0x20) = 0;
    *(int *)(bk + 0x24) = 0;
    *(int *)(bk + 0x28) = 0;
    *(int *)(bk + 0x40) = 0;
    *(int *)(bk + 0x44) = 0;
    *(int *)(bk + 0x48) = 0;
    *(int *)(bk + 0x4c) = 0;
    *(int *)(bk + 0x50) = 0;
    *(int *)(bk + 0x54) = 0;
    if (fade != 0)
        d2_ReleaseFade(fade);
    if (gm_Unk50a6c0((unsigned)&ai_CharsT + (U_I((unsigned)&ai_PlayerId) & 0xffff) * 0x21c, -(w1e8 + w1ec + iv)) != 0) {
        memset(msg, 0, 0x114);
        MSG_I(msg, 0xc) = U_I((unsigned)&ai_CharInts + (U_I((unsigned)&ai_PlayerId) & 0xffff) * 0x21c);
        MSG_B(msg, 0x30) = 1;
        MSG_B(msg, 0x26) = 0x60;
        MSG_I(msg, 0x14) = -1;
        MSG_B(msg, 8) = 9;
        MSG_I(msg, 0x34) = MSG_I(msg, 0xc);
        ai_Unk4e4060(msg);
        if (U_I((unsigned)&full_data_6f25b0) != 0)
            main_Unk539620(U_I((unsigned)&ai_PlayerId) & 0xffff);
        U_I((unsigned)&full_data_6f25b0) = 0;
    }
}
