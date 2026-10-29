/* 0x55d5c0, the run handler of the harvest event ("Ernten_start_%s" / "Ernten_end_%s" cutscene scripts "x:\engine\gfx\scripts\" "%slocations/%s/%s_%s.esc", "sp_%i" spots; ROUGH skeleton, 1440 bytes): the field object (0x4bc670 / 0x4bc5a0), the worker handle (0x4c3ae0), the scene (0x511e60) and the script (0x42f8f0 path, evt_LoadScript, evt_RunScriptArgs) are started and stopped (evt_FindById, evt_Stop) around the work; the end orders (0x4c4880), sends a message record (0x4c4b70), releases the group (0x524260) and frees the record */

void he_Unk55d5c0(int ev)
{
    int st = *(int *)(ev + 0x70);
    int b, p;
    char path[0x100];

    b = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
    if (b == 0 || st < 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    p = gm_FindChar(*(int *)(ev + 0xb0));
    if (p == 0) {
        sv_MoveToGroup(0, 0, 0, ev);
        he_Unk53e1f0((int *)ev);
        return;
    }
    init_BuildingSceneUniverse(b);
    sprintf(path, "%slocations/%s/%s_%s.esc", evt_Path, "", "Ernten_start_%s", "");
    ((void (*)(int, int))cm_SetPlant)(*(int *)(ev + 0xac), 0);
    sv_MoveToGroup(b, 0, *(unsigned short *)p, ev);
    he_Unk53e1f0((int *)ev);
}
