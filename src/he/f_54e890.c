/* 0x54e890, a Ghidra miss (disassembled; 752 B): the run handler of the beating event (see 0x54e770): the victim (edi) flag 0x800000 is cleared when it ends (state -2: the aggressor gets the relation -(8 + the rounds +0xb4) * 5); state 2 waits for the job (0x4c3ae0: 1 = done -> state 0, 2 = failed -> ends), in the later states the rounds count up (+0xb4, the victim pays the aggressor 0x53eec0 -+0xb8 with the news 0xb4 / 0xb5, due 0x18 units ahead) until the aggressor is gone or dead (ROUGH) */

void he_Unk54e890(int ev)
{
    char text[0x200];
    int edi, ebx, st, h;

    edi = gm_FindChar(*(int *)(ev + 0xb0));
    ebx = gm_FindChar(*(int *)(ev + 0xac));
    st = *(int *)(ev + 0x70);
    if (st == -1) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (st == -2) {
        if ((*(unsigned char *)(ev + 0x78) & 4) != 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        if (edi != 0) {
            cm_SendSetField(*(int *)(edi + 4), 0x1c8, 4, 0, ((unsigned)&ai_Relations + 0x1aca0));
            if (ebx != 0)
                cm_SendRelations(*(int *)(ebx + 4), *(int *)(edi + 4), (-8 - *(int *)(ev + 0xb4)) * 5, 1);
        }
        he_Unk53e1f0((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    if (edi == 0) {
        gm_AddTime(ev + 0x52, 0, 2, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
    if (st == 2) {
        h = cm_GetSlotState(*(int *)(ev + 0x84));
        if (h == 0)
            return;
        if (h == 1) {
            gm_AddTime(ev + 0x52, 0, 2, 0);
            cm_SendExSetHE(0, ev);
            return;
        }
        if (h == 2) {
            gm_AddTime(ev + 0x52, 0, 2, 0);
            cm_SendExSetHE(-1, ev);
            return;
        }
    }
    if (ebx != 0 && *(unsigned char *)(ebx + 8) != 0) {
        if (st == 1) {
            cm_SendSetField(*(int *)(edi + 4), 0x1c8, 4, ((unsigned)&ai_Relations + 0x1aca0), 0);
            cm_SendRelations(*(int *)(ebx + 4), *(int *)(edi + 4), (-2 - *(int *)(ev + 0xb4)) * 5, 1);
            gm_AddTime(ev + 0x52, 0, 2, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        if (st == 3) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
            return;
        }
        if (*(char *)(edi + 9) == 1)
            txt_Format(text, 0xb4, *(unsigned short *)edi);
        else
            txt_Format(text, 0xb5, *(unsigned short *)edi);
        he_Unk53eec0(*(unsigned short *)(ev + 8), text, -*(int *)(ev + 0xb8), 0);
        *(int *)(ev + 0xb4) = *(int *)(ev + 0xb4) + 1;
        gm_AddTime(ev + 0x52, 0, 0x18, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
        return;
    }
    cm_SendSetField(*(int *)(edi + 4), 0x1c8, 4, ((unsigned)&ai_Relations + 0x1aca0), 0);
    gm_AddTime(ev + 0x52, 0, 2, 0);
    *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
}
