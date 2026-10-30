/* 0x4c2d20, now and then (40% of the calls) a comment of the selected workers (ai_Unk7603a0[0x20]) about
   their favour with the player: the average gm_Liking favour over the selected workers selects the text
   ("SCHLECHTE_GUNST" < 0x32, "MITTLERE_GUNST" < 0x4b, "GUTE_GUNST" above), spoken by the more numerous
   sex (w / m, the last selected woman / man), -5 (a neutral voice) when the preferred one is missing
   @deep 65 code hunks, same operation / scheduling only: the whole original 0x4c2d20..0x4c2f2f read against the source, every constant and callee corresponds (the 0.4 double gate, snd_LoadVoiceBank of ARBEITER_KOMMENTARE_BEFEHL_HANDWERKER.sbf, the 0x20 selected-worker scan at 0x7603a0 with the +0x188 selected and +9 sex bytes, men/women counts, the float favour sum truncated per step (fiadd + __ftol), f /= men+women, the 0x32/0x4b thresholds, the w/m choice and the -5 fallback, SCHLECHTE/MITTLERE/GUTE_GUNST). Left: ours CSEs snd_VoiceBankOrders into edx across the tail where the original compares memory (cmp ds:0x6f0cbc,esi) and reloads in every play branch; the original's out-of-line early-return block at 0x4c2e2a; the init store order (mov edi,ebx) and eax/edx choice for the men+women add. Readability debt after this session: Q2 ai_Unk7603a0 (now unsigned short *[0x20]), gm_Liking. */

void snd_PlayFavourComment(void)
{
    unsigned short *c;
    int w;
    int m;
    int f;
    int men;
    int women;
    int i;

    women = 0;
    men = 0;
    f = 0;
    w = 0xffff;
    m = 0xffff;
    if (util_RandomF() > 0.4)
        return;
    if (snd_VoiceBankOrders == 0)
        snd_VoiceBankOrders = snd_LoadVoiceBank("ARBEITER_KOMMENTARE_BEFEHL_HANDWERKER.sbf", 0);
    for (i = 0; i < 0x20; i++) {
        c = (unsigned short *)ai_Unk7603a0[i];
        if (c != 0 && (char)c[0xc4] != 0) {
            if (*((char *)c + 9) == 0) {
                men++;
                m = *c;
                f += gm_Liking(ai_PlayerId, m, 1);
            } else {
                women++;
                w = *c;
                f += gm_Liking(ai_PlayerId, w, 1);
            }
        }
    }
    if (men + women != 0)
        f /= men + women;
    if (f < 0x32)
        f = 0;
    else if (f < 0x4b)
        f = 1;
    else if (f > 0x4b)
        f = 2;
    if (men < women) {
        if (w != 0xffff) {
            if (f == 0)
                snd_PlayVoice(w, snd_VoiceBankOrders, "SCHLECHTE_GUNST", -1);
            else if (f == 1)
                snd_PlayVoice(w, snd_VoiceBankOrders, "MITTLERE_GUNST", -1);
            else if (f >= 2)
                snd_PlayVoice(w, snd_VoiceBankOrders, "GUTE_GUNST", -1);
            return;
        }
        if (m != 0xffff)
            return;
    } else if (m != 0xffff) {
        if (f == 0)
            snd_PlayVoice(m, snd_VoiceBankOrders, "SCHLECHTE_GUNST", -1);
        else if (f == 1)
            snd_PlayVoice(m, snd_VoiceBankOrders, "MITTLERE_GUNST", -1);
        else if (f >= 2)
            snd_PlayVoice(m, snd_VoiceBankOrders, "GUTE_GUNST", -1);
        return;
    }
    snd_PlayVoice(-5, snd_VoiceBankOrders, "MITTLERE_GUNST", -1);
}
