/* 0x4c2f30, a comment about the player's favour from the selected character of the party record a (a random
   variation r, 3 when the first check is clear): "SCHLECHTE_GUNST" / "MITTLERE_GUNST" / "GUTE_GUNST" by
   favour / 33. x is volatile because the original reloads it from its slot before every use (matching trick,
   do not clean).
   @deep 14 code hunks, one scheduling cluster only: the whole original 0x4c2f30..0x4c30df read against the source; ours differs only in the position of the pop esi (ours right after the two out calls, the original after the level test at 0x4c2fea), which shifts the esp offsets of out[1] and x by 4 in that one block; the next 102 instructions are equal. Meaning fixes done in this session: out is a 0xb8-byte unsigned array (the 0xbc frame: x at R-0xbc, out at R-0xb8), r is a 32-bit unsigned (the original's mov bp,ax + and ebp,0xffff), x is volatile (the original stores it to its slot and reloads it at every use - a matching trick, not cleaned), and the special-kind / generic calls are one ternary so that the compiler emits the original's single shared test tail. Readability debt: Q1 name, Q2 gm_BuildingGoodsList / gm_Unk5099e0 / gm_Liking, Q4 the four raw casts (the party record +0x34 / +0x200 and the character +0x16c). */

void ai_Unk4c2f30(char *a)
{
    volatile int x;
    unsigned out[0x2e];
    unsigned int r;
    short *c;
    char *g;
    int level;

    x = 0;
    r = util_RandomU(3);
    c = *(short **)(*(char **)(a + 0x34) + 0x200);
    if (c == 0)
        return;
    g = *(char **)((char *)c + 0x16c);
    level = (int)gm_Liking((unsigned short)ai_PlayerId, *(unsigned short *)(g + 0x27), 1) / 33;
    /* the three building kinds with their own work check, all others the generic one; only the first out value
       decides (a running order?) */
    if ((*g == 0x20 || *g == 0x1f || *g == 0x1e) ? gm_Unk5099e0(g, out, 0) : gm_BuildingGoodsList(g, out, 0)) {
        if (out[1] == 0)
            x = 1;
    }
    if (level == 0) {
        if (x)
            snd_PlayVoice((unsigned short)*c, snd_VoiceBankGreeting, "SCHLECHTE_GUNST", 3);
        else
            snd_PlayVoice((unsigned short)*c, snd_VoiceBankGreeting, "SCHLECHTE_GUNST", r);
    } else if (level == 1) {
        if (x)
            snd_PlayVoice((unsigned short)*c, snd_VoiceBankGreeting, "MITTLERE_GUNST", 3);
        else
            snd_PlayVoice((unsigned short)*c, snd_VoiceBankGreeting, "MITTLERE_GUNST", r);
    } else if (level >= 2) {
        if (x)
            snd_PlayVoice((unsigned short)*c, snd_VoiceBankGreeting, "GUTE_GUNST", 3);
        else
            snd_PlayVoice((unsigned short)*c, snd_VoiceBankGreeting, "GUTE_GUNST", r);
    }
}
