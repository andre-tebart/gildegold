/* 0x477320, creates the surfaces of the item list @deep Q2 only left: D2SLOT.unk00/unk04 (the definition writes both -1 when creating / freeing each slot), and no reader of either field exists in any drafted function (grep D2SLOT/f8aa90/f8aaa0/f8aadc: only this unit; the surfaces are created and freed but not drawn yet), so no evidence for their role; the function, d2_MakeSurface, d2_ExtraSymbolsPic / d2_TextCursorPic / d2_TextCursorOffset and d2_ItemListSurf64/128/256 are renamed */

void d2_CreateItemListSurfaces(void)
{
    int desc[16];
    int i;

    d2_PointerPic = 0;
    d2_PointerLook = 0;
    d2_TextCursorOffset = (d2_TextCursorPic != d2_ExtraSymbolsPic) ? 0 : -7;
    memset(desc, 0, sizeof desc);
    desc[1] = 0x40;
    desc[2] = 0x40;
    d2_ItemListSurf64 = d2_MakeSurface(desc, 1);
    for (i = 0; i < 3; i++) {
        memset(desc, 0, sizeof desc);
        d2_ItemListSurf128[i].unk00 = -1;
        d2_ItemListSurf128[i].unk04 = -1;
        desc[1] = 0x80;
        desc[2] = 0x80;
        d2_ItemListSurf128[i].surf = d2_MakeSurface(desc, 3);
    }
    memset(desc, 0, sizeof desc);
    d2_ItemListSurf256.unk00 = -1;
    d2_ItemListSurf256.unk04 = -1;
    desc[1] = 0x100;
    desc[2] = 0x100;
    d2_ItemListSurf256.surf = d2_MakeSurface(desc, 3);
    for (i = 0; i < 6; i++) {
        d2_PointerItems[i].id = -1;
        d2_PointerItems[i].amount = 0;
    }
}
