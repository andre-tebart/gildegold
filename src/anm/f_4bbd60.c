/* 0x4bbd60, creates a dog (kind 0) at a free place (0x4bc2e0) @deep Q4: 3 raw *(T*) accesses on the animal object; typed fields need ch.h in anm.c, blocked by rd.c's d3_DefaultScene view (see notes/units/anm.md) */

void anm_CreateDog(ANMREC *a)
{
    float pos[4];

    if (!rd_DoorPos(pos))
        return;
    a->obj = ch_Create("hund_HUND", pos);
    if (a->obj == 0)
        return;
    *(float *)((char *)a->obj + 0x1a0) = 1.0f;
    a->pos[0] = pos[0];
    a->pos[1] = pos[1];
    a->pos[2] = pos[2];
    a->kind = 0;
    *(int *)((char *)a->obj + 4) = 2;
    *(int *)(*(char **)((char *)a->obj + 0x34) + 0x224) = 0;
    ch_AddAction(a->obj, 0x38, 1);
}
