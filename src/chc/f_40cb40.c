/* 0x40cb40, PreloadAnimation(ch, a, b, c, d): preloads four animations for the character (ch_PreloadAnims); 1 after an error, else 0 */

int chc_PreloadAnimation(CHARACTER **ch, char **a, char **b, char **c, char **d)
{
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "PreloadAnimation(): Invalid character");
        return 1;
    }
    ch_PreloadAnims(*ch, 4, *a, *b, *c, *d);
    return 0;
}
