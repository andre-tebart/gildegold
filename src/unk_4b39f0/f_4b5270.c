/* 0x4b5270, the share of players (kind 6 / 7) among the holders of the buildings of the 0x501190(1, 5, k) list
   (k 0: (1, 6)) */

void ai_PlayerShare(int k, G13STAT *s)
{
    AIBLD *b;
    unsigned short w;
    int same, n;

    same = 0;
    if (k != 0)
        b = G13_501190(1, 5, k);
    else
        b = G13_501190(1, 6);
    n = 0;
    for (; b != 0; b = gm_BuildNext(), n++) {
        w = *(unsigned short *)((char *)b + 0x27);
        if (w != 0xffff && (ai_CharsT[w].kind == 6 || ai_CharsT[w].kind == 7))
            same++;
    }
    s->same = same;
    s->n = n;
    if (n > 0)
        s->v = (float)same / n;
    else
        s->v = 0.0f;
}
