/* 0x5d73e0, is there a character that can be quicksaved (78 B): 0 -> no game (ai_GameState 0x6f25ac
   == 0); otherwise walks the character records (0x770b82, ai_Chars + 2, stride 0x21c) for one with the
   record flag +6 > 0, the kind +0 in {6, 7} and the dword +0x20a != -1; 0 if found, 1 when the walk
   passes gm_PlayerRecordBytes 0x7d5f82.
   @deep behaviour: 41 code hunks against the 78-byte original, 37 vs 35 instructions; the differences are VC6 register allocation (our state byte al + pointer eax, the original loads cl before mov eax,1 and keeps the pointer in ecx) and constant scheduling (our -1 materialized in edx with or edx,-1 where the original compares with the imm8 -1), not a missing statement: the record walk, the p[6] > 0 unsigned test (now jbe as the original), the kind 6/7 test and the -1 sentinel test all match. Meaning-class leads tried: the do-while pointer walk form, the return-1 placement (early constant, late constant, variable), the state test spelling (U_B(0x6f25ac) == 0 vs > 0), the p[6] > 0 vs != 0 (fixed one instruction). tools/spell.py: 38 of 72 differing, no hunk matched the menu - the allocation/scheduling byte work is match-deep's. Q4 blocked: the only raw access left is U_I(p + 0x20a), the dword at character +0x20a that this function tests for -1; no evidence for its meaning anywhere (ai.h's AICHAR pads 0x1e8..0x210, the only other reader is the raw save slice of ls.c 0x51a5b4), so a typed view field would be an invented name (Q2 flags unk20a). Q1/Q2/Q3/Q6 are done (ai_HasQuicksavePlayer, ai_GameState, ai_Chars + 2, gm_PlayerRecordBytes 0x7d5f82). */

int ai_HasQuicksavePlayer(void)
{
    unsigned char *p;

    if (ai_GameStateB == 0)
        return 0;
    p = (unsigned char *)ai_CharsC + 2;
    do {
        if (p[6] > 0 && (p[0] == 6 || p[0] == 7) && U_I(p + 0x20a) != -1)
            return 0;
        p += 0x21c;
    } while ((int)p < (int)gm_PlayerRecordBytes);
    return 1;
}
