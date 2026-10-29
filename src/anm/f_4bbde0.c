/* 0x4bbde0, creates a sheep (kind 4) at the dummy "dummy_SHEEP" @deep Q4: 3 raw *(T*) accesses on the animal object; typed fields need ch.h in anm.c, blocked by rd.c's d3_DefaultScene view (see notes/units/anm.md) */

void anm_CreateSheep(ANMREC *a)
{
    char *o;
    float pos[4];

    o = rd_RandomNamedObject("dummy_SHEEP");
    if (o == 0)
        return;
    d3_PointToWorld(o, o + 0x4c, pos);
    a->obj = ch_Create("schaf_SCHAF", pos);
    if (a->obj == 0)
        return;
    *(float *)((char *)a->obj + 0x1a0) = 1.5f;
    a->pos[0] = pos[0];
    a->pos[1] = pos[1];
    a->pos[2] = pos[2];
    a->kind = 4;
    *(int *)((char *)a->obj + 4) = 2;
    *(int *)(*(char **)((char *)a->obj + 0x34) + 0x224) = 0;
    ch_AddAction(a->obj, 0x38, 1);
}
