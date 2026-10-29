/* 0x4e7aa0, creates an applicant for the office kind k: rolls the job group (1 .. 12) and the rank (the range i6 .. i7) from
   the ranges of amt_GetCreateRanges, sends the create-character message (cm_SendCreateCharacter, character kind 0x13) with
   them and the title of the group / rank (gm_TitleNumber) and returns its handle; the alternating byte 0x60f118 (0 / 1, starts
   at 2 = random) is the last argument and flips afterwards. The age is rolled from the two overlapping dwords at +0 / +2 of the
   range record, as the original does (its shorts read as dwords), so it is not the age the range means
   @deep B3 71 code hunks + 1 fill (46 differing of 68 instructions, asmdiff). The whole original 0x4e7aa0..0x4e7b5b was read and the meaning is complete and equal: the amt_GetCreateRanges guard (return -1), the amt_Flip == 2 roll (Random(2) & 0xffff), the rank roll Random(r.i7 - r.i6 + 1) + r.i6, the group roll Random(12) & 0xffff + 1, gm_TitleNumber(group, rank), the age roll from the two overlapping dwords at +0 / +2 of the range record (the original reads the shorts as dwords; typed as AMTRANGEW), cm_SendCreateCharacter(0x13, -1, -1, age, -1, title, 0, amt_Flip) and amt_Flip = 1 - amt_Flip. Leads tried one edit one build each: (1) the callee's declared types - the 8th parameter unsigned char (gives the original's mov cl,[amt_Flip] instead of mov ecx/[and ecx,255]) and the 4th int instead of void* (no truncation; 103 -> 77 differing bytes, 61 -> 46 asm lines, kept); (2) the second roll written inside the argument list (e = gm_TitleNumber((x = util_Random(12)) + 1, v)) - no change, reverted. spell.py menu: only the 4e7ae2 width entry (unsigned short x) - x cannot become int, the original masks twice with and eax,0xffff. Left, all allocation: the original needs only esi (r.i6 in esi, v in eax pushed before the second util_Random) while we keep r.i6 in edi (an extra push edi/pop edi) and v in esi across the second call, and later [r+0] in edi / the title in esi vs the original's esi / eax - register allocation and the argument evaluation order (v pushed early), c2prio/slotmap. Q1-Q7 pass (named from the two-person event 0x552920 that creates its two applicants for the office amt_GetOpenOffice picked; gm_TitleNumber called under its canonical name, the overlapping reads typed as AMTRANGEW). */

int amt_CreateApplicant(int k)
{
    AMTRANGE r;
    int e, v;
    unsigned short x;

    if (amt_GetCreateRanges(k, &r) == 0)
        return -1;
    if (amt_Flip == 2)
        amt_Flip = util_Random(2) & 0xffff;
    x = util_Random(r.i7 - r.i6 + 1);
    v = x + r.i6;
    x = util_Random(12);
    e = gm_TitleNumber(x + 1, v);
    x = util_Random(((AMTRANGEW *)&r)->word1 - ((AMTRANGEW *)&r)->word0 + 1);
    v = ((int (*)(int, int, int, int, int, int, int, unsigned char))cm_SendCreateCharacter)(0x13, -1, -1, x + ((AMTRANGEW *)&r)->word0, -1, e, 0, amt_Flip);
    amt_Flip = 1 - amt_Flip;
    return v;
}
