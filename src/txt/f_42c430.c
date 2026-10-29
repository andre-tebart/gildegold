/* 0x42c430, returns the first non-space character of s, or NULL when s is empty or all spaces */

char *txt_SkipSpaces(char *s)
{
    int i;

    for (i = 0; i < strlen(s);)
        if (s[i++] != ' ')
            break;
    if (i == strlen(s))
        return NULL;
    return &s[i - 1];
}
