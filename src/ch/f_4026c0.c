/* 0x4026c0, characters in scene (-1: all scenes); all = 0 skips objects of type 1 */

int ch_Count(int scene, int all)
{
    int i, n = 0;
    CHARACTER *ch;

    for (i = 0; i < CH_MAX; i++) {
        ch = ch_List[i];
        if (ch && (scene == -1 || ch->scene == &d3_Scenes[scene]) && (all || ch->obj->type != 1))
            n++;
    }
    return n;
}
