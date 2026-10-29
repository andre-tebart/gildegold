/* 0x420db0, walks the reduced octree (children first) and gives every node that kept pixels the next
   palette index with its mean color */

void qnt_MakePalette(int idx, int lvl)
{
    int i;
    unsigned int c, g, b;

    if (qnt_Level[lvl][idx].mask) {
        for (i = 7; i >= 0; i--)
            if (qnt_Level[lvl][idx].mask & (1 << i))
                qnt_MakePalette(i + idx * 8, lvl + 1);
    }
    if (qnt_Level[lvl][idx].count) {
        qnt_Level[lvl][idx].index = qnt_Colors;
        c = qnt_Level[lvl][idx].count;
        g = qnt_Level[lvl][idx].g;
        b = qnt_Level[lvl][idx].b;
        qnt_PalR[qnt_Colors] = (qnt_Level[lvl][idx].r + (c >> 1)) / c;
        qnt_PalG[qnt_Colors] = ((c >> 1) + g) / c;
        qnt_PalB[qnt_Colors++] = ((c >> 1) + b) / c;
    }
}
