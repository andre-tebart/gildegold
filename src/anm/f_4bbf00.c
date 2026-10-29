/* 0x4bbf00, creates an animal of kind k at the dummy "dummy_<name of k>" (cows, pigs and horses with their own
   models, the others with the dummy's name)
   @deep Q4: 3 raw *(T*) accesses on the animal object; typed fields need ch.h in anm.c, blocked by rd.c's d3_DefaultScene view (see notes/units/anm.md) */

void anm_CreateAnimal(ANMREC *a, int k)
{
    float pos[4];
    char buf[0x80];
    char *o;

    sprintf(buf, "dummy_%s", anm_KindNames[k]);
    o = rd_RandomNamedObject(buf);
    if (o == 0)
        return;
    d3_PointToWorld(o, o + 0x4c, pos);
    if (k == 3)
        strcpy(buf, "kuh_KUH");
    else if (k == 7)
        strcpy(buf, "pferd_PFERD");
    else if (k == 6)
        strcpy(buf, "schwein_SCHWEIN");
    a->obj = ch_Create(buf, pos);
    if (a->obj == 0)
        return;
    *(float *)((char *)a->obj + 0x1a0) = 1.0f;
    a->pos[0] = pos[0];
    a->pos[1] = pos[1];
    a->pos[2] = pos[2];
    a->kind = k;
    *(int *)((char *)a->obj + 4) = 2;
    *(int *)(*(char **)((char *)a->obj + 0x34) + 0x224) = 0;
    ch_AddAction(a->obj, 0x38, 1);
}
