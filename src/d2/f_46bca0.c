/* 0x46bca0, sets the widget's item list: the n strings joined as "a|b|c|" */

void d2_SetList(int id, int n, ...)
{
    D2OBJ *w;
    va_list ap;
    int i;

    w = &d2_Objs[id];
    w->list[0] = 0;
    va_start(ap, n);
    for (i = 0; i < n; i++) {
        strcat(w->list, va_arg(ap, char *));
        strcat(w->list, "|");
    }
    va_end(ap);
}
