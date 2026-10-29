/* 0x57fdd0, the shot animation (587 B): rec, sc = the scene struct, run = the tutorial step, sel = the object
   (0 .. 2), mask = the pump mask; an unchanged selection (sc + 0x28) or out of range answers at once; two
   90-tick phases, each pumping 0x5309a0 (this function, mask) and calling 0x57f890 (rec, sc, run): the camera
   sc + 0x26 pushed back by the object's direction sc + 0x24 (0x454560) * 100 * the phase (the clock - the
   start) / 90 and the wimpel sc + 0x27 to -70 / -50 / 100 * the phase + 120 (0x43b790), the second phase with
   1 - the time, the clock 0x6333cc reset after each; then the three objects sc + 0x8c / 0x90 / 0x94 switched
   off (0x471b0) and the selected one on, sc + 0x28 = the selection, sc + 0x1b = the clock */


void ai_Unk57fdd0(char *rec, float *sc, char *run, float sel, int mask)
{
    float v[3];
    float w[3];
    float f;
    float start;
    int i;
    int h;

    start = (float)U_I((unsigned)&ai_Unk6333cc);
    if (sel != sc[0x28] && (int)sel <= (int)sc[0x37]) {
        while ((h = sv_RunFrame(ai_Unk57fdd0, mask)) != 0) {
            ai_Unk57f890(rec, (char *)sc, run);
            if ((float)(unsigned int)(U_I((unsigned)&ai_Unk6333cc) - (int)start) <= 90.0f) {
                v[0] = sc[9];
                v[2] = sc[0xb];
                f = (float)(unsigned int)(U_I((unsigned)&ai_Unk6333cc) - (int)start) / 90.0f;
                v[1] = 0;
                d3_Normalize((char *)v);
                w[0] = -70.0f;
                w[1] = -50.0f;
                w[2] = f * 100.0f + 120.0f;
                v[0] = *sc - v[0] * (f * 100.0f);
                v[1] = sc[1] - (f * 100.0f) * v[1];
                v[2] = sc[2] - (f * 100.0f) * v[2];
                d3_SetObjectPos(*(int *)&sc[0x26], (char *)v);
                d3_SetObjectPos(*(int *)&sc[0x27], (char *)w);
            } else {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        for (i = 0; i < 3; i++)
            d3if_Show(*(struct D3OBJECT **)&sc[0x23 + i], 0);
        d3if_Show(*(struct D3OBJECT **)&sc[0x23 + (int)sel], 1);
        sc[0x28] = sel;
        start = (float)U_I((unsigned)&ai_Unk6333cc);
        while ((h = sv_RunFrame(ai_Unk57fdd0, mask)) != 0) {
            ai_Unk57f890(rec, (char *)sc, run);
            if ((float)(unsigned int)(U_I((unsigned)&ai_Unk6333cc) - (int)start) <= 90.0f) {
                v[0] = sc[9];
                v[2] = sc[0xb];
                v[1] = 0;
                f = 1.0f - (float)(unsigned int)(U_I((unsigned)&ai_Unk6333cc) - (int)start) / 90.0f;
                d3_Normalize((char *)v);
                w[0] = -70.0f;
                w[1] = -50.0f;
                w[2] = f * 100.0f + 120.0f;
                v[0] = *sc - v[0] * (f * 100.0f);
                v[1] = sc[1] - (f * 100.0f) * v[1];
                v[2] = sc[2] - (f * 100.0f) * v[2];
                d3_SetObjectPos(*(int *)&sc[0x26], (char *)v);
                d3_SetObjectPos(*(int *)&sc[0x27], (char *)w);
            } else {
                U_I((unsigned)&sv_PopRequest) = 1;
            }
        }
        *(int *)((char *)sc + 0x6c) = U_I((unsigned)&ai_Unk6333cc);
    }
}
