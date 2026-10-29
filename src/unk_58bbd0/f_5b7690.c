/* 0x5b7690, a window over the kind record (1890 B, ROUGH SKELETON): 0x7e3f30 + kind * 0x24d, param_1 the kind
   record, a 0x750-byte frame; the body is a comment */

int ai_Unk5b7690(unsigned char *param_1)
{
    char *kind;

    kind = (char *)(U_I((unsigned)&ai_BTypes) + *param_1 * 0x24d);
    /* ROUGH SKELETON: the 0x750-byte frame */
    (void)kind;
    return 0;
}
