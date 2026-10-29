/* 0x469180, the same, giving the data and the index of the picture used */

int d2_GetPicEx(int idx, void **pic, int *real)
{
    int j = idx;

    d2_PicShift = 0;
    if (d2_Pics[idx].flags & 2)
        idx += d2_PicBase;
    if ((d2_Pics[idx].type == 5 || d2_Pics[idx].type == 8) && d2_Pics[idx].offset == 0) {
        if (!d2_Pics[d2_Pics[idx].base].data)
            d2_LoadPic(d2_Pics[idx].base, 0);
        j = d2_Pics[idx].base;
        d2_PicShift = idx - j;
    } else if (!d2_Pics[idx].data)
        d2_LoadPic(idx, 0);
    *pic = d2_Pics[j].data;
    *real = idx;
    return 1;
}
