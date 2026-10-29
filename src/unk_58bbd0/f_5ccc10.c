/* 0x5ccc10, the espionage overview window (1738 B, ROUGH SKELETON: no Ghidra export; from the
   disassembly): a 0x1450-byte frame (__chkstk 0x5de9c0); param_1 is the message record (the AIMSG: +0
   the id), param_2 the page 0 .. 5 (outside -> 0x60). The caption 0x5176d0 (0x1df7, the id); page 0:
   the record 0x4a63f0 (the message, 2, 0x80, 0, 0, 100.0f, the out), 0x4fea50 and the character
   0x770b80 + the word +0x27 * 0x21c, the price 0x50a640 * 0x5ece60 * (0x42ec00 (3) + 2) and the payment
   0x4c4830. Pages 1 .. 5 at 0x5ccd41. ROUGH SKELETON: the rest (the window, the rows, the payment
   chain) is left as a comment. */

int ai_Unk5ccc10(int param_1, int param_2)
{
    char text[0x100];
    int out[2];

    if (param_2 < 0 || param_2 > 5)
        return 0x60;
    txt_Format(text, (char *)0x1df7, *(unsigned short *)param_1);
    if (param_2 == 0) {
        if (AI_4A63F0_7(param_1, 2, 0x80, 0, 0, 100.0f, out) == 0)
            return 0x60;
        gm_FindById(out[0], param_2, param_2, out);
        /* ROUGH SKELETON: the character record, the price and the payment 0x4c4830 are not written */
        return 0;
    }
    /* ROUGH SKELETON: pages 1 .. 5 open the window at 0x5ccd41 (the rows and the pump 0x5309a0 are not
       written yet) */
    return 0;
}
