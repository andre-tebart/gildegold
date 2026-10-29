/* 0x4f9c00, advances the float 0x6f1b9c by the time since the stamp 0x91b470 (14 bytes, the layout of the clock 0x770ae0): 0x4fe280 (stamp, clock) gives the elapsed steps k; no step: the stamp is set to the clock; otherwise, when the float 0x6f1ba0 is not 0 and the clock's hour (word at +4) is between 7 and 0x14, 0x6f1b9c += k * the float 0x91aaf4, clamped to 0x6f1ba0 * 0.25, and the stamp = the clock */

void ord_AdvanceRatingBase(void)
{
    int k;
    double lim;

    k = gm_MinutesBetween(ord_StatisticsTime, ai_Time);  /* @original: gv.h's shared K&R declaration (the call
         pushes the two clock pointers, 0x4f9c01 push 0x770ae0 / 0x4f9c06 push 0x91b470); a prototype in ord.c
         is a lint cross-TU conflict with he.c / sv.c / gv.c, which call it with an int address literal */
    if (k == 0) {
        ord_StatisticsClock.year = ord_GameClock.year;
        ord_StatisticsClock.hourWord = ord_GameClock.hourWord;
        ord_StatisticsClock.minuteWord = ord_GameClock.minuteWord;
        ord_StatisticsClock.stamp = ord_GameClock.stamp;
        return;
    }
    if (ai_Rating != 0 && (unsigned short)ord_GameClock.hourWord > 6
        && (unsigned short)ord_GameClock.hourWord < 0x15) {
        ai_RatingBaseF = k * ord_RatingRate + ai_RatingBaseF;
        lim = ai_Rating * 0.25;
        if (ai_RatingBaseF < lim)
            ai_RatingBaseF = (float)lim;
        ord_StatisticsClock.year = ord_GameClock.year;
        ord_StatisticsClock.hourWord = ord_GameClock.hourWord;
        ord_StatisticsClock.minuteWord = ord_GameClock.minuteWord;
        ord_StatisticsClock.stamp = ord_GameClock.stamp;
    }
}
