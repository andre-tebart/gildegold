/* 0x57f890, the cannon control panel (383 B, ROUGH: the calls and the window-table stores are written; the three
   float -> int conversions (0x5de518 = __ftol) lost their arguments in the pseudocode): rec, sc = the scene
   struct, run = the tutorial step. The two fields sc + 0xb0 / 0xb4 read by 0x475980; the max 0x4eef50 (a, b, 1);
   the selection 0x4ef170 (rec, &sc + 0xdc, &sc + 0xe0), sc + 0xb0 set to (sc + 0xe0, MIN (the value, a)); the
   controls sc + 0xcc / 0xd0 enabled by the selection > 0 / > 1; the selected object (sc + 0xa0) checked in the
   window table 0xf8f080 (+0x2c and +0x28 = 1 for the selected id sc + 0xc8 / 0xcc / 0xd0, 0 for the others);
   0x4ef090 (the character 0x770b80 + 0x6f25b8 * 0x21c, the two values, sc + 0xa0 + 1, &l1, &l2); the fire control
   sc + 0xb8 enabled by the zoom sc + 0x20 = 0.01, 0x4ef090 and the run's cost sc + 8 + sc + 0xa0 * 4 against
   0x4eef50 and the run's flag +4 & 2; sc + 0xbc enabled by the run's flag +4 bit 1 inverted; the three bars
   sc + 0xa8 / 0xac / 0xc4 (0x475cb0, 0 .. 100, 0x5dbc20 / 0x57fdb0 / 1 through 0x5de518) and sc + 0xc0 (2);
   0x515960 (0x602568); the four windows 0x473650 (0x4144a360 / 0x4144a364 texts, the "__T__a" 0x62a364 format
   through 0x5176d0 and 0x509ee0, the screen width 0xf8f128 + 0x6ed938 * 0x3c4 / 2) placed with +0x5c = 1 and
   +0x14 = the width / 2, 0x4760b0 (0x54); the globals 0x925114 / 18 / 1c / 08 / 0c / 10 updated */

float unk_Unk4eef50(); void unk_Unk4ef170(char *o, unsigned *a, unsigned *b); int unk_Unk4ef090(); float ai_GetDefaultEffectScale();
int _ftol(); float ai_Unk57fdb0(); int d2_CreateWinText(); void d2_SetObjFont();  /* @original: the call at 4110 passes 1 argument to d2.h's 4-parameter prototype */
void txt_Format(char *out, int id0, ...); int gm_Unk509ee0(char *c, char p);

