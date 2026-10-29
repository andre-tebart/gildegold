/* 0x47cae0, draws shape i of a bank in mode 3 (light table) */

int d2_DrawShapeMode3(int x, int y, void *surf, void *pic, char i)
{
    D2BANK *bank = (D2BANK *)pic;
    D2SHP *s;
    char old;

    if ((unsigned char)i > bank->n)
        return 0;
    s = (D2SHP *)((char *)bank + bank->offs[(unsigned char)i]);
    old = s->mode;
    s->mode = 3;
    d2_ShpBlit(x, y, (D2SURF *)surf, (D2SHP *)((char *)bank + bank->offs[(unsigned char)i]));
    s->mode = old;
    return 1;
}
