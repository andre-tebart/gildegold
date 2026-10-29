/* 0x5cbe00, the espionage order handler (1195 B, ROUGH SKELETON: no Ghidra export; from the
   disassembly): param_1 is the message record (the AIMSG: +0 the id, +2 the type 6, +4 the handle, +0x194
   the amount, +0x1c8 the flags with 0x80), param_2 the request (dword +0: 3 (the message) or 4 (with
   +0x21c the owner), +0x214 / +0x218 the two character ids). Types other than 6: the amount < 5 returns
   0x20; the owner (the message when the request type is 3 and the message flag 0x80 is clear; else the
   request +0x21c) must exist and its flag 0x80 must be clear, else 0x60; the target 0x505df0 and the
   building 0x501190 (the second id, 1, 1) must exist, else 0x60; the close 0x4c5f60 (-5) and the carrier
   0x4c4a60 (0x1c8, 4, 0x80); the carrier's owner 0x770b82 + U_W (carrier + 0x27) * 0x21c of type 6 / 7
   gets the text 0x5176d0 (the target id, the carrier, the id); the rest (the window at 0x5cbf5b) is left
   as a comment. ROUGH SKELETON. */

int ai_EspionageOrderHandler(int param_1, int *param_2)  /* @original: the callers push (c, m); the draft body reads its own byte offsets of the parameters */
{
    char text[0x100];
    int owner;
    int target;
    int carrier;
    int chr;

    if (*(char *)(param_1 + 2) != 6) {
        if (*(int *)(param_1 + 0x194) < 5)
            return 0x20;
        if (*param_2 == 3 && !(*(unsigned char *)(param_1 + 0x1c8) & 0x80)) {
            owner = param_1;
        } else {
            if (*param_2 != 4)
                return 0x60;
            owner = param_2[0x87];
            if (owner == 0)
                return 0x60;
            if (*(unsigned char *)(owner + 0x1c8) & 0x80)
                return 0x60;
        }
        target = gm_FindChar(param_2[0x214]);
        if (target == 0)
            return 0x60;
        carrier = gm_BuildQuery(param_2[0x218], 1, 1);
        if (carrier == 0)
            return 0x60;
        cm_SendMoneyDelta(*(int *)(param_1 + 4), 0xfffffffb);
        cm_SendSetField(*(int *)(owner + 4), 0x1c8, 4, 0x80, 0);
        chr = ((unsigned)&ai_Chars + 0x2) + U_W(carrier + 0x27) * 0x21c;
        if (*(char *)chr == 6 || *(char *)chr == 7) {
            txt_Format(text, (char *)*(unsigned short *)target);   /* ROUGH: the format/args */
        }
        /* ROUGH SKELETON: the message chain and the return value are not written yet */
        return 0;
    }
    /* ROUGH SKELETON: type 6 opens the window at 0x5cbf5b (the carrier records 0x501190, the rows and
       the pump 0x5309a0 are not written yet) */
    return 0;
}
