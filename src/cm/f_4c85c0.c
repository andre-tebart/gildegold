/* 0x4c85c0, handler of type 0x1c: creates the job / message box of the long message (0xf8 bytes, AIMSG) and its
   texts (after it in ai_Unka68d40) */

int ai_Unk4c85c0(G17MSG *m, G17STAT *s)
{
    char msg[0xf8];
    char *c, *o, *t1, *t2;

    memcpy(msg, ai_Unka68d40, 0xf8);
    c = gm_FindChar(*(int *)(msg + 8));
    if (net_Conn.s != INVALID_SOCKET) {
        if (msg[0x36] & 1) {
            if (c == 0)
                return 1;
            if (c[2] == 7)
                return 1;
            if (c[2] != 6) {
                if (s == 0)
                    return 1;
                goto set;
            }
        }
        if (s == 0 && (msg[0x36] & 2))
            msg[0x36] = (msg[0x36] & ~2) | 4;
set:
        ai_Unk6f2618 = *(int *)&m->data[0];
    }
    if (msg[4] == 0x11 && he_IsMessageKindFiltered(*(int *)(msg + 0x58)) != 0)
        return 0;
    o = he_Unk53e040(msg);
    if (o == 0)
        return 1;
    if (o[0] == 0x11) {
        t1 = (char *)ai_Unka68d40 + 0xf8;
        if (o[0xf0] & 0x10) {
            t2 = t1 + strlen(t1) + 1;
            *(char **)(o + 0x7c) = mem_Alloc(strlen(t1) + strlen(t2) + 2, "messagebox");
            strcpy(*(char **)(o + 0x7c), t1);
            strcpy(*(char **)(o + 0x7c) + strlen(t1) + 1, t2);
            *(int *)(o + 0x80) = strlen(t2) + strlen(t1) + 2;
        } else {
            *(char **)(o + 0x7c) = mem_Alloc(strlen(t1) + 1, "messagebox");
            strcpy(*(char **)(o + 0x7c), t1);
            *(int *)(o + 0x80) = strlen(*(char **)(o + 0x7c)) + 1;
        }
    }
    if (s != 0) {
        s->state = 1;
        s->unk01 = 4;
        s->value = (int)o;
    }
    return 0;
}
