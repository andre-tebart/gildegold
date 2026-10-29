/* 0x4ceb80, waits for the event id to end, then stops it */

void ai_Unk4ceb80(int id)
{
    char *e;

    if (ai_Unk6f0d8c != 0)
        return;
    if (sv_RunFrame(0, 0x7d6c6) != 0) {
        while (dm_L.rup == 0 && kb_Last == 0) {
            if (evt_FindById(id) == 0)
                return;
            if (sv_RunFrame(0, 0x7d6c6) == 0)
                break;
        }
    }
    e = (char *)evt_FindById(id);
    if (e != 0)
        evt_Stop(e);
    di_ClearButtons();
}
