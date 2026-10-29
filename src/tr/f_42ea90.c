/* 0x42ea90, allocates n records and fills them with the two marker strings. */
int tr_startup(unsigned short n)
{
    TRREC rec;
    TRREC *p;

    if (!n)
        return -1;
    if (tr_Table)
        return -2;
    tr_Table = mem_Alloc(n * sizeof(TRREC), "tr_startup");
    if (!tr_Table)
        return -3;
    tr_First = tr_Table;
    tr_End = tr_Table + n;
    TR_ZERO(&rec, sizeof(rec));
    strncpy(rec.key1, tr_Key1, 13);
    strncpy(rec.key2, tr_Key2, 13);
    for (p = tr_First; p < tr_End; p++)
        TR_COPY(p, &rec, sizeof(rec));
    return 0;
}
