/* 0x4adb20, saves the hmt state to the savegame f */

int hmt_Save(VFSFILE *f)
{
    if (!HMT_FW(&hmt_CurrentPerson, 4, 1, f))
        return 0;
    if (!HMT_FW(&hmt_Person, 4, 1, f))
        return 0;
    if (!HMT_FW(&hmt_CurrentEvent, 4, 1, f))
        return 0;
    if (!HMT_FW(&hmt_Event, 4, 1, f))
        return 0;
    return HMT_FW(&hmt_PendingElection, 4, 1, f);
}
