/* 0x4bc2e0, pos = the "dummy_TUER" (or the object) of a random building of the gm_BuildQuery(1, 6) list */
int rd_DoorPos(float *pos)
{
    G15BLD *bs[0x100];
    int n;
    G15BLD *b;
    char *o;

    n = 0;
    b = (G15BLD *)gm_BuildQuery(1, 6);
    while (b != 0) {
        if (gm_IsHomeType((unsigned char *)b) == 0 && gm_IsType10((unsigned char *)b) == 0 && b->object != 0)
            bs[(unsigned short)n++] = b;
        b = (G15BLD *)gm_BuildNext();
    }
    b = bs[util_RandomS(n)];
    o = d3_FindObject(b->object, 0x300, 0, "dummy_TUER");
    if (o != 0)
        d3_PointToWorld(o, o + 0x4c, pos);
    else
        d3_PointToWorld(b->object, b->object + 0x4c, pos);
    return 1;
}
