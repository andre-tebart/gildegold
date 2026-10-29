/* 0x4e4b50, "cut_CutsceneReady": the party m (+0 the id, +0x26 bit 7: the owner side, +0x30 count, +0x34 ids) is asked whether every member is ready: in the
   owner's run (bit 7 clear) a request (0x4c4fd0) goes to every member that does not exist / belongs to another master (kinds 6 / 7), the last one is waited
   for; otherwise the first such member ends the run with 0. A bad master (0x4e4a50) gives -1; else the message (id + the 0x10 ids) is sent
   (0x4c4540 / 0x4c5f20 / 0x4c4580) and the answer 2 of the wait makes 0, anything else 1. ROUGH */

int ai_Unk4e4b50(char *m)
{
    int buf[0x12];
    int i, h;
    int *p;
    char *c;

    memset(buf, 0, sizeof(buf));
    i = 0;
    if ((signed char)m[0x26] >= 0) {
        h = -1;
        p = (int *)(m + 0x34);
        for (i = 0; i < (unsigned char)m[0x30]; i++, p++) {
            if (*p == -1)
                continue;
            c = (char *)gm_FindChar(*p);
            if (c == 0 || (*(int *)(c + 0x20c) != -1 && *(int *)(c + 0x20c) != *(int *)m && (c[2] == 6 || c[2] == 7)))
                h = ((int (*)())ai_Unk4c4fd0)(*(int *)m, *p);
        }
        if (h != -1) {
            while (cm_GetSlotState(h) == 0)
                sv_PumpNet();
        }
    } else {
        p = (int *)(m + 0x34);
        for (i = 0; i < (unsigned char)m[0x30]; i++, p++) {
            if (*p == -1)
                continue;
            c = (char *)gm_FindChar(*p);
            if (c != 0 && *(int *)(c + 0x20c) != -1 && *(int *)(c + 0x20c) != *(int *)m && (c[2] == 6 || c[2] == 7))
                return 0;
        }
    }
    if (ai_Unk4e4a50(m) == 0)
        return -1;
    buf[0] = *(int *)m;
    memcpy(buf + 1, m + 0x34, 0x40);
    cm_BeginGroup("cut_CutsceneReady");
    h = ((int (*)())ai_Unk4c5f20)(buf);
    cm_EndGroup("cut_CutsceneReady");
    while (cm_GetSlotState(h) == 0)
        sv_PumpNet();
    return cm_GetSlotState(h) != 2;
}
