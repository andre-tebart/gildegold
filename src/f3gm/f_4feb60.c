/* 0x4feb60, resets the goods records of the player p (0x53 records of 0x80 bytes from 0x7d6aa0 + p * 0x2a90, the template 0x6178f8): copies the first int and the one 0x24 bytes before, then by the kind of its object prototype (+0x30 the prototype number): kinds 0x17 / 0x25 take 0x4ff950 (p, i), the others the float of 0x4ff8c0 (p, i); +8 = 0x5087f0 (prototype, 100); the float at -0x10 is copied to -0xc. ROUGH */

void gm_InitPlayerRecord(int p)
{
    int i;
    char *rec;
    int *src;
    char *pc;
    float v;

    i = 0;
    src = ai_Unk6178f8;
    rec = ai_Unk7d6aa0 + p * 0x2a90;
    do {
        pc = ai_Goods + *(unsigned short *)(rec - 0x30) * 0x41;
        *(int *)(rec - 0x24) = src[-9];
        *(int *)rec = src[0];
        if (*pc == 0x17 || *pc == 0x25) {
            *(int *)(rec + 4) = gm_Unk4ff950(p, i);
        } else {
            *(float *)(rec - 0x10) = (float)gm_Unk4ff8c0(p, i);
        }
        v = gm_ItemPrice(*(unsigned short *)(rec - 0x30), 100);
        *(float *)(rec + 8) = v;
        *(int *)(rec - 0xc) = *(int *)(rec - 0x10);
        src += 0x20;
        i++;
        rec += 0x80;
    } while ((int)src < ((unsigned)((unsigned)ai_Unk6178f8 + 0x2a50) + 0x30));
}
