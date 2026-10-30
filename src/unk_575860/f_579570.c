/* 0x579570, the tutorial chapter 3 chain (5377 B, ROUGH from the disassembly, the same record shape as 0x578570
   / 0x578e40 plus the two sub tables of a step record: +0x50 the step count and +0x58 the step table (0x10 B
   per step: +0 the voice argument 0x2c6, +4 the chapter form kind, +8 the text id, +0xc a window / voice
   value), +0x5c the animation entry count, +0x60 the animation step callback and +0x64 the animation table
   (0x2c B per entry: +0 / +4 the position, +8 / +0xc / +0x10 / +0x14 / +0x18 / +0x1c the Bezier points and
   +0x20 / +0x24 / +0x28 the timings, read by 0x57b8a0). Ten 0x74-byte step records (mem_Alloc (0x74, name) +
   memset 0x74): +0 = 3, +4 the magic "3rtn" 0x6e747233 / 0x633373xx / 0x75747233, +0xc the text id, +0x10 the
   text key (CHAPTER_3_*), +0x2c = -1, +0x30, +0x3c, +0x48, +0x68, linked through +0x70. Every sub table has
   its own mem_Alloc and a failed alloc tears the whole chain down too. The screen width d2_ScreenW picks the
   animation floats (0x320 / 0x400 / else). The sample bank "TUTORIAL_CHAPTER_3.sbf" 0x6299b0 into 0x6f264c
   (0x4c2830 (name, 0)); the first record, or 0x578440 (the chain) and 0 after a failed alloc */

/* the sub tables of a chapter step record: the step table at +0x58 (0x10 B per step) and the animation table
   at +0x64 (0x2c B per entry); both are reloaded through the record for every store, as the original does */
#define CHSTEP(o) U_I((char *)U_I(r + 0x58) + (o))
#define CHANIM(o) U_I((char *)U_I(r + 0x64) + (o))

