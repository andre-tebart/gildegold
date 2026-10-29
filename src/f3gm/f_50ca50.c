/* 0x50ca50, changes the state byte (+0x12) of the town object id by d (not below 0) and sends the record */

void gm_Unk50ca50(int id, int d)
{
    char buf[0x80];
    char v;
    char *o;

    v = d;
    o = gm_Query(0, 2, 7, 1, id);
    if (o == 0) {
        sprintf(buf, "gm_ChangeObjektZustand():Could not find Object-ID: %i", id);
        return;
    }
    if ((unsigned char)o[0x12] + d < 0)
        v = 0;
    cm_BeginFieldBlock(o, id);
    cm_AddFieldValues(1, 1, 0x12, &v);
    cm_SendFieldDeltas();
}
