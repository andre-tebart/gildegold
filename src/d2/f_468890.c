/* 0x468890, index of the picture with this name, -1 if there is none */

int d2_FindPic(char *name)
{
    int i;

    for (i = 0; i < 0x800; i++)
        if (d2_Pics[i].name[0] && !strcmp(name, d2_Pics[i].name))
            return i;
    return -1;
}
