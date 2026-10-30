/* 0x4bb250, message for c with field 0x2c set: the text 0x4c + the season (gm_Season) of the calendar record
   gm_ClockToDate makes of the game clock
   @deep byte reached (the meaning fix: the calendar record is 10 bytes (f3gm.c gm_ClockToDate's out), not an int - the frame was 8 bytes short and the calls wrote past it; gm_ClockToDate(&t, d) with unsigned char d[10], then gm_Season(d)). Q1: no name evidence - the text is 0x4c + the season and the gate is AICHAR+0x2c != 0, a field with no role name anywhere (ai_Unk4bb150 has the same gate), so the message cannot be named. Q2: gm_ClockToDate / gm_Season are f3gm.c's gm_ClockToDate / gm_Season (same addresses) but declared locally in 5 TUs (sv.c and main.c even pass an int clock and a K&R form) - the project-wide rename needs the canonical prototypes in f3gm.h plus those callers fixed (a cross-unit shared-header decision). Q4: AICHAR+0x2c is in pad028[8] with no evidenced role; naming it unk2c would only move the debt. */

int ai_Unk4bb250(G14MSG *m)
{
    AICHAR *c;
    AITIME t;
    unsigned char d[10];                /* the calendar record gm_ClockToDate fills (f3gm.c's d[10]) */
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (*(int *)((char *)c + 0x2c) == 0)
        return 0x400;
    t = ai_TimeT;
    gm_ClockToDate(&t, d);
    txt_Format(buf, m->type + 0x12e7, gm_Season(d) + 0x4c);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
