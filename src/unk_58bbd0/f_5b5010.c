/* 0x5b5010, a window over the player's character record (779 B, ROUGH SKELETON): the record 0x770b80 +
   0x6f25b8 * 0x21c and the object walk 0x4ffe90 (2, ...); a 0x390-byte frame. The body is a comment */

int ai_Unk5b5010(void)
{
    char *per;

    per = (char *)((unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c);
    /* ROUGH SKELETON: the walk 0x4ffe90 (2, ...), the controls and the pump loop */
    (void)per;
    return 0;
}
