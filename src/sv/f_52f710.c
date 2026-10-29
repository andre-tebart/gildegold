/* 0x52f710, turns the book bk by dir pages (2 forward / -2 backward): checks the limits and that the page animations of the book model (+0x1ec) are finished ("buch\Buch_vorblaettern.baf" /
   "buch\Buch_zurueckblaettern.baf"), sets the page pictures of the n page widgets, starts the animation, advances the first page (+0x270), plays "Umblaettern_s"; returns 1 when turned; ROUGH */

int sv_Unk52f710(int *bk, char dir)
{
    char *s, *anim;
    int off, a, i, rec, *p, pg, lim;
    char fwd[0x1c], back[0x1c];
    char top[0x40], bottom[0x40];

    if (bk == 0)
        return 0;
    if (dir < 0) {
        if ((char)bk[0x9c] - 2 < 0)
            return 0;
    } else if ((int)*((char *)bk + 0x271) <= (char)bk[0x9c] + 2)
        return 0;
    off = 0;
    do {
        strcpy(fwd, "buch\\Buch_vorblaettern.baf");
        s = util_BaseName(fwd);
        anim = *(char **)(*(int *)(bk[0] + 0x1ec) + 0x178 + off);
        rec = *(int *)(bk[0] + 0x1ec) + 0x110 + off;
        if (anim != 0 && _strcmpi(anim, s) == 0 && dir > 0) {
            if ((*(unsigned char *)(rec + 0x6d) & 0x20) == 0)
                return 0;
            d3_ReleaseTexture(*(int *)(bk[0] + 0x1ec) + 0xf4, *(int *)(off + 0x178 + *(int *)(bk[0] + 0x1ec)));
        }
        strcpy(back, "buch\\Buch_zurueckblaettern.baf");
        s = util_BaseName(back);
        if (*(char **)(rec + 0x68) != 0 && _strcmpi(*(char **)(rec + 0x68), s) == 0) {
            if ((*(unsigned char *)(rec + 0x6d) & 0x20) == 0 && dir > 0)
                return 0;
            d3_ReleaseTexture(*(int *)(bk[0] + 0x1ec) + 0xf4, *(int *)(off + 0x178 + *(int *)(bk[0] + 0x1ec)));
        }
        off += 0x7c;
    } while (off < 0x174);
    i = 0;
    s = (char *)(*(int *)(bk[0] + 0x1ec) + 0x17d);
    do {
        if (*(int *)(s - 5) != 0 && (*(unsigned char *)s & 0x20) == 0)
            return 0;
        i++;
        s += 0x7c;
    } while (i < 3);
    pg = 0;
    if (*((char *)bk + 0x271) > 0) {
        p = bk + 0x8a;
        do {
            a = (int)(char)bk[0x9c];
            lim = a;
            if (dir <= 0)
                lim = a - 2;
            if (pg < lim || (dir <= 0 ? a + 2 : a + 4) < pg) {
                d2_SetViewTextures(*p, ch_EmptyName, ch_EmptyName);
                d2_EnableViewObjects(*p, 0);
            } else {
                lim = (pg & 3) + 1;
                sprintf(top, "Buch_Seite%i_oben_ns_nm", lim);
                sprintf(bottom, "Buch_Seite%i_unten_ns_nm", lim);
                d2_SetViewTextures(*p, top, bottom);
                d2_EnableViewObjects(*p, 1);
            }
            pg++;
            p++;
        } while (pg < (int)*((char *)bk + 0x271));
    }
    if (dir <= 0)
        d3t_PlayAnim(bk[0], "buch\\Buch_zurueckblaettern.baf", 0x12);
    else
        d3t_PlayAnim(bk[0], "buch\\Buch_vorblaettern.baf", 0x12);
    *(char *)(bk + 0x9c) = (char)bk[0x9c] + dir;
    sv_Unk52f340(bk);
    sb_PlaySample(SV_I((unsigned)&full_data_6f240c), "Umblaettern_s", 1, 0x3f, 0x7f, 0, 0);
    return 1;
}
