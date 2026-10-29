/* 0x551440, fills one free 16-byte slot of the 0x5512c0 event with a person and a target (ROUGH, from the disassembly): -1 when the slots are used up (n + the two flags 0x76061c / 0x760618 (+4) >= 10), the slot is in use (byte +9) or it is before the hour 0xb; the chance (n - m) / (n + 1) against 0x5eceec picks the order of the two try values 0x20a0 / 0x402c20 for the person's search 0x4a5b50 (player 0x6f25b8, up to 100.0), a target is the person's own target (+0x170) or comes from 0x4a63f0; a random roll 0x42ec20 against 0.1 may end it (-1); else the target's building number (0x4fea50, 0xa9 bytes each) goes in +0xb, the person is sent there (message 0x1c8) and the slot gets person, target, 1 / 9 (0x29 for a character with +0x1f0), a random byte of 0x5ed140 and 0xff (0x4c52a0 (person, target, -1) starts it) */

int ai_Unk4a5b50(struct AIPER *p, unsigned int a, int *flags, float lo, float hi, unsigned short *id);
int ai_Unk4a63f0(struct AIPER *p, int a, __int64 mask, int c, float f, int *out);
float util_RandomF();

void ai_Unk4c52a0(int a, int b, int c);

int he_Unk551440(unsigned char *slot, int n, int m)
{
    int pair[2];
    int out = -1;
    float x;
    int ch, tgt, o = 0, bld, r;
    int lim = n + (U_I((unsigned)&ai_Unk76061c) != 0) + (U_I((unsigned)&ai_Unk760618) != 0 ? 4 : 0);

    pair[0] = 0x20a0;
    pair[1] = 0x402c20;
    if (lim >= 10 || slot[9] != 0 || *(unsigned short *)(unsigned)&ai_TimeHour < 0xb)
        return -1;
    x = (float)(n - m) / (float)(n + 1);
    if (x >= (0.660000026f)) {
        pair[0] = 0x402c20;
        pair[1] = 0x20a0;
    }
    ch = (unsigned)&ai_Chars + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c;
    if (ai_Unk4a5b50(ch, 1, pair, 0, 100.0f, &out) == 0 && ai_Unk4a5b50(ch, 1, pair + 1, 0, 100.0f, &out) == 0)
        return -1;
    ch = (unsigned)&ai_Chars + (unsigned short)out * 0x21c;
    r = *(int *)(ch + 0x170);
    if (r != 0) {
        tgt = *(int *)(r + 1);
    } else {
        if (AI_4A63F0_7(ch, 0, 0xa7f4c3a0, 1, 0, 100.0f, &tgt) == 0)
            return -1;
    }
    if ((float)util_RandomF() < 0.1f * 0.0f)
        return -1;
    gm_FindById(&o, 0, 0, tgt);
    if (o == 0)
        return -1;
    bld = (o - U_I((unsigned)&ai_Blds)) / 0xa9;
    cm_SendSetField(*(int *)(ch + 4), 0x1c8, 4, 0, 0x40000000);
    *(int *)slot = *(int *)(ch + 4);
    *(int *)(slot + 4) = o;
    slot[8] = 1;
    slot[9] = 9;
    if (*(int *)(ch + 0x1f0) != 0)
        slot[9] = 0x29;
    slot[10] = *(unsigned char *)((unsigned)&ai_Unk5ed140 + (unsigned short)util_Random(0x10) * 4);
    slot[11] = (unsigned char)bld;
    slot[12] = 0xff;
    ai_Unk4c52a0(*(int *)(ch + 4), tgt, -1);
    return 0;
}
