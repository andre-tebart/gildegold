/* 0x4e5b50, the debug dump of the 8 relatives (the ids at +0x5c): "%i\t%s" of each one's +4 and name (+0x30), -1 and 0x143b3f4 when it does not exist */

void ai_Unk4e5b50(char *c, char *buf)
{
    char tmp[0x100];
    int *p;
    char *o;
    int i;

    p = (int *)(c + 0x5c);
    for (i = 8; i != 0; i--) {
        o = (char *)gm_FindChar(*p);
        if (o != 0)
            sprintf(tmp, "%i\t%s", *(int *)(o + 4), o + 0x30);
        else
            sprintf(tmp, "%i\t%s", -1, ai_Unk143b3f4);
        strcat(buf, tmp);
        p++;
    }
}
