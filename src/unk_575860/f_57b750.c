/* 0x57b750, the tutorial's main-outro record (186 B): mem_Alloc (0x74, "FTInitMainOutro" 0x62a238), 0 when it
   fails (0x578440 (0)); memset 0x74; +0x14 .. +0x20 / +0x30 / +0x34 / +0x38 / +0x40 / +0x44 / +0x70 = 0, then
   +0 = 7, +4 = the magic "RTUO" 0x4f555452, +8 = 5, +0xc = 0x3f27, +0x10 = the text "TUTORIAL_MAIN_OUTRO"
   0x62a224, +0x2c = -1, +0x48 = 0xb, +0x68 = -9; the record */

#undef CHSTEP
#undef CHANIM

int ai_Unk57b750(void)
{
    char *rec;

    rec = (char *)mem_Alloc(0x74, (char *)(unsigned)&full_data_62a238);
    if (!rec) {
        ai_Unk578440(0);
        return 0;
    }
    memset(rec, 0, 0x74);
    U_I(rec + 0x14) = 0;
    U_I(rec + 0x18) = 0;
    U_I(rec + 0x1c) = 0;
    U_I(rec + 0x20) = 0;
    U_I(rec + 0x30) = 0;
    U_I(rec + 0x34) = 0;
    U_I(rec + 0x38) = 0;
    U_I(rec + 0x40) = 0;
    U_I(rec + 0x44) = 0;
    U_I(rec + 0x70) = 0;
    U_I(rec) = 7;
    U_I(rec + 4) = 0x4f555452;
    U_I(rec + 8) = 5;
    U_I(rec + 0xc) = 0x3f27;
    U_I(rec + 0x10) = (unsigned)&full_data_62a224;
    U_I(rec + 0x2c) = -1;
    U_I(rec + 0x48) = 0xb;
    U_I(rec + 0x68) = -9;
    return (int)rec;
}
