/* 0x4edf80, the per-frame update of the effect slots: the time step dt (0x4fe280 between the old time 0x6f0e38 .. and the clock 0x770ae0, * 1/60 * 0.1) is added to the three floats +8 / +0xc / +0x10 (up to 1.0), and each
   of the 3 parts of a used slot (state at +0x74 + 0x68 * j: 0 idle, 1 running, 2 / 3 done) is advanced: running parts create / move their "kugel" object (0x446e10 / 0x43b790), finished ones release the object,
   report to the slot's counter (+0x16c) and callback (+0x168); a slot whose parts are all idle (3) loses flags 1 / 2 and is released (0x4edf10) when flag 4 is set. ROUGH */

void unk_Unk4edf80(void)
{
    int dt;
    int sc;
    float step;
    int off, idx;
    int id;
    char *e;
    int *st;
    int j, off16, idle;
    int k;
    unsigned fl;
    float a, b, c;

    dt = gm_MinutesBetween(ai_Unk6f0e38, ai_Time);
    sc = d3_CurrentScene;
    if (dt < 0)
        dt = 0;
    *(int *)ai_Unk6f0e38 = *(int *)ai_Time;
    *(int *)ai_Unk6f0e3c = *(int *)(ai_Time + 4);
    step = (float)dt * 0.016666668f * 0.1f;
    *(int *)ai_Unk6f0e40 = *(int *)(ai_Time + 8);
    *(int *)ai_Unk6f0e44 = *(int *)(ai_Time + 12);
    d3if_SelectScene(0, 1);
    off = 0;
    idx = 0;
    do {
        e = gv_Effects + off;
        id = *(int *)e;
        if (id != -1) {
            a = step + *(float *)(e + 8);
            idle = 0;
            *(float *)(e + 8) = a;
            b = step + *(float *)(e + 0xc);
            *(float *)(e + 0xc) = b;
            c = step + *(float *)(e + 0x10);
            *(float *)(e + 0x10) = c;
            if (1.0f <= a)
                a = 1.0f;
            *(float *)(e + 8) = a;
            if (1.0f <= b)
                b = 1.0f;
            *(float *)(e + 0xc) = b;
            if (1.0f <= c)
                c = 1.0f;
            *(float *)(e + 0x10) = c;
            if ((*(unsigned *)(e + 4) & 2) == 0) {
                if ((*(unsigned *)(e + 4) & 4) != 0)
                    unk_Unk4edf10(id);
            } else {
                j = 0;
                off16 = 0;
                st = (int *)(e + 0x74);
                do {
                    if (*st == 0) {
                        idle++;
                        if (idle > 2) {
                            fl = *(unsigned *)(e + 4);
                            *(unsigned *)(e + 4) = fl & 0xfffffffc;
                            if ((fl & 4) != 0)
                                unk_Unk4edf10(*(int *)e);
                        }
                    } else if (*st == 1) {
                        if (*(int *)(e + 0x14) == 0)
                            k = ai_StepShot();
                        else
                            k = ai_StepShotSlow(st - 0x17);
                        if (k != 0) {
                            k = idx + j;
                            if (*(int *)(gv_Effects + 0x15c + k * 4) == 0) {
                                *(int *)(gv_Effects + 0x15c + k * 4) = d3_CreateObject(0, ai_Unk5ebba4, ai_Unk5ebbb4, "kugel");
                                *(unsigned char *)(*(int *)(gv_Effects + 0x15c + k * 4) + 0x211) |= 4;
                                *(unsigned char *)(*(int *)(gv_Effects + 0x15c + k * 4) + 0x212) |= 0xc;
                                *(unsigned char *)(*(int *)(gv_Effects + 0x15c + k * 4) + 0x211) &= 0xf7;
                            }
                            d3_SetObjectPos(*(int *)(gv_Effects + 0x15c + k * 4), st - 0xb);
                        }
                        if (*st == 3) {
                            k = idx + j;
                            if (*(int *)(gv_Effects + 0x15c + k * 4) != 0) {
                                d3if_Delete((struct D3OBJECT *)*(int *)(gv_Effects + 0x15c + k * 4));
                                *(int *)(gv_Effects + 0x15c + k * 4) = 0;
                            }
                            if (*(int **)(e + 0x16c) != 0)
                                **(int **)(e + 0x16c) = j;
                            *(int *)(off16 + 8 + *(int *)(e + 0x16c)) = -1;
                            if (*(void (**)())(e + 0x168) != 0)
                                (**(void (**)())(e + 0x168))(*(int *)(e + 0x16c));
                            *st = 0;
                            *(int *)(gv_Effects + 0x150 + k * 4) = 0;
                        } else if (*st == 2) {
                            k = idx + j;
                            idle++;
                            if (*(int *)(gv_Effects + 0x15c + k * 4) != 0) {
                                d3if_Delete((struct D3OBJECT *)*(int *)(gv_Effects + 0x15c + k * 4));
                                *(int *)(gv_Effects + 0x15c + k * 4) = 0;
                            }
                            unk_Unk4ee360(*(unsigned *)(e + 4) & 1, *(int *)e, (char *)(st - 0x17), *(UNKHITS **)(e + 0x16c));
                            if (*(int **)(e + 0x16c) != 0)
                                **(int **)(e + 0x16c) = j;
                            if (*(void (**)())(e + 0x168) != 0)
                                (**(void (**)())(e + 0x168))(*(int *)(e + 0x16c));
                            *st = 0;
                            *(int *)(gv_Effects + 0x150 + k * 4) = 0;
                            **(int **)(e + 0x16c) = -1;
                        }
                    }
                    st += 0x1a;
                    j++;
                    off16 += 0x10;
                } while (off16 < 0x30);
                if (idle > 2) {
                    fl = *(unsigned *)(e + 4);
                    *(unsigned *)(e + 4) = fl & 0xfffffffc;
                    if ((fl & 4) != 0)
                        unk_Unk4edf10(*(int *)e);
                }
            }
        }
        idx += 0x5c;
        off += 0x170;
    } while (idx < 0x2e0);
    d3if_SelectScene(sc, 1);
}
