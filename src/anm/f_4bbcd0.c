/* 0x4bbcd0, creates a cat (kind 1) at a free place (0x4bc2e0) @deep Q4: 3 raw *(T*) accesses on the animal object (scale 0x1a0, kind 0x04, 0x34->0x224); typed fields need ch.h in anm.c, blocked by rd.c's char* d3_DefaultScene view and the d3.h/ch.h/rd.h conflicts listed in notes/units/anm.md */

void anm_CreateCat(ANMREC *a)
{
    float pos[4];

    if (!rd_DoorPos(pos))
        return;
    a->obj = ch_Create("katze_KATZE", pos);
    if (a->obj == 0)
        return;
    *(float *)((char *)a->obj + 0x1a0) = 1.0f;
    a->pos[0] = pos[0];
    a->pos[1] = pos[1];
    a->pos[2] = pos[2];
    a->kind = 1;
    *(int *)((char *)a->obj + 4) = 2;
    *(int *)(*(char **)((char *)a->obj + 0x34) + 0x224) = 0;
    ch_AddAction(a->obj, 0x38, 1);
}
