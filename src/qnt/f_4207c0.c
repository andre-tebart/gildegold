/* 0x4207c0, qnt_R / G / B: the 8 bits of a channel spread to their bit positions in the octree node
   index (the three tables are or-ed into the finest-level index)
   @deep behaviour: register/width spellings only - grade.py code 72, fill 1; unitdiff 92 B / 35 asm lines (2026-10-26) (the original narrows the 0x20 / 0x40 terms to mov dl,al / mov cl,al and keeps a in esi, ours mov edx,eax / and edx,0x20; same instruction multiset, no meaning error); readable met; 125 casts + 1024 u variants tried (notes/units/qnt.md) */

void qnt_InitTables(void)
{
    int i, a, a2;

    for (i = 0; i < 256; i++) {
        /* the casts give the original's byte moves (mov cl,al / mov dl,al); two different types, since the same
           cast twice is shared as one value (and edx,0xff) */
        a = (((unsigned char)i & 0x80) << 2 | (char)i & 0x40) << 2 | i & 0x20;
        a2 = a << 2; /* shared by the r and g table, as in the original */
        qnt_R[i] = (a2 | i & 0x10) << 1 | i >> 1 & 4;
        qnt_G[i] = i >> 2 & 2 | a2 | i & 0x10;
        qnt_B[i] = i >> 3 & 1 | a << 1 | i >> 1 & 8;
    }
    for (i = -255; i <= 255; i++)
        qnt_Sq[i] = i * i;
    qnt_SqPtr = qnt_Sq;
}
