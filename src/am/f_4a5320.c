/* 0x4a5320, amt_ExecGebaeudeBauen: builds the building type s1->unk04 (kind 5) on the plot s2->unk04 (kind 0xc):
   finds its model group "gb_<NAME>" (with a "_A" variant), message 0xd with the price, place and group path */

int ai_Unk4a5320(AIPER *p, AIARG *s1, AIARG *s2)
{
    AIMSG m;
    char name[0x104];
    char path[0x104];
    char found[0x100];
    int plot, alt;

    if (s1->kind != 5)
        return 0;
    if (s2->kind != 0xc)
        return 0;
    plot = s2->unk04;
    strcpy(name, "gb_");
    strcat(name, ai_BTypesT[s1->unk04].pad001);
    _strupr(name + 2);
    alt = 0;
    for (;;) {
        if (alt)
            sprintf(path, "%sgebaeude/*%s_%c.ogr", ai_DataPath, name, alt + '@');
        else
            sprintf(path, "%sgebaeude/*%s.ogr", ai_DataPath, name);
        if (vfs_GetPath(path, vfs_Root, vfs_Root, found) != 0)
            break;
        if (alt) {
            sprintf(name, "amt_ExecGebaeudeBauen(): Keine Group f\xfcr %s gefunden", ai_BTypesT[s1->unk04].pad001);
            return 0;
        }
        alt = 1;
    }
    sprintf((char *)&m + 0x9c, "%s", path);
    m.time = ai_TimeT;
    m.to = *(int *)((unsigned char *)p + 4);
    m.arg[1] = s1->unk04;
    m.type = 0xd;
    m.flags = 2;
    m.fromId = m.to;
    m.arg[0] = m.to;
    m.arg[2] = gm_Unk509dc0(p, m.arg[1]);
    ai_ObjectPosition((char *)&m + 0x80, plot);
    ai_ObjectAngle((char *)&m + 0x90, plot);
    cm_SendLongMessage(&m);
    return 1;
}
