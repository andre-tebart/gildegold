/* 0x5649b0, the run handler of the fire ("Brand_l" / "Stichflamme_s" sounds, "Brand_Betroffen", "dummy_FAHNE", the script "specialEvents\hausbrand.esc", "brand_he_%i_%i"; ROUGH skeleton, 2655 bytes): the fire objects are placed at the dummies of the burning building (d3_FindObject, d3_Unk458a20, util_Random), the sounds attached and played (sb_GetSample, d3snd_Attach / d3snd_Play / d3snd_PlayAt), the script loaded and run (evt_Load, evt_RunScriptArgs), the news to the owner (0x53dee0) and the burning handled by the state until it goes out */

void he_Unk5649b0(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b;
    char name[0x40];

    if (st < 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    sprintf(name, "brand_he_%i_%i", *(int *)(ev + 0xac), st);
    snd_QueueVoice(-7, 0, "Brand_l", 0, 0);
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 1, 0);
}
