/* 0x477420, frees the surfaces of the item list @deep Q2 only left: D2SLOT.unk00/unk04 (both set -1 after freeing the slot's surface); no reader of either field in any drafted function, no evidence for their role; the function and d2_FreeSurface are renamed */

void d2_FreeItemListSurfaces(void)
{
    int i;

    if (d2_ItemListSurf64 != 0) {
        d2_FreeSurface(d2_ItemListSurf64);
        d2_ItemListSurf64 = 0;
    }
    for (i = 0; i < 3; i++) {
        if (d2_ItemListSurf128[i].surf != 0) {
            d2_FreeSurface(d2_ItemListSurf128[i].surf);
            d2_ItemListSurf128[i].surf = 0;
            d2_ItemListSurf128[i].unk00 = -1;
            d2_ItemListSurf128[i].unk04 = -1;
        }
    }
    d2_FreeSurface(d2_ItemListSurf256.surf);
    d2_ItemListSurf256.unk00 = -1;
    d2_ItemListSurf256.unk04 = -1;
    d2_ItemListSurf256.surf = 0;
}
