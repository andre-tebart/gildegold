/* 0x5b4bf0, a text / record window (105 B, ROUGH SKELETON): a 0x50-byte record at the frame base (cleared by
   hand), the field 0x40400, the float 0x5eed6c and the text 0x174f; a 0x150-byte frame. The body is a comment */

int ai_Unk5b4bf0(void)
{
    int rec[0x14];

    memset(rec, 0, sizeof(rec));
    rec[3] = 0x40400;
    /* ROUGH SKELETON: the text 0x174f, the float 0x5eed6c and the window body */
    return 0;
}
