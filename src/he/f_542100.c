/* 0x542100, a Ghidra miss (disassembled): the run handler of the inn meeting event ("stammtisch", scripts 0x541b40): the inn (+0xac) must have its staff record 0x12d; a visitor figure (owner player not the player of 0x6f1bf8, alive) counts: with +0xc0 == 1 the "new " meeting starts and the counter goes down; else when the meeting time (+0xb0) has passed the meeting is executed ("exec") and a new one is planned (+0xb0 + 0x18 units, random 0x13 .. 0x16 at +0xb4); with no visitor the "cler" script (clear) runs and +0xc0 is raised (ROUGH) */

int gm_MinutesBetween();
void cm_BeginGroup(const char *s);

void he_Unk542100(int ev)
{
    int bld = 0;
    int r, o;
    int cmd[4];

    if (*(int *)(ev + 0x70) == -1 || *(int *)(ev + 0x70) == -2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    *(int *)(ev + 0x84) = -1;
    gm_FindById(&bld, 0, 0, *(int *)(ev + 0xac));
    if (bld == 0) {
        gm_AddTime(ev + 0x52, 0, 5, 0);
        cm_SendExSetHE(-1, ev);
        return;
    }
    r = gm_Query(*(int *)(bld + 0x5d), 2, 6, 0, 0x12d);
    if (r == 0) {
        gm_AddTime(ev + 0x52, 0, 5, 0);
        cm_SendExSetHE(-1, ev);
        return;
    }
    o = 0;
    if (*(unsigned short *)(bld + 0x27) != 0xffff)
        o = (unsigned)&ai_Chars + *(unsigned short *)(bld + 0x27) * 0x21c;
    if (o != 0 && o != U_I((unsigned)&ai_PlayerChar) && *(char *)(o + 8) != 0 && *(char *)(o + 2) != 0xf) {
        if (*(int *)(ev + 0xc0) == 1) {
            he_Unk541b40(0x6e657720, bld, r, -1);
            *(int *)(ev + 0xc0) = *(int *)(ev + 0xc0) - 1;
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 1, 0, 0);
            *(HETIME *)(ev + 0xb0) = ai_Time;
            gm_AddTime(ev + 0xb0, 0x18, 0, 0);
        } else {
            if (gm_MinutesBetween((unsigned)&ai_Time, ev + 0xb0) > 0) {
                gm_AddTime(ev + 0x52, 1, 0, 0);
                cm_SendExSetHE(0, ev);
                return;
            }
            he_Unk541b40(0x65786563, bld, r, -1);
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 1, 0, 0);
            *(HETIME *)(ev + 0xb0) = ai_Time;
            gm_AddTime(ev + 0xb0, 0x18, 0, 0);
        }
    } else {
        if (*(int *)(ev + 0xc0) == 0) {
            cmd[0] = 0x636c6572;
            cmd[1] = -1;
            cmd[2] = *(int *)(bld + 1);
            cm_BeginGroup("stammtisch clear HE");
            ai_Unk4c5d80(cmd);
            cm_EndGroup("stammtisch clear HE");
            *(int *)(ev + 0xc0) = *(int *)(ev + 0xc0) + 1;
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 1, 0, 0);
        *(HETIME *)(ev + 0xb0) = ai_Time;
        gm_AddTime(ev + 0xb0, 0x18, 0, 0);
    }
    *(int *)(ev + 0xb6) = 0;
    *(unsigned short *)(ev + 0xb4) = (unsigned short)(util_Random(4) + 0x13);
    *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
}
