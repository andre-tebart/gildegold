/* 0x4a94e0, decide callback: p (unk194 > 2, not flagged 0x400) picks a character within 18..49 units, not flagged
   0x400, by chance after 0x504e20, into m (kind 7) */

int ai_Unk4a94e0(int a, AIPER *p, int c, int d, unsigned char *m)
{
    int flags;
    unsigned short id;

    if (p->unk194 >= 3 && (p->flags1c8 & 0x400) == 0) {
        flags = ((unsigned)&ai_Unka69140 + 0x4973c0);
        if (ai_Unk4a58d0(p, 1, &flags, 0, (float)util_Random(0x20) + 18.0f, &id) != 0
            && (((AIPER *)ai_Chars)[id].flags1c8 & 0x400) == 0
            && util_RandomF() + gm_Mix(p, &((AIPER *)ai_Chars)[id], 3) >= 1.0f) {
            m[0] = 7;
            *(int *)(m + 4) = ((AIPER *)ai_Chars)[id].netId;
            return 1;
        }
    }
    return 0;
}
