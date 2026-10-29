/* 0x46bd30, copies item idx of the widget's list to out */

void d2_ListCopyItem(int id, int idx, char *out)
{
    char buf[1024];
    char *p, *q;
    int i;

    i = 0;
    strcpy(buf, d2_Objs[id].list);
    p = buf;
    q = strchr(buf + 1, '|');
    for (; i < idx; i++) {
        if (!q)
            return;
        p = q;
        q = strchr(q + 1, '|');
    }
    if (q) {
        *q = 0;
        strcpy(out, p);
    }
}
