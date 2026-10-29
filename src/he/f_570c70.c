/* 0x570c70, the text 0x40ff (the word of b) to a[0] (flag 1, 0x5f4) */

void he_Unk570c70(int *a, int b)
{
    char text[0x400];

    txt_Format(text, 0x40ff, *(unsigned short *)b);
    he_ShowNews(a[0], -1, text, 1, 0x5f4, 0);
}
