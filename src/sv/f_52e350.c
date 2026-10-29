/* 0x52e350, creates n buttons (widget kind 0xbf, picture 0x53) in a row in the form f at height y, the labels are the n string arguments; the widths are the widest label + 0x20, ids returned in out[];
   the row is centred (or the labels' width is used as the step when the form is too narrow) */

void sv_ButtonRow(int f, int y, int *out, int n, ...)
{
    char labels[0x20][0x40];
    int wmax, w, step, x, i, id;
    char *s;
    va_list ap;

    wmax = 0;
    va_start(ap, n);
    for (i = 0; i < n; i++) {
        s = va_arg(ap, char *);
        strcpy(labels[i], s);
        w = d2_TextWidth(s, (int)*(short *)((unsigned)&ai_Unkf8f39c + f * 0x3c4)) + 0x20;
        if (wmax < w)
            wmax = w;
    }
    w = *(short *)((unsigned)&ai_Unkf8f128 + f * 0x3c4);
    step = w / (n + 1);
    x = step;
    if (step < wmax) {
        step = wmax + 8;
        x = step * n;
        if (x - w == 0 || x < w)
            x = w - x;
        else
            x = x - w;
        x = x / 2 + step / 2;
    }
    x -= wmax / 2;
    for (i = 0; i < n; i++) {
        id = d2_WinCreateSlider(x, y, 0xbf, labels[i], f);
        out[i] = id;
        SV_WS(id, 0x74) = 0x53;
        d2_SetObjWidth(id, wmax);
        x += step;
    }
}
