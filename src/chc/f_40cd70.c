/* 0x40cd70, CharacterCount(): the number of characters in the default scene */

int chc_CharacterCount(void)
{
    int n = 0;
    int i;

    for (i = 0; i < CH_MAX; i++) {
        if (ch_List[i] && ch_List[i]->scene == d3_DefaultScene)
            n++;
    }
    return n;
}
