/* 0x4c9520, handler of type 0x2c: a place (data[8], data[9], word data[0xa], flag data[0xc], id data[0xd]) of the
   building data[4] is taken or freed */

int ai_Unk4c9520(G17MSG *m, G17STAT *s)
{
    char r = 2;
    int flag = 0;
    int save = d3_CurrentScene;
    char *o = 0;
    char *b, *x;

    if (ai_Unk1460ae0 != -1)
        veg_NextId = G17_I(*m, 0);
    b = gm_BuildQuery(1, 1, G17_I(*m, 4));
    if (b != 0) {
        x = veg_FindType(G17_S(*m, 0xa));
        if (x != 0 && m->data[0xc] == 0) {
            if (veg_Fits(*(int *)(b + 0x71), m->data[8], m->data[9], (unsigned char)x[0x42]) != 0)
                flag = 1;
        } else
            flag = 1;
        if (m->data[0xc] != 0) {
            o = veg_FindPlantAt(*(int *)(b + 0x71), m->data[8], m->data[9]);
            if (o != 0 && *(int *)(o + 0x10) != -1)
                return 1;
        } else if (G17_I(*m, 0xd) != -1) {
            o = veg_FindPlantAt(*(int *)(b + 0x71), m->data[8], m->data[9]);
            if (o != 0 && *(int *)(o + 0x10) == G17_I(*m, 0xd)) {
                d3if_SelectScene((char)init_BuildingUniverseV[*(unsigned char *)(ai_BTypes + (unsigned char)b[0] * 0x24d)], 1);
                d3if_Delete((struct D3OBJECT *)*(int *)(o + 0x14));
                *(int *)(o + 0x14) = 0;
                veg_LoadModel(o, 0);
                *(int *)(o + 0x10) = -1;
                d3if_SelectScene(save, 1);
            }
            if (s != 0) {
                s->value = (int)o;
                s->state = 1;
                s->unk01 = 5;
            }
            return 0;
        }
        if (flag != 0) {
            d3if_SelectScene((char)init_BuildingUniverseV[*(unsigned char *)(ai_BTypes + (unsigned char)b[0] * 0x24d)], 1);
            o = veg_SetPlant(*(int *)(b + 0x71), m->data[8], m->data[9], G17_S(*m, 0xa), m->data[0xc]);
            d3if_SelectScene(save, 1);
        }
        if (o != 0) {
            r = 1;
            *(int *)(o + 4) = *(int *)(b + 1);
            if (m->data[0xc] != 0)
                *(int *)(o + 0x10) = G17_I(*m, 0xd);
        }
    }
    if (s != 0) {
        s->value = (int)o;
        s->state = r;
        s->unk01 = 5;
    }
    return 0;
}
