/* 0x57b8a0, the tutorial per-frame update (910 B, ROUGH from the disassembly): - nothing without the gate
   0x6f265c, the step table r+0x64 (r = S+0x18) or its count r+0x5c < 1. The step callback r+0x60 called with
   `arg` answers v (edi): -1 and below -> 2, else a step index 0 .. r+0x5c-1 that differs from S+0x54 switches to
   the phase S+0x58 = 2, S+0x5c = the clock 0x6333cc, S+0x54 = v. Then S+0x60 (the position object, -1 = none) is
   0x472f50 (S+0x60) and 0x475aa0 (S+0x60, 1 when S+0x64 == 0x6f229c, else 0). By v: 0 -> S+0x64 = 0x6f229c,
   S+0x5c = clock, S+0x58 = 2, the object created by 0x46cd80 ((int) e[0], (int) e[1], 0x1d) when S+0x60 is -1
   (e = the step entry, 0x2c B) and 0x46cda0 ((int) e[0], (int) e[1], S+0x60); 1 -> the phases S+0x58:
   2 (wait: the clock past S+0x5c + e+0x20 -> 3, S+0x5c = clock), 3 (the Bezier 0x57b810 (e+0, e+8, e+0x10,
   e+0x18, t = (clock - S+0x5c) / e+0x24, the local vector) into 0x46cda0 (x, y, S+0x60) and return), 4 (the clock
   past S+0x5c + e+0x28 -> 2, then 0x46cda0 ((int) e[0], (int) e[1], S+0x60)); 2 -> 0x46e710 (S+0x60), S+0x60 = -1,
   S+0x58 = 1 */

void d2_RaiseObject();
void d2_EnableObject();
int d2_CreatePicObj();
struct G5CH; struct G9REC; struct VFSFILE; void d2_MoveObj(short x, short y, int obj);
void d2_RemoveObj();

void ai_Unk57b8a0(int arg)
{
    char *st;
    char *r;
    char *e;
    float v[2];
    int x;
    int y;

    if (!U_I((unsigned)&full_data_6f265c))
        return;
    st = S;
    r = *(char **)(st + 0x18);
    if (!*(int *)(r + 0x64))
        return;
    if (*(int *)(r + 0x5c) < 1)
        return;
    x = 0;
    if (*(int *)(r + 0x60)) {
        x = ((int (*)()) * (int *)(r + 0x60))(arg);
        st = S;
        if (x <= -1) {
            x = 2;
        } else if (x >= 0 && x < *(int *)(*(char **)(st + 0x18) + 0x5c) && x != U_I(st + 0x54)) {
            U_I(st + 0x58) = 2;
            U_I(st + 0x5c) = U_I((unsigned)&ai_Unk6333cc);
            U_I(st + 0x54) = x;
            st = S;
        }
    }
    if (U_I(st + 0x60) != -1) {
        d2_RaiseObject(U_I(st + 0x60));
        if (U_I(S + 0x64) == U_I((unsigned)&sv_FrameDepth))
            d2_EnableObject(U_I(S + 0x60), 1);
        else
            d2_EnableObject(U_I(S + 0x60), 0);
        st = S;
    }
    e = (char *)(*(int *)(*(char **)(st + 0x18) + 0x64) + U_I(st + 0x54) * 0x2c);
    if (x == 0) {
        U_I(st + 0x64) = U_I((unsigned)&sv_FrameDepth);
        U_I(st + 0x5c) = U_I((unsigned)&ai_Unk6333cc);
        U_I(st + 0x58) = 2;
        if (U_I(S + 0x60) == -1)
            U_I(S + 0x60) = d2_CreatePicObj((int)*(float *)e, (int)*(float *)(e + 4), 0x1d);
        d2_MoveObj((int)*(float *)e, (int)*(float *)(e + 4), U_I(S + 0x60));
        return;
    }
    if (x == 1) {
        if (U_I(st + 0x58) == 2) {
            if (!((float)U_I((unsigned)&ai_Unk6333cc) > (float)U_I(st + 0x5c) + *(float *)(e + 0x20)))
                return;
            U_I(st + 0x58) = 3;
            U_I(st + 0x5c) = U_I((unsigned)&ai_Unk6333cc);
            st = S;
        }
        if (U_I(st + 0x58) == 3) {
            ai_Unk57b810((float *)e, (float *)(e + 8), (float *)(e + 0x10), (float *)(e + 0x18),
                         ((float)U_I((unsigned)&ai_Unk6333cc) - (float)U_I(st + 0x5c)) / *(float *)(e + 0x24), v);
            d2_MoveObj((int)v[0], (int)v[1], U_I(S + 0x60));
            return;
        }
        if (U_I(st + 0x58) == 4) {
            if (!((float)U_I((unsigned)&ai_Unk6333cc) > (float)U_I(st + 0x5c) + *(float *)(e + 0x28)))
                return;
            U_I(st + 0x58) = 2;
            U_I(st + 0x5c) = U_I((unsigned)&ai_Unk6333cc);
            d2_MoveObj((int)*(float *)e, (int)*(float *)(e + 4), U_I(S + 0x60));
        }
        return;
    }
    if (x == 2) {
        if (U_I(S + 0x60) != -1)
            d2_RemoveObj(U_I(S + 0x60));
        U_I(S + 0x60) = -1;
        U_I(S + 0x58) = 1;
    }
}
