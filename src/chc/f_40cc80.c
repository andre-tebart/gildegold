/* 0x40cc80, returns the character standing on (or walking to) the dummy */

int chc_DummyBlocked(D3OBJECT **dummy, int *dist)
{
    D3VECTOR4 pos;
    CHARACTER *found = 0;
    CHACTION *a;
    int i;

    if (!*dummy) {
        evt_Error(evt_Cur, evt_Cur->pc, "DummyBlocked(): invalid dummy");
        return 0;
    }
    i = 0;
    d3_PointToWorld(*dummy, &(*dummy)->pos, (D3VECTOR *)&pos);
    for (; i < CH_MAX; i++) {
        if (ch_List[i] && ch_List[i]->scene == d3_DefaultScene && ch_List[i]->group == evt_NoGroup) {
            if (d3_VecEqual(&ch_List[i]->obj->pos, (D3VECTOR *)&pos, (float)*dist)) {
                found = ch_List[i];
                break;
            }
            a = ch_List[i]->actions;
            if (a && a->type == '-' && d3_VecEqual(&a->u.walk.tpos, (D3VECTOR *)&pos, (float)*dist)) {
                found = ch_List[i];
                break;
            }
        }
    }
    return (int)found;
}