void ai_Unk57f890(char *rec, char *sc, char *run)
{
    char buf[0x40];
    int a;
    int b;
    int c;
    int m;
    int n;
    int l1;
    int l2;
    int d;
    int w;

    a = d2_GetObjValue(*(int *)(sc + 0xb0));
    b = d2_GetObjValue(*(int *)(sc + 0xb4));
    c = (int)unk_Unk4eef50(a, b, 1);
    unk_Unk4ef170(rec, (int *)(sc + 0xdc), (int *)(sc + 0xe0));
    n = *(int *)(sc + 0xe0);
    if (n < a)
        a = n;
    d2_SetObjValue(*(int *)(sc + 0xb0), 0, n, a);
    d2_SetObjEnabled(*(int *)(sc + 0xcc), 0 < *(int *)(sc + 0xdc));
    d2_SetObjEnabled(*(int *)(sc + 0xd0), 1 < *(int *)(sc + 0xdc));
    *(int *)(U_I((unsigned)&d2_Objs) + 0x2c + *(int *)(sc + 0xc8) * 0x2ec) = *(int *)(sc + 0xa0) == 0;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x2c + *(int *)(sc + 0xcc) * 0x2ec) = *(int *)(sc + 0xa0) == 1;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x2c + *(int *)(sc + 0xd0) * 0x2ec) = *(int *)(sc + 0xa0) == 2;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x28 + *(int *)(sc + 0xc8) * 0x2ec) = *(int *)(sc + 0xa0) == 0;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x28 + *(int *)(sc + 0xcc) * 0x2ec) = *(int *)(sc + 0xa0) == 1;
    *(int *)(U_I((unsigned)&d2_Objs) + 0x28 + *(int *)(sc + 0xd0) * 0x2ec) = *(int *)(sc + 0xa0) == 2;
    m = unk_Unk4ef090((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, a, b, *(int *)(sc + 0xa0) + 1, &l1, &l2);
    if (*(float *)(sc + 0x20) > 0.01f || m == 0 || *(float *)(run + 8 + *(int *)(sc + 0xa0) * 4) < (float)c || (*(unsigned char *)(run + 4) & 2) != 0)
        w = 0;
    else
        w = 1;
    d2_SetObjEnabled(*(int *)(sc + 0xb8), w);
    d2_SetObjEnabled(*(int *)(sc + 0xbc), (~*(unsigned int *)(run + 4) >> 1) & 1);
    d2_SetObjValue(*(int *)(sc + 0xa8), 0, 100, (int)ai_GetDefaultEffectScale(1));
    ai_Unk57fdb0(sc, 1);
    d2_SetObjValue(*(int *)(sc + 0xac), 0, 100, (int)_ftol());
    d2_SetObjValue(*(int *)(sc + 0xc4), 0, 100, (int)_ftol());
    d2_SetForm(*(int *)(sc + 0xc0), 2);
    io_PrintText((char *)(unsigned)"$C");
    m = d2_CreateWinText(0, 0xf, (char *)U_I((unsigned)&full_data_144a360), U_I((unsigned)&d2_LastWindow));
    *(int *)(U_I((unsigned)&d2_Objs) + 0x5c + m * 0x2ec) = 1;
    *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + m * 0x2ec) = *(short *)((unsigned)&ai_Unkf8f128 + U_I((unsigned)&d2_LastWindow) * 0x3c4) / 2;
    d2_SetObjFont(m, 0x54);
    txt_Format(buf, (char *)(unsigned)&full_data_62a364, l2, l1);
    m = d2_CreateWinText(0, 0x23, buf, U_I((unsigned)&d2_LastWindow));
    *(int *)(U_I((unsigned)&d2_Objs) + 0x5c + m * 0x2ec) = 1;
    *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + m * 0x2ec) = *(short *)((unsigned)&ai_Unkf8f128 + U_I((unsigned)&d2_LastWindow) * 0x3c4) / 2;
    d2_SetObjFont(m, 0x54);
    m = d2_CreateWinText(*(short *)((unsigned)&ai_Unkf8f128 + U_I((unsigned)&d2_LastWindow) * 0x3c4) / 2, 0xf, (char *)U_I((unsigned)&full_data_144a364), U_I((unsigned)&d2_LastWindow));
    *(int *)(U_I((unsigned)&d2_Objs) + 0x5c + m * 0x2ec) = 1;
    *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + m * 0x2ec) = *(short *)((unsigned)&ai_Unkf8f128 + U_I((unsigned)&d2_LastWindow) * 0x3c4) / 2;
    d2_SetObjFont(m, 0x54);
    d = gm_Unk509ee0((char *)(unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c, *(unsigned char *)(unsigned)&ai_OwnPlayer);
    txt_Format(buf, (char *)(unsigned)&full_data_62a364, d, *(int *)(((unsigned)&ai_Selected + 0xc) + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c));
    m = d2_CreateWinText(*(short *)((unsigned)&ai_Unkf8f128 + U_I((unsigned)&d2_LastWindow) * 0x3c4) / 2, 0x23, buf, U_I((unsigned)&d2_LastWindow));
    *(int *)(U_I((unsigned)&d2_Objs) + 0x5c + m * 0x2ec) = 1;
    *(short *)(U_I((unsigned)&d2_Objs) + 0x14 + m * 0x2ec) = *(short *)((unsigned)&ai_Unkf8f128 + U_I((unsigned)&d2_LastWindow) * 0x3c4) / 2;
    d2_SetObjFont(m, 0x54);
    U_I((unsigned)&ai_Unk925114) = a;
    U_I((unsigned)&ai_Unk925118) = b;
    U_I((unsigned)&ai_Unk92511c) = *(int *)(sc + 0xa0);
    U_I((unsigned)&ai_Unk925108) = *(int *)(sc + 0x10);
    U_I(((unsigned)&ai_Unk925108 + 0x4)) = *(int *)(sc + 0x14);
    U_I(((unsigned)&ai_Unk925108 + 0x8)) = *(int *)(sc + 0x18);
}
