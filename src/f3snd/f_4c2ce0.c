/* 0x4c2ce0, plays the click comment of the first selected character (walks the selected flag +0x188 of the
   ai_Chars records, the same click snd_PlayWorkerClick plays on a click of that character) */

void snd_PlaySelectedClick(void)
{
    int i;
    char *p;

    for (i = 0, p = ai_Selected; (int)p < (int)(ai_Selected + 0x300 * 0x21c); p += 0x21c, i++) {
        if (*p != 0) {
            snd_PlayWorkerClick((G16CHARCLICK *)(ai_Chars + i * 0x21c));
            return;
        }
    }
}
