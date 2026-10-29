/* 0x51c870, the table 0x7604e8 (three words per entry at +0x18 words apart; the last entry is followed by the two further blocks 0x5dbfc0 and 0x4ed480) @deep B3 code hunks 50 after the LS_W restructure (the draft mixed if(fwrite!=0) and direct calls; the original is the LS_W macro for all five writes): the rest is VC6's induction-variable base - the original keeps esi at the entry pointer 0x7604e8 (lea [esi-0x60]/[esi]/[esi+0x68]/[esi+0xc8], add esi,4, cmp 0x760548) while ours bases esi at 0x760550 (the +0x68 word) with offsets -0xc8/-0x68/0/+0x60, same addresses, and VC6 then merges the loop's return-0 blocks to the function end where the original reuses the first check's inline epilogue at 0x51c88c. Read the whole function (24 entries of four words, then 0x5dbfc0 and 0x4ed480); tried do-while and while spellings. Q1/Q2/Q3/Q5 open (0x7604e8 table unnamed, callees ai_Unk5dbfc0/unk_Unk4ed480 undeclared here) */


int unk_Unk51c870(int f)
{
    int *p;

    LS_W((char *)(unsigned)&ai_Unk760610, 4)
    p = (int *)(unsigned)&full_data_7604e8;
    do {
        LS_W(p - 0x18, 4)
        LS_W(p, 4)
        LS_W(p + 0x1a, 4)
        LS_W(p + 0x32, 4)
        p = p + 1;
    } while ((int)p < (unsigned)((unsigned)full_data_7604e8 + 0x60));
    if (ai_Unk5dbfc0(f) != 0)
        return unk_Unk4ed480(f) != 0;
    return 0;
}
