/* 0x4ac1a0, the text id of line n (at most 9) of record k: 0x1e2b + n, or 0x1e35 + n if textSet is set @deep code hunks 53: the original keeps n only in its argument slot and reloads it after the call (m in esi), ours holds n in edi across the call (extra push/pop); statements and result are equal, register allocation differs (match-deep) */

unsigned short hmt_RecordTextId(unsigned short k, unsigned short n)
{
    unsigned short m;
    G11REC *e;

    m = n;
    if (m >= 9)
        m = 9;
    e = hmt_GetRecord(k);
    if (e != 0) {
        if (e->textSet == 0)
            return m + 0x1e2b;
        return m + 0x1e35;
    }
    return n;
}
