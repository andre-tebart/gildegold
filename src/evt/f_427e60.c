/* 0x427e60 */

void evt_GetToken(char *s, EVTTOKEN *t)
{
    char word[256];
    char found = 0;
    unsigned int start;
    int k, i, n, len;
    char *p;

    if (!*evt_Cur->pc || !evt_Cur->pc[1]) {
        t->type = EVT_END;
        return;
    }
    t->type = EVT_IDENT;
    start = 0;
    while (s[start] == ' ' && start < strlen(s))
        start++;
    for (k = start, len = strlen(s); !found && k < len; k++) {
        for (i = 1; i < 30; i++) {
            if (s[k] == evt_Ops[i][0]) {
                if (i == EVT_OP_DOT && isdigit(s[k - 1]))
                    continue;
                if (!(k - start))
                    for (i = 1; i < 30; i++)
                        if (evt_Match(s + k, evt_Ops[i])) {
                            k += strlen(evt_Ops[i]);
                            break;
                        }
                n = k - start;
                memcpy(word, s + start, n);
                word[n] = 0;
                evt_Cur->pc = s + strlen(word) + start;
                found = 1;
                break;
            }
        }
    }
    for (i = 1; i < 9; i++)
        if (word[0] == evt_Keywords[i][0] && evt_Match(word, evt_Keywords[i])) {
            t->type = EVT_KEYWORD;
            t->u.i = i;
            return;
        }
    for (i = 1; i < 30; i++)
        if (word[0] == evt_Ops[i][0] && evt_Match(word, evt_Ops[i])) {
            t->type = EVT_OP;
            t->u.i = i;
            if (i == EVT_OP_QUOTE) {
                p = strchr(evt_Cur->pc, '"');
                if (p) {
                    memcpy(t->u.s, evt_Cur->pc, p - evt_Cur->pc);
                    t->u.s[p - evt_Cur->pc] = 0;
                }
                t->type = EVT_STRING;
                evt_Cur->pc = p + 1;
            }
            evt_LastOp = i;
            if (i == EVT_OP_LPAREN)
                evt_Parens++;
            else if (i == EVT_OP_RPAREN)
                evt_Parens--;
            return;
        }
    if (isdigit(word[0])) {
        if (strchr(word, '.')) {
            t->type = EVT_FLOAT;
            t->u.f = (float)atof(word);
        } else {
            t->type = EVT_INT;
            t->u.i = atoi(word);
        }
    } else {
        if ((p = (char *)evt_FindVar(evt_Cur, word))) {
            t->type = EVT_VAR;
            t->u.p = p;
            return;
        }
        if ((n = evt_TypeOf(word))) {
            t->type = EVT_TYPE;
            t->u.i = n;
            return;
        }
        if ((p = (char *)evt_FindFunc(evt_Cur, word))) {
            t->type = EVT_FUNC;
            t->u.p = p;
            return;
        }
        if ((p = (char *)evt_FindBuiltin(word))) {
            t->type = EVT_BUILTIN;
            t->u.p = p;
            return;
        }
        if ((p = (char *)evt_FindInclude(evt_Cur, word))) {
            t->type = EVT_SCRIPT;
            t->u.p = p;
            return;
        }
    }
    if (t->type == EVT_IDENT)
        strcpy(t->u.s, word);
}
