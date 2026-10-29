/* 0x57de60, the tutorial training window (631 B, the 0x57dc50 sibling): the skill points 0x4ec3d0 (record) and
   their tenth 0x4ec3d0 / 10; the run record 0x4eaa50 (record), 0 for none -> 0; the factor 0x4eab20 (record, 3)
   (a float); the window "Locations_Domizil_skill" (0x62a2c4) with 0x476f80 (0, 0) / 0x475600, the sound
   0x52ea30 (0x6ed938), the text 0x403a (0x182, the byte 0x6f1bc5) into the entry 0x475b80 (window, 1, text),
   the number 0x5085b0 (tenth, 0x6f1bc5) / (points) / (tenth) into 0x475cb0 (entry), the texts 0x403b / +1 into
   the entries 0x475b80 (window, 2, text), 0xf8a668 = -1; the pump loop 0x5309a0 (this handler, 0x677c7):
   0x4eaa50 (record) 0 -> 0x6f22a0 = 1, the pump 0x20007, 0x4758f0 (window), 0; the click 0x14673a0 ->
   0x6f22a0 = 1; the window 0x5fe710 == the first 0x403b entry -> the field text 0x475980 (the 0x403a entry,
   0x6f1bc5) parsed 0x5085f0 and checked 0x521040 (the character 0x770b80 + player 0x6f25b8 * 0x21c, value,
   0x6f1bc5): non-zero -> 0x4ecd50 (record, value / points * the factor * 0.75), the result 1, 0x6f22a0 = 1 and
   0x4c47e0 (record +4, the character 0x770b84 + player * 0x21c, 0x6f1bc5, value); the second entry -> 0x6f22a0
   = 1; the pump 0 -> 0x4758f0 (window), the result */

int ahm_ObjectValue();
char * ahm_OwnerChar();
float ahm_Share();
int d2_GetObjValue();
int gm_ConvertMoney(int a, unsigned char k);
int gm_PlayerFactor(int a, unsigned char k);
int sv_CheckGoodAmount();
int ahm_SendAddValue();
void cm_SendMoveGoods(int a, int b, unsigned char c, int d);

int ai_Unk57de60(char *rec)
{
    char *rr;
    char *win;
    int sp;
    int q;
    float f;
    int fld;
    int b1;
    int b2;
    int v;
    int done;

    done = 0;
    sp = ahm_ObjectValue(rec);
    q = sp / 10;
    rr = (char *)ahm_OwnerChar(rec);
    if (!rr)
        return 0;
    f = ahm_Share(rr, 3);
    win = (char *)d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62a2c4);
    d2_SetPointerSprite(0, 0);
    d2_CentreViewWindows(win);
    d2_SetForm(win, 0);
    sv_FitForm(U_I((unsigned)&d2_LastWindow));
    d2_SetForm(win, 1);
    v = io_PrintText(0x403a, 0x182, *(unsigned char *)(unsigned)&ai_OwnPlayer);
    fld = d2_GetWinObj(win, 1, v);
    v = gm_ConvertMoney(q, *(unsigned char *)(unsigned)&ai_OwnPlayer);
    v = ((int (*)(int, int, int))gm_ConvertMoney)(sp, *(unsigned char *)(unsigned)&ai_OwnPlayer, v);
    v = ((int (*)(int, int, int))gm_ConvertMoney)(q, *(unsigned char *)(unsigned)&ai_OwnPlayer, v);
    d2_SetObjValue(fld, v);
    d2_SetForm(win, 2);
    v = io_PrintText(0x403b);
    b1 = d2_GetWinObj(win, 2, v);
    b2 = d2_GetWinObj(win, 2, v + 1);
    U_I((unsigned)&d2_ClickedId) = -1;
    for (;;) {
        rr = (char *)ahm_OwnerChar(rec);
        if (!rr) {
            U_I((unsigned)&sv_PopRequest) = 1;
            sv_RunFrame(ai_Unk57de60, 0x20007);
            d2_CloseView(win);
            return 0;
        }
        if (U_I(((unsigned)&dm_L + 0x20)) != 0) {
            U_I((unsigned)&sv_PopRequest) = 1;
        } else if (U_I((unsigned)&d2_ClickedId) != -1) {
            if (U_I((unsigned)&ai_Unk5fe710) == b1) {
                v = ((int (*)(int))gm_PlayerFactor)(d2_GetObjValue(fld, *(unsigned char *)(unsigned)&ai_OwnPlayer));
                if (sv_CheckGoodAmount((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, v, *(unsigned char *)(unsigned)&ai_OwnPlayer) != 0) {
                    ahm_SendAddValue(rr, ((float)v / (float)sp) * f * 0.75f);
                    done = 1;
                    U_I((unsigned)&sv_PopRequest) = 1;
                    cm_SendMoveGoods(*(int *)(rr + 4), *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c), *(unsigned char *)(unsigned)&ai_OwnPlayer, v);
                }
            } else if (U_I((unsigned)&ai_Unk5fe710) == b2) {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        if (sv_RunFrame(ai_Unk57de60, 0x677c7) == 0) {
            d2_CloseView(win);
            return done;
        }
    }
}
