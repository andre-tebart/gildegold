/* 0x57aeb0, the tutorial chapter 5 chain (2207 B, ROUGH from the disassembly, the same record shape as the
   chapters 1 .. 4 plus one step record with an animation table: eight 0x74-byte records (mem_Alloc (0x74, name)
   + memset 0x74), +0 = 5 (the last one 6), +4 the magic "5rtn" 0x6e747235 / 0x633573xx, +0xc the text id, +0x10
   the text key (CHAPTER_5_*), +0x2c = -1, +0x30, +0x3c, +0x48, +0x68, linked through +0x70. FTInitChapter5Step2
   carries the animation table at +0x64 (0x58 B = two 0x2c entries, see ai_Unk577e50 / 0x57b8a0), its entry count
   +0x5c = 2 and the step callback +0x60 = 0x5772d0; the screen width d2_ScreenW picks the floats. The sample
   bank "TUTORIAL_CHAPTER_5.sbf" 0x629fd4 into 0x6f2654 (0x4c2830 (name, 0)); the first record, or 0x578440
   (the chain) and 0 after a failed alloc */

int ai_Unk57aeb0(void)
{
    char *rec;
    char *prev;
    char *r;

    /* FTInitChapter5Intro */
    rec = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62a210);
    if (!rec)
        goto fail;
    memset(rec, 0, 0x74);
    U_I(rec) = 0x5;
    U_I(rec + 0x4) = 0x6e747235;
    U_I(rec + 0xc) = 0x3fb5;
    U_I(rec + 0x10) = (unsigned)&full_data_62a1ec;
    U_I(rec + 0x2c) = 0xffffffff;
    U_I(rec + 0x30) = 0x8e;
    U_I(rec + 0x34) = (unsigned)&ai_Unk577540;
    U_I(rec + 0x48) = 0xb;
    U_I(rec + 0x68) = 0x5f77;
    U_I(rec + 0x6c) = 0x1f70;
    prev = rec;

    /* FTInitChapter5Step1 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62a1fc);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x5;
    U_I(r + 0x4) = 0x63357331;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3fb7;
    U_I(r + 0x10) = (unsigned)&full_data_62a1c4;
    U_I(r + 0x14) = 0x3fb9;
    U_I(r + 0x18) = (unsigned)&full_data_62a1b0;
    U_I(r + 0x24) = 0x3fba;
    U_I(r + 0x28) = (unsigned)(full_data_62a188 + 0x14);
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x163;
    U_I(r + 0x40) = 0x2c;
    U_I(r + 0x44) = 0x1aa;
    U_I(r + 0x48) = 0xb;
    U_I(r + 0x68) = 0x1877;
    U_I(r + 0x6c) = 0x1f78;
    prev = r;

    /* FTInitChapter5Step3 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62a1d8);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x5;
    U_I(r + 0x4) = 0x63357333;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3fc5;
    U_I(r + 0x10) = (unsigned)&full_data_62a174;
    U_I(r + 0x14) = 0x3fc7;
    U_I(r + 0x18) = (unsigned)&full_data_62a160;
    U_I(r + 0x24) = 0x3fc8;
    U_I(r + 0x28) = (unsigned)(full_data_62a134 + 0x18);
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x40) = 0xf;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x18f7;
    U_I(r + 0x6c) = 0x1f78;
    prev = r;

    /* FTInitChapter5Step2 (with its animation table) */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62a188);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x5;
    U_I(r + 0x4) = 0x63357332;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3fbb;
    U_I(r + 0x10) = (unsigned)&full_data_62a120;
    U_I(r + 0x14) = 0x3fbe;
    U_I(r + 0x18) = (unsigned)&full_data_62a10c;
    U_I(r + 0x1c) = 0x3fbd;
    U_I(r + 0x20) = (unsigned)&full_data_62a0f4;
    U_I(r + 0x24) = 0x3fbf;
    U_I(r + 0x28) = (unsigned)(full_data_62a0cc + 0x14);
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0xd5;
    U_I(r + 0x34) = (unsigned)&ai_Unk577230;
    U_I(r + 0x3c) = (unsigned)&ai_Unk577290;
    U_I(r + 0x40) = 0x17;
    U_I(r + 0x44) = 0x58c;
    U_I(r + 0x48) = 0x9;
    U_I(r + 0x4c) = 0x29;
    U_I(r + 0x50) = 0x0;
    U_I(r + 0x54) = 0x0;
    U_I(r + 0x5c) = 0x2;
    U_I(r + 0x60) = (unsigned)&ai_Unk5772d0;
    U_I(r + 0x68) = 0x1877;
    U_I(r + 0x6c) = 0x1f78;
    /* FTInitAnimChapter5Step2: two animation entries */
    U_I(r + 0x64) = (unsigned)mem_Alloc(0x58, (char *)(unsigned)&full_data_62a134);
    if (!U_I(r + 0x64))
        goto fail;
    if (d2_ScreenW == 0x320) {
        CHANIM(0x0) = 0x430c0000;
        CHANIM(0x4) = 0x432a0000;
        CHANIM(0x8) = 0x437a0000;
        CHANIM(0xc) = 0x43910000;
        CHANIM(0x10) = 0x43e10000;
        CHANIM(0x14) = 0x43910000;
        CHANIM(0x18) = 0x44160000;
        CHANIM(0x1c) = 0x43988000;
    } else if (d2_ScreenW == 0x400) {
        CHANIM(0x0) = 0x437a0000;
        CHANIM(0x4) = 0x437f0000;
        CHANIM(0x8) = 0x43c80000;
        CHANIM(0xc) = 0x43c80000;
        CHANIM(0x10) = 0x44160000;
        CHANIM(0x14) = 0x43e10000;
        CHANIM(0x18) = 0x44318000;
        CHANIM(0x1c) = 0x43c30000;
    } else {
        CHANIM(0x0) = 0x441c8000;
        CHANIM(0x4) = 0x43af0000;
        CHANIM(0x8) = 0x441f0000;
        CHANIM(0xc) = 0x43d70000;
        CHANIM(0x10) = 0x43bc0000;
        CHANIM(0x14) = 0x43dc0000;
        CHANIM(0x18) = 0x43a80000;
        CHANIM(0x1c) = 0x43a00000;
    }
    CHANIM(0x20) = 0x420c0000;
    CHANIM(0x24) = 0x43310000;
    CHANIM(0x28) = 0x42d40000;
    if (d2_ScreenW == 0x320) {
        CHANIM(0x2c) = 0x442f0000;
        CHANIM(0x30) = 0x439b0000;
        CHANIM(0x34) = 0x442f0000;
        CHANIM(0x38) = 0x439b0000;
        CHANIM(0x3c) = 0x442f0000;
        CHANIM(0x40) = 0x439b0000;
        CHANIM(0x44) = 0x442f0000;
        CHANIM(0x48) = 0x439b0000;
    } else if (d2_ScreenW == 0x400) {
        CHANIM(0x2c) = 0x44670000;
        CHANIM(0x30) = 0x439b0000;
        CHANIM(0x34) = 0x44670000;
        CHANIM(0x38) = 0x439b0000;
        CHANIM(0x3c) = 0x44670000;
        CHANIM(0x40) = 0x439b0000;
        CHANIM(0x44) = 0x44670000;
        CHANIM(0x48) = 0x439b0000;
    } else {
        CHANIM(0x2c) = 0x440c0000;
        CHANIM(0x30) = 0x43d20000;
        CHANIM(0x34) = 0x44318000;
        CHANIM(0x38) = 0x43b90000;
        CHANIM(0x3c) = 0x44570000;
        CHANIM(0x40) = 0x43a00000;
        CHANIM(0x44) = 0x447f0000;
        CHANIM(0x48) = 0x439b0000;
    }
    CHANIM(0x4c) = 0x0;
    CHANIM(0x50) = 0x40e00000;
    CHANIM(0x54) = 0x41a80000;
    prev = r;

    /* FTInitChapter5Step2 (the second record with that name in the original) */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62a188);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x5;
    U_I(r + 0x4) = 0x63357332;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3fc0;
    U_I(r + 0x10) = (unsigned)&full_data_62a0b8;
    U_I(r + 0x14) = 0x3fc3;
    U_I(r + 0x18) = (unsigned)&full_data_62a0a4;
    U_I(r + 0x1c) = 0x3fc2;
    U_I(r + 0x20) = (unsigned)&full_data_62a08c;
    U_I(r + 0x24) = 0x3fc4;
    U_I(r + 0x28) = (unsigned)(full_data_62a064 + 0x14);
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0xd5;
    U_I(r + 0x40) = 0x14;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0x9;
    U_I(r + 0x4c) = 0x2d;
    U_I(r + 0x50) = 0x0;
    U_I(r + 0x54) = 0x0;
    U_I(r + 0x68) = 0x1877;
    prev = r;

    /* FTInitChapter5Step4 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62a0cc);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x5;
    U_I(r + 0x4) = 0x63357334;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3fc9;
    U_I(r + 0x10) = (unsigned)&full_data_62a050;
    U_I(r + 0x14) = 0x3fcb;
    U_I(r + 0x18) = (unsigned)&full_data_62a03c;
    U_I(r + 0x24) = 0x3fcc;
    U_I(r + 0x28) = (unsigned)(full_data_62a014 + 0x14);
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x163;
    U_I(r + 0x34) = (unsigned)&ai_Unk5775c0;
    U_I(r + 0x40) = 0x2e;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x18f7;
    U_I(r + 0x6c) = 0x3f78;
    prev = r;

    /* FTInitChapter5Step5 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62a064);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x5;
    U_I(r + 0x4) = 0x63357335;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3fcd;
    U_I(r + 0x10) = (unsigned)&full_data_62a014;
    U_I(r + 0x14) = 0x3fcf;
    U_I(r + 0x18) = (unsigned)&full_data_62a000;
    U_I(r + 0x24) = 0x3fd0;
    U_I(r + 0x28) = (unsigned)(full_data_629fd4 + 0x18);
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0xd5;
    U_I(r + 0x34) = (unsigned)&ai_Unk577610;
    U_I(r + 0x40) = 0x2f;
    U_I(r + 0x44) = 0x2148;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x68) = 0x18f7;
    U_I(r + 0x6c) = 0xbf78;
    prev = r;

    /* FTInitChapter5Step6 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62a064);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x6;
    U_I(r + 0x4) = 0x63357336;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3fd1;
    U_I(r + 0x10) = (unsigned)&full_data_629fc0;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x34) = (unsigned)&ai_Unk577690;
    U_I(r + 0x40) = 0x30;
    U_I(r + 0x48) = 0xb;
    U_I(r + 0x68) = 0x18f7;
    U_I(r + 0x6c) = 0xbf78;
    prev = r;
    U_I((unsigned)&full_data_6f2654) = snd_LoadVoiceBank((char *)(unsigned)&full_data_629fd4, 0);
    return (int)rec;
fail:
    ai_Unk578440(rec);
    return 0;
}
