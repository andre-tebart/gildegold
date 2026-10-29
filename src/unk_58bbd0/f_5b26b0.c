/* 0x5b26b0, a two-choice window of a kind record (2688 B, ROUGH SKELETON): 0x520fe0 (param_1) first; the kind
   record 0x7e3f30 + kind * 0x24d; with no building (param_2) the +0x5d owner and its 0x4ffe90 (the +0x14);
   the options are the objects 0x4ffe90 (owner, 2, 0, 0x2a, 6) / (owner, 2, 0, 0x116, 6) against the kind bytes
   +0x240 / +0x241 / +0x242, then the window "0x62cd1c" with the caption texts and the click / pump loop. Only
   the prologue and the first lookups are drafted; the window body is left as a comment. The 0x724 frame holds
   two 0x40-byte record blocks and the text buffers */

int ai_StorageWindow(int param_1, int param_2)
{
    char *kind;
    int own;
    int a;
    int b;

    if (sv_CheckCharacterFlag(param_1) != 0)
        return 0;
    kind = (char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)param_1 * 0x24d);
    own = param_2;
    if (own == 0) {
        b = gm_Query(U_I(param_1 + 0x5d), 1, 4, 2);
        if (b != 0)
            own = *(int *)(b + 0x14);
        else
            own = U_I(param_1 + 0x5d);
    }
    a = gm_Query(own, 2, 0, 0x2a, 6);
    if (a == 0)
        a = gm_Query(own, 2, 0, 0x116, 6);
    /* ROUGH SKELETON: the window "0x62cd1c" (0x4741f0 / 0x475600), the option controls 0x475b80 over the
       texts, the kind bytes +0x240 .. +0x242, the click dispatch 0xf8a668 / 0x5fe710 and the pump 0x5309a0
       (this, 0x677c7); the tail at 0x5b3123 closes with 0x4758f0 */
    return 0;
}
