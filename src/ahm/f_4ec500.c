/* 0x4ec500, creates the alchemist character for the object o (p[2]) of the building kind 0x1e from the order record p (+0xc .. +0xf four shares, +0x10 a value for +0xa,
   +0x14 / +0x20 the first place with its value, +0x18 / +0x1c the name indexes); the first free slot of ai_Chars (or one of kind 0xf), the relations to all others random
   -0x20 .. 0x1f; returns the character id or 0xffff ("ahm_AllocAhmInfo" allocates the record at +0x65). The character record is written
   through raw offsets: ai.h's AICHAR names id, kind, netId, health, name, jobId, title, bld / bld2, money, flags1cc, mood3Rate, partner, unk215 .. unk217 and
   mask; the unnamed ones (+0x10 .. +0x20 the start values, +0x50, +0x5c, +0x80, +0x9, +0xc, +0x18c, +0x190, +0x1f0, +0x1f4, +0x20c) carry no evidence yet.
   @deep B3 draft, 196 differing instructions: the whole statement/offset set was read against the original (asmdiff) and is complete and equal - no missing/extra statement, constant, type or call; the difference is the compiler's allocation from the first instruction on (a zero-register in ebx pushed for gm_FindById's 0,0 and reused for the byte/dword 0 stores vs our immediates and edi=0; the -1 in ebx vs eax; the character pointer frame base ebp vs eax; the ai_NextObjectId store schedule after the name strcpy and the util_Random block; the +0x210/-1, +0x18c, +0x20c stores). Last-bytes pass: spell.py menu assessed - 4ec510 rep-stos (memset/zero-init spellings; the memset(c,0,0x21c) already has the original's rep stos form), 4ec6af / 4ec761 / 4ec927 dup-tail (no if/else arm to move: the tails are the loop and call continuations), 4ec8ab reload (the [sp+0x14] list head is already re-read from memory at every use, the expression and parameter spellings give the same code), 4ec717 / 4ec916 code (the [esi+0x1c] store and the relation-loop store order are scheduling, the multiset is equal). No meaning-class lead with a concrete source form is left; what remains is allocation / scheduling. Q2: ai_Unk144a7a8 and ai_Unk144a80c are the two character-name pointer tables in the exe's BSS (0x144a7a8 / 0x144a80c, filled at runtime: no writer in any exported function, no string or data evidence for first/last name) - no evidenced name. Q4: 54 raw reads/stores of the 0x21c record; the AICHAR fields named in the comment can be typed, but +0x10 .. +0x20 (1000.0f / 12.0f / the random start values), +0x50, +0x5c, +0x80, +0x9, +0xc, +0x18c, +0x190, +0x1f0, +0x1f4, +0x20c have no evidence, and the typed rewrite re-schedules the store block (the unit's typed-view lesson). Q1/Q5/Q6/Q7 done: named, described, the three callees have prototypes in ahm.h. */

#define AHM_ZERO6(m) memset(m, 0, 24)

