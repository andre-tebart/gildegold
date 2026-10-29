/* 0x576040, one step of the campaign state machine (763 B): `a` is the expected index, `b` and `c` the event
   kind / result, `d` is only handed to the callback. The record 0x5eec60: +0 the state, +4 the pending state,
   +0xc the time of the step (0x6333cc), +0x10 the step counter, +0x18 the run record; the run record holds the
   callback +0x38 (called with the four arguments as an int[4]), +0x3c the mode (1 / 3), +0x40 the expected index,
   +0x44 / +0x48 the transition times, +0x4c an index, +0x50 a stored value. Every path ends with the step
   counter +0x10 incremented; the failed gates at the top return without it. */

void ai_Unk576040(int a, int b, int c, int d)
{
    int p[4];
    int (*cb)(int *);
    char *r;

    if (!U_I((unsigned)&full_data_6f265c))
        return;
    if (!*(int *)(S + 0x18))
        return;
    p[0] = a;
    p[1] = b;
    p[2] = c;
    p[3] = d;
    r = *(char **)(S + 0x18);
    if (*(int *)(r + 8) == 2)
        goto st2;
    if (U_I(S + 4) != 4 && U_I(S + 4) != 0xa)
        return;
    /* 0x5760a4 */
    cb = *(int (**)(int *))(r + 0x38);
    if (cb) {
        cb(p);
        U_I(S + 0x10)++;
        return;
    }
    if (*(int *)(r + 0x40) == 3) {
        if (U_I((unsigned)&ai_Unk6333cc) > *(int *)(r + 0x44) + U_I(S + 0xc)) {
            U_I(S) = *(int *)(r + 0x48);
            r = *(char **)(S + 0x18);
            U_I(S + 4) = *(int *)(r + 0x48);
            U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
            U_I(S + 0x2c) = 1;
        }
        return;
    }
    if (*(int *)(r + 0x40) != a)
        goto inc;
    if (c == 0) {
        U_I(S) = *(int *)(r + 0x48);
        U_I(S + 4) = U_I(S);
    } else {
        U_I(S) = 0xb;
        U_I(S + 4) = U_I(S);
    }
    goto clocked;

st2:
    if (U_I(S + 4) != 4 && U_I(S + 4) != 9 && U_I(S + 4) != 0xa)
        return;
    cb = *(int (**)(int *))(r + 0x38);
    if (cb) {
        cb(p);
        return;
    }
    if (U_I(S + 0x3c) == 1) {
        if (c != 0) {
            if (c != 1 || *(int *)(r + 0x40) != a)
                goto apply;
            U_I(S + 4) = 4;
            U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
            U_I(S + 0x30) = 0;
            ai_Unk577c60();
            ai_Unk57b8a0(2);
            goto apply;
        }
        if (*(int *)(r + 0x40) != a)
            goto apply;
        U_I(S + 4) = 5;
        U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
        if (b)
            U_I(S + 0x50) = b;
        goto apply;
    }
    if (U_I(S + 0x3c) != 3)
        goto inc;
    if (U_I(S + 4) == 9) {
        if (*(int *)(r + 0x4c) && *(int *)(r + 0x4c) == a) {
            U_I(S) = 0xa;
            U_I(S + 4) = 0xa;
            U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
            U_I(S + 0x10)++;
            return;
        }
        if (*(int *)(r + 0x40) != a || c != 1)
            goto inc;
        U_I(S) = 4;
        U_I(S + 4) = 4;
        U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
        U_I(S + 0x30) = 0;
        ai_Unk577c60();
        ai_Unk57b8a0(2);
        U_I(S + 0x10)++;
        return;
    }
    if (U_I(S + 4) == 0xa) {
        if (*(int *)(r + 0x40) != a || c != 1)
            goto inc;
        U_I(S) = 0xb;
        U_I(S + 4) = 0xb;
        goto clocked;
    }
    if (U_I(S + 4) == 4) {
        if (*(int *)(r + 0x40) != a || c != 0)
            goto inc;
        U_I(S) = 9;
        U_I(S + 4) = 9;
        goto clocked;
    }
    goto inc;

apply:
    U_I(S) = U_I(S + 4);
    U_I(S + 0x10)++;
    return;
clocked:
    U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
    U_I(S + 0x10)++;
    return;
inc:
    U_I(S + 0x10)++;
}
