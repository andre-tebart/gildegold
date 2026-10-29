/* 0x427dd0 */

int evt_Match(char *s, char *word)
{
    int n = 0;

    for (; n < 0x80 && word[n]; n++)
        if (s[n] != word[n])
            return 0;
    return n;
}
