/* 0x56efc0, the alloc of the event 0x9a (the run 0x56f0b0, freed by 0x56f040): the due clock +0x52 = now; when the search 0x56f360 (the record of +0xb0, the owner of +0xac) finds, due +0x18 hours and the state 0 scheduled (0x4c4c60), else the handle -1 */

void he_Unk56efc0(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    if (he_Unk56f360(HM_I(ev, 0xac), gm_FindChar(HM_I(ev, 0xb0))) != 0) {
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    } else
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
}
