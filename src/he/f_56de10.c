/* 0x56de10, the run handler of the research event 0x98 (the random event of a character +0xc; ROUGH): -1 / -2 free, paused (+0x78 & 4) returns, a missing character / research record reschedules with 0x4c4c60 (-1, ev); the clock +0x52 = now + 1 day, the hour = the day's window (0x617830 / 0x617840 by (day + 1) % 4) plus a random part, the minute = random(4) * 15; with a pending 0x4ecd50 value (+0xac) it is applied and the handler rescheduled; else the task 0x4eb180 (building's +0x16c, 1) decides: <= the building's +0x65 +4 gives a random offset by the task kind (1 .. 4, negated; else positive), above it the scene action 0x56e070 (character, ev, building, task) runs; then 0x4ecd50 (character, offset) and 0x4c4c60 (0, ev) */

int ahm_ResearchState();
int ahm_SendAddValue();
float util_RandomF();
void he_Unk56e070();

void he_Unk56de10(int ev)
{
    char *c;
    int day, st, h, v;
    float f;
    int a, b;

    day = gm_ClockSeason((void *)(unsigned)&ai_Time);
    h = (day + 1) % 4;
    c = (char *)gm_FindChar(*(int *)(ev + 0xc));
    st = *(int *)(ev + 0x70);
    if (st == -1 || st == -2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (*(unsigned char *)(ev + 0x78) & 4)
        return;
    if (c == 0 || ahm_HasRecord(c) == 0 || *(int *)(ev + 0x70) == -2 || *(int *)(ev + 0x70) == -1) {
        cm_SendExSetHE(-1, ev);
        return;
    }
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(int *)(ev + 0x52) += 1;
    *(unsigned short *)(ev + 0x56) = util_Random((int)(*(float *)((unsigned)&ai_Unk617840 + h * 4) - *(float *)((unsigned)&ai_Unk617830 + h * 4))) +
                                     (int)*(float *)((unsigned)&ai_Unk617830 + h * 4);
    *(int *)(ev + 0x58) = util_Random(4) * 15;
    if (*(int *)(ev + 0xac) != 0) {
        f = (util_RandomF() + *(float *)(unsigned)&ai_Unk5eb780) * 0.25f;
        ahm_SendAddValue(c, f);
        *(int *)(ev + 0xac) = 0;
        cm_SendExSetHE(0, ev);
        return;
    }
    v = ahm_ResearchState(*(int *)(c + 0x16c), &b, &a, 1);
    f = (util_RandomF() + *(float *)(unsigned)&ai_Unk5eb780) * 0.25f;
    if (f <= *(float *)(*(int *)(*(int *)(c + 0x16c) + 0x65) + 4)) {
        switch (v) {
        case 1:
            f = -(util_RandomF() + *(float *)(unsigned)&ai_Unk5eb780) * (0.100000001f);
            break;
        case 2:
            f = -(util_RandomF() * (0.150000006f) + (0.100000001f));
            break;
        case 3:
        case 4:
            f = -(util_RandomF() * 0.2f + (0.100000001f));
            break;
        default:
            f = util_RandomF() * (0.150000006f) + (0.0500000007f);
            break;
        }
        ahm_SendAddValue(c, f);
    } else {
        he_Unk56e070(c, ev, *(void **)(c + 0x16c), v);
    }
    cm_SendExSetHE(0, ev);
}
