/* 0x578e40, the tutorial chapter 2 chain (1839 B, ROUGH from the disassembly, the same shape as 0x578570): nine
   0x74-byte step records (mem_Alloc (0x74, name) + memset 0x74): +0 = 2, +4 the magic ("2rtn" 0x6e747232, then
   0x63327341 .. 0x63327347, the outro 0x75747232), +0xc the text id (0x3f53 .. 0x3f70), +0x10 the text key
   (CHAPTER_2_INTRO, CHAPTER_2_A_MAIN .. CHAPTER_2_F_MAIN, CHAPTER_2_OUTRO), +0x2c = -1, +0x30 (0x8e / 0x163),
   +0x3c = 0x576490 (the step predicate; absent on the outro), +0x48 (0xb / 9 / 0xa), +0x68 (0x1837 / 0x5837),
   the last +0x6c = 0x1e00, the records linked through +0x70; the sample bank "TUTORIAL_CHAPTER_2.sbf" 0x629734
   into 0x6f2648 (0x4c2830 (name, 0)); the first record, or 0x578440 (the chain) and 0 after a failed alloc */

int ai_Unk578e40(void)
{
    char *rec;
    char *prev;
    char *r;


    /* FTInitChapter2Intro */
    rec = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62998c);
    if (!rec)
        goto fail;
    memset(rec, 0, 0x74);
    U_I(rec) = 0x2;
    U_I(rec + 0x4) = 0x6e747232;
    U_I(rec + 0xc) = 0x3f53;
    U_I(rec + 0x10) = (unsigned)&full_data_629968;
    U_I(rec + 0x2c) = 0xffffffff;
    U_I(rec + 0x30) = 0x8e;
    U_I(rec + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(rec + 0x48) = 0xb;
    U_I(rec + 0x68) = 0x1837;
    prev = rec;

    /* FTInitChapter2Step1 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629978);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x2;
    U_I(r + 0x4) = 0x63327341;
    U_I(r + 0xc) = 0x3f55;
    U_I(r + 0x10) = (unsigned)&full_data_629940;
    U_I(r + 0x14) = 0x3f57;
    U_I(r + 0x18) = (unsigned)&full_data_62992c;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x38) = (unsigned)&ai_Unk576830;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x1837;
    prev = r;

    /* FTInitChapter2Step2 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629954);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x2;
    U_I(r + 0x4) = 0x63327342;
    U_I(r + 0xc) = 0x3f59;
    U_I(r + 0x10) = (unsigned)&full_data_6298f0;
    U_I(r + 0x14) = 0x3f5b;
    U_I(r + 0x18) = (unsigned)&full_data_6298dc;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0xd5;
    U_I(r + 0x34) = (unsigned)&ai_Unk5768a0;
    U_I(r + 0x38) = (unsigned)&ai_Unk5768d0;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576500;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xb;
    U_I(r + 0x68) = 0x5837;
    prev = r;

    /* FTInitChapter2Step3 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629904);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x2;
    U_I(r + 0x4) = 0x63327343;
    U_I(r + 0xc) = 0x3f5d;
    U_I(r + 0x10) = (unsigned)&full_data_6298a0;
    U_I(r + 0x14) = 0x3f5f;
    U_I(r + 0x18) = (unsigned)&full_data_62988c;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x38) = (unsigned)&ai_Unk576920;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576590;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0xe00;
    prev = r;

    /* FTInitChapter2Step4 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_6298b4);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x2;
    U_I(r + 0x4) = 0x63327344;
    U_I(r + 0xc) = 0x3f61;
    U_I(r + 0x10) = (unsigned)&full_data_629850;
    U_I(r + 0x14) = 0x3f63;
    U_I(r + 0x18) = (unsigned)&full_data_62983c;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x38) = (unsigned)&ai_Unk576970;
    U_I(r + 0x3c) = (unsigned)&ai_Unk5765e0;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1e00;
    prev = r;

    /* FTInitChapter2Step5 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629864);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x2;
    U_I(r + 0x4) = 0x63327345;
    U_I(r + 0xc) = 0x3f65;
    U_I(r + 0x10) = (unsigned)&full_data_629800;
    U_I(r + 0x14) = 0x3f67;
    U_I(r + 0x18) = (unsigned)&full_data_6297ec;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x34) = (unsigned)&ai_Unk576a80;
    U_I(r + 0x38) = (unsigned)&ai_Unk5769c0;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576600;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1e00;
    prev = r;

    /* FTInitChapter2Step6 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629814);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x2;
    U_I(r + 0x4) = 0x63327346;
    U_I(r + 0xc) = 0x3f69;
    U_I(r + 0x10) = (unsigned)&full_data_6297b0;
    U_I(r + 0x14) = 0x3f6b;
    U_I(r + 0x18) = (unsigned)&full_data_62979c;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x34) = (unsigned)&ai_Unk576a80;
    U_I(r + 0x38) = (unsigned)&ai_Unk576ab0;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576640;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1e00;
    prev = r;

    /* FTInitChapter2Step7 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_6297c4);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x2;
    U_I(r + 0x4) = 0x63327347;
    U_I(r + 0xc) = 0x3f6d;
    U_I(r + 0x10) = (unsigned)&full_data_629760;
    U_I(r + 0x14) = 0x3f6f;
    U_I(r + 0x18) = (unsigned)&full_data_62974c;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x38) = (unsigned)&ai_Unk576b50;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576680;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1e00;
    prev = r;

    /* FTInitChapter2Outro */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629774);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x2;
    U_I(r + 0x4) = 0x75747232;
    U_I(r + 0x8) = 0x3;
    U_I(r + 0xc) = 0x3f70;
    U_I(r + 0x10) = (unsigned)&full_data_629724;
    U_I(r + 0x2c) = 0xffffffff;
    U_I(r + 0x30) = 0x163;
    U_I(r + 0x48) = 0xb;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1e00;
    prev = r;
    U_I((unsigned)&full_data_6f2648) = snd_LoadVoiceBank((char *)(unsigned)&full_data_629734, 0);
    return (int)rec;
fail:
    ai_Unk578440(rec);
    return 0;
}
