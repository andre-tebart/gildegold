/* 0x511ad0, "init_LocalData": selects the town universe, makes the player records (0x4feb30 / 0x4feb60) unless the flag 0x40 is set, finds the two judges (the characters of the kind 0xc with the byte 0 / 1 at +0x228: the "Richter Katholisch" / "Richter Evangelisch", stored in 0x6f1bfc / 0x6f1c00) and the list of the characters of the kind 0xb (0x6f1c04); a missing judge is an error; names the buildings (0x511890) unless the flags 0x44 are set; then 0x50cc60 @deep behaviour: 102 code hunks / 124 of 256 bytes, meaning read whole against 0x511ad0..0x511bc0 (the scene select, the player-record loop over the slot table, the two kind 0xc judge scans with the +0x228 byte 0 / 1, the kind 0xb collector walk, the two error texts, the name pass, the final call) - no meaning error left. Fixed this session: Q3 (0x7e4b10 -> INIT_PLAYER_SLOTS_END, 0x7d5f8c -> INIT_CHARACTERS_END + 0xc, 0x7d5f80 -> INIT_CHARACTERS_END) and Q7 with the real void gm_ResetStock(void) prototype, which is also the behaviour fix of the tail: the original ends in a tail jump (jmp 0x50cc60 at 0x511bb6), which VC6 only emits with the prototype (the implicit int declaration gave call + ret). Left (allocation / codegen): the judge scan's registers - the original loads only ai_JudgeProtestant into edi before the loop and keeps n in edx (test edx,edx / dec edx), ours keeps n in ebp (push ebp/pop ebp) and both snapshots inside the loop (mov ecx,ds:X / mov edx,esi), and the original tests p[-10] in memory (cmp BYTE [eax-0xa],0xc) where ours loads it into dl first; the kind 0xb walk's collector pointer: the original keeps q in ecx with the kind 0xb in dl, ours uses esi; the pop edi / pop esi placement before the two judge tests. 82 +/- lines in the diff, every one read - all register or slot, same instructions. */


void init_LocalData(void)
{
    char *p;
    char *j1, *j2;
    int n, i;
    char **q;
    char *c;

    d3if_SelectScene(0, 0);
    if ((ai_GameFlags & 0x40) == 0) {
        gm_UpdatePlayers();
        i = 0;
        p = ai_PlayerSlotsV[0];
        do {
            if (*p == 0)
                break;
            gm_InitPlayerRecord(i);
            p += 0x2f4;
            i++;
        } while ((int)p < INIT_PLAYER_SLOTS_END);
    }
    n = 2;
    p = ai_Chars[0] + 0x228;
    do {
        j1 = ai_JudgeCatholic;
        j2 = ai_JudgeProtestant;
        if (n == 0)
            break;
        if (p[-10] == 0xc) {
            if (*p == 0)
                ai_JudgeCatholic = p - 0xc;
            else {
                if (*p != 1)
                    goto next;
                ai_JudgeProtestant = p - 0xc;
            }
            n--;
        }
    next:
        p += 0x21c;
        j1 = ai_JudgeCatholic;
        j2 = ai_JudgeProtestant;
    } while ((int)p < INIT_CHARACTERS_END + 0xc);
    q = ai_KindBChars;
    c = ai_Chars[0];
    ai_JudgeCatholic = j1;
    ai_JudgeProtestant = j2;
    do {
        if (c[2] == 0xb) {
            *q = c;
            q++;
        }
        c += 0x21c;
    } while ((int)c < INIT_CHARACTERS_END);
    if (j1 == 0) {
        err_Message("init_LocalData(): gm_RichterKatholisch==NULL");
        j2 = ai_JudgeProtestant;
    }
    if (j2 == 0)
        err_Message("init_LocalData(): gm_RichterEvangelisch==NULL");
    if ((ai_GameFlags & 0x44) == 0)
        init_NameBuildings();
    gm_ResetStock();
}
