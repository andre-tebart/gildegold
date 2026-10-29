/* 0x55de30, the run handler of the promotion event ("Effekte/befoerdern.esc" cutscene; ROUGH skeleton, 928 bytes): the figure +0xac and its building (gm_FindBuildingById) are looked up, the old rank is dropped (gm_DropRank), the effect script is loaded and run in the scene (evt_Load, evt_RunScriptArgs, d3if_SelectScene, ch_Delete of the old hand object, ch_SetHandObject), the job handle is cleaned (0x4c3ae0, 0x4c4c60, 0x4c4a60) and the record freed */

void he_Unk55de30(int ev)
{
    int p;

    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    p = gm_FindChar(*(int *)(ev + 0xac));
    if (p == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    cm_SendExSetHE(-1, ev);
    cm_SendSetField(*(int *)(p + 4), 0x1cc, 4, 0, ((unsigned)&txt_Keys + 0x181c0));
    he_Unk53e1f0((int *)ev);
}
