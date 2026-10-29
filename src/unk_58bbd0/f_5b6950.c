/* 0x5b6950, a window over a record (1170 B, ROUGH SKELETON): param_1 read at +1, the window "0x62ce8c"
   (0x4741f0), a 0x100-byte frame; the body is a comment */

int ai_Unk5b6950(int param_1)
{
    int rec;

    rec = U_I(param_1 + 1);
    /* ROUGH SKELETON: the window "0x62ce8c" over the record */
    (void)rec;
    return 0;
}
