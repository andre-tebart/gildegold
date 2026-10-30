/* 0x4ad7a0, at the start of a game: counts the members of the three councils (each other character counts 0.33,
   a new one 0.67 more) and creates new characters (0x4c4650 type 3) until each has two, with age 7, title
   hmt_Titles[i], money (1 + rand) * 0.5 * 8000000 and the council bits
   @deep B: 16 code hunks read: all are the same operation spelled differently, no meaning error. The council loop (for i = 2; i >= 0) exits on lea eax,[edi-2]/test/jge here against lea eax,[edi-1]/test/jne in the original (edi is i+2: the original tests i != -1), with sub ecx,4 and dec edi swapped; the wait loop while (!cm_GetSlotState(h)) sv_PumpNet() is rotated (ours tests after the call at 4ad989, the original jumps back to a top test at 4ad977); the money expression multiplies fmul 0x5eb7ac before fimul hmt_Unk5ee114 in the original, after it here; two xor edi,edi / xor ebx,ebx stores swapped. Q1 name and Q2 (ai_Unk5... callees, HMT_ macros) still open */

void hmt_Unk4ad7a0(void)
{
    int age;
    int title;
    float cnt[3];
    int last[3];
    int titles[3];
    int ids[15];
    int i, j, k, n, id, h, money;
    unsigned int bits;
    AICHAR *p;

    ids[0] = 1;
    ids[1] = 2;
    ids[2] = 3;
    ids[3] = 4;
    ids[4] = 5;
    ids[5] = 6;
    ids[6] = 7;
    ids[7] = 8;
    ids[8] = 9;
    ids[9] = 10;
    ids[10] = 11;
    ids[11] = 12;
    ids[12] = 13;
    ids[13] = 14;
    ids[14] = 15;
    cnt[0] = 0;
    cnt[1] = 0;
    cnt[2] = 0;
    last[0] = -1;
    last[1] = -1;
    last[2] = -1;
    titles[0] = 15;
    titles[1] = 0x15;
    titles[2] = 0x1b;
    for (j = 0; j < 768; j++) {
        p = &ai_CharsT[j];
        if (HMT_B(p, 0x0d) >= 7
            && (p->kind == 6 || p->kind == 7 || p->kind == 5 || p->kind == 4 || p->kind == 3 || p->kind == 2)) {
            for (i = 2; i >= 0; i--) {
                if ((p->flags1cc & hmt_ElectionExcludedBits[i]) == 0 && (hmt_ElectionGroupBits[i] & p->flags1cc) != 0) {
                    if (p->netId != last[(i + 1) % 3] && p->netId != last[(i + 2) % 3]) {
                        last[i] = p->netId;
                        cnt[i] += 0.67f;
                    }
                    cnt[i] += 0.33f;
                }
            }
        }
    }
    gm_Shuffle(15, ids);
    k = 0;
    for (i = 0; i < 3; i++) {
        for (n = (int)cnt[i]; n < 2; n++) {
            id = gm_TitleNumber(ids[k++], util_RandomU(3) + 4);
            h = ((int (*)(int, int, int, int, int, int, int, int))cm_SendCreateCharacter)(3, -1, -1, util_RandomU(8) + 0x19, -1, id, 0, gm_Unk503f30(id));
            if (h != -1) {
                while (!cm_GetSlotState(h))
                    sv_PumpNet();
            }
            if (cm_GetSlotState(h) != 1)
                return;
            age = 7;
            money = (int)((util_RandomF() + 1.0f) * 0.5f * hmt_Unk5ee114);
            title = titles[i];
            p = cm_GetSlotValue(h);
            cm_SendMoveGoods(p->netId, -1, ai_OwnPlayer, money);
            cm_BeginFieldBlock((AIPER *)p, p->netId);
            cm_AddFieldDelta(1, 1, 0xd, &age);
            cm_AddFieldDelta(1, 1, 0x167, &title);
            cm_SendFieldDeltas();
            bits = hmt_ElectionGroupBits[i];
            if (i == 2)
                bits |= hmt_ElectionGroupBits[util_RandomU(i)];
            cm_SendSetField(p->netId, 0x1cc, 4, bits, 0);
        }
    }
}
