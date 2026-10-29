/* 0x57ced0, a second tutorial message builder (189 B, ROUGH SKELETON, the 0x57cde0 shape): a 0x100-byte frame;
   0x4eb180 (arg 1, &local, &local2, 0) - non-zero answers 0; the 0xf8-byte struct zeroed and filled from the
   record (+1 dword, the word +0x27), the clock globals 0x770ae0 / 0x770ae4 / 0x770ae8 / 0x770aec, the character
   record 0x770b84 + the word * 0x21c, the byte 0x95 at +4 and 1 at +0x3a, 0x4c4b70 (the local); 1 */

int ai_Unk57ced0(char *rec)
{
    char buf[0x100];
    int a;
    int b;

    if (ahm_ResearchState(rec, &a, &b, 0) != 0)
        return 0;
    memset(buf, 0, 0xf8);
    /* ROUGH: the fields and the message are not written yet */
    cm_SendLongMessage(buf + 0x10);
    return 1;
}
