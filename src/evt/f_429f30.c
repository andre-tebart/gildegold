/* 0x429f30 @deep B: hunks regs 180, code 162. Read asmdiff: evt_Cur and val hold the opposite callee-saved registers (ours edi/esi, original esi/edi) throughout; the string element copy is an inline two-pointer loop in the original vs relative-addressing strcpy in ours; one jump-table index is in ecx vs edx. No missing statement, no wrong constant. The val split / declaration orders / strcpy forms are recorded in notes/units/evt.md; c2prio puts val above the first evt_Cur web. */

int evt_Eval(char *end)
{
    EVTTOKEN t;
    char buf[512];
    int val = 0;
    int next = EVT_OP_ASSIGN;
    int op = EVT_OP_ASSIGN;
    int result = 0;
    int idx;
    char *p, *pos;
    EVTVAR *v;
    EVTFRAME *f;
    int l;

    if (evt_Cur->flags & 1) {
        while ((evt_Cur->pc < end - 1 || !end) && next != 11) {
            if (next == EVT_OP_SEMI)
                goto semi;
            if (end) {
                p = evt_FindLogic(evt_Cur->pc, "&&", end);
                if (p && p < end)
                    result = val = evt_Eval(p);
            } else {
                p = evt_FindLogic(evt_Cur->pc, "&&", evt_Cur->source + evt_Cur->sourcelen);
                if (p)
                    result = val = evt_Eval(p);
            }
            evt_GetToken(evt_Cur->pc, &t);
            next = 0;
            if (t.type == EVT_OP) {
                evt_Scope = 0;
                next = t.u.i;
            } else {
            if (t.type == EVT_BUILTIN)
                val = evt_CallBuiltin(t.u.p);
            if (t.type == EVT_VAR) {
                v = t.u.p;
                pos = evt_Cur->pc;
                evt_GetToken(pos, &t);
                if (t.type == EVT_OP && t.u.i == EVT_OP_LBRACKET) {
                    idx = evt_Eval(0);
                } else {
                    idx = 0;
                    evt_Cur->pc = pos;
                }
                if (idx > v->count - 1 || idx < 0) {
                    sprintf(buf, "Array index out of bounce: '%s'", v->name);
                    evt_Error(evt_Cur, evt_Cur->pc, buf);
                    evt_Stop(evt_Cur);
                    return 0;
                }
                switch (v->type) {
                case 1:
                case 7:
                    val = v->data.i[idx];
                    break;
                case 2:
                    val = v->data.b[idx];
                    break;
                case 6:
                    strcpy(evt_StrResult, (char *)(val = (int)(v->data.s + idx * 0x60)));
                    break;
                }
            } else if (t.type == EVT_INT) {
                val = t.u.i;
            } else if (t.type == EVT_FLOAT) {
            } else if (t.type == EVT_STRING) {
                strcpy(evt_StrResult, t.u.s);
                val = (int)evt_StrResult;
            } else if (t.type == EVT_IDENT) {
                sprintf(buf, "Unknown symbol: '%s'", t.u.s);
                evt_Error(evt_Cur, evt_Cur->pc, buf);
                evt_Stop(evt_Cur);
                return 0;
            } else if (t.type == EVT_SCRIPT) {
                evt_Scope = t.u.p;
                next = EVT_OP_ASSIGN;
                op = EVT_OP_ASSIGN;
                evt_Cur->pc++;
            }
            }
            switch (next) {
            case 17:
                return result < evt_Eval(end);
            case 19:
                return result > evt_Eval(end);
            case 1:
                return result == evt_Eval(end);
            case 7:
                return result != evt_Eval(end);
            case 12:
                val = evt_Eval(end);
                break;
            case 13:
            case 28:
                return result;
            case 16:
                return result <= evt_Eval(end);
            case 18:
                return result >= evt_Eval(end);
            case 23:
                return evt_Eval(end) & result;
            case 22:
                return evt_Eval(end) | result;
            }
            switch (op) {
            case 2:
                result = val;
                break;
            case 4:
                result += val;
                break;
            case 6:
                result -= val;
                break;
            case 20:
                result /= val;
                break;
            case 21:
                result *= val;
                break;
            case 24:
                result |= val;
                break;
            case 25:
                result &= val;
                break;
            }
            op = next;
            if (t.type == EVT_END || !(evt_Cur->flags & 1))
                break;
        }
        if (next == EVT_OP_SEMI) {
semi:
            f = evt_Cur->frame;
            l = f->level;
            if (l > 1 && f->loops[l - 1].start && f->loops[l - 1].braced) {
                f->level = l - 1;
                f = evt_Cur->frame;
                if (f->loops[f->level].type == 1)
                    evt_Cur->pc = f->loops[f->level].start;
            }
        }
    }
    evt_Scope = 0;
    return result;
}
