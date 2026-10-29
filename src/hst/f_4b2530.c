/* 0x4b2530, leaked name (its messages): runs every word of text k (or a string above 0x4800) through the text
   commands of hst_CommandNames / hst_CommandCallbacks; a leading "[_SET_n" / "[_USE_n" selects list n of hst_Lists (it
   must be in use)
   @deep only allocation left (317/317 instructions, 82 differing: 30 slots + 120 code hunks) plus a Q4 block. Meaning read equal: the two AI_ZERO clears (buf 0x17d0, word 0x100), the k < 0x4800 / strlen 0x17c0 / strncpy path and the string-copy else, the '[_SET' / '[_USE' prefix compare (hst_ListModes 0x601b00), word[0] = buf[6] + atoi + the 0..3 range check, row = &hst_Lists[n] + row->inUse, p = buf + 7 / p = buf, the word loop (space starts a command, slot 8 or atoi for the _USE prefix, q = word + 8 / word + 2, the 27-name loop over hst_CommandNames and the hst_CommandCallbacks call with q + strlen), the per-word re-clear and the tail command. What is left is the AI_ZERO temp allocation (ours gives _zn the dead parameter slot [ebp+0x8] and _zp [ebp-0x8], the original gives both their own slots) and the variable slot pair [-0x4]/[-0x8] swapped (slot / n / i order) - the same AI_ZERO problem as hst_ParseContext and the ai_Token* @deep reasons. Q4 block: the 8 '*(int *)(p + 1) == *(int *)hst_ListModes[n]' 4-byte prefix compares; a memcmp(p + 1, hst_ListModes[n], 4) spelling adds a call and was measured WORSE (it also lost the byte match of hst_ParseCommandline_1st_Pass), so it needs a typed 4-byte view of the mode string (shared-header decision). */

int (*hst_TokenCallbacks[15])(int mode, G12ARG *args, int k, char *fmt, char *out) = { ai_TokenMayor, ai_TokenBishop, ai_TokenRandomGuildMaster, ai_TokenRandomOfficeHolderFemale, ai_TokenRandomOfficeHolder, ai_TokenRandomOfficePerson, ai_TokenRichestInhabitant, ai_TokenBestInn, ai_TokenMoney, ai_TokenCityTreasury, ai_TokenRandomChurchProfession, ai_TokenRandomRich, ai_TokenRandomNpcInhabitant, ai_TokenRandomMerchant, ai_TokenRandomPlayer };  /* 0x601b10 */
int (*hst_CommandCallbacks[27])(G12ROW *row, int slot, char *fmt) = { ai_CommandFest, ai_Unk4afe80, ai_Unk4aff30, ai_CommandFire, ai_CommandTornado, ai_CommandCityTreasury, ai_CommandWealthTax, ai_CommandWealth, ai_CommandOfficeStanding, ai_CommandKillPlayer, ai_CommandExecution, ai_CommandPrison, ai_CommandOfficeInventoryPlus, ai_CommandLaw, ai_CommandLawChange, ai_CommandFaithChange, ai_CommandRiot, ai_Unk4b1060, ai_CommandDemand, ai_Unk4b1600, ai_Unk4b16e0, ai_Unk4b17c0, ai_Unk4b1880, ai_Unk4b1950, ai_Unk4b1a20, ai_Unk4b1bd0, ai_CommandInventoryPlus };  /* 0x601b50 */

int hst_ParseCommandline_2nd_Pass(int k)
{
    char buf[0x17d0];
    char word[0x100];
    G12ROW *row;
    char *p, *w, *q;
    int started, slot, n, i;

    row = 0;
    if (!txt_Loaded)
        return 0;
    AI_ZERO(buf, 0x17d0);
    AI_ZERO(word, 0x100);
    if (k < 0x4800) {
        if (txt_Ptrs[k] == 0)
            return 0;
        if (strlen(txt_Ptrs[k]) > 0x17c0) {
            sprintf(word, "hst_ParseCommandline_2nd_Pass() failed: Label %i is too long", k);
            return 0;
        }
        strncpy(buf, txt_Ptrs[k], 0x17c0);
    } else {
        char *s = (char *)k, *d = buf;

        while ((*d++ = *s++) != 0)
            ;
    }
    if (*(int *)(buf + 1) == *(int *)hst_ListModes[0] || *(int *)(buf + 1) == *(int *)hst_ListModes[1]) {
        word[0] = buf[6];
        n = atoi(word);
        if (n <= -1 || n >= 4) {
            sprintf(word, "hst_ParseCommandline_2nd_Pass() failed: Invalid GROUP reference in Label %i",
                    k - hst_TextFirst);
            return 0;
        }
        row = &hst_Lists[n];
        if (row->inUse == 0)
            return 0;
        p = buf + 7;
    } else
        p = buf;
    AI_ZERO(word, 0x100);
    started = 0;
    w = word;
    while (*p != 0) {
        if (*p == ' ') {
            if (started) {
                slot = 8;
                if (*(int *)(word + 1) == *(int *)hst_ListModes[1]) {
                    slot = atoi(word);
                    q = word + 8;
                } else
                    q = word + 2;
                for (i = 0; i < 27; i++) {
                    if (memcmp(q, hst_CommandNames[i], strlen(hst_CommandNames[i])) == 0)
                        break;
                }
                if (i < 27)
                    hst_CommandCallbacks[i](row, slot, q + strlen(hst_CommandNames[i]));
                AI_ZERO(word, 0x100);
                started = 0;
                w = word;
                continue;
            }
        } else {
            if (!started)
                started = 1;
            *w++ = *p;
        }
        p++;
    }
    if (started) {
        slot = 8;
        if (*(int *)(word + 1) == *(int *)hst_ListModes[1]) {
            slot = atoi(word);
            q = word + 8;
        } else
            q = word + 2;
        for (i = 0; i < 27; i++) {
            if (memcmp(q, hst_CommandNames[i], strlen(hst_CommandNames[i])) == 0)
                break;
        }
        if (i < 27)
            hst_CommandCallbacks[i](row, slot, q + strlen(hst_CommandNames[i]));
    }
    return 1;
}
