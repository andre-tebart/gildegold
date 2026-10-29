/* 0x578570, the tutorial chapter 1 chain (2254 B, ROUGH from the disassembly): eleven 0x74-byte step records,
   each mem_Alloc (0x74, name) + memset 0x74 with the fields +0 = 1, +4 the step magic ("1rtn" 0x6e747231,
   then 0x63317341 / 0x63317343 .. 0x6331734a, 0x75747231), +0xc the text id (0x3f2a .. 0x3f51), +0x10 the text
   key (CHAPTER_1_INTRO, CHAPTER_1_A_MAIN .. CHAPTER_1_J_MAIN, CHAPTER_1_OUTRO), +0x2c = -1, +0x30 (0x8e / 0x163
   / 0), +0x3c = 0x576490 (the step predicate), +0x48 (0xb / 0xa / 9 / 0), +0x68 (0, 1, 3, 7, 0x17 .. 0x1837), the
   records linked through +0x70 (the last +8 = 3); then the sample bank "TUTORIAL_CHAPTER_1.sbf" 0x6293f8 into
   0x6f2644 (0x4c2830 (name, 0)); the first record, or 0x578440 (the chain) and 0 after a failed alloc */

int ai_Unk578570(void)
{
    char *rec;
    char *prev;
    char *r;

    /* FTInitChapter1Intro */
    rec = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629710);
    if (!rec)
        goto fail;
    memset(rec, 0, 0x74);
    U_I(rec) = 0x1;
    U_I(rec + 0x4) = 0x6e747231;
    U_I(rec + 0xc) = 0x3f2a;
    U_I(rec + 0x10) = (unsigned)&full_data_6296ec;
    U_I(rec + 0x2c) = 0xffffffff;
    U_I(rec + 0x30) = 0x8e;
    U_I(rec + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(rec + 0x48) = 0xb;
    prev = rec;

    /* FTInitChapter1Step1 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_6296fc);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x63317341;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3f2c;
    U_I(r + 0x10) = (unsigned)&full_data_6296c4;
    U_I(r + 0x14) = 0x3f2e;
    U_I(r + 0x18) = (unsigned)&full_data_6296b0;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x163;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(r + 0x40) = 0x16;
    U_I(r + 0x48) = 0xb;
    prev = r;

    /* FTInitChapter1Step3 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_6296d8);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x63317343;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3f30;
    U_I(r + 0x10) = (unsigned)&full_data_629674;
    U_I(r + 0x14) = 0x3f33;
    U_I(r + 0x18) = (unsigned)&full_data_629660;
    U_I(r + 0x1c) = 0x3f32;
    U_I(r + 0x20) = (unsigned)&full_data_629650;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(r + 0x40) = 0x6;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0x9;
    U_I(r + 0x68) = 0x1;
    prev = r;

    /* FTInitChapter1Step4 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629688);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x63317344;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3f35;
    U_I(r + 0x10) = (unsigned)&full_data_629614;
    U_I(r + 0x14) = 0x3f38;
    U_I(r + 0x18) = (unsigned)&full_data_629600;
    U_I(r + 0x1c) = 0x3f37;
    U_I(r + 0x20) = (unsigned)&full_data_6295f0;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(r + 0x40) = 0x8;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x68) = 0x3;
    prev = r;

    /* FTInitChapter1Step5 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629628);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x63317345;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3f3a;
    U_I(r + 0x10) = (unsigned)&full_data_6295b4;
    U_I(r + 0x14) = 0x3f3d;
    U_I(r + 0x18) = (unsigned)&full_data_6295a0;
    U_I(r + 0x1c) = 0x3f3c;
    U_I(r + 0x20) = (unsigned)&full_data_629590;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x38) = (unsigned)&ai_Unk5766d0;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(r + 0x40) = 0x1;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0x9;
    U_I(r + 0x68) = 0x7;
    prev = r;

    /* FTInitChapter1Step6 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_6295c8);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x63317346;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3f3f;
    U_I(r + 0x10) = (unsigned)&full_data_629554;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(r + 0x40) = 0x3;
    U_I(r + 0x48) = 0xb;
    U_I(r + 0x68) = 0x7;
    prev = r;

    /* FTInitChapter1Step7 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629568);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x63317347;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3f41;
    U_I(r + 0x10) = (unsigned)&full_data_62952c;
    U_I(r + 0x14) = 0x3f43;
    U_I(r + 0x18) = (unsigned)&full_data_629518;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(r + 0x40) = 0xc;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x17;
    prev = r;

    /* FTInitChapter1Step8 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629540);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x63317348;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3f45;
    U_I(r + 0x10) = (unsigned)&full_data_6294dc;
    U_I(r + 0x14) = 0x3f47;
    U_I(r + 0x18) = (unsigned)&full_data_6294c8;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(r + 0x40) = 0x13;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x817;
    prev = r;

    /* FTInitChapter1Step9 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_6294f0);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x63317349;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3f49;
    U_I(r + 0x10) = (unsigned)&full_data_629488;
    U_I(r + 0x14) = 0x3f4b;
    U_I(r + 0x18) = (unsigned)&full_data_629474;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(r + 0x40) = 0x14;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x1817;
    prev = r;

    /* FTInitChapter1Step10 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62949c);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x6331734a;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3f4d;
    U_I(r + 0x10) = (unsigned)&full_data_629438;
    U_I(r + 0x14) = 0x3f4f;
    U_I(r + 0x18) = (unsigned)&full_data_629424;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x163;
    U_I(r + 0x38) = (unsigned)&ai_Unk5767d0;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(r + 0x40) = 0xd;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x1837;
    prev = r;

    /* FTInitChapter1Outro */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62944c);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x75747231;
    U_I(r + 0x8) = 0x3;
    U_I(r + 0xc) = 0x3f51;
    U_I(r + 0x10) = (unsigned)&full_data_6293e8;
    U_I(r + 0x2c) = 0xffffffff;
    U_I(r + 0x30) = 0x163;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(r + 0x48) = 0xb;
    U_I(r + 0x68) = 0x1837;
    prev = r;
    U_I((unsigned)&full_data_6f2644) = snd_LoadVoiceBank((char *)(unsigned)&full_data_6293f8, 0);
    return (int)rec;
fail:
    ai_Unk578440(rec);
    return 0;
}
