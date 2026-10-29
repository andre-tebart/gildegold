/* 0x4c3850, 1 if all parts of the split message m are received (or m is not split) */

int ai_Unk4c3850(G17MSG *m)
{
    G17MSG *r, *p, *q;
    int n;

    if (m->type == 7 || m->next7 == 0)
        return 1;
    for (r = ai_Unka68d24; r != 0; r = r->next) {
        if (r->unk03 == m->unk03 && r->seq == m->next7) {
            if (*(unsigned short *)r->data < 0x7f)
                return 1;
            for (n = 0x7e, p = r; n < *(unsigned short *)r->data; n += 0x80) {
                for (q = ai_Unka68d24; q != 0; q = q->next)
                    if (q->unk03 == p->unk03 && q->seq == p->next7)
                        break;
                if (q == 0)
                    return 0;
                p = q;
            }
            return 1;
        }
    }
    return 0;
}
