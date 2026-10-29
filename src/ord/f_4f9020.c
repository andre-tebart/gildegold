/* 0x4f9020, restores the statistics record from a saved record of 15 words (the fields of ORDSAVE): the five
   columns, ai_Rating / ai_RatingBaseF, the three values, the rate and the clock stamp; then rebuilds the rating
   bytes 0x91b460[0..4] from the characters' skill bytes +0x80..+0x84 (kind below 10, health above 0) and [5] from
   the weighted level +0xd, and runs ord_ReportStatistics. The inverse of ord_UpdateStatistics (0x4f8ed0).
   @deep readable, 69 asm / 160 B left; allocation only: frame 0x18 vs 0x1c (7 vs 8 slots, the original zeroes a slot at 0x8), push ebx at entry vs after fld p[6], the walk base is health +8 vs kind +2, epilogue order; tried and kept the clock struct copy, packed ORDSTIME, q = p + 11 base, ORDCHARKIND walk, declaration order; reverted the health pointer; next slotmap / vargen */

void ord_LoadStatistics(void *p)
{
    ORDSAVE *s = p;
    ORDSTIME *clk;
    ORDCHAR *c;
    ORDCHARKIND *k;
    float rating, base;
    int sum[5];
    int n, w, i;

    ord_Statistics.col[0] = s->col[0];
    ord_Statistics.col[1] = s->col[1];
    ord_Statistics.col[2] = s->col[2];
    ord_Statistics.col[3] = s->col[3];
    ord_Statistics.col[4] = s->col[4];
    rating = s->rating;
    ord_Statistics.rating = rating;
    base = s->base;
    ord_Statistics.base = base;
    ord_Statistics.v1c = s->v1c;
    ord_Statistics.v20 = s->v20;
    ord_Statistics.v24 = s->v24;
    ord_RatingRateBits = s->rate;
    clk = (ORDSTIME *)((char *)p + 0x2c);
    ord_StatisticsClock = *clk;
    ai_RatingBaseF = base;
    ai_Rating = rating;
    n = 0;
    w = 0;
    for (i = 0; i < 5; i++)
        sum[i] = 0;
    for (k = ORD_CHARKINDS; k < ORD_CHARKINDS + 768; k++) {
        c = (ORDCHAR *)((char *)k - 2);
        if (c->id != 0xffff && k->kind < 10 && k->health > 0) {
            n++;
            w += k->level * 5;
            for (i = 0; i < 5; i++)
                sum[i] += k->stats[i];
        }
    }
    for (i = 0; i < 5; i++)
        ord_RatingBytes[i] = (char)(int)((double)sum[i] / (double)n);
    ord_RatingBytes[5] = (char)(int)((double)w / (double)n);
    ord_ReportStatistics();
}
