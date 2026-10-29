/* 0x4a66f0: the positions of two buildings' models (0x61) in scene 0: a (raised by 0.8 of its height), b; -1
   when one is missing or they are the same */

int ai_Unk4a66f0(AIBLD *x, float *a, AIBLD *y, float *b)
{
    int scene = d3_CurrentScene;
    float mn[4];
    float mx[4];

    if (x != 0 && y != 0 && x->object != 0 && y->object != 0 && x != y) {
        d3if_SelectScene(0, 1);
        d3_PointToWorld((void *)x->object, x->object + 0x4c, a);
        d3t_GetObjectBounds((void *)mn, (void *)mx, (void *)x->object);
        a[1] = (mx[1] - mn[1]) * 0.8f + a[1];
        d3_PointToWorld((void *)y->object, y->object + 0x4c, b);
        d3if_SelectScene(scene, 1);
        return 0;
    }
    return -1;
}
