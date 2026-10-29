/* 0x4bd6a0, the building site named name, 0 if unknown */

G15BK *rd_GetBKSize(char *name)
{
    int i;
    char buf[0x40];

    for (i = 0; i < ai_NBauplaetze; i++) {
        if (_strcmpi(name, ai_Bauplaetze[i].name) == 0)
            return &ai_Bauplaetze[i];
    }
    sprintf(buf, "rd_GetBKSize(): Unbekannter Bauplatz '%s'", name);
    return 0;
}
