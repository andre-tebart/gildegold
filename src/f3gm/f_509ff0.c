/* 0x509ff0, the character that lives in the building the object o is in (word +0x25 of the building), or o's own character */

unsigned short *gm_Unk509ff0(unsigned short *o)
{
    int b;

    gm_FindById(&b, 0, (int *)&o, *(int *)(o + 5));
    if (o == 0) {
        if (b != 0 && *(unsigned short *)(b + 0x25) != 0xffff)
            return (unsigned short *)ai_Chars[*(unsigned short *)(b + 0x25)];
        o = 0;
    }
    return o;
}
