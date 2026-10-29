/* 0x5784a0, allocates the tutorial's main-intro record (204 B): mem_Alloc (0x74, "FTInitMainIntro" 0x6293d8),
   0 while it fails (0x578440 (0)); the record zeroed (memset 0x74) and its fields set: +0 = 0, +4 = the magic
   "RTNI" 0x494e5452, +8 = 4, +0xc = 0x3f24, +0x10 = the text key "TUTORIAL_MAIN_INTRO" 0x6293a8, +0x14 ..
   +0x20 = 0, +0x2c = -1, +0x30 = 0x8e, +0x34 / +0x38 = 0, +0x40 / +0x44 = 0, +0x48 = 0xb, +0x68 / +0x6c /
   +0x70 = 0; the sample bank handle 0x6f2640 = 0x4c2830 ("TUTORIAL_INTRO_OUTRO.sbf" 0x6293bc, 0); the record */

int ai_Unk5784a0(void)
{
    char *rec;

    rec = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_6293d8);
    if (!rec) {
        ai_Unk578440(0);
        return 0;
    }
    memset(rec, 0, 0x74);
    U_I(rec) = 0;
    U_I(rec + 4) = 0x494e5452;
    U_I(rec + 8) = 4;
    U_I(rec + 0xc) = 0x3f24;
    U_I(rec + 0x10) = (unsigned)&full_data_6293a8;
    U_I(rec + 0x14) = 0;
    U_I(rec + 0x18) = 0;
    U_I(rec + 0x1c) = 0;
    U_I(rec + 0x20) = 0;
    U_I(rec + 0x2c) = -1;
    U_I(rec + 0x30) = 0x8e;
    U_I(rec + 0x34) = 0;
    U_I(rec + 0x38) = 0;
    U_I(rec + 0x40) = 0;
    U_I(rec + 0x44) = 0;
    U_I(rec + 0x48) = 0xb;
    U_I(rec + 0x68) = 0;
    U_I(rec + 0x6c) = 0;
    U_I(rec + 0x70) = 0;
    U_I((unsigned)&full_data_6f2640) = snd_LoadVoiceBank((char *)(unsigned)&full_data_6293bc, 0);
    return (int)rec;
}
