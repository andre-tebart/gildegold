/* 0x472d20, removes the objects of a window (all of them, or all but the frame) */

int d2_RemoveWindowObjects(win, all)
int win;
int all;
{
    int i, ob;

    if (win > 0x60)
        return 0;
    if (!d2_Wins[win].active)
        return 0;
    {
        for (i = d2_Wins[win].nobjs - 1; i >= 0; i--) {
            ob = d2_Wins[win].objs[i];
            if (ob == d2_Wins[win].btnup)
                d2_Wins[win].btnup = -1;
            else if (ob == d2_Wins[win].btndown)
                d2_Wins[win].btndown = -1;
            else if (ob == d2_Wins[win].bar)
                d2_Wins[win].bar = -1;
            ob = d2_Wins[win].objs[i];
            if (d2_Objs[ob].type != 0x40 || all != 0)
                d2_RemoveObj(ob);
        }
        if (d2_Wins[win].text)
            *d2_Wins[win].text = 0;
        d2_Wins[win].scrollY = 0;
        d2_Wins[win].contentHeight = 0;
        d2_Wins[win].scrollRemY = 0;
        d2_Wins[win].appliedScrollY = 0;
        return 1;
    }
}
