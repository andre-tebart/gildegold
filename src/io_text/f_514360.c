/* 0x514360, the digits at the end of s[0 .. n] (scanning back from n while isdigit): copied to out, returns the start index */


int io_TrailingDigits(char *s, int n, char *out)
{
    int i;

    for (i = n; i >= 0; i--) {
        if (!isdigit(s[i]))
            break;
    }
    i++;
    memcpy(out, s + i, n - i + 1);
    out[n - i + 1] = 0;
    return i;
}
