/* 0x4b35e0, 0x4b3280 with both passes (0x4b2c90, commands if cmd): 5 when they fail */

int hst_NextEventCmd(char *name, char *out, int cmd)
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
        d = ai_Time.year - year[0];
        if (d == 1) {
            r = d;
            if (!hst_ParseEventText(out, hst_TextCurrent, cmd))
                r = 5;
            else if (*out == 0)
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
    } while (r == 0 || r == 6 || r == 5);
    return r;
}
