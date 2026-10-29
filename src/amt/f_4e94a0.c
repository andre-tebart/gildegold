/* 0x4e94a0, a mayor's (+0x169 = 0x1e .. 0x21) bonus step: -1 when the bit 0x4000000 of +0x1c8 is set, else 1 or -2 by the money +0x194 > 2 */

int amt_Unk4e94a0(char *c)
{
    if ((unsigned char)c[0x169] >= 0x1e && (unsigned char)c[0x169] <= 0x21) {
        if ((*(unsigned int *)(c + 0x1c8) & 0x4000000) != 0)
            return -1;
        return (((*(int *)(c + 0x194) >= 3) - 1) & -3) + 1;
    }
    return 0;
}
