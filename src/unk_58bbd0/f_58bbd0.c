/* 0x58bbd0, the production window's table init (400 B): the five-entry table 0x6f4f08 (-1 / -1 every 0x14 B) and
   0x6f52e8 (0 / -1); the eight 0x58-byte records 0x6f5334 + i * 0x58 (i = 0 .. 7, the id 0x10 + i * 0x78 and the
   slot values -1); the 0xe-entry records 0x6f5a84 + i * 0x38 (the id 0x10 + i * 0x45, +4 = 0x1a, -1 fields); the
   0xe-entry grids 0x6f4f64 and 0x6f58a4 (x = (i % 4) * 0x40 + 0x10, y = (i / 4) * 0x40 + 0x10 / 0xd0)
   @deep B: 231 code hunks left after the two meaning fixes the original's code proves (the two grid loops compute x with the signed % 4 idiom (and eax,X/jns/dec/or/inc) and y with the signed / 4 (cdq/and edx,3/add/sar), not n & 3 / n >> 2; the note already says i % 4 / i / 4). What remains is code shape: the table walks compare with jl in the original (signed) and jb here (pointer compare), the 0x58-byte record init unrolls the 9 -1 stores in the original where VC6 makes rep stos of our for loop, and the field stores interleave with the loop increments differently (the original writes p[4]=0x10, p[5]=n before the -1 block and p[14] after the pointer bump). All hunks read; no other missing statement or constant found. Q1/Q2/Q3/Q4 (raw 0x6f4f08 addresses, U_I) also open */

void ai_Unk58bbd0(void)
{
    int *p;
    int *q;
    int i;
    int n;

    for (p = (int *)(unsigned)&full_data_6f4f10; p < (int *)(unsigned)((unsigned)full_data_6f4f10 + 0x50); p += 5) {
        p[-2] = -1;
        p[0] = -1;
    }
    for (p = (int *)(unsigned)&full_data_6f52f0; p < (int *)(unsigned)((unsigned)full_data_6f52f0 + 0x28); p += 5) {
        p[-2] = 0;
        p[0] = -1;
    }
    n = 8;
    for (p = (int *)(unsigned)&full_data_6f5334; n < 0x788; p += 0x16) {
        p[-5] = 0;
        for (q = p, i = 4; i != 0; i--) {
            q[-4] = 0;
            q[0] = -1;
            q++;
        }
        p[4] = 0x10;
        p[5] = n;
        for (i = 6; i < 0xf; i++)
            p[i] = -1;
        n += 0x78;
    }
    U_I((unsigned)&full_data_6f5a60) = 0;
    for (p = (int *)(unsigned)&full_data_6f5a84, n = 0x10; n < 0x460; p += 0xe) {
        p[2] = n;
        p[-1] = 0;
        p[0] = -1;
        p[1] = 0x1a;
        p[3] = -1;
        p[4] = -1;
        p[5] = -1;
        p[6] = -1;
        p[9] = -1;
        p[7] = -1;
        p[8] = -1;
        p[0xc] = -1;
        n += 0x45;
    }
    for (p = (int *)(unsigned)&full_data_6f4f64, n = 0; p < (int *)(unsigned)((unsigned)full_data_6f4f64 + 0x380); p += 0xe, n++) {
        p[-1] = 0;
        p[0] = -1;
        p[1] = (n % 4) * 0x40 + 0x10;
        p[3] = -1;
        p[4] = -1;
        p[5] = -1;
        p[6] = -1;
        p[2] = (n / 4) * 0x40 + 0x10;
        p[9] = -1;
        p[7] = -1;
        p[8] = -1;
        p[0xc] = -1;
    }
    for (p = (int *)(unsigned)&full_data_6f58a4, n = 0; p < (int *)(unsigned)((unsigned)full_data_6f58a4 + 0x1c0); p += 0xe, n++) {
        p[-1] = 0;
        p[0] = -1;
        p[1] = (n % 4) * 0x40 + 0x10;
        p[3] = -1;
        p[4] = -1;
        p[5] = -1;
        p[6] = -1;
        p[2] = (n / 4) * 0x40 + 0xd0;
        p[9] = -1;
        p[7] = -1;
        p[8] = -1;
        p[0xc] = -1;
    }
}
