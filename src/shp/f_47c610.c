/* 0x47c610, appends a shape to a bank */

int d2_BankAppendShape(D2SHP *s, D2BANK *bank)
{
    char tag[] = "SHAPBANK";

    if (bank->n >= 0xff)
        return 0;
    if (bank->size == 0 || bank->n == 0) {
        bank->size = 0x845;
        memcpy(bank, tag, 8);
        bank->pad00[8] = 1;
        bank->kind = s->type;
        if (bank->kind == 0)
            d2_BankSetPalette(bank, d2_Palette);
    }
    if ((unsigned char)bank->kind != s->type)
        return 0;
    memcpy((char *)bank + bank->size, s, s->size);
    bank->offs[bank->n] = bank->size;
    bank->size += s->size;
    if (s->w > bank->w)
        bank->w = s->w;
    if (s->h > bank->h)
        bank->h = s->h;
    bank->n++;
    return 1;
}
