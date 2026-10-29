/* 0x5770e0, a transition callback (0x30 B; a start the ret+nop scan missed: it follows 0x5770a0's nop fill and
   runs to 0x57710f): for the event kind 0x20 with the step p[3] 0x155 it answers 1 while the state record's +0x3c
   is 3, else 0; every other event goes on to 0x5764b0 */

int ai_Unk5770e0(int *p)
{
    if (p[0] == 0x20) {
        if (p[3] == 0x155 && U_I(S + 0x3c) == 3)
            return 1;
        return 0;
    }
    return ai_Unk5764b0(p);
}
