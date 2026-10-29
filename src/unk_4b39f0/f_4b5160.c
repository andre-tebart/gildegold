/* 0x4b5160, timed condition of mission x: not ok resets its time; ok starts it (once) and holds after min
   minutes */

#define G13_4FE1E0 ((void (*)(AITIME *, char, char, char))gm_SetTime)

int ai_TimedCondition(int x, int ok, int min)
{
    G13MIS *m = (G13MIS *)x;

    if (ok) {
        if (m->time.year == 0 && m->time.hour == 0 && *(int *)((char *)m + 0x0e) == 0
            && *(int *)((char *)m + 0x12) == 0) {
            m->time.year = ai_Time.year;
            G13_4FE1E0(&m->time, *((char *)&ai_Time + 4), *((char *)&ai_Time + 6), *((char *)&ai_Time + 0x0a));
        }
        return gm_MinutesBetween(&m->time, &ai_Time) >= min;
    }
    m->time.year = 0;
    gm_SetTime(&m->time, 0, 0, 0);
    return 0;
}
