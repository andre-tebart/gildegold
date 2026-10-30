/* 0x4a0240, the nearest partner of character p */

G5CH *ai_Unk4a0240(G5CH *p)
{
    G5CH *best = 0;
    G5CH *o;
    float d, bd;
    unsigned int n;
    int i;
    AIPER *list[5];
    char key[8];
    unsigned char info[16];
    char out[24];
    unsigned short id;
    int flags;

    if (((unsigned char *)p)[0x166] == 0) {
        amt_GetKindB(((unsigned char *)p)[0x167], key);
        for (o = G5_CHARS; o < G5_CHARS + 768; o++) {
            if (((unsigned char *)o)[0x166] != 0 && amt_FindOffice(o, key, out) != 0 && info[0] < 2) {
                if (best == 0) {
                    bd = gm_Liking(p->id, o->id, 1);
                    best = o;
                } else {
                    d = gm_Liking(p->id, o->id, 1);
                    if (d < bd) {
                        bd = d;
                        best = o;
                    }
                }
            }
        }
    } else {
        if (p->unk168 != 0 && (n = amt_GetOfficeApplicants((char)p->unk168, 5, list)) != 0) {
            for (i = 0; i < (int)n; i++) {
                o = (G5CH *)list[i];
                if (o != p) {
                    if (best == 0) {
                        bd = gm_Liking(p->id, o->id, 1);
                        best = o;
                    } else {
                        d = gm_Liking(p->id, o->id, 1);
                        if (d < bd) {
                            best = (G5CH *)list[i];
                            bd = d;
                        }
                    }
                }
            }
            if (best != 0 && bd <= (float)(int)n * 8.0f + 52.0f)
                return best;
        }
        best = 0;
        flags = 0xc00;
        if (ai_Unk4a58d0((AIPER *)p, 1, &flags, 0, 25.0f, &id) != 0)
            return &G5_CHARS[id];
    }
    return best;
}
