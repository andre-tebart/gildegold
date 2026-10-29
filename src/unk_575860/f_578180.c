/* 0x578180, the tutorial start (493 B, called from 0x53bb20): -3 while the gate 0x6f265c is set; the table
   0x62918c (5 ints, counted to 0x6291a0) must hold at least one 1 (else -2); the record chain built:
   0x5784a0 () into 0x6f2658, its +0x70 tail; for every i whose 0x62918c[i] is 1 the callback 0x6291a0[i] ()
   appended at the tail (its own tail walked), then 0x57b750 (); failures go to 0x578370 () and -1. Then the
   state record 0x5eec60 is reset: +0 / +4 = 1, +8 / +0xc = the clock 0x6333cc, +0x10 / +0x14 = 0,
   +0x18 = 0x6f2658, +0x1c = 0, +0x20 = -1, the form +0x24 = 0x4741f0 (0, 0, "tutorial\inline_form"
   0x629390) (0x475590 (id, 0), 0x475850 (id, 3)), +0x28 / +0x2c / +0x30 = 0, the form +0x34 =
   0x4741f0 (0, 0, "tutorial\aux_form" 0x62937c) (0x475590 (id, 0), 0x475850 (id, 3)), +0x3c / +0x40 /
   +0x54 / +0x58 / +0x5c = 0, +0x60 / +0x68 / +0x6c = -1, the gates 0x6f265c / 0x6f2660 = 1, 0 */

int ai_Unk5784a0();
int ai_Unk57b750();
void ai_Unk578370();
int sb_UnloadSampleBank();
int snd_LoadVoiceBank();
void * mem_Alloc(int size, char *tag);
void ai_Unk578440();

int ai_Unk578180(void)
{
    char *p;
    char *rec;
    char *tail;
    char *r;
    int cnt;
    int i;

    if (U_I((unsigned)&full_data_6f265c) != 0)
        return -3;
    cnt = 0;
    for (p = (char *)(unsigned)&full_data_62918c; p < (char *)(unsigned)((unsigned)full_data_62918c + 0x14); p += 4) {
        if (U_I(p) == 1)
            cnt++;
    }
    if (!cnt)
        return -2;
    rec = (char *)ai_Unk5784a0();
    *(char **)(unsigned)&full_data_6f2658 = rec;
    if (!rec)
        goto fail;
    tail = rec;
    while (*(char **)(tail + 0x70))
        tail = *(char **)(tail + 0x70);
    for (i = 0; i < 0x14; i += 4) {
        if (U_I((unsigned)&full_data_62918c + i) == 1) {
            r = (char *)((int (*)())U_I((unsigned)&full_data_6291a0 + i))();
            *(char **)(tail + 0x70) = r;
            if (!r)
                goto fail;
            tail = r;
            while (*(char **)(tail + 0x70))
                tail = *(char **)(tail + 0x70);
        }
    }
    r = (char *)ai_Unk57b750();
    *(char **)(tail + 0x70) = r;
    if (!r)
        goto fail;
    U_I(S) = 1;
    U_I(S + 4) = 1;
    U_I(S + 8) = U_I((unsigned)&ai_Unk6333cc);
    U_I(S + 0xc) = U_I((unsigned)&ai_Unk6333cc);
    U_I(S + 0x10) = 0;
    U_I(S + 0x14) = 0;
    *(char **)(S + 0x18) = *(char **)(unsigned)&full_data_6f2658;
    U_I(S + 0x1c) = 0;
    U_I(S + 0x20) = -1;
    U_I(S + 0x24) = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_629390);
    d2_SetViewEnabled(U_I(S + 0x24), 0);
    d2_PlaceMainWindow(U_I(S + 0x24), 3);
    U_I(S + 0x28) = 0;
    U_I(S + 0x2c) = 0;
    U_I(S + 0x30) = 0;
    U_I(S + 0x34) = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62937c);
    d2_SetViewEnabled(U_I(S + 0x34), 0);
    d2_PlaceMainWindow(U_I(S + 0x34), 3);
    U_I(S + 0x3c) = 0;
    U_I(S + 0x40) = 0;
    U_I(S + 0x54) = 0;
    U_I(S + 0x58) = 0;
    U_I(S + 0x5c) = 0;
    U_I(S + 0x60) = -1;
    U_I(S + 0x68) = -1;
    U_I(S + 0x6c) = -1;
    U_I((unsigned)&full_data_6f265c) = 1;
    U_I((unsigned)&full_data_6f2660) = 1;
    return 0;
fail:
    ai_Unk578370();
    return -1;
}
