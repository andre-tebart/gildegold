/* 0x4e6030, the office of the character c (the table 0x925120, first 30 records, found by the id +4): copies the record to *office and the record of its kind
   (0x60ed58, 0x18 bytes) to *kind; 1 when found (c exists, +0x166 non-zero)
   @deep B3 80 code hunks + 3 fill, 55 differing of 50/49 instructions; the whole original disassembly (0x4e6030..0x4e60a8) was read against the source: same statement sequence (guard on c / id 0xffff / c[0x166], 30-record loop on amt_Table[i].owner, copy office then amt_Kinds[kind], return 1/0); the differences are the registers from instruction 0 (original c=ecx, i=edx, netId=esi, office/kind=edi, eax=return; ours c=ecx, byte=dl, ...) - allocation only. spell.py dup-tail: duplicate return 0 in the if and at the end, merged single return, early-return guard/polarity swap tried, one edit one build each, same 89/128 bytes and 55 lines every time. Readability Q1-Q7 pass (AMTCHARVIEW id/netId fields); the amt_FindOffice alias spellings in am.c/cm.c/gv.c/he.c/unk_58bbd0.c are the older type views (G5CH* /void*), merging them needs call-site casts (recorded lead) */

int amt_FindOffice(char *c, AMTKIND *kind, AMTREC *office)
{
    int i;

    if (c == 0 || ((AMTCHARVIEW *)c)->id == 0xffff || c[0x166] == 0)
        return 0;
    for (i = 0; i < 30; i++) {
        if (((AMTCHARVIEW *)c)->netId == amt_Table[i].owner) {
            *office = amt_Table[i];
            *kind = amt_Kinds[amt_Table[i].kind];
            return 1;
        }
    }
    return 0;
}
