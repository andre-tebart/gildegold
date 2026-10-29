/* 0x426f60, 1 when the named entry of bank exists and is a variation set (type 2), also 1 when the driver
   is closed; 0 otherwise */

int sb_IsVariation(SBBANK *bank, char *name)
{
    SBENTRY *e;

    if (!sb_Driver)
        return 1;
    e = sb_FindEntry(bank, name);
    if (e && e->type == 2)
        return 1;
    return 0;
}
