/* 0x57cde0, a tutorial message builder (227 B, ROUGH SKELETON): a 0x100-byte frame; 0x4eb180 (arg 1, &local,
   &local2, 0) - non-zero answers 0 at once; the 0xf8-byte struct zeroed (rep stos 0x3e dwords) and filled from
   the record (+1 dword, the word +0x27), the clock globals 0x770ae0 / 0x770ae4 / 0x770ae8 / 0x770aec, the
   character record 0x770b84 + the word * 0x21c, the random 0x42ec00 (0x5a0); then the message from the locals
   and the record (the rest is not written yet, see the disassembly) */

int ai_Unk57cde0(char *rec)
{
    char buf[0x100];
    int a;
    int b;

    if (ahm_ResearchState(rec, &a, &b, 0) != 0)
        return 0;
    memset(buf, 0, 0xf8);
    /* ROUGH: the fields and the message are not written yet */
    return 0;
}
