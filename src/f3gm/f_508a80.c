/* 0x508a80, formats the amount v for the player slot k into out: "%i%c" below 1000, else with a '.' every three digits ("%s%c", the unit char 0x11 at the end), "0%c" for zero, a '-' in front for negative values. ROUGH */

void gm_Unk508a80(int v, unsigned char k, char *out)
{
    int n, i, j, e;
    char a[0x40], b[0x80];
    char *q;

    n = gm_ConvertMoney(v < 0 ? -v : v, k);
    if (v < 0) {
        if (n != 0) {
            if (n < 1000) {
                sprintf(out, "-%i%c", n, 0x11);
                return;
            }
            _itoa(n, a, 10);
            e = (int)((strlen(a) - 1) / 3.0f) + strlen(a);
            j = -1;
            i = strlen(a) - 1;
            if (i >= 0) {
                q = b + e;
                do {
                    *q = a[i];
                    q--;
                    if (i != 0 && j > 0 && (j + 1) % 3 == 0)
                        *q-- = '.';
                    j++;
                    i--;
                } while (i >= 0);
            }
            b[e] = 0;
            sprintf(out, "-%s%c", b, 0x11);
            return;
        }
    } else {
        if (n > 999) {
            _itoa(n, a, 10);
            e = (int)((strlen(a) - 1) / 3.0f) + strlen(a);
            j = -1;
            i = strlen(a) - 1;
            if (i >= 0) {
                q = b + e;
                do {
                    *q = a[i];
                    q--;
                    if (i != 0 && j > 0 && (j + 1) % 3 == 0)
                        *q-- = '.';
                    j++;
                    i--;
                } while (i >= 0);
            }
            b[e] = 0;
            sprintf(out, "%s%c", b, 0x11);
            return;
        }
        if (n != 0) {
            sprintf(out, "%i%c", n, 0x11);
            return;
        }
    }
    sprintf(out, "0%c", 0x11);
}
