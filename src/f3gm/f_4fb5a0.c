/* 0x4fb5a0, picks the variant of the object o (+0x184 its model record): byte +0x23a of the model = the id of o (+4) modulo the variant count of the model class (+0x1e4, or & 3 when 5 or more) unless the model is the shared one 0xfd7ba0; when the class is the current one (0x5f1294) 0x446f30 refreshes it. ROUGH */

void gm_SelectModelVariant(char *o)
{
    unsigned v;
    char *m;
    int cls;

    v = 0;
    if (o != 0 && (m = *(char **)(o + 0x184)) != 0) {
        cls = *(int *)(*(int *)(m + 0x34) + 0x1ec);
        if (*(char **)(m + 0x88) != (char *)(unsigned)&d3_Scenes) {
            v = *(unsigned *)(*(int *)(cls + 0x104) + 0x1e4);
            if ((int)v < 5)
                v = *(unsigned *)(o + 4) % v;
            else
                v = *(unsigned *)(o + 4) & 3;
            m[0x23a] = (char)v;
        }
        if (ch_CurrentGroup == *(int *)(m + 0x2c))
            d3if_SelectTextureSet(*(int *)(m + 0x34), cls + 0xf4, v, 1);
    }
}
