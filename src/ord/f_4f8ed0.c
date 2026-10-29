/* 0x4f8ed0, recomputes the statistics (0x4f8ae0) into the record 0x91b4c0 (10 floats), smooths the rating out[5] into the floats 0x6f1b9c / 0x6f1ba0 (first time: 0x6f1b9c = 0.95 * rating), stores the time stamp and sends the
   message 0x4c5800 with the record, the two floats, the change rate 0x91aaf4 and the clock. ROUGH
   @deep far draft (119 code / 4 fill hunks, 183 of 288 bytes; ROUGH from the layout session). Read against tools/asmdiff 0x4f8ed0: the original fuses the rating smoothing into the FPU stack (fst / fld / fmul QWORD with the 0.95 double constant, the 0.5 factor and the fsub/fmul/fadd chain) where ours reloads the globals; it copies the 10-float record with rep movs before the smoothing (ours after the first-value test), keeps cur / base in edi / edx and builds the 15-int message in place. The statements (ord_ComputeStatistics(st), first = ai_RatingBaseF == 0.0f, memcpy of the record, the first / else rating update on ai_Rating / ai_RatingBaseF, base / cur, ord_RatingRate = (cur - base) * 0.0007336182f, the 15-int m build with m[5] / m[6] the two floats, m[10] the rate, the 14-byte time stamp copied to ord_StatisticsTime and into m[11..14], cm_SendMessage41(m)) are the same, but the float / double spellings (0.95f vs 0.95 in the original's fmul QWORD) and the block order were not settled, so the ROUGH marker stays and that is the first lead. Q4 left: 14 raw accesses - the ai_Time byte copies (gv.h has no AITIME view; ai.h cannot be included), the ((float *)m)[5] / ((int *)st)[7] record/message views (needs one named 15-int message view) and the float bits of ai_RatingBase where the F view does not fit. Done here: Q1 name kept, Q2 clean (cm_SendMessage41 for 0x4c5800 = cm.c's 'message 0x41', ord_StatisticsTime for 0x91b470), Q4 down from 23 to 14 and the byte diff 4932 -> 4894 by giving 0x91aaf4 (ord_RatingRate, float) and 0x6f1ba0 (ai_Rating) and the float view of 0x6f1b9c (ai_RatingBaseF) their own names instead of char-pointer arithmetic. */

void ord_UpdateStatistics(void)
{
    float st[10];
    int first;
    int m[15];
    float cur, base;

    ord_ComputeStatistics(st);
    first = ai_RatingBaseF == 0.0f;
    memcpy(ord_StatisticsRecord, st, 0x28);
    if (first) {
        ai_Rating = st[5];
        ai_RatingBaseF = st[5] * 0.95f;
    } else
        ai_Rating = (st[5] - ai_Rating) * 0.5f + ai_Rating;
    base = ai_RatingBaseF;
    cur = ai_Rating;
    ord_RatingRate = (cur - base) * 0.0007336182f;
    memcpy(m, st, 0x14);
    ((float *)m)[5] = cur;
    ((float *)m)[6] = base;
    m[7] = ((int *)st)[7];
    m[8] = ((int *)st)[8];
    m[9] = ((int *)st)[9];
    m[10] = *(int *)(ord_StatisticsHistory - 0xc);
    *(int *)ord_StatisticsTime = *(int *)ai_Time;
    *(int *)(ord_StatisticsTime + 4) = *(int *)(ai_Time + 4);
    *(int *)(ord_StatisticsTime + 8) = *(int *)(ai_Time + 8);
    *(short *)(ord_StatisticsTime + 0xc) = *(short *)(ai_Time + 0xc);
    m[11] = *(int *)ai_Time;
    m[12] = *(int *)(ai_Time + 4);
    m[13] = *(int *)(ai_Time + 8);
    *(short *)(m + 14) = *(short *)(ai_Time + 0xc);
    cm_SendMessage41(m);
}
