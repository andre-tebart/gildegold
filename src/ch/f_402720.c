/* 0x402720, the same, counting only characters at pos */

int ch_CountAt(int scene, int all, D3VECTOR *pos, float eps)
{
    int i, n = 0;
    CHARACTER *ch;

    for (i = 0; i < CH_MAX; i++) {
        ch = ch_List[i];
        if (ch && (scene == -1 || ch->scene == &d3_Scenes[scene]) && (all || ch->obj->type != 1)
            && d3_VecEqual(&ch->obj->pos, pos, eps))
            n++;
    }
    return n;
}
