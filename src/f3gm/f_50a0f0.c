/* 0x50a0f0, walks up the owner chain of the object o (the id at +6) to the first character */

int gm_Unk50a0f0(int o)
{
    int r, id, bld, chr;

    id = *(int *)(o + 6);
    while (1) {
        if (id == -1 || (r = gm_FindById(&o, &bld, &chr, id)) == 0)
            return 0;
        if (o != 0)
            return o;
        if (chr != 0)
            break;
        if (bld != 0)
            id = *(int *)(bld + 6);
        chr = 0;
        o = 0;
    }
    return 0;
}
