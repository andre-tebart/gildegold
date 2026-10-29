/* 0x5b5610, a record window (121 B, ROUGH SKELETON): param_1 == 0 returns, else a 0x50-byte record (cleared) with
   the floats 0x5eed68 / 0x5eed6c; a 0x90-byte frame. The body is a comment */

int ai_Unk5b5610(int param_1)
{
    int rec[0x14];

    if (param_1 == 0)
        return 0;
    memset(rec, 0, sizeof(rec));
    /* ROUGH SKELETON: the fields 0x5eed68 / 0x5eed6c */
    return 0;
}
