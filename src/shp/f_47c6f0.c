/* 0x47c6f0, draws shape i of a bank; 1 if drawn */

int d2_DrawShape(int x, int y, void *surf, void *pic, unsigned char i)
{
    D2BANK *bank = (D2BANK *)pic;
    char buf[128];

    if (bank == 0)
        return 0;
    if (i > bank->n) {
        sprintf(buf, "shp_ShowShapeFromBank:Shapenr is invalidate! Bank:%s", (char *)bank + 10);
        return 0;
    }
    d2_ShpBlit(x, y, (D2SURF *)surf, (D2SHP *)((char *)bank + bank->offs[i]));
    return 1;
}
