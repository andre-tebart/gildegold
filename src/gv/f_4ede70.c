/* 0x4ede70, frees the objects (3 per slot at +0x15c, 0x447190) of all effect slots; the scene is switched with 0x4478e0 around it */

void gv_FreeEffectObjects(void)
{
    int sc;
    int *p, *q;
    int i;

    sc = d3_CurrentScene;
    d3if_SelectScene(0, 1);
    p = (int *)(gv_Effects + 0x15c);
    do {
        i = 3;
        q = p;
        do {
            if (*q != 0) {
                d3if_Delete((struct D3OBJECT *)*q);
                *q = 0;
            }
            q++;
            i--;
        } while (i != 0);
        p += 0x5c;
    } while ((int)p < (int)((unsigned)&gv_Effects + 0xcdc));
    d3if_SelectScene(sc, 1);
}
