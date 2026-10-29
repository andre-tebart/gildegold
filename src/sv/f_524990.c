/* 0x524990, rebuilds the member list panel of the group record g: the rows (0x20 of 10 words from 0x75f280: icon, name, frame, character index +0xc, ...) of characters that are gone or have no group are freed, the
   characters whose +0x17c group is g (types by 0x6f2368 / 0x504c00) get a row (portrait 0x5dc, name 0x5143d0, a frame 0x472aa0 / button 0x473650, and for kinds 0x10 / 0x13 / 4 two small buttons 0x5df / 0x5e0 with
   0x52eb50), the rows are enabled / checked by the character flags (+0x188 .. 0x770d08), the bars / texts per row (0x4fc8e0, "%s (%i)") and the images (0x524900 gives up to 2: 0x4731d0 / 0x473350 at x 0x38);
   with the button state 0x51d the checked rows are moved (0x524260 (g, 0, id, 0)) and 0x52ae40 runs. ROUGH */

extern int ai_Unk5fe710;  /* 0x5fe710 */
extern int ai_Unk7662c0;
extern char ai_Unk6f2368;
int io_CharacterName();                     /* 0x5143d0 */
int d2_CreateChildWindow(short x, short y, short w, short h, unsigned int flags, int win);                     /* 0x472aa0 */
int d2_WinOpenPicStyled();                     /* 0x473350 */
int d2_CreateWinText(short x, short y, char *text, int win);                     /* 0x473650 */
void d2_SetObjText();                     /* 0x4734d0 */
int gm_Unk504c00();                     /* 0x504c00 */
void snd_PlayWorkerClick(struct G16CHARCLICK *c);                     /* 0x4c2c70 */
void d2_SetObjEnabled();                     /* 0x475e40 */
void d2_AddPointerItem(int id, int amount);                     /* 0x476b90 */
float unk_Unk4fc8e0();                     /* 0x4fc8e0 */
int d2_WinOpenPic(short x, short y, int pic, int win);                     /* 0x4731d0 */