int ai_Unk579570(void)
{
    char *rec;
    char *prev;
    char *r;

    /* FTInitChapter2Intro (the original's copy-paste name for the chapter 3 intro) */
    rec = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62998c);
    if (!rec)
        goto fail;
    memset(rec, 0, 0x74);
    U_I(rec) = 0x3;
    U_I(rec + 0x4) = 0x6e747233;
    U_I(rec + 0xc) = 0x3f72;
    U_I(rec + 0x10) = (unsigned)&full_data_629e1c;
    U_I(rec + 0x2c) = 0xffffffff;
    U_I(rec + 0x30) = 0x8e;
    U_I(rec + 0x34) = (unsigned)&ai_Unk5774d0;
    U_I(rec + 0x48) = 0xb;
    U_I(rec + 0x68) = 0x5837;
    U_I(rec + 0x6c) = 0x1e00;
    prev = rec;

    /* FTInitChapter3Step1 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629e2c);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x3;
    U_I(r + 0x4) = 0x63337331;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3f74;
    U_I(r + 0x10) = (unsigned)&full_data_629dec;
    U_I(r + 0x14) = 0x3f7a;
    U_I(r + 0x18) = (unsigned)&full_data_629dd8;
    U_I(r + 0x24) = 0x3f79;
    U_I(r + 0x28) = (unsigned)(full_data_629db0 + 0x14);
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0xd5;
    U_I(r + 0x3c) = (unsigned)&ai_Unk5764b0;
    U_I(r + 0x40) = 0x1d;
    U_I(r + 0x44) = 0x58c;
    U_I(r + 0x48) = 0xb;
    U_I(r + 0x50) = 0x3;
    U_I(r + 0x54) = 0x0;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1e10;
    /* FTInitScriptChapter3Step1: three steps */
    U_I(r + 0x58) = (unsigned)mem_Alloc(0x30, (char *)(unsigned)&full_data_629e00);
    if (!U_I(r + 0x58))
        goto fail;
    CHSTEP(0x0) = 0x2c6;
    CHSTEP(0x4) = 0x1;
    CHSTEP(0x8) = 0x3f76;
    CHSTEP(0xc) = (unsigned)&full_data_629d98;
    CHSTEP(0x10) = 0x2c6;
    CHSTEP(0x14) = 0x0;
    CHSTEP(0x18) = 0x3f77;
    CHSTEP(0x1c) = (unsigned)&full_data_629d80;
    CHSTEP(0x20) = 0x2c6;
    CHSTEP(0x24) = 0x2;
    CHSTEP(0x28) = 0x3f78;
    CHSTEP(0x2c) = (unsigned)&full_data_629d68;
    prev = r;

    /* FTInitChapter3Step2 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629db0);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x3;
    U_I(r + 0x4) = 0x63337332;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3f7b;
    U_I(r + 0x10) = (unsigned)&full_data_629d38;
    U_I(r + 0x14) = 0x3f80;
    U_I(r + 0x18) = (unsigned)&full_data_629d24;
    U_I(r + 0x1c) = 0x3f7f;
    U_I(r + 0x20) = (unsigned)&full_data_629d0c;
    U_I(r + 0x24) = 0x3f81;
    U_I(r + 0x28) = (unsigned)(full_data_629ce0 + 0x18);
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0xd5;
    U_I(r + 0x34) = (unsigned)&ai_Unk577110;
    U_I(r + 0x3c) = (unsigned)&ai_Unk5770e0;
    U_I(r + 0x40) = 0x1f;
    U_I(r + 0x44) = 0x58c;
    U_I(r + 0x48) = 0x9;
    U_I(r + 0x4c) = 0x20;
    U_I(r + 0x50) = 0x2;
    U_I(r + 0x54) = 0x0;
    U_I(r + 0x5c) = 0x1;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1e30;
    /* FTInitScriptChapter3Step2: two steps */
    U_I(r + 0x58) = (unsigned)mem_Alloc(0x20, (char *)(unsigned)&full_data_629d4c);
    if (!U_I(r + 0x58))
        goto fail;
    CHSTEP(0x0) = 0x2c6;
    CHSTEP(0x4) = 0x1;
    CHSTEP(0x8) = 0x3f7d;
    CHSTEP(0xc) = (unsigned)&full_data_629cc8;
    CHSTEP(0x10) = 0x2c6;
    CHSTEP(0x14) = 0x0;
    CHSTEP(0x18) = 0x3f7e;
    CHSTEP(0x1c) = (unsigned)&full_data_629cb0;
    /* FTInitAnimChapter3Step2: one animation entry */
    U_I(r + 0x64) = (unsigned)mem_Alloc(0x2c, (char *)(unsigned)&full_data_629ce0);
    if (!U_I(r + 0x64))
        goto fail;
    if (d2_ScreenWB == 0x320) {
        CHANIM(0x0) = 0x442dc000;
        CHANIM(0x4) = 0x43960000;
        CHANIM(0x8) = 0x441c8000;
        CHANIM(0xc) = 0x43c80000;
        CHANIM(0x10) = 0x43b70000;
        CHANIM(0x14) = 0x43be0000;
        CHANIM(0x18) = 0x43bc0000;
        CHANIM(0x1c) = 0x43908000;
    } else if (d2_ScreenWB == 0x400) {
        CHANIM(0x0) = 0x44660000;
        CHANIM(0x4) = 0x43960000;
        CHANIM(0x8) = 0x44538000;
        CHANIM(0xc) = 0x43dc0000;
        CHANIM(0x10) = 0x44010000;
        CHANIM(0x14) = 0x43e10000;
        CHANIM(0x18) = 0x43f00000;
        CHANIM(0x1c) = 0x43bb8000;
    } else {
        CHANIM(0x0) = 0x44804000;
        CHANIM(0x4) = 0x43a00000;
        CHANIM(0x8) = 0x44768000;
        CHANIM(0xc) = 0x43dc0000;
        CHANIM(0x10) = 0x44240000;
        CHANIM(0x14) = 0x44160000;
        CHANIM(0x18) = 0x44038000;
        CHANIM(0x1c) = 0x43dc0000;
    }
    CHANIM(0x20) = 0x420c0000;
    CHANIM(0x24) = 0x430e0000;
    CHANIM(0x28) = 0x428e0000;
    prev = r;

    /* FTInitChapter3Step4 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629c9c);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x3;
    U_I(r + 0x4) = 0x63337334;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3f82;
    U_I(r + 0x10) = (unsigned)&full_data_629c6c;
    U_I(r + 0x14) = 0x3f88;
    U_I(r + 0x18) = (unsigned)&full_data_629c58;
    U_I(r + 0x1c) = 0x3f87;
    U_I(r + 0x20) = (unsigned)&full_data_629c48;
    U_I(r + 0x24) = 0x3f89;
    U_I(r + 0x28) = (unsigned)(full_data_629c20 + 0x14);
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x8e;
    U_I(r + 0x3c) = (unsigned)&ai_Unk5764b0;
    U_I(r + 0x40) = 0x21;
    U_I(r + 0x44) = 0x58c;
    U_I(r + 0x48) = 0xa;
    U_I(r + 0x4c) = 0x22;
    U_I(r + 0x50) = 0x3;
    U_I(r + 0x54) = (unsigned)&ai_Unk577490;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1e70;
    /* FTInitScriptChapter3Step4: three steps */
    U_I(r + 0x58) = (unsigned)mem_Alloc(0x30, (char *)(unsigned)&full_data_629c80);
    if (!U_I(r + 0x58))
        goto fail;
    CHSTEP(0x0) = 0x2c6;
    CHSTEP(0x4) = 0x1;
    CHSTEP(0x8) = 0x3f84;
    CHSTEP(0xc) = (unsigned)&full_data_629c08;
    CHSTEP(0x10) = 0x2c6;
    CHSTEP(0x14) = 0x0;
    CHSTEP(0x18) = 0x3f85;
    CHSTEP(0x1c) = (unsigned)&full_data_629bf0;
    CHSTEP(0x20) = 0x2c6;
    CHSTEP(0x24) = 0x0;
    CHSTEP(0x28) = 0x3f86;
    CHSTEP(0x2c) = (unsigned)&full_data_629bd4;
    prev = r;

    /* FTInitChapter3Step6 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629c20);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x3;
    U_I(r + 0x4) = 0x63337336;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3f8a;
    U_I(r + 0x10) = (unsigned)&full_data_629ba4;
    U_I(r + 0x14) = 0x3f90;
    U_I(r + 0x18) = (unsigned)&full_data_629b90;
    U_I(r + 0x1c) = 0x3f8f;
    U_I(r + 0x20) = (unsigned)&full_data_629b78;
    U_I(r + 0x24) = 0x3f91;
    U_I(r + 0x28) = (unsigned)(full_data_629b4c + 0x18);
    U_I(r + 0x2c) = 0x58c;
    U_I(r + 0x30) = 0xd5;
    U_I(r + 0x3c) = (unsigned)&ai_Unk5771b0;
    U_I(r + 0x40) = 0x23;
    U_I(r + 0x44) = 0x58c;
    U_I(r + 0x48) = 0x9;
    U_I(r + 0x4c) = 0x24;
    U_I(r + 0x50) = 0x3;
    U_I(r + 0x54) = 0x0;
    U_I(r + 0x5c) = 0x2;
    U_I(r + 0x60) = (unsigned)&ai_Unk577320;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1f70;
    /* FTInitScriptChapter3Step6: three steps */
    U_I(r + 0x58) = (unsigned)mem_Alloc(0x30, (char *)(unsigned)&full_data_629bb8);
    if (!U_I(r + 0x58))
        goto fail;
    CHSTEP(0x0) = 0x2c6;
    CHSTEP(0x4) = 0x1;
    CHSTEP(0x8) = 0x3f8c;
    CHSTEP(0xc) = (unsigned)&full_data_629b34;
    CHSTEP(0x10) = 0x2c6;
    CHSTEP(0x14) = 0x0;
    CHSTEP(0x18) = 0x3f8d;
    CHSTEP(0x1c) = (unsigned)&full_data_629b1c;
    CHSTEP(0x20) = 0x2c6;
    CHSTEP(0x24) = 0x0;
    CHSTEP(0x28) = 0x3f8e;
    CHSTEP(0x2c) = (unsigned)&full_data_629b04;
    /* FTInitAnimChapter3Step6: two animation entries */
    U_I(r + 0x64) = (unsigned)mem_Alloc(0x58, (char *)(unsigned)&full_data_629b4c);
    if (!U_I(r + 0x64))
        goto fail;
    if (d2_ScreenWB == 0x320) {
        CHANIM(0x0) = 0x43080000;
        CHANIM(0x4) = 0x43d70000;
        CHANIM(0x8) = 0x43a80000;
        CHANIM(0xc) = 0x43e10000;
        CHANIM(0x10) = 0x43e90000;
        CHANIM(0x14) = 0x43870000;
        CHANIM(0x18) = 0x43e88000;
        CHANIM(0x1c) = 0x43430000;
    } else if (d2_ScreenWB == 0x400) {
        CHANIM(0x0) = 0x437a0000;
        CHANIM(0x4) = 0x4400c000;
        CHANIM(0x8) = 0x43e90000;
        CHANIM(0xc) = 0x44070000;
        CHANIM(0x10) = 0x440d8000;
        CHANIM(0x14) = 0x43be0000;
        CHANIM(0x18) = 0x440fc000;
        CHANIM(0x1c) = 0x43898000;
    } else {
        CHANIM(0x0) = 0x43990000;
        CHANIM(0x4) = 0x44110000;
        CHANIM(0x8) = 0x43e40000;
        CHANIM(0xc) = 0x44160000;
        CHANIM(0x10) = 0x441a0000;
        CHANIM(0x14) = 0x44020000;
        CHANIM(0x18) = 0x441c8000;
        CHANIM(0x1c) = 0x43af0000;
    }
    CHANIM(0x20) = 0x420c0000;
    CHANIM(0x24) = 0x430e0000;
    CHANIM(0x28) = 0x428e0000;
    if (d2_ScreenWB == 0x320) {
        CHANIM(0x2c) = 0x43e88000;
        CHANIM(0x30) = 0x43430000;
        CHANIM(0x34) = 0x438f0000;
        CHANIM(0x38) = 0x43910000;
        CHANIM(0x3c) = 0x43cb0000;
        CHANIM(0x40) = 0x44048000;
        CHANIM(0x44) = 0x42e60000;
        CHANIM(0x48) = 0x43ff0000;
    } else if (d2_ScreenWB == 0x400) {
        CHANIM(0x2c) = 0x440fc000;
        CHANIM(0x30) = 0x43898000;
        CHANIM(0x34) = 0x43da0000;
        CHANIM(0x38) = 0x43aa0000;
        CHANIM(0x3c) = 0x44060000;
        CHANIM(0x40) = 0x441b0000;
        CHANIM(0x44) = 0x43620000;
        CHANIM(0x48) = 0x44148000;
    } else {
        CHANIM(0x2c) = 0x441c8000;
        CHANIM(0x30) = 0x43af0000;
        CHANIM(0x34) = 0x43ee0000;
        CHANIM(0x38) = 0x43c30000;
        CHANIM(0x3c) = 0x44150000;
        CHANIM(0x40) = 0x44250000;
        CHANIM(0x44) = 0x438f0000;
        CHANIM(0x48) = 0x44228000;
    }
    CHANIM(0x4c) = 0x420c0000;
    CHANIM(0x50) = 0x430e0000;
    CHANIM(0x54) = 0x428e0000;
    prev = r;

    /* FTInitChapter3Step7 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629af0);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x3;
    U_I(r + 0x4) = 0x63337337;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3f92;
    U_I(r + 0x10) = (unsigned)&full_data_629ac8;
    U_I(r + 0x2c) = 0x58c;
    U_I(r + 0x30) = 0x47;
    U_I(r + 0x3c) = (unsigned)&ai_Unk5764b0;
    U_I(r + 0x40) = 0x3;
    U_I(r + 0x44) = 0x1aa;
    U_I(r + 0x48) = 0xb;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1f70;
    prev = r;

    /* FTInitChapter3Step8 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629adc);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x3;
    U_I(r + 0x4) = 0x63337338;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3f94;
    U_I(r + 0x10) = (unsigned)&full_data_629a9c;
    U_I(r + 0x14) = 0x3f97;
    U_I(r + 0x18) = (unsigned)&full_data_629a88;
    U_I(r + 0x1c) = 0x3f96;
    U_I(r + 0x20) = (unsigned)&full_data_629a70;
    U_I(r + 0x24) = 0x3f98;
    U_I(r + 0x28) = (unsigned)(full_data_629a48 + 0x14);
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0xd5;
    U_I(r + 0x3c) = (unsigned)&ai_Unk577330;
    U_I(r + 0x40) = 0x17;
    U_I(r + 0x44) = 0x58c;
    U_I(r + 0x48) = 0x9;
    U_I(r + 0x4c) = 0x24;
    U_I(r + 0x50) = 0x0;
    U_I(r + 0x54) = 0x0;
    U_I(r + 0x5c) = 0x5;
    U_I(r + 0x60) = (unsigned)&ai_Unk5773f0;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1f70;
    /* FTInitAnimChapter3Step8: five animation entries */
    U_I(r + 0x64) = (unsigned)mem_Alloc(0xdc, (char *)(unsigned)&full_data_629ab0);
    if (!U_I(r + 0x64))
        goto fail;
    if (d2_ScreenWB == 0x320) {
        CHANIM(0x0) = 0x43e88000;
        CHANIM(0x4) = 0x43430000;
        CHANIM(0x8) = 0x44060000;
        CHANIM(0xc) = 0x43dc0000;
        CHANIM(0x10) = 0x43b20000;
        CHANIM(0x14) = 0x43d20000;
        CHANIM(0x18) = 0x430c0000;
        CHANIM(0x1c) = 0x432a0000;
    } else if (d2_ScreenWB == 0x400) {
        CHANIM(0x0) = 0x440fc000;
        CHANIM(0x4) = 0x43898000;
        CHANIM(0x8) = 0x43f30000;
        CHANIM(0xc) = 0x43c30000;
        CHANIM(0x10) = 0x43f80000;
        CHANIM(0x14) = 0x440c0000;
        CHANIM(0x18) = 0x43750000;
        CHANIM(0x1c) = 0x437f0000;
    } else {
        CHANIM(0x0) = 0x441c8000;
        CHANIM(0x4) = 0x43af0000;
        CHANIM(0x8) = 0x43cb0000;
        CHANIM(0xc) = 0x43910000;
        CHANIM(0x10) = 0x43300000;
        CHANIM(0x14) = 0x43910000;
        CHANIM(0x18) = 0x42fc0000;
        CHANIM(0x1c) = 0x43340000;
    }
    CHANIM(0x20) = 0x420c0000;
    CHANIM(0x24) = 0x430e0000;
    CHANIM(0x28) = 0x428e0000;
    if (d2_ScreenWB == 0x320) {
        CHANIM(0x2c) = 0x42960000;
        CHANIM(0x30) = 0x432a0000;
        CHANIM(0x34) = 0x434e0000;
        CHANIM(0x38) = 0x43960000;
        CHANIM(0x3c) = 0x43d50000;
        CHANIM(0x40) = 0x43820000;
        CHANIM(0x44) = 0x43e88000;
        CHANIM(0x48) = 0x43430000;
    } else if (d2_ScreenWB == 0x400) {
        CHANIM(0x2c) = 0x43340000;
        CHANIM(0x30) = 0x437f0000;
        CHANIM(0x34) = 0x43620000;
        CHANIM(0x38) = 0x43c30000;
        CHANIM(0x3c) = 0x440d8000;
        CHANIM(0x40) = 0x43be0000;
        CHANIM(0x44) = 0x440fc000;
        CHANIM(0x48) = 0x43898000;
    } else {
        CHANIM(0x2c) = 0x43620000;
        CHANIM(0x30) = 0x43a00000;
        CHANIM(0x34) = 0x438a0000;
        CHANIM(0x38) = 0x43cd0000;
        CHANIM(0x3c) = 0x441c8000;
        CHANIM(0x40) = 0x43e10000;
        CHANIM(0x44) = 0x441c8000;
        CHANIM(0x48) = 0x43aa0000;
    }
    CHANIM(0x4c) = 0x420c0000;
    CHANIM(0x50) = 0x430e0000;
    CHANIM(0x54) = 0x428e0000;
    if (d2_ScreenWB == 0x320) {
        CHANIM(0x58) = 0x43e88000;
        CHANIM(0x5c) = 0x43430000;
        CHANIM(0x60) = 0x438f0000;
        CHANIM(0x64) = 0x43910000;
        CHANIM(0x68) = 0x43cb0000;
        CHANIM(0x6c) = 0x44048000;
        CHANIM(0x70) = 0x42e60000;
        CHANIM(0x74) = 0x43ff0000;
    } else if (d2_ScreenWB == 0x400) {
        CHANIM(0x58) = 0x440fc000;
        CHANIM(0x5c) = 0x43898000;
        CHANIM(0x60) = 0x43da0000;
        CHANIM(0x64) = 0x43aa0000;
        CHANIM(0x68) = 0x44060000;
        CHANIM(0x6c) = 0x441b0000;
        CHANIM(0x70) = 0x43620000;
        CHANIM(0x74) = 0x44148000;
    } else {
        CHANIM(0x58) = 0x441c8000;
        CHANIM(0x5c) = 0x43af0000;
        CHANIM(0x60) = 0x43ee0000;
        CHANIM(0x64) = 0x43c30000;
        CHANIM(0x68) = 0x44150000;
        CHANIM(0x6c) = 0x44250000;
        CHANIM(0x70) = 0x438f0000;
        CHANIM(0x74) = 0x44228000;
    }
    CHANIM(0x78) = 0x420c0000;
    CHANIM(0x7c) = 0x430e0000;
    CHANIM(0x80) = 0x428e0000;
    if (d2_ScreenWB == 0x320) {
        CHANIM(0x84) = 0x442f0000;
        CHANIM(0x88) = 0x439b0000;
        CHANIM(0x8c) = 0x442f0000;
        CHANIM(0x90) = 0x439b0000;
        CHANIM(0x94) = 0x442f0000;
        CHANIM(0x98) = 0x439b0000;
        CHANIM(0x9c) = 0x442f0000;
        CHANIM(0xa0) = 0x439b0000;
    } else if (d2_ScreenWB == 0x400) {
        CHANIM(0x84) = 0x44670000;
        CHANIM(0x88) = 0x439b0000;
        CHANIM(0x8c) = 0x44670000;
        CHANIM(0x90) = 0x439b0000;
        CHANIM(0x94) = 0x44670000;
        CHANIM(0x98) = 0x439b0000;
        CHANIM(0x9c) = 0x44670000;
        CHANIM(0xa0) = 0x439b0000;
    } else {
        CHANIM(0x84) = 0x440c0000;
        CHANIM(0x88) = 0x43d20000;
        CHANIM(0x8c) = 0x44318000;
        CHANIM(0x90) = 0x43b90000;
        CHANIM(0x94) = 0x44570000;
        CHANIM(0x98) = 0x43a00000;
        CHANIM(0x9c) = 0x447f0000;
        CHANIM(0xa0) = 0x439b0000;
    }
    CHANIM(0xa4) = 0x0;
    CHANIM(0xa8) = 0x40e00000;
    CHANIM(0xac) = 0x41a80000;
    if (d2_ScreenWB == 0x320) {
        CHANIM(0xb0) = 0x442f0000;
        CHANIM(0xb4) = 0x438c0000;
        CHANIM(0xb8) = 0x442f0000;
        CHANIM(0xbc) = 0x438c0000;
        CHANIM(0xc0) = 0x442f0000;
        CHANIM(0xc4) = 0x438c0000;
        CHANIM(0xc8) = 0x442f0000;
        CHANIM(0xcc) = 0x438c0000;
    } else if (d2_ScreenWB == 0x400) {
        CHANIM(0xb0) = 0x44670000;
        CHANIM(0xb4) = 0x438c0000;
        CHANIM(0xb8) = 0x44670000;
        CHANIM(0xbc) = 0x438c0000;
        CHANIM(0xc0) = 0x44670000;
        CHANIM(0xc4) = 0x438c0000;
        CHANIM(0xc8) = 0x44670000;
        CHANIM(0xcc) = 0x438c0000;
    } else {
        CHANIM(0xb0) = 0x440c0000;
        CHANIM(0xb4) = 0x43d20000;
        CHANIM(0xb8) = 0x44318000;
        CHANIM(0xbc) = 0x43b90000;
        CHANIM(0xc0) = 0x44570000;
        CHANIM(0xc4) = 0x43a00000;
        CHANIM(0xc8) = 0x447f0000;
        CHANIM(0xcc) = 0x438c0000;
    }
    CHANIM(0xd0) = 0x0;
    CHANIM(0xd4) = 0x40e00000;
    CHANIM(0xd8) = 0x41a80000;
    prev = r;

    /* FTInitChapter3Step9 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629a48);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x3;
    U_I(r + 0x4) = 0x63337339;
    U_I(r + 0x8) = 0x1;
    U_I(r + 0xc) = 0x3f99;
    U_I(r + 0x10) = (unsigned)&full_data_629a1c;
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0x47;
    U_I(r + 0x3c) = (unsigned)&ai_Unk576490;
    U_I(r + 0x40) = 0x3;
    U_I(r + 0x44) = 0x163;
    U_I(r + 0x48) = 0xb;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1f70;
    prev = r;

    /* FTInitChapter3Step10 */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_629a30);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x3;
    U_I(r + 0x4) = 0x63337341;
    U_I(r + 0x8) = 0x2;
    U_I(r + 0xc) = 0x3f9b;
    U_I(r + 0x10) = (unsigned)&full_data_629a08;
    U_I(r + 0x1c) = 0x3f9d;
    U_I(r + 0x20) = (unsigned)&full_data_6299f0;
    U_I(r + 0x24) = 0x3f9f;
    U_I(r + 0x28) = (unsigned)(full_data_6299c8 + 0x14);
    U_I(r + 0x2c) = 0x429;
    U_I(r + 0x30) = 0xd5;
    U_I(r + 0x3c) = (unsigned)&ai_Unk577450;
    U_I(r + 0x40) = 0x12;
    U_I(r + 0x44) = 0x58c;
    U_I(r + 0x48) = 0x9;
    U_I(r + 0x4c) = 0x26;
    U_I(r + 0x50) = 0x0;
    U_I(r + 0x54) = 0x0;
    U_I(r + 0x5c) = 0x1;
    U_I(r + 0x68) = 0x5c37;
    U_I(r + 0x6c) = 0x1e70;
    /* FTInitAnimChapter3Step8 (the copy-paste name again): one animation entry */
    U_I(r + 0x64) = (unsigned)mem_Alloc(0x2c, (char *)(unsigned)&full_data_629ab0);
    if (!U_I(r + 0x64))
        goto fail;
    if (d2_ScreenWB == 0x320) {
        CHANIM(0x0) = 0x43e88000;
        CHANIM(0x4) = 0x43430000;
        CHANIM(0x8) = 0x43e40000;
        CHANIM(0xc) = 0x43960000;
        CHANIM(0x10) = 0x43580000;
        CHANIM(0x14) = 0x438c0000;
        CHANIM(0x18) = 0x43260000;
        CHANIM(0x1c) = 0x432a0000;
    } else if (d2_ScreenWB == 0x400) {
        CHANIM(0x0) = 0x440fc000;
        CHANIM(0x4) = 0x43898000;
        CHANIM(0x8) = 0x440d8000;
        CHANIM(0xc) = 0x43b90000;
        CHANIM(0x10) = 0x43ad0000;
        CHANIM(0x14) = 0x43cd0000;
        CHANIM(0x18) = 0x438c0000;
        CHANIM(0x1c) = 0x437f0000;
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
    CHANIM(0x24) = 0x432a0000;
    CHANIM(0x28) = 0x42d40000;
    prev = r;

    /* FTInitChapter3Outro */
    r = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_6299c8);
    *(char **)(prev + 0x70) = r;
    if (!r)
        goto fail;
    memset(r, 0, 0x74);
    U_I(r) = 0x3;
    U_I(r + 0x4) = 0x75747233;
    U_I(r + 0x8) = 0x3;
    U_I(r + 0xc) = 0x3fa0;
    U_I(r + 0x10) = (unsigned)&full_data_6299a0;
    U_I(r + 0x2c) = 0xffffffff;
    U_I(r + 0x30) = 0x163;
    U_I(r + 0x48) = 0xb;
    U_I(r + 0x68) = 0x5837;
    U_I(r + 0x6c) = 0x1f70;
    prev = r;
    U_I((unsigned)&full_data_6f264c) = snd_LoadVoiceBank((char *)(unsigned)&full_data_6299b0, 0);
    return (int)rec;
fail:
    ai_Unk578440(rec);
    return 0;
}
