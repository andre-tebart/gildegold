/* 0x47c770, draws a half dark shape i of a bank */

int d2_DrawShapeHalfDark(int x, int y, void *surf, void *pic, char i)
{
    D2BANK *bank = (D2BANK *)pic;
    D2SHP *s;
    int old;
    char buf[128];

    if (bank == 0)
        return 0;
    if ((unsigned char)i > bank->n) {
        sprintf(buf, "shp_ShowShapeFromBank:Shapenr is invalidate! Bank:%s", (char *)bank + 10);
        return 0;
    }
    s = (D2SHP *)((char *)bank + bank->offs[(unsigned char)i]);
    if (d2_LightTableExternal == 0) {
        old = d2_SurfPitch;
        d2_SurfPitch = ((D2SURF *)surf)->pitch;
        switch (s->type) {
        case 0:
            return 0;
        case 1:
            d2_ShpBlit16HalfDark(x, y, (D2SURF *)surf, s);
            return 1;
        case 2:
            return 1;
        }
        d2_SurfPitch = old;
    }
    return 1;
}
