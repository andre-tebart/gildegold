/* 0x52e4b0, like 0x52e350 with the picture 0x54 / widget kind 0xc5 and the label width measured with the font 0x54 */

void sv_Unk52e4b0(int f, int y, int *out, int n, ...)
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
        w = d2_TextWidth(s, 0x54);
        if (wmax < w + 0x20)
            wmax = w + 0x20;
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
        id = d2_WinCreateSlider(x, y, 0xc5, labels[i], f);
        out[i] = id;
        SV_WS(id, 0x74) = 0x54;
        d2_SetObjWidth(id, wmax);
        x += step;
    }
}
