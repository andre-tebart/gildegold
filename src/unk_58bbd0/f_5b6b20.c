/* 0x5b6b20, a window over a record (902 B, ROUGH SKELETON): param_1 read at +1, 0x509b10 (the record, 0x64)
   first, a 0x11c-byte frame; the body is a comment */

int ai_Unk5b6b20(int param_1)
{
    int rec;

    rec = U_I(param_1 + 1);
    gm_EquipmentValue(param_1, 0x64);
    /* ROUGH SKELETON: the 0x11c-byte frame */
    (void)rec;
    return 0;
}
