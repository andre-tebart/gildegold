/* 0x478870, copies a string to d2_CaptionCopy */

void d2_CopyCaption(char *s)
{
    char c;
    int off = (int)d2_CaptionCopy - (int)s;

    do {
        c = *s;
        s[off] = c;
        s++;
    } while (c != 0);
}
