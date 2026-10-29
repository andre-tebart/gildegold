/* 0x42c480, truncates s before its trailing spaces (the last character is left alone) */

void txt_TrimRight(char *s)
{
    int i = strlen(s) - 1;

    if (s[i] == ' ' && i > 0) {
        while (i > 0) {
            if (s[i] != ' ') {
                s[i + 1] = 0;
                return;
            }
            i--;
        }
    }
}
