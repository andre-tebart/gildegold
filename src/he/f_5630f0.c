/* 0x5630f0, the run handler of the town gate event ("STADT"; ROUGH skeleton, 2192 bytes): the job handle (0x4c3ae0), the group (0x524260 (.., id, ev), 0x4c52e0), the scene (d3if_SelectScene, d3_FindObject / d3_Unk458a20 positions, d3_VecEqual against the gate position, 0x541280 / 0x50aa00 / 0x4e5050 helpers) and the random walk (util_Random) are driven by the state; the clock is posted by 0x4c4c60 and 0x4fe140 */

void he_Unk5630f0(int ev)
{
    int st = *(int *)(ev + 0x70);
    int p;

    if (st < 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    p = gm_FindChar(*(int *)(ev + 0xb0));
    if (p == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    sv_MoveToGroup(0, 0, *(unsigned short *)p, ev);
    ai_Unk4c52e0(*(int *)(p + 4), -1, -1, 0, 0, "STADT");
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
}
