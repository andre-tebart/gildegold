/* 0x5205f0, reads the table 0x7604e8 (the counterpart of 0x51c870): the count 0x760610 and the entries; from 0x10050 the two further blocks 0x5dc0c0 and 0x4ed910. Returns 1 @deep read against the original 0x5205f0..0x5206af: meaning verified - the 0x760610 count, the entry loop p = 0x7604e8 with the reads at p-0x60 / p / p+0x68 / p+0xc8 (our int offsets -0x18 / 0 / +0x1a / +0x32 are the same addresses) to p < 0x760548 (24 entries), then the version gate (our > 0x1004f is the original's jl 0x10050) and the two calls 0x5dc0c0(f) / 0x4ed910(f, ls_Version). Left is codegen: VC6 re-bases our loop pointer to p+0x68 (reads [esi-0xc8] / [esi-0x68] / [esi+0x60], condition lea eax,[esi-0x68]) where the original keeps esi = 0x7604e8 and reads [esi-0x60] / [esi] / [esi+0x68] / [esi+0xc8], plus the f register (edi in the original). Readability: Q1, Q2 (ai_Unk5dc0c0 / unk_Unk4ed910), Q3 (0x760610, 0x7604e8, 0x760548). */


int unk_Unk5205f0(int f)
{
    int *p;

    if (ls_Read((int)(unsigned)&ai_Unk760610, 4, 1, f) == 0)
        return 0;
    p = (int *)(unsigned)&full_data_7604e8;
    do {
        LS_R(p - 0x18, 4)
        LS_R(p, 4)
        LS_R(p + 0x1a, 4)
        LS_R(p + 0x32, 4)
        p++;
    } while ((int)p < (unsigned)((unsigned)full_data_7604e8 + 0x60));
    if (ls_Version > 0x1004f && ai_Unk5dc0c0(f) != 0 && unk_Unk4ed910(f, ls_Version) != 0)
        return 1;
    return 0;
}
