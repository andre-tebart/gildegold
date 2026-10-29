/* 0x577d60, releases the tutorial progress form S+0x68 (36 B, a Ghidra-missed start after 0x577cd0's nop fill):
   nothing while the id is -1; else 0x4758f0 (id) and the id reset to -1 */

void ai_Unk577d60(void)
{
    if (U_I(S + 0x68) != -1) {
        d2_CloseView(U_I(S + 0x68));
        U_I(S + 0x68) = -1;
    }
}
