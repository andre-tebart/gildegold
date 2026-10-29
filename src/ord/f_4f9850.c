/* 0x4f9850, ord_ReportStatistics, in a game year >= 8 divisible by 4: averages the last four rounds of the
   statistics history (ord_StatisticsRounds[12..15]) into a = the share without the second object, b = the share
   without the first object, c = the combined score and d = the overall score; rates a / b / c against 0.15 (the
   texts 0x1c11 .. 0x1c14 when the share is low, 0x1c15 .. 0x1c18 when it is high) and the income rating of
   gm_IncomeRating (0x4f9ec0) against 0 (0x1c13 / 0x1c17). The lowest of a / b / c and the rating (k, the later
   column on a tie: the tests are <=) above 0.2 picks the tier phrase: below 0.3 the base text id ids[k], below
   0.5 ids[k + 2], else ids[k + 4], formatted as ids[x] and ids[x] + 1 with "%s%s"; the four base ids are 0x1bf5 /
   0x1bfb / 0x1c01 / 0x1c07. d above 0.55 formats the sentence 0x1c0d (ending 0x1c0e), below 0.45 the sentence
   0x1c0f (ending 0x1c10), and sends it (flag 1) and then the tier phrase to every player character (kind 6, its
   netId) through he_ShowNews (0x53dda0, event 0x5f8); with a d in between the tier phrase goes out alone.
   @deep meaning settled, only allocation left. Read 0x4f9850 hunk by hunk against tools/disasm: the year gate is ai_TimeYear >= 8 and % 4, the four averages are the last four history rounds (noSecond / noFirst / combined / score), the text tests are <= 0.15 (test ah,0x41; jnp) and v >= 0.0 (QWORD zero), the min tests are <= (a tie takes the later column), the tier ids are ids[k] / ids[k+2] / ids[k+4], and d has two separate ifs (0x1c0d / 0x1c0f) each with its own copy of the 768-record news loop plus the any tail. Kept fixes: frame 0x428 (two 0x200 buffers), % 4, txt0/1/3 polarity, v >= 0.0, the <= min tests, the two-if d structure, the index loops (no pre-check, as the original), typed ORDSTATROUND and netId (Q4), ai_TimeYear (Q3), description (Q5/Q6). Remaining 3 code hunks are allocation: 4f9907 (17) ours homes the income rating with fst [esp+0x20] where the original keeps it in st0 (fcom without pop) and never stores it; 4f997f (80) ours keeps the running minimum m in st0 (single test ah,5; jp) where the original keeps m in [esp+0x10] and emits and eax,0x4100; jne plus the reverse compare (fld a; fcomp b; test ah,5; jnp) per <= test; 4f9a28 (20) the tier txt_Format call order from the buffer slot swap. Tried and measured (no change): m declared first / last, the two buffers in either declaration order and under four name pairs (the allocator always puts the sentence at [esp+0x38] and the tier text at [esp+0x238]; the original has them the other way round), the first min test as b < a, < vs <= (kept <=), v >= 0 as int (1 B worse). Next: slotmap / c2prio on m and v and the buffer order. */

void ord_ReportStatistics(void)
{
    float a, b, c, d, v, m;
    int txt0, txt1, txt2, txt3, k, i;
    int ids[4];                         /* the base text id of the four columns; the higher tiers are the entries
                                           2 / 4 further, as the original indexes them (the tie k on a 4-element
                                           table is the original's own indexing, kept for the byte check) */
    char sentence[0x200];
    char tierText[0x200];
    int any;

    if (ai_TimeYear < 8 || ai_TimeYear % 4 != 0)
        return;
    a = (ord_StatisticsRounds[12].noSecond + ord_StatisticsRounds[13].noSecond
         + ord_StatisticsRounds[14].noSecond + ord_StatisticsRounds[15].noSecond) * 0.25f;
    b = (ord_StatisticsRounds[12].noFirst + ord_StatisticsRounds[13].noFirst
         + ord_StatisticsRounds[14].noFirst + ord_StatisticsRounds[15].noFirst) * 0.25f;
    c = (ord_StatisticsRounds[12].combined + ord_StatisticsRounds[13].combined
         + ord_StatisticsRounds[14].combined + ord_StatisticsRounds[15].combined) * 0.25f;
    d = (ord_StatisticsRounds[12].score + ord_StatisticsRounds[13].score
         + ord_StatisticsRounds[14].score + ord_StatisticsRounds[15].score) * 0.25f;
    v = gm_IncomeRating();
    txt0 = a <= 0.15 ? 0x1c11 : 0x1c15;
    txt1 = b <= 0.15 ? 0x1c12 : 0x1c16;
    txt2 = v >= 0.0 ? 0x1c13 : 0x1c17;
    txt3 = c <= 0.15 ? 0x1c14 : 0x1c18;
    k = 0;
    any = 0;
    m = a;
    if (b <= m) {
        k = 1;
        m = b;
    }
    if (v <= m) {
        k = 2;
        m = v;
    }
    if (c <= m) {
        k = 3;
        m = c;
    }
    if (m > 0.2) {
        ids[0] = 0x1bf5;
        ids[1] = 0x1bfb;
        ids[2] = 0x1c01;
        ids[3] = 0x1c07;
        if (m < 0.3)
            k = ids[k];
        else if (m < 0.5)
            k = ids[k + 2];
        else
            k = ids[k + 4];
        txt_Format(tierText, "%s%s", k, k + 1);
        any = 1;
    }
    if (d > 0.55) {
        txt_Format(sentence, 0x1c0d, txt0, txt1, txt2, txt3, 0x1c0e);
        for (i = 0; i < 768; i++)
            if (ORD_CHARS[i].kind == 6
                && (he_ShowNews(ORD_CHARS[i].netId, -1, sentence, 1, 0x5f8, 0), any))
                he_ShowNews(ORD_CHARS[i].netId, -1, tierText, 0, 0x5f8, 0);
        return;
    }
    if (d < 0.45) {
        txt_Format(sentence, 0x1c0f, txt0, txt1, txt2, txt3, 0x1c10);
        for (i = 0; i < 768; i++)
            if (ORD_CHARS[i].kind == 6
                && (he_ShowNews(ORD_CHARS[i].netId, -1, sentence, 1, 0x5f8, 0), any))
                he_ShowNews(ORD_CHARS[i].netId, -1, tierText, 0, 0x5f8, 0);
        return;
    }
    if (any) {
        for (i = 0; i < 768; i++)
            if (ORD_CHARS[i].kind == 6)
                he_ShowNews(ORD_CHARS[i].netId, -1, tierText, 0, 0x5f8, 0);
    }
}
