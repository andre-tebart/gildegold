/* 0x44e4a0, loads an animation stream "animations/" + name and links it into the animation list */

D3IOANIMDATA *d3_LoadAnimStream(char *name, char *file, char unk)
{
    char buf[256];
    D3IOANIMDATA *a;

    strcpy(buf, d3_AnimDir);
    strcat(buf, name);
    a = (D3IOANIMDATA *)d3_FindAnim(file);
    if (a != 0) {
        sprintf(buf, "d3_LoadAnimStream(): animation already in stock: %s", name);
        return a;
    }
    a = (D3IOANIMDATA *)d3_LoadBinaryAnimStream(buf, file, unk);
    if (a != 0) {
        a->next = d3_AnimList;
        d3_AnimList = a;
        a->prev = &d3_AnimHead;
        a->next->prev = a;
    }
    return a;
}
