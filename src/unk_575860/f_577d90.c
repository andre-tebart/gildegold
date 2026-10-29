/* 0x577d90, the tutorial progress slider (179 B, the state 0xa only): the state record, else 0x577d60 (the form
   released); the progress form created when S+0x68 is -1 (0x577cd0); the slider S+0x6c set by 0x475cb0 (id, 0,
   400, value) with value = (total - min(total, the clock 0x6333cc - S+0xc)) / total * 400.0f */

#define TDIFF (U_I((unsigned)&ai_Unk6333cc) - *(int *)(st + 0xc))
#define MINU(a, b) ((unsigned)(a) < (unsigned)(b) ? (a) : (b))

void ai_Unk577d90(void)
{
    char *st;
    char *r;
    int total;
    int t;

    st = S;
    r = *(char **)(st + 0x18);
    if (!r || U_I(st) != 0xa) {
        ai_Unk577d60();
        return;
    }
    if (U_I(st + 0x68) == -1) {
        ai_Unk577cd0();
        st = S;
    }
    total = *(int *)(*(char **)(st + 0x18) + 0x44);
    t = MINU(TDIFF ? TDIFF : 0, total);
    d2_SetObjValue(U_I(st + 0x6c), 0, 0x190, (int)((float)(total - t) / (float)total * 400.0f));
}
