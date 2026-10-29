/* 0x47c970, draws shape i of a bank in mode 2 (half dark) */

int d2_DrawShapeMode2(int x, int y, void *surf, void *pic, unsigned char i)
{
    D2BANK *bank = (D2BANK *)pic;
    D2SHP *s;
    char old;
    char buf[128];

    if (bank == 0)
        return 0;
    if (i > bank->n) {
        sprintf(buf, "shp_ShowShapeFromBank:Shapenr is invalidate! Bank:%s", (char *)bank + 10);
        return 0;
    }
    s = (D2SHP *)((char *)bank + bank->offs[i]);
    old = s->mode;
    s->mode = 2;
    d2_ShpBlit(x, y, (D2SURF *)surf, (D2SHP *)((char *)bank + bank->offs[i]));
    s->mode = old;
    return 1;
}
