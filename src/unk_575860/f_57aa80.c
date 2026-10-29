/* 0x57aa80, the tutorial chapter 4 chain (1072 B, ROUGH from the disassembly, the same shape as the chapters 1 ..
   3): five 0x74-byte step records (mem_Alloc (0x74, name) + memset 0x74): +0 = 4, +4 the magic "4rtn" 0x6e747234
   / 0x633473xx / 0x75747234, +0xc the text id, +0x10 the text key (CHAPTER_4_INTRO, CHAPTER_4_B_MAIN,
   CHAPTER_4_D_MAIN, CHAPTER_4_E_MAIN, CHAPTER_4_OUTRO), +0x2c = -1, +0x30, +0x3c = 0x576490, +0x48, +0x68, the
   last +0x6c = 0x1f70; linked through +0x70; the sample bank "TUTORIAL_CHAPTER_4.sbf" 0x629e50 into 0x6f2650
   (0x4c2830 (name, 0)); the first record, or 0x578440 (the chain) and 0 after a failed alloc */

int ai_Unk57aa80(void)
{
    char *rec;
    char *prev;
    char *r;


    /* FTInitChapter4Intro */
    rec = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629fac);
    if (!rec)
        goto fail;
    memset(rec, 0, 0x74);
    U_I(rec) = 0x3;
    U_I(rec + 0x4) = 0x6e747234;
    U_I(rec + 0xc) = 0x3fa2;
    U_I(rec + 0x10) = (unsigned)&full_data_629f88;
    U_I(rec + 0x2c) = 0xffffffff;
    U_I(rec + 0x30) = 0x8e;
    U_I(rec + 0x34) = (unsigned)&ai_Unk5774d0;
    U_I(rec + 0x48) = 0xb;
    U_I(rec + 0x68) = 0x5837;
    U_I(rec + 0x6c) = 0x1f70;
    prev = rec;

    /* FTInitChapter4Step4 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629f98);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x63347334;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3fa4;
    U_I(r + 0x10) = (unsigned)&full_data_629f60;
    U_I(r + 0x14) = 0x3fa7;
    U_I(r + 0x18) = (unsigned)&full_data_629f4c;
    U_I(r + 0x1c) = 0x3fa6;
    U_I(r + 0x20) = (unsigned)&full_data_629f3c;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x34) = (unsigned)&ai_Unk5770a0;
    U_I(r + 0x38) = (unsigned)&ai_Unk576d40;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576e70;
    U_I(r + 0x40) = 0x1;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x5a37;
    U_I(r + 0x6c) = 0x1f70;
    U_I(r + 0x70) = 0x0;
    prev = r;

    /* FTInitChapter4Step2 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629f74);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x63347332;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3fa9;
    U_I(r + 0x10) = (unsigned)&full_data_629f00;
    U_I(r + 0x14) = 0x3fac;
    U_I(r + 0x18) = (unsigned)&full_data_629eec;
    U_I(r + 0x1c) = 0x3fab;
    U_I(r + 0x20) = (unsigned)&full_data_629edc;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x34) = (unsigned)&ai_Unk5770a0;
    U_I(r + 0x38) = (unsigned)&ai_Unk576ba0;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576cd0;
    U_I(r + 0x40) = 0x1;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x5b77;
    U_I(r + 0x6c) = 0x1f70;
    U_I(r + 0x70) = 0x0;
    prev = r;

    /* FTInitChapter4Step5 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629f14);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x1;
    U_I(r + 0x4) = 0x63347335;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3fae;
    U_I(r + 0x10) = (unsigned)&full_data_629ea0;
    U_I(r + 0x14) = 0x3fb1;
    U_I(r + 0x18) = (unsigned)&full_data_629e8c;
    U_I(r + 0x1c) = 0x3fb0;
    U_I(r + 0x20) = (unsigned)&full_data_629e7c;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x34) = (unsigned)&ai_Unk5770a0;
    U_I(r + 0x38) = (unsigned)&ai_Unk576ee0;
    U_I(r + 0x3c) = (unsigned)&ai_Unk577020;
    U_I(r + 0x40) = 0x1;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x5f77;
    U_I(r + 0x6c) = 0x1f70;
    U_I(r + 0x70) = 0x0;
    prev = r;

    /* FTInitChapter4Outro */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629eb4);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x4;
    U_I(r + 0x4) = 0x75747234;
    U_I(r + 0x8) = 0x3;
    U_I(r + 0xc) = 0x3fb3;
    U_I(r + 0x10) = (unsigned)&full_data_629e40;
    U_I(r + 0x2c) = 0xffffffff;
    U_I(r + 0x30) = 0x163;
    U_I(r + 0x48) = 0xb;
    U_I(r + 0x68) = 0x1837;
    U_I(r + 0x6c) = 0x1f70;
    prev = r;
    U_I((unsigned)&full_data_6f2650) = snd_LoadVoiceBank((char *)(unsigned)&full_data_629e50, 0);
    return (int)rec;
fail:
    ai_Unk578440(rec);
    return 0;
}
