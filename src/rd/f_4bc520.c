/* 0x4bc520, a random object of the default scene named name, 0 if none */

char *rd_RandomNamedObject(char *name)
{
    G15FIND f;

    f.n = 0;
    strcpy(f.name, name);
    d3_ForEachObject(d3_DefaultScene, 0, 0x380, rd_CollectNamed, &f);
    if (f.n == 0)
        return 0;
    return f.objs[util_Random(f.n)];
}
