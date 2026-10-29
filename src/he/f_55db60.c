/* 0x55db60, a run handler: ends the job of the building +0xac (0x4c5050 (.., 0)) and frees the record; a missing figure releases the group (0x524260) first */

void he_Unk55db60(int ev)
{
    int p;

    p = gm_FindChar(*(int *)(ev + 0xac));
    if (p != 0)
        ((void (*)(int, int))cm_SetPlant)(*(int *)(ev + 0xac), 0);
    else
        sv_MoveToGroup(0, 0, 0, ev);
    he_Unk53e1f0((int *)ev);
}
