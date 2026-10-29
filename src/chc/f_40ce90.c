/* 0x40ce90, LookAtObject(ch, obj): turns the character towards the object's world position through action 7; 1 after an error, else 0 */

int chc_LookAtObject(CHARACTER **ch, D3OBJECT **obj)
{
    D3VECTOR4 pos;
    float angle;

    if (*ch && *obj) {
        d3_PointToWorld(*obj, &(*obj)->pos, (D3VECTOR *)&pos);
        angle = d3_YawTo((*ch)->obj, (D3VECTOR *)&pos);
        ch_AddAction(*ch, 7, (int)(angle * 57.295776f), 0);
        return 0;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "LookAtObject(): invalid dest- or targetobject");
    return 1;
}
