/* 0x56f050, the alloc of the event 0x9b (not while paused, flags +0x78 & 4): due = now +0x30 hours (0x4fe140), state 0 scheduled (0x4c4c60) */

void he_Unk56f050(int ev)
{
    if (*(unsigned char *)(ev + 0x78) & 4)
        return;
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0x30, 0, 0);
    *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
}