unsigned short ahm_CreateAlchemist(int *p)
{
    unsigned char *o;
    char *c;
    char *sp;
    int n, i, j, k;
    float v;
    int *g;

    gm_FindById(&o, 0, 0, p[2]);
    if (o == 0)
        return 0xffff;
    if (ai_BTypes[o[0] * 0x24d] != 0x1e)
        return 0xffff;
    for (n = 0; n < 0x300; n++)
        if (*(short *)ai_Chars[n] == -1)
            break;
    if (n == 0x300) {
        for (n = 0; n < 0x300; n++)
            if (ai_Chars[n][2] == 0x0f)
                break;
        if (n == 0x300)
            return 0xffff;
    }
    if (ai_Chars[n][2] == 0x0f)
        ai_CharCount--;
    c = ai_Chars[n];
    memset(c, 0, 0x21c);
    *(int *)(c + 4) = -1;
    *(unsigned short *)c = 0xffff;
    memset(c + 0x5c, 0xff, 0x20);
    *(unsigned char **)(c + 0x170) = o;
    *(unsigned char **)(c + 0x16c) = o;
    ai_CharCount++;
    *(unsigned short *)c = (unsigned short)n;
    *(int *)(c + 4) = ai_NextObjectId;
    ai_NextObjectId++;
    c[2] = 0;
    *(int *)(c + 0x1f0) = 0;
    *(int *)(c + 0x210) = -1;
    c[0x215] = 0;
    c[0x216] = 0;
    c[0x217] = 0;
    c[0x218] = 1;
    *(float *)(c + 0x1e4) = 1.0f;
    c[8] = 100;
    *(unsigned short *)(c + 0xa) = (unsigned char)((char *)p)[0x10];
    c[0x164] = 0x52;
    c[0x166] = 0;
    c[0x165] = 0;
    c[0x1f4] = 0;
    *(int *)(c + 0x190) = 4;
    *(int *)(c + 0x194) = 0;
    c[0xd] = 1;
    *(int *)(c + 0x18c) = -1;
    *(int *)(c + 0x20c) = -1;
    *(short *)(c + 0x50) = 0x7fff;
    for (i = 0; i < 5; i++)
        c[0x80 + i] = (char)(util_Random(0x40) + 0x54);
    c[9] = 0;
    strcpy(c + 0x30, ai_Unk144a7a8[p[6]]);
    strcpy(c + 0x40, ai_Unk144a80c[p[7]]);
    *(float *)(c + 0x1c) = 1000.0f;
    v = util_RandomSq();
    *(float *)(c + 0x20) = 12.0f;
    *(float *)(c + 0x14) = v * 7.0f + 30.0f - 2.0f;
    k = (int)(*(float *)(c + 0x1c) * 0.2f);
    *(float *)(c + 0x18) = *(float *)(c + 0x1c);
    *(float *)(c + 0x10) = *(float *)(c + 0x1c) * 0.33f + (float)util_Random(k) - *(float *)(c + 0x1c) * 0.1f;
    j = 0;
    for (i = 0; i < 0x90000; i += 0x300) {
        ai_Relations[0][*(unsigned short *)c + i] = (signed char)util_Random(0x40) - 0x20;
        ai_RelationMemory[0][*(unsigned short *)c + i] = 0;
        ai_Relations[0][*(unsigned short *)c * 0x300 + j] = (signed char)util_Random(0x40) - 0x20;
        ai_RelationMemory[0][*(unsigned short *)c * 0x300 + j] = 0;
        j++;
    }
    g = (int *)gm_Query(*(int *)(o + 0x5d), 1, 0, 0x24b);
    gm_PlaceOnStage(*(int *)(c + 4), *(int *)(o + 1), g != 0 ? *(int *)((char *)g + 2) : -1, 0, 0);
    gm_ChooseHeadIcon(c);
    if (*(int *)(o + 0x65) == 0)
        *(void **)(o + 0x65) = mem_Alloc(0x3c, "ahm_AllocAhmInfo");
    memset(*(void **)(o + 0x65), 0, 0x3c);
    **(int **)(o + 0x65) = *(int *)(c + 4);
    *(float *)(*(char **)(o + 0x65) + 4) = ((float)(unsigned char)((char *)p)[0xf] * 0.003968254f + 1.0f) * 0.5f;
    (*(char **)(o + 0x65))[8] = ((char *)p)[0xc];
    (*(char **)(o + 0x65))[9] = ((char *)p)[0xd];
    (*(char **)(o + 0x65))[10] = ((char *)p)[0xe];
    (*(char **)(o + 0x65))[11] = ((char *)p)[0xf];
    *(int *)(*(char **)(o + 0x65) + 0xc) = p[5];
    sp = gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0xff);
    if (sp != 0) {
        while (*(unsigned short **)(sp + 0x14) != 0)
            gm_RemoveObject(sp + 0x14, **(unsigned short **)(sp + 0x14));
    }
    *(int *)(*(char **)(o + 0x65) + 0x10) = p[8];
    if (sp != 0)
        gm_AddObjekt(*(int *)(sp + 2), p[5], 2);
    gm_RefreshGoodsLists(ai_OwnPlayer);
    *(int *)(c + 0x1cc) |= 4;
    return *(unsigned short *)c;
}