void sv_Unk524990(unsigned char *g)
{
    int i, n, id, k, row, cnt;
    int *p;
    char *c, *b, *gr;
    char buf[0x100];
    char text[0x80];
    int im[2];
    int y;

    if (sv_PlayerBarForm == -1)
        return;
    b = ai_BTypes + *g * 0x24d;
    d2_SetForm(sv_PlayerBarForm, 1);
    d2_ClearPointerItems();
    for (p = &ai_Unk75f280[3 + 1]; (int)p < ((unsigned)&ai_Unk75f280 + 0x50c); p += 10) {
        id = p[1];
        if (id != 0xffff && (*(int *)(ai_Chars[id] + 0x184) == 0 || ai_Chars[id][8] == 0)) {
            if (p[-1] != -1) {
                d2_RemoveObj(p[-1]);
                p[-1] = -1;
            }
            if (*p != -1) {
                d2_RemoveObj(*p);
                *p = -1;
            }
            d2_RemoveObj(p[-3]);
            p[-3] = -1;
            d2_RemoveObj(p[2]);
            p[2] = -1;
            d2_RemoveObj(p[-2]);
            p[-2] = -1;
            if (p[3] != -1)
                d2_RemoveWindow(p[3]);
            p[3] = -1;
            p[1] = 0xffff;
        }
    }
    for (i = 0; i < 0x300; i++) {
        c = ai_Chars[i];
        if (*(unsigned char **)(c + 0x17c) != g || *(int *)(c + 0x184) == 0 || c[8] == 0)
            continue;
        if (ai_Unk6f2368 == 0) {
            if (!((c[0x165] == b[0x22f] || c[0x165] == b[0x230]) && c[0x164] == 0))
                continue;
        } else if (ai_Unk6f2368 == 1) {
            k = gm_Unk504c00((unsigned char)c[0x165]);
            if (!(c[0x164] == 0 && (k == 10 || k == 0xb || k == 0xc)))
                continue;
        }
        for (n = 0; n < 0x20; n++)
            if (ai_Unk75f280[n * 10 + 4] == i)
                break;
        if (n < 0x20)
            continue;
        for (n = 0; n < 0x20; n++)
            if (ai_Unk75f280[n * 10 + 4] == 0xffff)
                break;
        if (n > 0x20)
            break;
        io_CharacterName(i, 1, 1, buf);
        d2_SetForm(sv_PlayerBarForm, 1);
        y = n * 0x4e;
        ai_Unk75f280[n * 10 + 1] = d2_WinOpenPic(6, y, 0x5dc, d2_LastWindow);
        ai_Unk75f280[n * 10] = d2_WinOpenPic(8, y + 0x11, *(int *)(c + 0x18c), d2_LastWindow);
        row = d2_CreateWinText(0, y + 4, ch_EmptyName, d2_LastWindow);
        ai_Unk75f280[n * 10 + 6] = row;
        *(int *)(d2_Objs + 0x5c + row * 0x2ec) = 1;
        *(short *)(d2_Objs + 0x14 + row * 0x2ec) = 0x5f;
        *(int *)(d2_Objs + 0x48 + ai_Unk75f280[n * 10] * 0x2ec) = 1;
        *(char **)(d2_Objs + 0x2e8 + ai_Unk75f280[n * 10] * 0x2ec) = c;
        if (ai_Unk75f280[n * 10 + 7] != -1) {
            d2_RemoveWindow(ai_Unk75f280[n * 10 + 7]);
            ai_Unk75f280[n * 10 + 7] = -1;
        }
        ai_Unk75f280[n * 10 + 5] = i;
        ai_Unk75f280[n * 10 + 8] = 0;
        if (b[0] == 0x10 || b[0] == 0x13 || b[0] == 4) {
            d2_SetForm(sv_PlayerBarForm, 1);
            ai_Unk75f280[n * 10 + 7] = d2_CreateChildWindow(6, y + 0x3f, 0x50, 0xf, 0, d2_LastWindow);
            d2_WinOpenPic(4, 1, 0x5df, d2_LastWindow);
            sv_Unk52eb50(0x10, 4, ai_Chars[ai_Unk75f280[n * 10 + 5]], 2, 0x4f1);
            d2_WinOpenPic(0x28, 1, 0x5e0, d2_LastWindow);
            sv_Unk52eb50(0x34, 4, ai_Chars[ai_Unk75f280[n * 10 + 5]], 3, 0x4f1);
        }
        d2_SetObjValue(ai_Unk75f280[n * 10], c[0x188] != 0);
    }
    for (p = &ai_Unk75f280[4 + 1]; (int)p < ((unsigned)&ai_Unk75f280 + 0x510); p += 10) {
        id = *p;
        if (d2_ClickedId != -1 && id != 0xffff && ai_Unk5fe710 == p[-4]) {
            if (*(int *)(d2_Objs + 0x28 + p[-4] * 0x2ec) == 0)
                ai_Chars[id][0x188] = 0;
            else {
                ai_Chars[id][0x188] = 1;
                snd_PlayWorkerClick(ai_Chars[id]);
            }
        }
        if (id != 0xffff && *(int *)(*(int *)(ai_Chars[id] + 0x184) + 0x2c) == *(int *)(g + 1)
            && (*(unsigned *)(ai_Chars[id] + 0x1c8) & 0x800) == 0 && ai_Chars[id][0x180] != 0x14)
            d2_SetObjEnabled(p[-4], 1);
        else
            d2_SetObjEnabled(p[-4], 0);
    }
    for (p = &ai_Unk75f280[2 + 1]; (int)p < ((unsigned)&ai_Unk75f280 + 0x508); p += 10) {
        id = p[2];
        if (id == 0xffff)
            continue;
        if (ai_Chars[id][0x188] == 0)
            d2_SetObjValue(p[-2], 0);
        else {
            d2_AddPointerItem(*(int *)(d2_Objs + 8 + p[-2] * 0x2ec), 1);
            d2_SetObjValue(p[-2], 1);
        }
        if (*p != -1) {
            d2_RemoveObj(*p);
            *p = -1;
        }
        if (p[1] != -1) {
            d2_RemoveObj(p[1]);
            p[1] = -1;
        }
        d2_SetForm(sv_PlayerBarForm, 1);
        if (p[3] != -1) {
            unk_Unk4fc8e0(ai_Chars[id]);
            sprintf(text, "%s (%i)", ai_Chars[id] + 0x30, (int)0);
            d2_SetObjText(p[3], text);
            *(short *)(d2_Objs + 0x14 + p[3] * 0x2ec) = 0x5f;
        }
        cnt = sv_Unk524900(ai_Chars[id], im);
        if (cnt != 0) {
            int im2 = cnt > 1 ? im[1] : -1;
            if (im[0] != -1) {
                if (*p == -1)
                    *p = d2_WinOpenPic(0x38, (p[2] * 0 + 0) + 0x10, im[0], d2_LastWindow);
            }
            if (im2 != -1) {
                if (p[1] == -1)
                    p[1] = d2_WinOpenPic(0x38, 0x28, im2, d2_LastWindow);
            }
        }
    }
    for (p = &ai_Unk75f280[4]; (int)p < ((unsigned)&ai_Unk75f280 + 0x510); p += 10) {
        if (*p != 0xffff) {
            int on = ai_Chars[*p][0x188] != 0;
            p[4] = on;
            *(int *)(d2_Objs + 0x28 + p[-4] * 0x2ec) = on;
            *(int *)(d2_Objs + 0x2c + p[-4] * 0x2ec) = on;
        }
    }
    if (d2_ClickedId == 0x51d) {
        for (p = &ai_Unk75f280[4]; (int)p < ((unsigned)&ai_Unk75f280 + 0x510); p += 10)
            if (p[4] != 0)
                sv_MoveToGroup(g, 0, *p, 0);
        sv_Unk52ae40();
    }
    if (ai_Unk7662c0 != 0 && *(char *)(ai_Unk7662c0 + 0x188) == 0)
        ai_Unk7662c0 = 0;
}
