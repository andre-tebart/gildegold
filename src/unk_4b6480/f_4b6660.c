/* 0x4b6660, message 2: c gets 2..5 % of the worth of the first character (kind 3 / 4) that applied for an
   open office (state 3, applicants taken) of c's office column; the text gets the office's 0x253 + kind.
   The goto labels give the original's block layout (the shared 0x400 exit before the found body). */

int ai_MsgOfficeApplicantPayment(G14MSG *m)
{
    AICHAR *c, *p;
    int i, j, n, v;
    double k;
    G14REC r[6];
    char buf[0x100];

    c = gm_FindChar(m->id);
    if (c == 0)
        return 1;
    if (c->officeKind == 0)
        goto none;
    n = amt_KindColumn(c->officeKind);
    if (n == 0 || n >= 7)
        goto none;
    n = amt_GetColumnOffices(n, 6, r);
    if (n == 0)
        goto none;
    for (i = 0, p = 0; i < n; i++) {
        if (p != 0)
            goto found;
        if (r[i].state == 3 && r[i].cnt != 0) {
            for (j = 0; j < 768; j++) {
                if (ai_Chars[j].appliedOffice == r[i].kind && (ai_Chars[j].kind == 3 || ai_Chars[j].kind == 4)) {
                    p = &ai_Chars[j];
                    break;
                }
            }
            if (p != 0)
                goto found;
        }
    }
none:
    return 0x400;
found:
    k = util_Random(4) + 2.0;
    v = (int)(gm_GetWorth(p->id) * k * 0.01);
    txt_Format(buf, m->type + 0x12e7, p->id, c->id, r[i].kind + 0x253, v);
    cm_SendMoveGoodsUnchecked(m->id, p->netId, ai_OwnPlayer, v);
    he_ShowNews(c->netId, c->netId, buf, strlen(buf) > 0x100, 0x603, 0);
    return 0;
}
