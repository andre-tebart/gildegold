/* 0x55dbe0, the run handler "Entlassen" (the dismissal of a worker, jump table 0x55dbc0; ROUGH): states -2 / -1 free; the figure +0xac (0x505df0), its job record +0x16c (0x4c3ba0 / 0x4c3d90 / 0x4c49e0 mark it, 0x50c520 the wage check against -0x1e), the relation 0x4c4b00, the job end 0x4c5970 / 0x4c5b90, the contact 0x4fdcb0 and the walk away (0x4c52e0 (.., "Entlassen" 0x627860)); the record is freed, state -1 on a failed check */

void he_Unk55dbe0(int ev)
{
    int st = *(int *)(ev + 0x70);
    int p;

    if (st == -2 || st == -1) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    p = gm_FindChar(*(int *)(ev + 0xac));
    if (p == 0) {
        *(int *)(ev + 0x70) = -1;
        return;
    }
    if (*(int *)(p + 0x16c) != 0) {
        cm_BeginFieldBlock(p, *(int *)(p + 4));
        cm_AddFieldValue(1, 1, 4, 0);
        cm_SendFieldValues();
        ((void (*)(int))gm_Opinion)(*(int *)(p + 0x16c));
        cm_SendRelations(*(int *)(p + 4), *(int *)(p + 0x16c), -0x1e, 1);
    }
    ((void (*)(int, int))ai_Unk4c5970)(0, 0);
    ai_Unk4c5b90(*(int *)(p + 4));
    ai_Unk4c52e0(*(int *)(p + 4), -1, -1, 0, 0, "Entlassen");
    he_Unk53e1f0((int *)ev);
}
