/* 0x4c38e0, joins the parts of the split message m into ai_Unka68d40 and frees them; returns the length */

unsigned int ai_Unk4c38e0(G17MSG *m)
{
    G17MSG *r, *p;
    int n;

    if (m->next7 == 0 || ai_Unka68d24 == 0)
        return 0;
    for (r = ai_Unka68d24; r->unk03 != m->unk03 || r->seq != m->next7; r = r->next)
        if (r->next == 0)
            return 0;
    m->next7 = 0;
    ai_Unkf81164 = *(unsigned short *)r->data;
    memcpy(ai_Unka68d40, r->data + 2, 0x7e);
    r->type = 0;
    for (n = 0x7e, p = r; n < (int)ai_Unkf81164; n += 0x80) {
        if (ai_Unka68d24 == 0)
            return 0;
        for (r = ai_Unka68d24; r->unk03 != p->unk03 || r->seq != p->next7; r = r->next)
            if (r->next == 0)
                return 0;
        p->next7 = 0;
        memcpy(ai_Unka68d40 + n, r->data, 0x80);
        r->type = 0;
        p = r;
    }
    r->next7 = 0;
    return ai_Unkf81164;
}
