/* 0x509ee0, the amount of the character c's object of the prototype word of the player p */

int gm_Unk509ee0(char *c, char p)
{
    char *o;

    o = gm_Query(*(void **)(c + 0x178), 1, 0, PLW54(p));
    if (o)
        return *(int *)(o + 0xe);
    return 0;
}
