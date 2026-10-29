/* 0x47cd20, starts an animation of a bank at (x, y); its number or -1 @deep B3, 24 code hunks, no meaning error: the same fields are stored with the same values and order (x, bank = data, frame 0, timer 0, y, shape from the bank palette) and the returns are i / -1; the hunks are the branch shape (the original keeps two inline or eax,-1; ret tails where VC6 merges ours into one shared tail, jge vs jl) and the parameter-load scheduling (dx / si / bl load points), i.e. codegen */

int d2_StartAnim(short x, short y, void *data, unsigned char z)
{
    int i;
    D2ANIM *a;

    for (i = 0, a = d2_Anims; (int)a < (int)((unsigned)d2_Anims + 0x110); i++, a++) {
        if (a->bank == 0) {
            if (i >= 16)
                return -1;
            d2_Anims[i].x = x;
            d2_Anims[i].bank = (D2BANK *)data;
            d2_Anims[i].frame = 0;
            d2_Anims[i].timer = 0;
            d2_Anims[i].y = y;
            d2_Anims[i].shape = 0;
            d2_Anims[i].shape = *((unsigned char *)data + ((D2BANK *)data)->pal);
            if (z & 1)
                d2_Anims[i].shadow = 1;
            return i;
        }
    }
    return -1;
}
