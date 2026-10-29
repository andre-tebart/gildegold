/* 0x5cd8c0, the spy-slot handler (713 B, ROUGH SKELETON: no Ghidra export; from the disassembly):
   param_1 the message record (the AIMSG: +4 the handle), param_2 the request (the two words at +0 and
   the handle +4), param_3 a flag. The building walk 0x501190 (1, 6) / 0x501030 looks for a building
   whose kind record (0x7e3f30 + the kind byte * 0x24d) starts with 0xf; none returns 0x10. The found
   building builds the request record (the kind byte 0x2c, the type byte 2, the message handle, the
   player's character 0x770b84 + U_W (0x6f25b8) * 0x21c, the two words of the request and the four -1
   slots) and sends it; the rest is left as a comment. The call site 0x5cde90 passes a fourth 0. ROUGH
   SKELETON. */

int ai_Unk5cd8c0(int param_1, int *param_2, int param_3, int param_4)
{
    int bld;

    bld = gm_BuildQuery(1, 6);
    while (bld != 0) {
        if (*(unsigned char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)bld * 0x24d) == 0xf)
            break;
        bld = gm_BuildNext();
    }
    if (bld == 0)
        return 0x10;
    /* ROUGH SKELETON: the AIMSG record (the kind byte 0x2c, the type byte 2, the handle, the player's
       character, the request words and the four -1 slots) and the send are not written yet */
    return 0x10;
}
