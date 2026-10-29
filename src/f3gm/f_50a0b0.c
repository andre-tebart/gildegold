/* 0x50a0b0, the character number of the owner of the object o */

unsigned short gm_Unk50a0b0(unsigned short *o)
{
    int b;

    gm_FindById(&b, 0, (int *)&o, *(int *)(o + 5));
    if (o != 0)
        return *o;
    if (b != 0)
        return *(unsigned short *)(b + 0x27);
    return 0xffff;
}
