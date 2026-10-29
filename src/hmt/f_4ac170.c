/* 0x4ac170, a random text offset 0..9 for record k, 0x1e2b if there is none @deep code hunks 39: the original loads e->textSet (+0x8) into ecx without using it and tail-calls util_Random(10) with 0x1e2b live in esi; our same-meaning source emits neither; the dead load and the live range are source-shape work (match-deep) */

unsigned short hmt_RandomTextOffset(unsigned short k)
{
    unsigned short ret;
    G11REC *e;

    ret = 0x1e2b;
    e = hmt_GetRecord(k);
    if (e != 0)
        return util_Random(10);
    return ret;
}
