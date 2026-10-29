/* 0x4f9d40, the weighted sum of the +0x18 ints (rec[6]) of the twelve table records 0x11 .. 0x1c of gv_GetTableRecord (0x4f2720) with the weights 0.06 0.05 0.08 0.04 0.13 0.1 0.13 0.12 0.14 0.15 and two zeros (the array is twelve long like the record range); the ints themselves are stored to out[0 .. 11] @deep meaning read and settled against tools/disasm 0x4f9d40; 40 asm / 172 B -> 19 asm / 45 B this session. Fixed: the weight table is one initializer (float w[12] = {twelve values}) - the original zero-fills w[10] / w[11] through eax when the zeros are the initializer's implicit tail, and stores the ten explicit weights as immediates; sum is declared and initialized before the table (sum = 0 as an immediate at [esp+0xc], not the register store the later statement gave). The frame (0x58), the rec[6] / w[i - 0x11] addressing, the gv_GetTableRecord(i, rec) call and the fild/fmul/fadd/fstp accumulation all match. Tried and reverted: the sum statement after the table (33 asm / 114 B), the w[10]/w[11] statements (the register-form zero stores disappear), the two body statements swapped (WORSE, 43 asm / 68 B). Left: one hunk, the loop's register and schedule - ours keeps i in esi and &w in edi (mov esi,0x11 / lea edi,[esp+0x34], fild after the store, fmul [edi-4]) where the original has i in edi and &w in esi (mov edi,0x11 / lea esi,[esp+0x34], fild before the out store, fmul [esi]); that is the esi/edi allocation plus the fild / fmul / add / inc order = match-deep. Readability Q1-Q7 pass. */

float ord_WeightedRecordSum(int *out)
{
    float sum = 0;
    float w[12] = {0.06f, 0.05f, 0.08f, 0.04f, 0.13f, 0.1f, 0.13f, 0.12f, 0.14f, 0.15f};
    int rec[9];
    int i;

    for (i = 0x11; i < 0x1d; i++) {
        gv_GetTableRecord(i, rec);
        out[i - 0x11] = rec[6];
        sum += (float)rec[6] * w[i - 0x11];
    }
    return sum;
}
