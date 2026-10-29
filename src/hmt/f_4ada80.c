/* 0x4ada80, loads the hmt state from the savegame f; on failure it is reset; 1 or 0 @deep code hunks 8: the original's success return is out of line (jne over the success block), ours returns inline (je); if / else with an empty then tried before, no change (match-deep) */

int hmt_Load(void *f)
{
    if (ls_Read(&hmt_CurrentPerson, 4, 1, f) && ls_Read(&hmt_Person, 4, 1, f)
        && ls_Read(&hmt_CurrentEvent, 4, 1, f) && ls_Read(&hmt_Event, 4, 1, f)
        && ls_Read(&hmt_PendingElection, 4, 1, f)) {
    } else {
        hmt_CurrentPerson = -1;
        hmt_CurrentEvent = -1;
        hmt_Person = -1;
        hmt_Event = -1;
        hmt_PendingElection = -1;
        return 0;
    }
    return 1;
}
