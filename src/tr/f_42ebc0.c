/* 0x42ebc0, frees the record table tr_startup allocated and clears tr_Table/tr_First/tr_End;
 * int 3 when no table is open or it lies below tr_First (an assertion kept from the original) */

void tr_shutdown(void)
{
    if (tr_Table && tr_Table >= tr_First) {
        mem_Free(tr_Table);
        tr_First = NULL;
        tr_End = NULL;
        tr_Table = NULL;
    } else
        __asm int 3
}
