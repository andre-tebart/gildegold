/* 0x421410, palette index with the smallest squared RGB distance to the 5-bit color (r, g, b) @deep byte work only (behaviour + readable met): 2 B / 4 asm lines, the g / b registers swapped in the two sub ebx of the SqPtr terms (the original subtracts in the PalR, PalB, PalG load order); grade.py diffclass verdict order 12; all 6 term orders, 24 statement perms, per-term temps and parenthesizations tried (notes/units/qnt.md) */

int qnt_Nearest(unsigned int r, unsigned int g, unsigned int b)
{
    unsigned int i, d, best;
    int bi;

    r = (r & 0xf8) + 4;
    g = (g & 0xf8) + 4;
    b = (b & 0xf8) + 4;
    best = 200000;
    for (i = 0; i < qnt_HeapCount; i++) {
        d = qnt_SqPtr[qnt_PalR[i] - r] + qnt_SqPtr[qnt_PalB[i] - b] + qnt_SqPtr[qnt_PalG[i] - g];
        if (d < best) {
            best = d;
            bi = i;
        }
    }
    return bi;
}
