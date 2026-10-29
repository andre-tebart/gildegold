/* 0x4bc5a0, the vegetation type with id type, 0 if none */

G15TREE *veg_FindType(int type)
{
    int i;

    for (i = 0; veg_Types[i].id != 0; i++) {
        if (veg_Types[i].id == type)
            return &veg_Types[i];
    }
    return 0;
}
