/* 0x501400, stocks a new building b with the objects its building kind needs (the table 0x61a348, one record of 0x15 bytes per kind: the kind byte and ten object numbers): the container object 0xff (query (2, 6, 0, 0xff), created by 0x500190 when missing) gets each listed object with the amount 2 when it has none */

int gm_IsHomeType(unsigned char *b);
int gm_IsType10(unsigned char *b);
int gm_BuildingClass(int kind);

void gm_StockBuildingObjects(char *b)
{
    short *w;
    int o, o2, k;
    char *t;

    w = (short *)(ai_Unk61a348 + 1);
    do {
        if (*b == *((char *)w - 1)) {
            o = (int)gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0xff);
            if (o != 0 || (o = gm_CreateObject(*(int *)(b + 1), 0xff, 1)) != 0) {
                short *p = w;
                for (k = 10; k != 0; k--, p++) {
                    if (*p != 0 && gm_Query(*(int *)(o + 0x14), 1, 0, *p) == 0)
                        gm_CreateObject(*(int *)(o + 2), *p, 2);
                }
            }
        }
        w = (short *)((char *)w + 0x15);
    } while ((int)w < ((unsigned)&ai_Unk61a348 + 0x30a));
}
