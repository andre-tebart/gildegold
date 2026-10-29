/* 0x42a620 */

int evt_CallBuiltin(EVTBUILTIN *f)
{
    char strs[8][0x60];
    int args[8];
    int r;
    int i;
    union {
        char b;                     /* the byte argument as stored */
        int word;                   /* the same slot read as the int the builtin gets */
    } c;

    evt_Cur->pc++;
    r = -99;
    for (i = 0; i < f->nargs; i++) {
        r = evt_Eval(0);
        if (f->argtype[i] == 7) {
            args[i] = r;
        } else if (f->argtype[i] == 1) {
            args[i] = r;
        } else if (f->argtype[i] == 2) {
            c.b = r;
            args[i] = c.word;
        } else if (f->argtype[i] == 6) {
            strcpy(strs[i], evt_StrResult);
            args[i] = (int)strs[i];
        }
    }
    if (f->nargs && evt_LastOp != EVT_OP_RPAREN) {
        evt_Error(evt_Cur, evt_Cur->pc, "Expecting ')'...possible wrong parameter-count");
        evt_Stop(evt_Cur);
        return 0;
    }
    if (!f->nargs)
        evt_Cur->pc++;
    switch (f->nargs) {
    case 0:
        r = f->fn0();
        break;
    case 1:
        r = f->fn1(&args[0]);
        break;
    case 2:
        r = f->fn2(&args[0], &args[1]);
        break;
    case 3:
        r = f->fn3(&args[0], &args[1], &args[2]);
        break;
    case 4:
        r = f->fn4(&args[0], &args[1], &args[2], &args[3]);
        break;
    case 5:
        r = f->fn5(&args[0], &args[1], &args[2], &args[3], &args[4]);
        break;
    case 6:
        r = f->fn6(&args[0], &args[1], &args[2], &args[3], &args[4], &args[5]);
        break;
    case 7:
        r = f->fn7(&args[0], &args[1], &args[2], &args[3], &args[4], &args[5], &args[6]);
        break;
    }
    if (f->rettype == 7)
        r = evt_FloatResult;
    if ((evt_Cur->mode == 1 && evt_Cur->waitfn) || evt_Cur->waitfn == (void *)evtc_Wait) {
        for (i = 0; i < f->nargs; i++) {
            evt_Cur->args[i] = args[i];
            if (f->argtype[i] == 6) {
                evt_Cur->args[i] = (int)malloc(0x60);
                strcpy((char *)evt_Cur->args[i], strs[i]);
            }
        }
        evt_Cur->call = f;
    }
    return r;
}
