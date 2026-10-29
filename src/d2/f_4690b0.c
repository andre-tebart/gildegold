/* 0x4690b0, data of the picture idx, loading it if needed; a frame (type 5 or 8) of another picture gives
   the data of that picture and sets d2_PicShift to the distance of the two */

void *d2_GetPic(int idx)
{
    int base;

    d2_PicShift = 0;
    if (d2_Pics[idx].flags & 2)
        idx += d2_PicBase;
    if ((d2_Pics[idx].type == 5 || d2_Pics[idx].type == 8) && d2_Pics[idx].offset == 0) {
        base = d2_Pics[idx].base;
        if (!d2_Pics[base].data)
            d2_LoadPic(base, 0);
        base = idx;
        idx = d2_Pics[idx].base;
        d2_PicShift = base - idx;
    } else if (!d2_Pics[idx].data)
        d2_LoadPic(idx, 0);
    return d2_Pics[idx].data;
}
