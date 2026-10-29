/* 0x512f30, sets the 16 character animations (ch_PreloadAnims, 0x404200) of the characters that live in or belong
   to the building b (its scene object +0x16c or the group id +0x2c), from the class animation record of the table
   0x61ae08 whose kind is b's class (init_ClassAnims)
   @deep read the whole original 0x512f30..0x513040 against the source: the class lookup in the 19-record table 0x61ae08 (kind = ai_BTypes[b[0]*0x24d]), sv_CollectGroupCharacters(b), the walk of ai_Chars + 0x184 in steps of 0x21c up to +0x184+0x300*0x21c, the q[-6] == b / group == b->id test and the 18-argument ch_PreloadAnims call (r->names 0..15, 0x10) all correspond - meaning equal. Readability done: Q1 init_SetCharacterAnimations, Q2 (ch_PreloadAnims 0x404200, sv_CollectGroupCharacters 0x52b2a0 renamed in sv.c, init_ClassAnims), Q3 (the end is (char *)ai_Chars + 0x184 + 0x300*0x21c), Q4 (new INIT_ANIMSET view, INIT_CHAR.group at 0x2c, INIT_BLD.id), Q7 (varargs prototype). Tried: dropping the redundant i (same code). Left (allocation): ours keeps b in ebx where the original uses ebp and the class-byte address in edi vs edx, idx in esi vs ecx - the prologue (512f30) and the character loop (512f9e) hunks */


void init_SetCharacterAnimations(unsigned char *b)
{
    int idx;
    unsigned k;
    int *q;
    int ch;
    INIT_ANIMSET *r;

    idx = 0;
    if (init_ClassAnims[0].kind != 0) {
        k = init_ClassAnims[0].kind;
        do {
            if (k == (unsigned char)ai_BTypes[*b * 0x24d])
                break;
            k = init_ClassAnims[idx + 1].kind;
            idx++;
        } while (k != 0);
    }
    r = init_ClassAnims + idx;
    sv_CollectGroupCharacters((char *)b);
    q = (int *)((char *)ai_Chars + 0x184);
    do {
        ch = *q;
        if (ch != 0 && (q[-6] == (int)b || ((INIT_CHAR *)ch)->group == ((INIT_BLD *)b)->id)) {
            ch_PreloadAnims((struct CHARACTER *)ch, 0x10, r->names[0], r->names[1], r->names[2], r->names[3],
                            r->names[4], r->names[5], r->names[6], r->names[7], r->names[8], r->names[9],
                            r->names[10], r->names[11], r->names[12], r->names[13], r->names[14], r->names[15]);
        }
        q += 0x87;
    } while ((int)q < (int)((char *)ai_Chars + 0x184 + 0x300 * 0x21c));
}
