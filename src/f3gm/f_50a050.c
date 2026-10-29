/* 0x50a050, the same with the owner word (+0x27) */

unsigned short *gm_Unk50a050(unsigned short *o)
{
    int b;

    gm_FindById(&b, 0, (int *)&o, *(int *)(o + 5));
    if (o == 0) {
        if (b != 0 && *(unsigned short *)(b + 0x27) != 0xffff)
            return (unsigned short *)ai_Chars[*(unsigned short *)(b + 0x27)];
        o = 0;
    }
    return o;
}
