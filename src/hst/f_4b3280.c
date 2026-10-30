/* 0x4b3280, the next history event (3 texts per entry from hst_TextCurrent: date, text, ..) of last year: 1 with
   its date in name and its text in out; 2: not yet; 3 / 4: history off / ended */

int hst_NextEvent(char *name, char *out)
{
    int year[4];
    int r, d;

    do {
        if (!hst_TextsOn)
            return 3;
        if (!hst_ParseDate(hst_TextCurrent, 3, year, name)) {
            hst_TextsOn = 0;
            return 4;
        }
        d = ai_TimeT.year - year[0];
        if (d == 1) {
            r = d;
            hst_ParseText_Real(out, hst_TextCurrent + 1);
            if (*out == 0)
                r = 6;
        } else if (d > 1)
            r = 0;
        else {
            r = 2;
            break;
        }
        hst_TextCurrent += 3;
        if (hst_TextLast < hst_TextCurrent)
            hst_TextsOn = 0;
    } while (r == 0 || r == 6);
    return r;
}
