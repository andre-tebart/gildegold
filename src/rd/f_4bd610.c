/* 0x4bd610, d3_ForEachObject callback: the object named f->name (or with one leading character more) within 100
   of f->pos; stops there */

char rd_FindSiteCB(char *obj, G15BKFIND *f)
{
    float v[4];

    if (_strnicmp(obj, f->name, strlen(f->name)) == 0 || _strnicmp(obj + 1, f->name, strlen(f->name)) == 0) {
        d3_PointToWorld(obj, obj + 0x4c, v);
        if (d3_VecEqual(f->pos, v, 100.0f)) {
            f->found = obj;
            return 0;
        }
    }
    return 1;
}
