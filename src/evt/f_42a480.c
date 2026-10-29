/* 0x42a480 */

int evt_Assign(EVTVAR *v)
{
    EVTTOKEN t;
    int idx = 0;
    int r;

    evt_GetToken(evt_Cur->pc, &t);
    if (t.type == EVT_OP) {
        if (t.u.i == EVT_OP_LBRACKET) {
            idx = evt_Eval(0);
            evt_GetToken(evt_Cur->pc, &t);
        }
        switch (t.u.i) {
        case EVT_OP_ASSIGN:
            if (v->type == 1) {
                v->data.i[idx] = evt_Eval(0);
                return 0;
            }
            if (v->type == 2) {
                v->data.b[idx] = evt_Eval(0);
                return 0;
            }
            if (v->type == 7) {
                v->data.f[idx] = (float)evt_Eval(0);
                return 0;
            }
            if (v->type == 6) {
                r = evt_Eval(0);
                if (r)
                    strcpy(v->data.s + idx * 0x60, evt_StrResult);
            }
            break;
        case EVT_OP_INC:
            if (v->type == 1) {
                v->data.i[idx]++;
                return 0;
            }
            if (v->type == 2) {
                v->data.b[idx]++;
                return 0;
            }
            break;
        case EVT_OP_DEC:
            if (v->type == 1) {
                v->data.i[idx]--;
                return 0;
            }
            if (v->type == 2) {
                v->data.b[idx]--;
                return 0;
            }
            break;
        }
    }
    return 0;
}
