/* 0x46c490, makes a radio button group of the n objects given; -1 if there is no free group */

int d2_CreateRadioGroup(int n, ...)
{
    va_list ap;
    int i;
    int j;

    for (i = 0; i < 8; i++)
        if (!d2_Radios[i].n)
            break;
    if (i >= 8)
        return -1;
    if (n > 0x20) {
        err_Message("d2_CreateRadioGroup: Too many objecthandles...");
        return -1;
    }
    d2_Radios[i].n = n;
    d2_Radios[i].sel = -1;
    va_start(ap, n);
    for (j = 0; j < n; j++) {
        d2_Radios[i].objs[j] = va_arg(ap, int);
        d2_Objs[d2_Radios[i].objs[j]].checkable = 1;
        d2_Objs[d2_Radios[i].objs[j]].style &= ~2;
    }
    va_end(ap);
    return i;
}
